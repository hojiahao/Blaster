# Blaster - 多人联网射击游戏

基于 **Unreal Engine 5.7** 开发的多人在线第一人称射击游戏（FPS），实现了完整的网络同步架构、射击战斗系统、生命值与伤害系统、消灭重生机制、得分统计以及动画/UI 系统。

## 项目特性

### 核心玩法
- **多人联机**：基于 Steam Online Subsystem 实现会话创建、查找、加入
- **射击系统**：屏幕中心射线追踪、动态准星扩散、FOV 变焦瞄准、自动连射
- **武器系统**：武器拾取装备、投射物发射、弹药管理、弹壳物理模拟
- **伤害系统**：投射物命中检测、伤害应用、命中特效与音效

### 角色系统
- **生命值系统**：100 点生命值，受伤时 HUD 实时更新
- **消灭机制**：死亡动画、溶解材质特效、消灭粒子效果
- **重生系统**：自动随机 PlayerStart 重生
- **得分统计**：击杀得分、死亡次数追踪

### 动画系统
- **Aim Offset**：水平/垂直瞄准偏移
- **Turn In Place**：原地转身动画
- **左手 IK**：武器握持位置自适应
- **身体倾斜**：移动时的 Lean 效果

### 网络同步
- **属性复制**：条件复制优化带宽
- **RPC 调用链**：Server → NetMulticast 架构
- **网络频率**：66Hz 更新，33Hz 最低保障

## 技术栈

| 类别 | 技术 |
|------|------|
| 引擎 | Unreal Engine 5.7 |
| 语言 | C++ / Blueprint |
| 网络 | Steam Online Subsystem |
| 插件 | MultiplayerSessions (自定义) |

## 项目结构

```
Source/Blaster/
├── Character/
│   ├── BlasterCharacter.cpp      # 主角色 - 输入、相机、Aim Offset、生命值
│   └── BlasterAnimInstance.cpp   # 动画实例 - IK、Lean、状态同步
├── BlasterComponents/
│   └── CombatComponent.cpp       # 战斗组件 - 射击、瞄准、准星、自动射击
├── Weapon/
│   ├── Weapon.cpp                # 武器基类 - 状态管理、弹药
│   ├── ProjectileWeapon.cpp      # 投射物武器
│   ├── Projectile.cpp            # 投射物 - 碰撞、伤害
│   ├── ProjectileBullet.cpp      # 子弹实现
│   └── Casing.cpp                # 弹壳物理
├── HUD/
│   ├── BlasterHUD.cpp            # 准星渲染
│   ├── CharacterOverlay.cpp      # 角色 UI 控件（血条等）
│   └── OverheadWidget.cpp        # 头顶 UI
├── GameMode/
│   ├── BlasterGameMode.cpp       # 主游戏模式 - 消灭处理、重生
│   └── LobbyGameMode.cpp         # 大厅模式
├── PlayerController/
│   └── BlasterPlayerController.cpp # HUD 更新
├── PlayerState/
│   └── BlasterPlayerState.cpp    # 得分、失败统计
├── Interfaces/
│   └── InteractWithCrosshairsInterface.h  # 准星交互接口
└── BlasterTypes/
    └── TurningInPlace.h          # 原地转身枚举

Plugins/
├── MultiplayerSessions/          # 自定义多人会话插件
│   ├── MultiplayerSessionsSubsystem.cpp  # 会话管理
│   └── Menu.cpp                  # UI 菜单
└── VisualStudioTools/            # VS 集成工具
```

## 核心功能实现

### 1. 网络同步架构

```cpp
// 射击 RPC 调用链：客户端 → 服务器 → 所有客户端
UFUNCTION(Server, Reliable)
void ServerFire(const FVector_NetQuantize& TraceHitTarget);

UFUNCTION(NetMulticast, Reliable)
void MulticastFire(const FVector_NetQuantize& TraceHitTarget);

// 消灭同步：服务器广播到所有客户端
UFUNCTION(NetMulticast, Reliable)
void MulticastElim();
```

### 2. 动态准星系统

```cpp
// 准星扩散 = 基础值 + 速度因子 + 空中因子 - 瞄准因子 + 射击因子
HUDPackage.CrosshairSpread = 0.5f
    + CrosshairVelocityFactor
    + CrosshairInAirFactor
    - CrosshairAimFactor
    + CrosshairShootingFactor;

// 准星颜色：瞄准敌人时变红
if (HitActor->Implements<UInteractWithCrosshairsInterface>())
    HUDPackage.CrosshairsColor = FLinearColor::Red;
```

### 3. 伤害与消灭系统

```cpp
// 伤害接收
void ABlasterCharacter::ReceiveDamage(float Damage, ...) {
    Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);
    UpdateHUDHealth();
    if (Health == 0.f) {
        // 触发消灭流程
        GameMode->PlayerEliminated(this, AttackerController, VictimController);
    }
}

// 消灭效果：动画 + 溶解特效 + 粒子
void ABlasterCharacter::Elim() {
    MulticastElim();  // 广播到所有客户端
}
```

### 4. 得分统计

```cpp
// 击杀得分
AttackerPlayerState->AddToScore(1.f);
// 死亡计数
VictimPlayerState->AddToDefeats(1);
```

### 5. Aim Offset 与 Turn In Place

```cpp
// 瞄准偏移超过 90° 时触发原地转身
if (AO_Yaw > 90.f)
    TurningInPlace = ETurningInPlace::ETIP_Right;
else if (AO_Yaw < -90.f)
    TurningInPlace = ETurningInPlace::ETIP_Left;
```

## 操作说明

| 按键 | 功能 |
|------|------|
| WASD | 移动 |
| 鼠标移动 | 视角控制 |
| 鼠标左键 | 射击 |
| 鼠标右键 | 瞄准 |
| E | 拾取/装备武器 |
| C | 下蹲 |
| Space | 跳跃 |

## 构建与运行

### 环境要求
- Unreal Engine 5.7
- Visual Studio 2022 / Rider
- Steam SDK (用于多人联机)

### 构建步骤
1. 克隆仓库
2. 右键 `Blaster.uproject` → Generate Visual Studio project files
3. 打开生成的 `.sln` 文件
4. 编译并运行

### 多人测试
1. 在编辑器中选择 `Multiplayer Options` → `Number of Players: 2+`
2. 选择 `Net Mode: Play As Listen Server` 或 `Play As Client`
3. 运行游戏进行多人测试

## 开发进度

- [x] 基础角色移动与相机
- [x] Enhanced Input 输入系统
- [x] 武器拾取与装备
- [x] 射击系统与投射物
- [x] 动态准星系统
- [x] Aim Offset 与 Turn In Place
- [x] 左手 IK 与身体倾斜
- [x] 多人网络同步 (Steam)
- [x] 生命值与伤害系统
- [x] 消灭动画与溶解特效
- [x] 重生系统
- [x] 得分与失败统计
- [x] 自动连射
- [x] 武器弹药系统
- [ ] 多种武器类型
- [ ] 游戏模式扩展
- [ ] 计分板 UI

## 开发者

- **hojiahao**

## License

MIT License
