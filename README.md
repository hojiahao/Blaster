# Blaster - 多人联网射击游戏

基于 **Unreal Engine 5.7.1** 开发的多人在线第一人称射击游戏（FPS），实现了完整的网络同步架构、多种武器系统、战斗系统、生命值与伤害系统、消灭重生机制、得分统计以及动画/UI 系统。

## 项目特性

### 核心玩法
- **多人联机**：基于 Steam Online Subsystem 实现会话创建、查找、加入
- **射击系统**：屏幕中心射线追踪、动态准星扩散、FOV 变焦瞄准、自动/半自动射击
- **武器系统**：7种武器类型、武器拾取装备、弹药管理、换弹系统
- **伤害系统**：HitScan/Projectile 两种伤害模式、范围伤害、命中特效与音效
- **手榴弹系统**：投掷手榴弹、范围爆炸伤害

### 武器类型
| 武器 | 类型 | 特性 |
|------|------|------|
| Assault Rifle | HitScan | 自动射击、高射速 |
| Pistol | HitScan | 半自动、精准 |
| Submachine Gun | HitScan | 自动射击、中射速 |
| Shotgun | HitScan | 散弹、高伤害 |
| Sniper Rifle | HitScan | 高精度、狙击镜 |
| Rocket Launcher | Projectile | 范围爆炸伤害 |
| Grenade Launcher | Projectile | 抛物线弹道、范围伤害 |

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
- **动画蒙太奇**：射击、换弹、消灭、投掷手榴弹

### 网络同步
- **属性复制**：条件复制优化带宽
- **RPC 调用链**：Server → NetMulticast 架构
- **网络频率**：66Hz 更新，33Hz 最低保障
- **客户端-服务器时间同步**：精准的倒计时和状态同步

### 游戏模式
- **热身阶段**：等待玩家就绪
- **比赛进行**：限时对战
- **冷却阶段**：显示胜者、准备新一轮

## 技术栈

| 类别 | 技术 |
|------|------|
| 引擎 | Unreal Engine 5.7.1 |
| 语言 | C++ / Blueprint |
| 网络 | Steam Online Subsystem |
| 输入 | Enhanced Input System |
| 插件 | MultiplayerSessions (自定义) |

## UE 5.7.1 / FPS 适配说明

本项目已完全适配 UE 5.7.1 和 FPS 游戏模式：

### FPS 相机设置
```cpp
// FPS Camera Setup - 相机直接附加到胶囊体（无 Spring Arm）
FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
FollowCamera->SetupAttachment(GetCapsuleComponent());
FollowCamera->SetRelativeLocation(FVector(0.f, 0.f, BaseEyeHeight));
FollowCamera->bUsePawnControlRotation = true;

// FPS 控制器旋转设置
bUseControllerRotationYaw = true;
bUseControllerRotationPitch = true;
GetCharacterMovement()->bOrientRotationToMovement = false;
```

### FPS 网格可见性
```cpp
// 隐藏本地玩家的身体网格
if (IsLocallyControlled())
{
    GetMesh()->SetOwnerNoSee(true);
}
```

### UE 5.7.1 API 更新
- **TObjectPtr**：使用 `TObjectPtr<>` 替代裸指针，提升垃圾回收安全性
- **Enhanced Input**：使用 UE5 的 Enhanced Input System 替代旧版输入系统
- **PlaySlotAnimationAsDynamicMontage**：替代已废弃的 `PlayAnimation` API
- **FVector_NetQuantize**：网络传输中使用量化向量优化带宽

## 项目结构

```
Source/Blaster/
├── Character/
│   ├── BlasterCharacter.cpp      # 主角色 - FPS相机、输入、Aim Offset、生命值
│   └── BlasterAnimInstance.cpp   # 动画实例 - IK、Lean、状态同步
├── BlasterComponents/
│   └── CombatComponent.cpp       # 战斗组件 - 射击、瞄准、准星、换弹、手榴弹
├── Weapon/
│   ├── Weapon.cpp                # 武器基类 - 状态管理、弹药
│   ├── HitScanWeapon.cpp         # 命中扫描武器 - 步枪、手枪、霰弹枪
│   ├── Shotgun.cpp               # 霰弹枪 - 多弹丸散射
│   ├── ProjectileWeapon.cpp      # 投射物武器 - 火箭、榴弹
│   ├── Projectile.cpp            # 投射物基类 - 碰撞、轨迹
│   ├── ProjectileBullet.cpp      # 子弹投射物
│   ├── ProjectileRocket.cpp      # 火箭投射物 - 范围爆炸
│   ├── ProjectileGrenade.cpp     # 手榴弹投射物 - 弹跳、延时爆炸
│   ├── RocketMovementComponent.cpp # 火箭运动组件
│   ├── Casing.cpp                # 弹壳物理模拟
│   └── WeaponTypes.h             # 武器类型枚举
├── HUD/
│   ├── BlasterHUD.cpp            # 准星渲染
│   ├── CharacterOverlay.cpp      # 角色 UI（血条、弹药、手榴弹）
│   ├── Announcement.cpp          # 游戏公告 UI
│   └── OverheadWidget.cpp        # 头顶 UI
├── GameMode/
│   ├── BlasterGameMode.cpp       # 主游戏模式 - 消灭处理、重生、比赛状态
│   └── LobbyGameMode.cpp         # 大厅模式
├── GameState/
│   └── BlasterGameState.cpp      # 游戏状态 - 最高分追踪
├── PlayerController/
│   └── BlasterPlayerController.cpp # HUD 更新、时间同步
├── PlayerState/
│   └── BlasterPlayerState.cpp    # 得分、失败统计
├── Interfaces/
│   └── InteractWithCrosshairsInterface.h  # 准星交互接口
└── BlasterTypes/
    ├── TurningInPlace.h          # 原地转身枚举
    └── CombatState.h             # 战斗状态枚举

Plugins/
└── MultiplayerSessions/          # 自定义多人会话插件
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

### 3. 武器系统

```cpp
// 武器类型枚举
UENUM(BlueprintType)
enum class EWeaponType : uint8
{
    EWT_AssaultRifle,
    EWT_RocketLauncher,
    EWT_Pistol,
    EWT_SubmachineGun,
    EWT_Shotgun,
    EWT_SniperRifle,
    EWT_GrenadeLauncher,
    EWT_MAX
};

// 战斗状态
enum class ECombatState : uint8
{
    ECS_Unoccupied,      // 可以战斗
    ECS_Reloading,       // 换弹中
    ECS_ThrowingGrenade, // 投掷手榴弹中
    ECS_SwappingWeapons, // 交换武器中
    ECS_MAX
};
```

### 4. 伤害系统

```cpp
// HitScan 伤害 - 即时命中
UGameplayStatics::ApplyDamage(
    HitCharacter,
    Damage,
    InstigatorController,
    this,
    UDamageType::StaticClass()
);

// 范围爆炸伤害 - 火箭/手榴弹
UGameplayStatics::ApplyRadialDamageWithFalloff(
    this,
    Damage,          // 基础伤害
    10.f,            // 最小伤害
    GetActorLocation(),
    DamageInnerRadius,
    DamageOuterRadius,
    1.f,             // 伤害衰减
    UDamageType::StaticClass(),
    TArray<AActor*>(),
    this,
    FiringController
);
```

### 5. 手榴弹系统

```cpp
void UCombatComponent::ThrowGrenade()
{
    if (Grenades == 0) return;
    CombatState = ECombatState::ECS_ThrowingGrenade;
    Character->PlayThrowGrenadeMontage();
    AttachActorToLeftHand(EquippedWeapon);
    ShowAttachedGrenade(true);
}

void UCombatComponent::LaunchGrenade()
{
    ShowAttachedGrenade(false);
    ServerLaunchGrenade(HitTarget);
}
```

## 操作说明

| 按键 | 功能 |
|------|------|
| WASD | 移动 |
| 鼠标移动 | 视角控制 |
| 鼠标左键 | 射击 |
| 鼠标右键 | 瞄准 |
| E | 拾取/装备武器 |
| R | 换弹 |
| G | 投掷手榴弹 |
| C | 下蹲 |
| Space | 跳跃 |

## 构建与运行

### 环境要求
- Unreal Engine 5.7.1
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

- [x] 基础角色移动与 FPS 相机
- [x] Enhanced Input 输入系统
- [x] 武器拾取与装备
- [x] 射击系统（HitScan/Projectile）
- [x] 动态准星系统
- [x] Aim Offset 与 Turn In Place
- [x] 左手 IK 与身体倾斜
- [x] 多人网络同步 (Steam)
- [x] 生命值与伤害系统
- [x] 消灭动画与溶解特效
- [x] 重生系统
- [x] 得分与失败统计
- [x] 自动/半自动射击
- [x] 武器弹药与换弹系统
- [x] 多种武器类型（7种）
- [x] 手榴弹投掷系统
- [x] 比赛状态管理（热身/进行/冷却）
- [x] 狙击镜 UI
- [ ] 武器切换系统
- [ ] 计分板 UI
- [ ] 更多游戏模式

## 开发者

- **hojiahao**

## License

MIT License
