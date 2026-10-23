// CPlayer::~CPlayer() -- player destructor: clears the cached manager pointers, releases the player def, destroys the player-mode list, deletes the 66 input-process scoped pointers (reverse order), releases the hero log book counted pointer and the two creature intelligent pointers, then tail-calls ~CInitBaseClass
// Leaf: ??1CPlayer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00488020, length 1200 bytes, tail `jmp` to the base destructor.
// Label: the catalog row says CTCFireheartMinigame::~CTCFireheartMinigame (propagated). The body stores vtable 0x01231CC4,
// which rtti-vtables.tsv names CPlayer (slot 0 = 0x0044BE30, the scalar deleting dtor, the only caller of this function).
// Callees: list<EPlayerMode> destructor @ 0x0044B970 (clear + free the sentinel), CBaseIntelligentPointer::~CBaseIntelligentPointer
// @ 0x00A01C10, operator delete @ 0x00BFE9BC (counted block), CInitBaseClass::~CInitBaseClass @ 0x0099A410; each input process is
// deleted through its virtual scalar deleting destructor (slot 0, argument 1); the def is released through Delete (slot 1).
// Layout: the landed CPlayer ctor 0x0044BC10 model (Ego_r.pdb names): retail STLport list is 4 bytes (PDB 12), so PlayerModes sits at
// +0x210 and PlayerDef at +0x21c.
// Shape: the user body clears five cached pointers (the store order 0xc, 0x10, 0x14, 0x1c, 0x18 is source order); member dtors follow in
// reverse declaration order, all inline except the list and the intelligent-pointer base. edi holds 0 for the whole function.
void operator delete(void* p);                  // 0x00BFE9BC

struct CInitBaseClass {
    virtual ~CInitBaseClass();                  // 0x0099A410
    bool m_flag04;                              // +0x04
};
struct C3DVector { float X, Y, Z; };
struct CRightHandedSet { C3DVector Up, Forward; };
struct CCamera {                                // 0x38 bytes
    C3DVector WorldPosition;
    CRightHandedSet RHSet;
    bool HeightLocked;
    bool ViewVecZLocked;
    unsigned long FOVFlags;
    float HorizontalFOV;
    float VerticalFOV;
    float Zoom;
};
struct CThingRespawnInfo {                      // 0x10 bytes
    C3DVector DiedPosition;
    bool DeadInfoSet;
};
struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                              // +0x04
    ~CBaseIntelligentPointer();                 // 0x00A01C10
};
struct CThingPlayerCreature;
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {
};
struct CHeroLogBook;
struct CCountedImp { };
struct SCountBlock {                            // the counted pointer's shared block
    long Count;                                 // +0x00
    void (CCountedImp::*Destroy)();             // +0x04
    CCountedImp* Object;                        // +0x08
};
template<class T> struct CCountedPointer {
    T* Data;                                    // +0x00
    SCountBlock* Info;                          // +0x04
    __forceinline ~CCountedPointer()
    {
        if (Info) {
            --Info->Count;
            if (Info->Count == 0) {
                (Info->Object->*Info->Destroy)();
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
struct CInputProcess {
    virtual ~CInputProcess();                   // slot 0 (scalar deleting dtor)
};

struct CInputProcessBButtonExitMode : CInputProcess { };
struct CInputProcessBetting : CInputProcess { };
struct CInputProcessBlock : CInputProcess { };
struct CInputProcessBoastUI : CInputProcess { };
struct CInputProcessCameraLookAround : CInputProcess { };
struct CInputProcessClickPastText : CInputProcess { };
struct CInputProcessCombat : CInputProcess { };
struct CInputProcessConsole : CInputProcess { };
struct CInputProcessControlCreature : CInputProcess { };
struct CInputProcessControlCreatureActivateZTarget : CInputProcess { };
struct CInputProcessControlCreatureActivateZTargetOnPress : CInputProcess { };
struct CInputProcessControlCreatureRightStick : CInputProcess { };
struct CInputProcessControlFreeCamera : CInputProcess { };
struct CInputProcessControlSpirit : CInputProcess { };
struct CInputProcessCreatureMovement : CInputProcess { };
struct CInputProcessCreatureMovementWatchForControlAngleChange : CInputProcess { };
struct CInputProcessCreditsUI : CInputProcess { };
struct CInputProcessCutScene : CInputProcess { };
struct CInputProcessCycleSpecialCameraModes : CInputProcess { };
struct CInputProcessDead : CInputProcess { };
struct CInputProcessDebugControls : CInputProcess { };
struct CInputProcessDigging : CInputProcess { };
struct CInputProcessFireheartMinigame : CInputProcess { };
struct CInputProcessFirstPerson : CInputProcess { };
struct CInputProcessFirstPersonLookAround : CInputProcess { };
struct CInputProcessFirstPersonTargeting : CInputProcess { };
struct CInputProcessFishing : CInputProcess { };
struct CInputProcessFreezeControls : CInputProcess { };
struct CInputProcessHeroAbilitiesScreen : CInputProcess { };
struct CInputProcessHeroInformationScreens : CInputProcess { };
struct CInputProcessInGameMenu : CInputProcess { };
struct CInputProcessInventory : CInputProcess { };
struct CInputProcessInventoryClothing : CInputProcess { };
struct CInputProcessInventoryExperienceScreen : CInputProcess { };
struct CInputProcessInventoryMagicScreen : CInputProcess { };
struct CInputProcessInventoryMapScreen : CInputProcess { };
struct CInputProcessInventoryQuestsScreen : CInputProcess { };
struct CInputProcessInventoryStatsScreen : CInputProcess { };
struct CInputProcessInventoryTradeScreen : CInputProcess { };
struct CInputProcessInventoryWeapons : CInputProcess { };
struct CInputProcessJumpingAndRolling : CInputProcess { };
struct CInputProcessLightning : CInputProcess { };
struct CInputProcessMain : CInputProcess { };
struct CInputProcessOracleMinigame : CInputProcess { };
struct CInputProcessParalysed : CInputProcess { };
struct CInputProcessPhotojournalCapture : CInputProcess { };
struct CInputProcessProjectileTargetingAnalogueZoom : CInputProcess { };
struct CInputProcessQuestCompletionUI : CInputProcess { };
struct CInputProcessQuickAccessItems : CInputProcess { };
struct CInputProcessQuickAccessMenu : CInputProcess { };
struct CInputProcessRebootGame : CInputProcess { };
struct CInputProcessSetRangedWeaponMode : CInputProcess { };
struct CInputProcessSetRangedWeaponThirdPersonMode : CInputProcess { };
struct CInputProcessSpecialAbilities : CInputProcess { };
struct CInputProcessStrafe : CInputProcess { };
struct CInputProcessTargetLockCycleTargets : CInputProcess { };
struct CInputProcessTargetLockRightStickTargetSelect : CInputProcess { };
struct CInputProcessTavernGame : CInputProcess { };
struct CInputProcessToggleViewHeroMode : CInputProcess { };
struct CInputProcessUseEnvironment : CInputProcess { };
struct CInputProcessUseRangedWeapon : CInputProcess { };
struct CInputProcessUseRangedWeaponZLock : CInputProcess { };
struct CInputProcessWatchForRangedWeaponThirdPersonModeTermination : CInputProcess { };
struct CInputProcessWatchForWillChargeUpThirdPersonModeTermination : CInputProcess { };
struct CInputProcessYesNoQuestion : CInputProcess { };
struct CInputProcessZTarget : CInputProcess { };
template<class T> struct scoped_ptr {
    T* ptr;
    ~scoped_ptr() { delete ptr; }
};
enum EPlayerMode { PLAYER_MODE_NONE };
struct SListNode;
struct CPlayerModeList {                        // STLport list<EPlayerMode> (4 bytes: the node proxy)
    SListNode* _M_node;
    ~CPlayerModeList();                         // 0x0044B970
};
struct CPlayerDef {
    virtual void v00();
    virtual void Delete() const;                // slot 1 (+0x04)
    long RefCount;                              // +0x04
};
template<class T> struct CDefPointer {
    T* Object;
    ~CDefPointer()
    {
        if (Object) {
            T* object = Object;
            if (--const_cast<CPlayerDef*>(object)->RefCount == 0)
                object->Delete();
            Object = 0;
        }
    }
};
struct CGamePlayerInterface;
struct CPlayerManager;
struct CMainGameComponent;
struct CGameDefinitionManager;
struct CWorld;
struct CThingManager;
struct CWorldMap;

struct CPlayer : CInitBaseClass {               // vtable 0x01231CC4
    bool DrawingFreeCamDebug;                   // +0x08
    bool PlayerIsCharacter;                     // +0x09
    CGamePlayerInterface* PlayerInterface;      // +0x0c
    CPlayerManager* PlayerManager;              // +0x10
    CMainGameComponent* Component;              // +0x14
    CGameDefinitionManager* DefinitionManager;  // +0x18
    CWorld* World;                              // +0x1c
    CThingManager* ThingManager;                // +0x20
    CWorldMap* PWorldMap;                       // +0x24
    long Number;                                // +0x28
    CIntelligentPointer<CThingPlayerCreature> ControlledCreature;   // +0x2c
    CIntelligentPointer<CThingPlayerCreature> PlayerCharacter;      // +0x34
    long PlayerRespawnDelay;                    // +0x3c
    CCamera CurrentFreeCamera;                  // +0x40
    CCamera FreeCamera;                         // +0x78
    CCamera OldFreeCamera;                      // +0xb0
    CThingRespawnInfo RespawnInfo;              // +0xe8
    CCountedPointer<CHeroLogBook> PHeroLogBook; // +0xf8
    scoped_ptr<CInputProcessMain> InputProcessMain; // +0x100
    scoped_ptr<CInputProcessControlCreature> InputProcessControlCreature; // +0x104
    scoped_ptr<CInputProcessZTarget> InputProcessZTarget; // +0x108
    scoped_ptr<CInputProcessDead> InputProcessDead; // +0x10c
    scoped_ptr<CInputProcessInventory> InputProcessInventory; // +0x110
    scoped_ptr<CInputProcessInventoryClothing> InputProcessInventoryClothing; // +0x114
    scoped_ptr<CInputProcessInventoryWeapons> InputProcessInventoryWeapons; // +0x118
    scoped_ptr<CInputProcessHeroAbilitiesScreen> InputProcessHeroAbilitiesScreen; // +0x11c
    scoped_ptr<CInputProcessInventoryMapScreen> InputProcessInventoryMapScreen; // +0x120
    scoped_ptr<CInputProcessInventoryStatsScreen> InputProcessInventoryStatsScreen; // +0x124
    scoped_ptr<CInputProcessInventoryMagicScreen> InputProcessInventoryMagicScreen; // +0x128
    scoped_ptr<CInputProcessInventoryExperienceScreen> InputProcessInventoryExperienceScreen; // +0x12c
    scoped_ptr<CInputProcessInventoryTradeScreen> InputProcessInventoryTradeScreen; // +0x130
    scoped_ptr<CInputProcessInventoryQuestsScreen> InputProcessInventoryQuestsScreen; // +0x134
    scoped_ptr<CInputProcessHeroInformationScreens> InputProcessHeroInformationScreens; // +0x138
    scoped_ptr<CInputProcessFirstPerson> InputProcessFirstPerson; // +0x13c
    scoped_ptr<CInputProcessClickPastText> InputProcessClickPastText; // +0x140
    scoped_ptr<CInputProcessYesNoQuestion> InputProcessYesNoQuestion; // +0x144
    scoped_ptr<CInputProcessFreezeControls> InputProcessFreezeControls; // +0x148
    scoped_ptr<CInputProcessControlFreeCamera> InputProcessControlFreeCamera; // +0x14c
    scoped_ptr<CInputProcessCreatureMovement> InputProcessCreatureMovement; // +0x150
    scoped_ptr<CInputProcessUseEnvironment> InputProcessUseEnvironment; // +0x154
    scoped_ptr<CInputProcessBlock> InputProcessBlock; // +0x158
    scoped_ptr<CInputProcessDebugControls> InputProcessDebugControls; // +0x15c
    scoped_ptr<CInputProcessQuickAccessItems> InputProcessQuickAccessItems; // +0x160
    scoped_ptr<CInputProcessCombat> InputProcessCombat; // +0x164
    scoped_ptr<CInputProcessSpecialAbilities> InputProcessSpecialAbilities; // +0x168
    scoped_ptr<CInputProcessControlCreatureRightStick> InputProcessControlCreatureRightStick; // +0x16c
    scoped_ptr<CInputProcessCameraLookAround> InputProcessRightStickLookAround; // +0x170
    scoped_ptr<CInputProcessCycleSpecialCameraModes> InputProcessCycleSpecialCameraModes; // +0x174
    scoped_ptr<CInputProcessRebootGame> InputProcessRebootGame; // +0x178
    scoped_ptr<CInputProcessJumpingAndRolling> InputProcessJumpingAndRolling; // +0x17c
    scoped_ptr<CInputProcessFirstPersonTargeting> InputProcessFirstPersonTargeting; // +0x180
    scoped_ptr<CInputProcessUseRangedWeapon> InputProcessUseRangedWeapon; // +0x184
    scoped_ptr<CInputProcessControlCreatureActivateZTarget> InputProcessControlCreatureActivateZTarget; // +0x188
    scoped_ptr<CInputProcessControlCreatureActivateZTargetOnPress> InputProcessControlCreatureActivateZTargetOnPress; // +0x18c
    scoped_ptr<CInputProcessFirstPersonLookAround> InputProcessLeftStickLookAround; // +0x190
    scoped_ptr<CInputProcessInGameMenu> InputProcessInGameMenu; // +0x194
    scoped_ptr<CInputProcessControlSpirit> InputProcessControlSpirit; // +0x198
    scoped_ptr<CInputProcessTavernGame> InputProcessTavernGame; // +0x19c
    scoped_ptr<CInputProcessSetRangedWeaponMode> InputProcessSetRangedWeaponMode; // +0x1a0
    scoped_ptr<CInputProcessCutScene> InputProcessCutScene; // +0x1a4
    scoped_ptr<CInputProcessFishing> InputProcessFishing; // +0x1a8
    scoped_ptr<CInputProcessDigging> InputProcessDigging; // +0x1ac
    scoped_ptr<CInputProcessParalysed> InputProcessParalysed; // +0x1b0
    scoped_ptr<CInputProcessBoastUI> InputProcessBoastUI; // +0x1b4
    scoped_ptr<CInputProcessSetRangedWeaponThirdPersonMode> InputProcessSetRangedWeaponThirdPerson; // +0x1b8
    scoped_ptr<CInputProcessUseRangedWeaponZLock> InputProcessUseRangedWeaponZLock; // +0x1bc
    scoped_ptr<CInputProcessWatchForRangedWeaponThirdPersonModeTermination> InputProcessWatchForRangedWeaponThirdPersonModeTermination; // +0x1c0
    scoped_ptr<CInputProcessWatchForWillChargeUpThirdPersonModeTermination> InputProcessWatchForWillChargeUpThirdPersonModeTermination; // +0x1c4
    scoped_ptr<CInputProcessTargetLockCycleTargets> InputProcessTargetLockCycleTargets; // +0x1c8
    scoped_ptr<CInputProcessQuickAccessMenu> InputProcessQuickAccessMenu; // +0x1cc
    scoped_ptr<CInputProcessQuestCompletionUI> InputProcessQuestCompletionUI; // +0x1d0
    scoped_ptr<CInputProcessTargetLockRightStickTargetSelect> InputProcessTargetLockRightStickTargetSelect; // +0x1d4
    scoped_ptr<CInputProcessBButtonExitMode> InputProcessBButtonExitMode; // +0x1d8
    scoped_ptr<CInputProcessCreditsUI> InputProcessCreditsUI; // +0x1dc
    scoped_ptr<CInputProcessBetting> InputProcessBetting; // +0x1e0
    scoped_ptr<CInputProcessLightning> InputProcessLightning; // +0x1e4
    scoped_ptr<CInputProcessCreatureMovementWatchForControlAngleChange> InputProcessWatchForControlAngleChange; // +0x1e8
    scoped_ptr<CInputProcessOracleMinigame> InputProcessOracleMinigame; // +0x1ec
    scoped_ptr<CInputProcessFireheartMinigame> InputProcessFireheartMinigame; // +0x1f0
    scoped_ptr<CInputProcessStrafe> InputProcessStrafe; // +0x1f4
    scoped_ptr<CInputProcessConsole> InputProcessConsole; // +0x1f8
    scoped_ptr<CInputProcessProjectileTargetingAnalogueZoom> InputProcessProjectileTargetingAnalogueZoom; // +0x1fc
    scoped_ptr<CInputProcessPhotojournalCapture> InputProcessPhotojournalCapture; // +0x200
    scoped_ptr<CInputProcessToggleViewHeroMode> InputProcessToggleViewHeroMode; // +0x204
    bool Local;                                 // +0x208
    bool ShowWorldThing;                        // +0x209
    bool ZTargeting;                            // +0x20a
    bool ProjectileTargetLocked;                // +0x20b
    bool RightTriggerTargetLocked;              // +0x20c
    CPlayerModeList PlayerModes;                // +0x210
    bool SuppressFullModeRemoval;               // +0x214
    bool DisallowModeChanges;
    bool InitialPlayerModeSet;
    bool AggressiveMode;
    bool SpellMode;
    bool ExpressionShiftMode;
    bool PCProjectileWeaponThirdPersonAimingMode;
    bool KillEverythingMode;
    CDefPointer<CPlayerDef> PlayerDef;          // +0x21c
    bool UsingFreeCamera;                       // +0x220
    bool FreeCameraTrackingPlayer;
    bool FreeCameraTrackingPlayerYZ;
    bool ControllingFreeCamera;
    bool KeepAbilitiesDuringCutscenes;
    long JoystickDeviceNumber;                  // +0x224
    ~CPlayer();
};

CPlayer::~CPlayer()
{
    PlayerInterface = 0;
    PlayerManager = 0;
    Component = 0;
    World = 0;
    DefinitionManager = 0;
}
