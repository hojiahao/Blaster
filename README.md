# Blaster - 多人联网射击游戏

基于 **Unreal Engine 5.7.1** 开发的多人在线第一人称射击游戏（FPS），实现了完整的网络同步架构、服务器端回溯（Server-Side Rewind）延迟补偿系统、多种武器系统、战斗系统、生命值与护盾系统、拾取物系统、Buff 系统、消灭重生机制、得分统计、消灭广播、高延迟检测、领先者皇冠特效、返回主菜单、团队对抗（红蓝队）、夺旗模式（Capture the Flag），以及动画/UI 系统。

## 项目特性

### 核心玩法

- **多人联机**：基于 Steam Online Subsystem 实现会话创建、查找、加入，自定义 MultiplayerSessions 插件封装完整的会话管理流程
- **大厅系统**：LobbyGameMode 支持 Seamless Travel，2 名玩家就绪后自动跳转至对战地图
- **射击系统**：屏幕中心射线追踪、动态准星扩散、FOV 变焦瞄准、自动/半自动射击
- **武器系统**：7 种武器类型、武器拾取装备、弹药管理、换弹系统、双武器切换动画
- **伤害系统**：HitScan / Projectile / Shotgun 三种伤害模式、范围伤害、命中特效与音效
- **爆头系统**：头部命中造成额外伤害（HeadShotDamage），Server-Side Rewind 优先检测头部碰撞盒
- **延迟补偿**：Server-Side Rewind 系统，支持高延迟玩家的公平游戏体验
- **作弊验证**：服务器端射击验证（`ServerFire_Validate`）、射速验证、命中确认
- **手榴弹系统**：投掷手榴弹、范围爆炸伤害（`ApplyRadialDamageWithFalloff`）
- **拾取物系统**：血包、护盾、速度加成、跳跃加成、弹药补给
- **Buff 系统**：持续回血（HealRampUp）、持续回盾（ShieldRampUp）、速度加成、跳跃加成，Buff 效果通过 `NetMulticast` 同步到所有客户端
- **消灭广播**：击杀事件通过 `ClientElimAnnouncement` RPC 广播给所有玩家，支持 "You elimmed X" / "X elimmed you" 等个性化显示
- **领先者皇冠**：得分最高的玩家头顶显示 Niagara 皇冠特效（`MulticastGainedTheLead` / `MulticastLostTheLead`）
- **高延迟检测**：周期性检测 Ping，超过阈值时显示高延迟警告动画，并通过 `ServerReportPingStatus` 自动禁用 Server-Side Rewind
- **返回主菜单**：游戏内按 ESC 弹出返回主菜单 UI，通过 `MultiplayerSessionsSubsystem` 销毁会话后退出
- **团队模式**：红队 vs 蓝队对抗，自动队伍分配与平衡、禁止友方伤害、团队计分、队伍颜色（材质 + 溶解特效）、HUD 实时队伍分数显示
- **夺旗模式（CTF）**：`CaptureTheFlagGameMode` 继承自 `TeamsGameMode`，旗帜（`Flag`）拾取/掉落/归还、`FlagZone` 区域触发得分、`TeamPlayerStart` 队伍专属出生点

### 武器类型

| 武器               | 类型         | 特性                                        |
| ---------------- | ---------- | ----------------------------------------- |
| Assault Rifle    | HitScan    | 自动射击、高射速                                  |
| Pistol           | HitScan    | 半自动、精准                                    |
| Submachine Gun   | HitScan    | 自动射击、中射速、strap 物理模拟                       |
| Shotgun          | HitScan    | 散弹（10 发弹丸）、高伤害、头部/身体分别计数                  |
| Sniper Rifle     | HitScan    | 高精度、狙击镜 UI                                |
| Rocket Launcher  | Projectile | 范围爆炸伤害、自定义 RocketMovementComponent、飞行音效循环 |
| Grenade Launcher | Projectile | 抛物线弹道、弹跳音效、延时爆炸                           |

### 角色系统

- **生命值系统**：100 点生命值，受伤时 HUD 实时更新
- **护盾系统**：100 点最大护盾值，优先吸收伤害
- **Buff 系统**：速度加成（1600 基础 / 850 蹲伏）、跳跃加成（4000 Z 速度）、持续回血、持续回盾
- **消灭机制**：死亡动画、溶解材质特效（Timeline + DynamicMaterialInstance）、消灭粒子效果
- **重生系统**：自动随机 PlayerStart 重生、默认武器生成
- **得分统计**：击杀得分、死亡次数追踪、最高分玩家追踪
- **碰撞盒系统**：18 个身体部位碰撞盒用于精确命中检测
- **玩家退出处理**：`ServerLeaveGame` RPC → GameMode 移除玩家 → 通过 `OnLeftGame` 委托触发会话销毁

### 碰撞盒布局（Server-Side Rewind）

| 部位  | 碰撞盒名称                                          |
| --- | ---------------------------------------------- |
| 头部  | head                                           |
| 躯干  | pelvis, spine_02, spine_03                     |
| 手臂  | upperarm_l, upperarm_r, lowerarm_l, lowerarm_r |
| 手部  | hand_l, hand_r                                 |
| 背包  | backpack, blanket                              |
| 腿部  | thigh_l, thigh_r, calf_l, calf_r               |
| 脚部  | foot_l, foot_r                                 |

### 拾取物系统

| 拾取物           | 效果    | 说明                    |
| ------------- | ----- | --------------------- |
| Health Pickup | 恢复生命值 | 5 秒内恢复 100 点生命值       |
| Shield Pickup | 恢复护盾  | 5 秒内恢复 100 点护盾值       |
| Speed Pickup  | 速度加成  | 30 秒内移动速度提升至 1600/850 |
| Jump Pickup   | 跳跃加成  | 30 秒内跳跃 Z 速度提升至 4000  |
| Ammo Pickup   | 弹药补给  | 根据武器类型补充 30 发弹药       |

拾取物通过 `PickupSpawnPoint` 随机生成，拾取后通过定时器在 `[SpawnPickupTimeMin, SpawnPickupTimeMax]` 范围内随机重新生成。

### 动画系统

- **Aim Offset**：水平/垂直瞄准偏移（Pitch 值跨网络修正 270°→-90° 映射）
- **Turn In Place**：原地转身动画（用于其他玩家的第三人称代理表现）
- **左手 IK（FABRIK）**：武器握持位置自适应，根据 CombatState 动态启用/禁用
- **身体倾斜（Lean）**：移动时基于 Yaw 变化率计算的身体倾斜效果
- **右手旋转修正**：本地控制玩家的右手朝向命中目标方向
- **动画蒙太奇**：射击（RifleAim / RifleHip 分支）、换弹（7 种武器各自 Section）、消灭、投掷手榴弹、武器切换

### 网络同步

- **属性复制**：条件复制优化带宽（`COND_OwnerOnly`）
- **RPC 调用链**：`Server, Reliable` → `NetMulticast, Reliable` 架构
- **RPC 验证**：`ServerFire_Validate` / `ServerShotgunFire_Validate` 验证射速合法性
- **网络频率**：66Hz 更新，33Hz 最低保障
- **客户端-服务器时间同步**：`ServerRequestServerTime` / `ClientReportServerTime` 往返时间计算
- **Server-Side Rewind（延迟补偿）**：4 秒帧历史记录、命中回溯验证
- **本地预测**：客户端本地射击预测（`LocalFire`），服务器 Multicast 跳过已预测的本地客户端

### Server-Side Rewind 系统

| 功能            | 说明                                                              |
| ------------- | --------------------------------------------------------------- |
| 帧历史记录         | 服务器每帧记录所有角色 18 个碰撞盒位置（`TDoubleLinkedList<FFramePackage>`）       |
| HitScan 回溯    | `ConfirmHit`：先检测头部碰撞盒，未命中再检测全部碰撞盒                               |
| Projectile 回溯 | `ProjectileConfirmHit`：使用 `PredictProjectilePath` 进行轨迹预测验证      |
| Shotgun 回溯    | `ShotgunConfirmHit`：对每个弹丸分别检测头部和身体，汇总伤害                         |
| 帧插值           | `InterpBetweenFrames`：在两个历史帧之间使用 `VInterpTo` / `RInterpTo` 精确插值 |
| 爆头检测          | 优先检测 head 碰撞盒，区分 `bHeadShot` 返回不同伤害值                            |
| 高延迟处理         | Ping 超过阈值时通过 `HighPingDelegate` 自动禁用 SSR                        |

### ProjectileWeapon SSR 分支逻辑

```cpp
Server + LocallyControlled (Host)  → 使用 Replicated Projectile，SSR=false
Server + Not LocallyControlled     → 使用 Non-Replicated Projectile，SSR=true
Client + LocallyControlled         → 使用 Non-Replicated Projectile，SSR=true，记录 TraceStart/InitialVelocity
Client + Not LocallyControlled     → 使用 Non-Replicated Projectile，SSR=false
Not Using SSR + Server Authority   → 使用 Replicated Projectile，SSR=false
```

### 作弊验证

- **射速验证**：`ServerFire_Validate` 验证 FireDelay 偏差 < 0.001f
- **命中验证**：服务器端回溯验证命中的有效性
- **伤害计算**：所有伤害计算仅在服务器执行（`HasAuthority()` 检查）

### 地图

| 地图             | 说明                   |
| -------------- | -------------------- |
| GameStartupMap | 游戏启动地图（主菜单入口）        |
| Lobby          | 大厅等待地图，玩家在此创建/加入会话   |
| TransitionMap  | Seamless Travel 过渡地图 |
| BlasterMap     | 主对战地图                |

### 游戏模式

- **LobbyGameMode**：等待 2 名玩家加入后通过 `bUseSeamlessTravel` 无缝跳转至 BlasterMap
- **BlasterGameMode**：
  - **热身阶段**（10s）：等待玩家就绪，显示 Announcement UI
  - **比赛进行**（120s）：限时对战，实时倒计时
  - **冷却阶段**（10s）：显示胜者信息、准备新一轮
  - **自定义 MatchState**：新增 `Cooldown` 状态，扩展引擎默认状态机
- **TeamsGameMode**（继承 BlasterGameMode）：
  - **自动队伍分配**：`PostLogin` 时根据队伍人数自动分配玩家至红队或蓝队，保持队伍平衡
  - **友方伤害禁止**：`CalculateDamage` 重写，同队伤害返回 0
  - **团队计分**：击杀敌方玩家时通过 `BlasterGameState::RedTeamScores()` / `BlueTeamScores()` 更新队伍总分
  - **玩家退出处理**：`Logout` 时自动从对应队伍数组中移除玩家
  - **比赛开始同步**：`HandleMatchHasStarted` 确保所有玩家在比赛开始时分配到队伍
- **CaptureTheFlagGameMode**（继承 TeamsGameMode）：
  - **旗帜物体**：`Flag` 继承自 `AWeapon`，支持拾取、掉落、重置至初始位置
  - **夺旗区域**：`FlagZone` 通过 `OnComponentBeginOverlap` 检测旗帜进入，触发 `FlagCaptured` 得分
  - **队伍出生点**：`TeamPlayerStart` 根据队伍标签分配专属出生位置
- **消灭处理**：`PlayerEliminated` → 更新得分 → 更新最高分 → 广播消灭通知 → 触发 Elim
- **重生机制**：随机选择 `PlayerStart` 位置，调用 `RestartPlayerAtPlayerStart`

### 团队系统

#### 队伍枚举（ETeam）

```cpp
UENUM(BlueprintType)
enum class ETeam : uint8
{
    ET_RedTeam,   // 红队
    ET_BlueTeam,  // 蓝队
    ET_NoTeam,    // 未分配
    ET_MAX
};
```

#### 队伍分配与同步

- **PlayerState**：`ETeam Team` 属性通过 `ReplicatedUsing = OnRep_Team` 同步到所有客户端
- **GameState**：维护 `TArray<ABlasterPlayerState*> RedTeam` / `BlueTeam` 队伍数组
- **队伍分数**：`RedTeamScore` / `BlueTeamScore` 通过 `OnRep` 回调实时更新 HUD

#### 队伍颜色

- **角色材质**：`SetTeamColor(ETeam)` 根据队伍切换角色材质（`RedMaterial` / `BlueMaterial`）
- **溶解特效**：死亡时使用对应队伍颜色的溶解材质实例（`RedDissolveMatInst` / `BlueDissolveMatInst`）

#### 队伍 HUD

- **CharacterOverlay**：显示 `RedTeamScore` / `BlueTeamScore` 文本
- **PlayerController**：`InitTeamScores()` / `HideTeamScores()` 根据游戏模式动态切换队伍分数 UI 的显示

### HUD/UI 系统

| 组件               | 说明                                                 |
| ---------------- | -------------------------------------------------- |
| CharacterOverlay | 血条、护盾条、弹药（武器/备弹）、手榴弹数、击杀/死亡数、倒计时、高延迟警告、红蓝队分数       |
| BlasterHUD       | 5 片式动态准星渲染（Center/Left/Right/Top/Bottom）、准星扩散与颜色变化 |
| Announcement     | 比赛状态公告（热身倒计时、冷却阶段胜者信息）                             |
| ElimAnnouncement | 击杀广播通知，支持滚动叠加显示、定时自动移除                             |
| OverheadWidget   | 头顶 3D UI（网络角色名/调试信息），`SetOwnerNoSee(true)`         |
| ReturnToMainMenu | ESC 菜单，销毁 Session 后返回主菜单                           |
| Sniper Scope     | 狙击镜 UI（BlueprintImplementableEvent）                |

### 自定义碰撞通道

```cpp
// Blaster.h
#define ECC_SkeletalMesh ECollisionChannel::ECC_GameTraceChannel1  // 角色骨骼网格碰撞
#define ECC_HitBox       ECollisionChannel::ECC_GameTraceChannel2  // SSR 碰撞盒验证
```

### 武器自定义深度高亮

| 状态  | Custom Depth Stencil Value  |
| --- | --------------------------- |
| 可拾取 | `CUSTOM_DEPTH_BLUE` (251)   |
| 已装备 | 禁用 Custom Depth             |
| 副武器 | `CUSTOM_DEPTH_TAN` (252)    |
| 拾取物 | `CUSTOM_DEPTH_PURPLE` (250) |

## 技术栈

| 类别  | 技术                                                          |
| --- | ----------------------------------------------------------- |
| 引擎  | Unreal Engine 5.7.1                                         |
| 语言  | C++ / Blueprint                                             |
| 网络  | Steam Online Subsystem                                      |
| 输入  | Enhanced Input System（IMC + InputAction）                    |
| 插件  | MultiplayerSessions（自定义 GameInstanceSubsystem）              |
| 渲染  | DX12 / SM6、Ray Tracing、Substrate、Virtual Shadow Maps、Nanite |
| VFX | Niagara（皇冠特效、拾取物特效）、Cascade（枪口火焰、弹道光束、冲击粒子）                 |
| 音频  | Sound Cue（武器音效、命中音效、弹壳音效、拾取音效、火箭飞行循环音效）                     |

## UE 5.7.1 / FPS 适配说明

### FPS 相机设置

```cpp
// 相机直接附加到胶囊体（无 Spring Arm）
FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
FollowCamera->SetupAttachment(GetCapsuleComponent());
FollowCamera->SetRelativeLocation(FVector(0.f, 0.f, BaseEyeHeight));
FollowCamera->bUsePawnControlRotation = true;

// FPS 控制器旋转设置
bUseControllerRotationYaw = true;
bUseControllerRotationPitch = false;
GetCharacterMovement()->bOrientRotationToMovement = false;
```

### FPS 网格可见性

```cpp
GetMesh()->SetOwnerNoSee(true);          // 隐藏本地玩家身体
OverheadWidget->SetOwnerNoSee(true);     // 隐藏本地玩家头顶 UI
AttachedGrenade->SetOwnerNoSee(true);    // 隐藏本地玩家手雷模型
```

### UE 5.7.1 API 适配

- **TObjectPtr**：全部 UPROPERTY 指针使用 `TObjectPtr<>` 替代裸指针
- **Enhanced Input**：使用 `UEnhancedInputComponent::BindAction` 替代旧版 `BindAction`（字符串绑定）
- **PlaySlotAnimationAsDynamicMontage**：替代已废弃的 `PlayAnimation` API
- **FVector_NetQuantize / FVector_NetQuantize100**：网络传输中使用量化向量优化带宽
- **SM6 Shader**：DX12 + SM6 / Vulkan SM6 / Metal SM6
- **Substrate 材质**：启用 Substrate 材质系统
- **Virtual Shadow Maps**：启用虚拟阴影贴图
- **Custom Depth 3 层**：支持多层 Custom Depth Stencil

## MultiplayerSessions 插件

自定义 `UGameInstanceSubsystem` 封装 Steam Online Subsystem 会话管理：

| 功能   | 方法                                               |
| ---- | ------------------------------------------------ |
| 创建会话 | `CreateSession(NumPublicConnections, MatchType)` |
| 查找会话 | `FindSessions(MaxSearchResults)`                 |
| 加入会话 | `JoinSession(SessionResult)`                     |
| 销毁会话 | `DestroySession()`                               |
| 启动会话 | `StartSession()`                                 |

自定义委托暴露给 UI 层（Menu / ReturnToMainMenu）绑定回调：

- `FMultiplayerOnCreateSessionComplete`
- `FMultiplayerOnFindSessionsComplete`
- `FMultiplayerOnJoinSessionComplete`
- `FMultiplayerOnDestroySessionComplete`

## 项目结构

```cpp
Source/Blaster/
├── Character/
│   ├── BlasterCharacter.h/.cpp      # 主角色 - FPS 相机、Enhanced Input、Aim Offset、18 个碰撞盒、溶解特效、皇冠特效
│   └── BlasterAnimInstance.h/.cpp   # 动画实例 - FABRIK IK、Lean、右手旋转、状态同步
├── BlasterComponents/
│   ├── CombatComponent.h/.cpp       # 战斗组件 - 射击、瞄准、准星、换弹、手榴弹、武器切换、FOV 插值
│   ├── BuffComponent.h/.cpp         # Buff 组件 - 速度/跳跃加成（NetMulticast 同步）、持续回血/回盾
│   └── LagCompensationComponent.h/.cpp # 延迟补偿 - SSR 帧历史、HitScan/Projectile/Shotgun 回溯验证
├── Weapon/
│   ├── Weapon.h/.cpp                # 武器基类 - 状态管理、弹药、Custom Depth、散射、SSR 标记
│   ├── WeaponTypes.h                # 武器类型枚举（7 种）、TRACE_LENGTH、Custom Depth 常量
│   ├── HitScanWeapon.h/.cpp         # 即时命中武器 - 射线追踪、光束粒子、爆头检测、SSR 请求
│   ├── Shotgun.h/.cpp               # 霰弹枪 - 10 弹丸散射、头部/身体分别计数伤害
│   ├── ProjectileWeapon.h/.cpp      # 投射物武器 - SSR 分支逻辑（4 种情况）
│   ├── Projectile.h/.cpp            # 投射物基类 - 碰撞、Niagara 轨迹、定时销毁、范围爆炸
│   ├── ProjectileBullet.h/.cpp      # 子弹 - ProjectileMovementComponent、爆头检测、SSR
│   ├── ProjectileRocket.h/.cpp      # 火箭 - 自定义运动组件、飞行循环音效、爆炸范围伤害
│   ├── ProjectileGrenade.h/.cpp     # 手雷 - 弹跳（OnProjectileBounce）、延时爆炸
│   ├── RocketMovementComponent.h/.cpp # 火箭运动 - 忽略阻挡命中、仅由 CollisionBox 触发爆炸
│   ├── Casing.h/.cpp                # 弹壳 - 物理模拟、碰撞音效、自动销毁
│   └── Flag.h/.cpp                  # 旗帜 - CTF 模式旗帜物体、拾取/掉落/重置
├── Pickups/
│   ├── Pickup.h/.cpp                # 拾取物基类 - 旋转动画、Niagara 特效、延迟绑定 Overlap
│   ├── HealthPickup.h/.cpp          # 血包 - 调用 BuffComponent::Heal
│   ├── ShieldPickup.h/.cpp          # 护盾 - 调用 BuffComponent::ReplenishShield
│   ├── SpeedPickup.h/.cpp           # 速度加成 - 调用 BuffComponent::BuffSpeed
│   ├── JumpPickup.h/.cpp            # 跳跃加成 - 调用 BuffComponent::BuffJump
│   ├── AmmoPickup.h/.cpp            # 弹药补给 - 调用 CombatComponent::PickupAmmo
│   └── PickupSpawnPoint.h/.cpp      # 生成点 - 随机类型、定时器重生、OnDestroyed 回调
├── HUD/
│   ├── BlasterHUD.h/.cpp            # 准星渲染（5 片 Texture + 动态 Spread + 颜色变化）
│   ├── CharacterOverlay.h/.cpp      # 角色 HUD（血条、护盾、弹药、手榴弹、倒计时、高延迟、队伍分数）
│   ├── Announcement.h/.cpp          # 比赛公告 UI（热身/冷却/胜者信息）
│   ├── ElimAnnouncement.h/.cpp      # 击杀广播（滚动显示、2.5s 自动移除）
│   ├── OverheadWidget.h/.cpp        # 头顶 3D UI（网络角色调试信息）
│   └── ReturnToMainMenu.h/.cpp      # 返回主菜单（销毁 Session、Seamless 退出）
├── GameMode/
│   ├── BlasterGameMode.h/.cpp       # 主模式 - 消灭/重生/比赛状态机（WaitingToStart→InProgress→Cooldown）
│   ├── TeamsGameMode.h/.cpp         # 团队模式 - 自动队伍分配、友伤禁止、团队计分
│   ├── CaptureTheFlagGameMode.h/.cpp # 夺旗模式 - 旗帜夺取得分、继承 TeamsGameMode
│   └── LobbyGameMode.h/.cpp         # 大厅模式 - 2 人就绪后 Seamless Travel
├── GameState/
│   └── BlasterGameState.h/.cpp      # 游戏状态 - TopScoringPlayers 追踪与更新、红蓝队伍数组与分数
├── PlayerController/
│   └── BlasterPlayerController.h/.cpp # HUD 更新、时间同步、Ping 检测、消灭广播、队伍分数 UI、返回主菜单
├── PlayerState/
│   └── BlasterPlayerState.h/.cpp    # 得分/失败统计、队伍分配、OnRep 回调更新 HUD
├── CaptureTheFlag/
│   └── FlagZone.h/.cpp              # 夺旗区域触发（旗帜进入区域时计分）
├── PlayerStart/
│   └── TeamPlayerStart.h/.cpp       # 队伍专属出生点
├── Interfaces/
│   └── InteractWithCrosshairsInterface.h # 准星交互接口（瞄准敌人时准星变红）
├── BlasterTypes/
│   ├── Team.h                       # 队伍枚举（RedTeam/BlueTeam/NoTeam）
│   ├── TurningInPlace.h             # 原地转身枚举（Left/Right/NotTurning）
│   └── CombatState.h                # 战斗状态枚举（Unoccupied/Reloading/ThrowingGrenade/SwappingWeapons）
├── Blaster.h                        # 自定义碰撞通道定义（ECC_SkeletalMesh、ECC_HitBox）
├── Blaster.cpp                      # 模块初始化
└── Blaster.Build.cs                 # 构建配置（EnhancedInput、Niagara、OnlineSubsystem、Slate）

Plugins/
└── MultiplayerSessions/             # 自定义多人会话插件
    └── Source/MultiplayerSessions/
        ├── MultiplayerSessionsSubsystem.h/.cpp  # 会话管理 Subsystem
        └── Menu.h/.cpp                           # 主菜单 UI（Host/Join 按钮）
```

## 核心功能实现

### 1. 网络同步架构

```cpp
// 射击 RPC 调用链：客户端 → 服务器（验证） → 所有客户端
UFUNCTION(Server, Reliable, WithValidation)
void ServerFire(const FVector_NetQuantize& TraceHitTarget, float FireDelay);

UFUNCTION(NetMulticast, Reliable)
void MulticastFire(const FVector_NetQuantize& TraceHitTarget);

// Multicast 跳过已本地预测的客户端
void UCombatComponent::MulticastFire_Implementation(const FVector_NetQuantize& TraceHitTarget)
{
    if (Character && Character->IsLocallyControlled() && !Character->HasAuthority()) return;
    LocalFire(TraceHitTarget);
}
```

### 2. 动态准星系统

```cpp
// 准星扩散 = 基础值 + 速度因子 + 空中因子 - 瞄准因子 + 射击因子
HUDPackage.CrosshairSpread = 0.5f
    + CrosshairVelocityFactor   // [0, 1] 基于 MaxWalkSpeed
    + CrosshairInAirFactor      // 空中时 2.25，着地后快速恢复
    - CrosshairAimFactor        // 瞄准时 0.58
    + CrosshairShootingFactor;  // 射击时 0.75，快速衰减

// 准星颜色：瞄准实现 IInteractWithCrosshairsInterface 的 Actor 时变红
if (TraceHitResult.GetActor()->Implements<UInteractWithCrosshairsInterface>())
    HUDPackage.CrosshairsColor = FLinearColor::Red;
```

### 3. 霰弹枪伤害计算

```cpp
// 每个弹丸独立检测命中，分别统计头部和身体命中次数
TMap<ABlasterCharacter*, uint32> HitMap;       // 身体命中计数
TMap<ABlasterCharacter*, uint32> HeadShotHitMap; // 头部命中计数

// 最终伤害 = 身体命中数 × Damage + 头部命中数 × HeadShotDamage
for (auto DamagePair : DamageMap)
{
    UGameplayStatics::ApplyDamage(DamagePair.Key, DamagePair.Value, ...);
}
```

### 4. 护盾与伤害计算

```cpp
void ABlasterCharacter::ReceiveDamage(...)
{
    float DamageToHealth = Damage;
    if (Shield > 0.f)
    {
        if (Shield >= Damage)
        {
            Shield -= Damage;      // 护盾完全吸收
            DamageToHealth = 0.f;
        }
        else
        {
            DamageToHealth -= Shield; // 护盾部分吸收，剩余伤害作用于生命值
            Shield = 0.f;
        }
    }
    Health = FMath::Clamp(Health - DamageToHealth, 0.f, MaxHealth);
}
```

### 5. 手榴弹系统

```cpp
// 投掷流程：CombatState → ThrowingGrenade → 播放蒙太奇 → 武器切至左手 → 显示手雷模型
void UCombatComponent::ThrowGrenade();

// 发射流程：隐藏手雷模型 → ServerLaunchGrenade RPC → 服务器 SpawnActor<AProjectile>
void UCombatComponent::LaunchGrenade();

// 手雷弹跳 → 延时销毁 → Destroyed() 中调用 ExplodeDamage()
void AProjectileGrenade::Destroyed() { ExplodeDamage(); Super::Destroyed(); }
```

### 6. 消灭广播系统

```cpp
// 服务器遍历所有 PlayerController 广播击杀信息
for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
{
    BlasterPlayer->BroadcastElim(AttackerPlayerState, VictimPlayerState);
}

// 客户端接收后显示个性化消息
void ABlasterPlayerController::ClientElimAnnouncement_Implementation(APlayerState* Attacker, APlayerState* Victim)
{
    if (Attacker == Self && Victim != Self)
        BlasterHUD->AddElimAnnouncement("You", Victim->GetPlayerName());
    // ...
}
```

### 7. Server-Side Rewind 帧历史管理

```cpp
// 每帧保存帧包（服务器端）
void ULagCompensationComponent::SaveFramePackage()
{
    // 移除超过 MaxRecordTime (4秒) 的旧帧
    while (HistoryLength > MaxRecordTime)
        FrameHistory.RemoveNode(FrameHistory.GetTail());
    // 添加新帧到链表头部
    FrameHistory.AddHead(ThisFrame);
}

// 查找目标帧（在链表中二分搜索 HitTime 对应的两个相邻帧）
FFramePackage GetFrameToCheck(ABlasterCharacter* HitCharacter, float HitTime)
{
    // 从 Head（最新）向 Tail（最旧）遍历
    // 找到 OlderTime < HitTime < YoungerTime 的区间
    // 使用 InterpBetweenFrames 插值
}
```

### 8. 客户端-服务器时间同步

```cpp
// 客户端发送本地时间 → 服务器记录接收时间并回传 → 客户端计算 RTT 和时间差
void ClientReportServerTime_Implementation(float TimeOfClientRequest, float TimeServerReceivedClientRequest)
{
    float RoundTripTime = GetWorld()->GetTimeSeconds() - TimeOfClientRequest;
    SingleTripTime = 0.5f * RoundTripTime;
    ClientServerDelta = (TimeServerReceivedClientRequest + SingleTripTime) - GetWorld()->GetTimeSeconds();
}
// 每 5 秒同步一次（TimeSyncFrequency）
```

## 操作说明

| 按键    | 功能           |
| ----- | ------------ |
| WASD  | 移动           |
| 鼠标移动  | 视角控制         |
| 鼠标左键  | 射击           |
| 鼠标右键  | 瞄准（FOV 缩放）   |
| E     | 拾取/装备武器/切换武器 |
| R     | 换弹           |
| G     | 投掷手榴弹        |
| C     | 下蹲/站起（切换）    |
| Space | 跳跃           |
| ESC   | 返回主菜单        |

## 构建与运行

### 环境要求

- Unreal Engine 5.7.1
- Visual Studio 2022 / Rider
- Steam SDK（用于多人联机）

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

### 核心系统

- [x] 基础角色移动与 FPS 相机
- [x] Enhanced Input 输入系统
- [x] 武器拾取与装备
- [x] 射击系统（HitScan / Projectile / Shotgun）
- [x] 动态准星系统
- [x] Aim Offset 与 Turn In Place
- [x] 左手 IK（FABRIK）与身体倾斜
- [x] 多人网络同步（Steam）
- [x] 生命值与伤害系统
- [x] 护盾系统
- [x] 消灭动画与溶解特效
- [x] 重生系统与默认武器
- [x] 得分与失败统计
- [x] 自动/半自动射击
- [x] 武器弹药与换弹系统
- [x] 多种武器类型（7 种）
- [x] 手榴弹投掷系统
- [x] 比赛状态管理（热身/进行/冷却）
- [x] 狙击镜 UI
- [x] 武器切换系统（双武器）
- [x] 武器切换动画
- [x] 拾取物系统（血包、护盾、速度、跳跃、弹药）
- [x] Buff 系统（速度/跳跃/回血/回盾）
- [x] 爆头伤害加成（HeadShotDamage）
- [x] 消灭广播系统（ElimAnnouncement）
- [x] 领先者皇冠特效（Niagara Crown）
- [x] 高延迟检测与警告 UI
- [x] 返回主菜单系统
- [x] 玩家退出处理（ServerLeaveGame）
- [x] 武器自定义深度高亮

### Server-Side Rewind（延迟补偿）

- [x] 帧历史记录系统（TDoubleLinkedList）
- [x] 角色碰撞盒系统（18 个身体部位）
- [x] HitScan 服务器回溯验证
- [x] Projectile 服务器回溯验证
- [x] Shotgun 服务器回溯验证
- [x] 帧间插值算法
- [x] 爆头/身体命中区分
- [x] 高延迟自动禁用 SSR

### 作弊验证

- [x] 射速验证（ServerFire_Validate）
- [x] 命中验证（Server Score Request）
- [x] 服务器端伤害计算

### 网络系统

- [x] MultiplayerSessions 插件（Create/Find/Join/Destroy Session）
- [x] 大厅系统（LobbyGameMode + Seamless Travel）
- [x] 客户端-服务器时间同步
- [x] 本地射击预测（LocalFire）
- [x] 高延迟广播（ServerReportPingStatus）

### 团队系统

- [x] 队伍枚举（ETeam：RedTeam / BlueTeam / NoTeam）
- [x] 团队游戏模式（TeamsGameMode）
- [x] 自动队伍分配与平衡（PostLogin / HandleMatchHasStarted）
- [x] 友方伤害禁止（CalculateDamage 重写）
- [x] 团队计分系统（RedTeamScore / BlueTeamScore）
- [x] 队伍分数网络同步（OnRep_RedTeamScore / OnRep_BlueTeamScore）
- [x] 队伍颜色材质（角色材质 + 溶解特效）
- [x] 队伍分数 HUD 显示与动态切换
- [x] 玩家退出队伍清理（Logout）

### 夺旗模式（Capture the Flag）

- [x] 夺旗游戏模式（CaptureTheFlagGameMode，继承 TeamsGameMode）
- [x] 旗帜物体（Flag，继承 AWeapon，拾取/掉落/重置）
- [x] 夺旗区域触发（FlagZone，旗帜进入区域时计分）
- [x] 队伍专属出生点（TeamPlayerStart）
- [x] 旗帜持有状态同步（bHoldingTheFlag，动画层联动）

### 得分展示系统

- [x] 个人得分/死亡数实时 HUD 显示（CharacterOverlay：ScoreAmount / DefeatsAmount）
- [x] 红蓝队伍分数实时 HUD 显示（CharacterOverlay：RedTeamScore / BlueTeamScore）
- [x] 赛后最高分玩家/队伍胜者公告（Announcement：InfoText，GetInfoText / GetTeamsInfoText）
- [x] 击杀广播通知（ElimAnnouncement：滚动显示、2.5s 自动移除）

## 开发者

- **hojiahao**

## License

MIT License
