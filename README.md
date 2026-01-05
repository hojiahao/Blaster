# Blaster - 多人联网射击游戏

基于 **Unreal Engine 5.7** 开发的多人在线射击游戏，实现了完整的网络同步架构、射击战斗系统、动画系统和 UI/HUD 系统。

## 项目特性

- **多人联机**：基于 Steam Online Subsystem 实现会话创建、查找、加入
- **射击系统**：屏幕中心射线追踪、动态准星扩散、FOV 变焦瞄准
- **动画系统**：Aim Offset、Turn In Place、左手 IK、身体倾斜
- **武器系统**：武器拾取装备、投射物发射、弹壳物理模拟
- **网络同步**：属性复制 + RPC 调用，支持多人实时对战

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
│   ├── BlasterCharacter.cpp      # 主角色 - 输入、相机、Aim Offset
│   └── BlasterAnimInstance.cpp   # 动画实例 - IK、Lean、状态同步
├── BlasterComponents/
│   └── CombatComponent.cpp       # 战斗组件 - 射击、瞄准、准星
├── Weapon/
│   ├── Weapon.cpp                # 武器基类
│   ├── ProjectileWeapon.cpp      # 投射物武器
│   ├── Projectile.cpp            # 投射物
│   └── Casing.cpp                # 弹壳
├── HUD/
│   ├── BlasterHUD.cpp            # 准星渲染
│   └── OverheadWidget.cpp        # 头顶 UI
├── GameMode/
│   └── LobbyGameMode.cpp         # 大厅模式
└── PlayerController/
    └── BlasterPlayerController.cpp

Plugins/MultiplayerSessions/      # 自定义多人会话插件
```

## 核心功能实现

### 1. 网络同步

```cpp
// 射击 RPC 调用链：客户端 → 服务器 → 所有客户端
UFUNCTION(Server, Reliable)
void ServerFire(const FVector_NetQuantize& TraceHitTarget);

UFUNCTION(NetMulticast, Reliable)
void MulticastFire(const FVector_NetQuantize& TraceHitTarget);
```

### 2. 动态准星系统

```cpp
// 准星扩散 = 基础值 + 速度因子 + 空中因子 - 瞄准因子 + 射击因子
HUDPackage.CrosshairSpread = 0.5f
    + CrosshairVelocityFactor
    + CrosshairInAirFactor
    - CrosshairAimFactor
    + CrosshairShootingFactor;
```

### 3. Aim Offset 与 Turn In Place

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

## 开发者

- **hojiahao**

## License

MIT License
