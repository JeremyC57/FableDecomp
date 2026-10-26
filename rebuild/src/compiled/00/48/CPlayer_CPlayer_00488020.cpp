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

class CInitBaseClass { public:
    virtual ~CInitBaseClass();                  // 0x0099A410
    bool m_flag04;                              // +0x04
};
class C3DVector { public: float X, Y, Z; };
class CRightHandedSet { public: C3DVector Up, Forward; };
class CCamera { public:                                // 0x38 bytes
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
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
class CThingPlayerCreature;
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
class CHeroLogBook;
struct CCountedImp { };
struct SCountBlock {                            // the counted pointer's shared block
    long Count;                                 // +0x00
    void (CCountedImp::*Destroy)();             // +0x04
    CCountedImp* Object;                        // +0x08
};
template<class T> class CCountedPointer { public:
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

class CInputProcessBButtonExitMode : public CInputProcess { public: };
class CInputProcessBetting : public CInputProcess { public: };
class CInputProcessBlock : public CInputProcess { public: };
class CInputProcessBoastUI : public CInputProcess { public: };
class CInputProcessCameraLookAround : public CInputProcess { public: };
class CInputProcessClickPastText : public CInputProcess { public: };
class CInputProcessCombat : public CInputProcess { public: };
class CInputProcessConsole : public CInputProcess { public: };
class CInputProcessControlCreature : public CInputProcess { public: };
class CInputProcessControlCreatureActivateZTarget : public CInputProcess { public: };
class CInputProcessControlCreatureActivateZTargetOnPress : public CInputProcess { public: };
class CInputProcessControlCreatureRightStick : public CInputProcess { public: };
class CInputProcessControlFreeCamera : public CInputProcess { public: };
class CInputProcessControlSpirit : public CInputProcess { public: };
class CInputProcessCreatureMovement : public CInputProcess { public: };
class CInputProcessCreatureMovementWatchForControlAngleChange : public CInputProcess { public: };
class CInputProcessCreditsUI : public CInputProcess { public: };
class CInputProcessCutScene : public CInputProcess { public: };
class CInputProcessCycleSpecialCameraModes : public CInputProcess { public: };
class CInputProcessDead : public CInputProcess { public: };
class CInputProcessDebugControls : public CInputProcess { public: };
class CInputProcessDigging : public CInputProcess { public: };
class CInputProcessFireheartMinigame : public CInputProcess { public: };
class CInputProcessFirstPerson : public CInputProcess { public: };
class CInputProcessFirstPersonLookAround : public CInputProcess { public: };
class CInputProcessFirstPersonTargeting : public CInputProcess { public: };
class CInputProcessFishing : public CInputProcess { public: };
class CInputProcessFreezeControls : public CInputProcess { public: };
class CInputProcessHeroAbilitiesScreen : public CInputProcess { public: };
class CInputProcessHeroInformationScreens : public CInputProcess { public: };
class CInputProcessInGameMenu : public CInputProcess { public: };
class CInputProcessInventory : public CInputProcess { public: };
class CInputProcessInventoryClothing : public CInputProcess { public: };
class CInputProcessInventoryExperienceScreen : public CInputProcess { public: };
class CInputProcessInventoryMagicScreen : public CInputProcess { public: };
class CInputProcessInventoryMapScreen : public CInputProcess { public: };
class CInputProcessInventoryQuestsScreen : public CInputProcess { public: };
class CInputProcessInventoryStatsScreen : public CInputProcess { public: };
class CInputProcessInventoryTradeScreen : public CInputProcess { public: };
class CInputProcessInventoryWeapons : public CInputProcess { public: };
class CInputProcessJumpingAndRolling : public CInputProcess { public: };
class CInputProcessLightning : public CInputProcess { public: };
class CInputProcessMain : public CInputProcess { public: };
class CInputProcessOracleMinigame : public CInputProcess { public: };
class CInputProcessParalysed : public CInputProcess { public: };
class CInputProcessPhotojournalCapture : public CInputProcess { public: };
class CInputProcessProjectileTargetingAnalogueZoom : public CInputProcess { public: };
class CInputProcessQuestCompletionUI : public CInputProcess { public: };
class CInputProcessQuickAccessItems : public CInputProcess { public: };
class CInputProcessQuickAccessMenu : public CInputProcess { public: };
class CInputProcessRebootGame : public CInputProcess { public: };
class CInputProcessSetRangedWeaponMode : public CInputProcess { public: };
class CInputProcessSetRangedWeaponThirdPersonMode : public CInputProcess { public: };
class CInputProcessSpecialAbilities : public CInputProcess { public: };
class CInputProcessStrafe : public CInputProcess { public: };
class CInputProcessTargetLockCycleTargets : public CInputProcess { public: };
class CInputProcessTargetLockRightStickTargetSelect : public CInputProcess { public: };
class CInputProcessTavernGame : public CInputProcess { public: };
class CInputProcessToggleViewHeroMode : public CInputProcess { public: };
class CInputProcessUseEnvironment : public CInputProcess { public: };
class CInputProcessUseRangedWeapon : public CInputProcess { public: };
class CInputProcessUseRangedWeaponZLock : public CInputProcess { public: };
class CInputProcessWatchForRangedWeaponThirdPersonModeTermination : public CInputProcess { public: };
class CInputProcessWatchForWillChargeUpThirdPersonModeTermination : public CInputProcess { public: };
class CInputProcessYesNoQuestion : public CInputProcess { public: };
class CInputProcessZTarget : public CInputProcess { public: };
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
class CPlayerDef { public:
    virtual void v00();
    virtual void Delete() const;                // slot 1 (+0x04)
    long RefCount;                              // +0x04
};
template<class T> class CDefPointer { public:
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
class CGamePlayerInterface;
class CPlayerManager;
class CMainGameComponent;
class CGameDefinitionManager;
class CWorld;
class CThingManager;
class CWorldMap;

class CPlayer : public CInitBaseClass { public:               // vtable 0x01231CC4
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