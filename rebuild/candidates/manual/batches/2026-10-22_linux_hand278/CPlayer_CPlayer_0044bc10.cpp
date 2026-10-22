// CPlayer::CPlayer() -- player constructor: CInitBaseClass base, the controlled-creature and player-character intelligent pointers, the three free cameras' lock flags, the respawn info flag, the hero log book pointer, 66 input-process scoped pointers, the player-mode list sentinel and the player def pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0044BC10, length 533 bytes (+ int3 padding), plain `ret`.
// Callees: CInitBaseClass::CInitBaseClass @ 0x0099A310, CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10,
// malloc @ 0x00BFEA0E (STLport list node allocator). Vtables: CPlayer 0x01231CC4 (RTTI), CIntelligentPointer
// <CThingPlayerCreature> 0x01231C4C.
// Label: the catalog row is named CAIStateGroup_JackOfBladesBase (propagated); the vtable it stores is CPlayer's
// (rtti-vtables 0x01231CC4 slot 0 = 0x0044BE30) and its single caller 0x0044A1D0 pushes the new player into a vector.
// Shape: member constructors in declaration order; STLport 4.x `_List_base` stores a null node then mallocs the 12-byte
// sentinel, links it to itself through a local and stores it last. The TU is built without /EHsc (no unwind frame although members have destructors).
// Layout: Ego_r.pdb CPlayer up to +0x208; retail STLport list is 4 bytes (PDB 12), so PlayerDef sits at +0x21c (PDB +0x224).
// Leaf: ??0CPlayer
#include <stdlib.h>

struct CInitBaseClass {
    virtual void v00();
    bool m_flag04;                              // +0x04 (cleared by the base constructor)
    CInitBaseClass();                           // 0x0099A310
};
struct C3DVector { float X, Y, Z; };
struct CRightHandedSet { C3DVector Up, Forward; };
struct CCamera {                                // 0x38 bytes
    C3DVector WorldPosition;                    // +0x00
    CRightHandedSet RHSet;                      // +0x0c
    bool HeightLocked;                          // +0x24
    bool ViewVecZLocked;                        // +0x25
    unsigned long FOVFlags;                     // +0x28
    float HorizontalFOV;                        // +0x2c
    float VerticalFOV;                          // +0x30
    float Zoom;                                 // +0x34
    CCamera() : HeightLocked(false), ViewVecZLocked(false) { }
};
struct CThingRespawnInfo {                      // 0x10 bytes
    C3DVector DiedPosition;                     // +0x00
    bool DeadInfoSet;                           // +0x0c
    CThingRespawnInfo() : DeadInfoSet(false) { }
};
struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                              // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
};
struct CThingPlayerCreature;
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {   // <CThingPlayerCreature>: vtable 0x01231C4C
    virtual void v00();
};
struct CHeroLogBook;
template<class T> struct CCountedPointer {
    T* Data;
    long* Info;
    CCountedPointer() : Data(0), Info(0) { }
};
template<class T> struct scoped_ptr {
    T* ptr;
    scoped_ptr() : ptr(0) { }
};
struct CInputProcessBButtonExitMode;
struct CInputProcessBetting;
struct CInputProcessBlock;
struct CInputProcessBoastUI;
struct CInputProcessCameraLookAround;
struct CInputProcessClickPastText;
struct CInputProcessCombat;
struct CInputProcessConsole;
struct CInputProcessControlCreature;
struct CInputProcessControlCreatureActivateZTarget;
struct CInputProcessControlCreatureActivateZTargetOnPress;
struct CInputProcessControlCreatureRightStick;
struct CInputProcessControlFreeCamera;
struct CInputProcessControlSpirit;
struct CInputProcessCreatureMovement;
struct CInputProcessCreatureMovementWatchForControlAngleChange;
struct CInputProcessCreditsUI;
struct CInputProcessCutScene;
struct CInputProcessCycleSpecialCameraModes;
struct CInputProcessDead;
struct CInputProcessDebugControls;
struct CInputProcessDigging;
struct CInputProcessFireheartMinigame;
struct CInputProcessFirstPerson;
struct CInputProcessFirstPersonLookAround;
struct CInputProcessFirstPersonTargeting;
struct CInputProcessFishing;
struct CInputProcessFreezeControls;
struct CInputProcessHeroAbilitiesScreen;
struct CInputProcessHeroInformationScreens;
struct CInputProcessInGameMenu;
struct CInputProcessInventory;
struct CInputProcessInventoryClothing;
struct CInputProcessInventoryExperienceScreen;
struct CInputProcessInventoryMagicScreen;
struct CInputProcessInventoryMapScreen;
struct CInputProcessInventoryQuestsScreen;
struct CInputProcessInventoryStatsScreen;
struct CInputProcessInventoryTradeScreen;
struct CInputProcessInventoryWeapons;
struct CInputProcessJumpingAndRolling;
struct CInputProcessLightning;
struct CInputProcessMain;
struct CInputProcessOracleMinigame;
struct CInputProcessParalysed;
struct CInputProcessPhotojournalCapture;
struct CInputProcessProjectileTargetingAnalogueZoom;
struct CInputProcessQuestCompletionUI;
struct CInputProcessQuickAccessItems;
struct CInputProcessQuickAccessMenu;
struct CInputProcessRebootGame;
struct CInputProcessSetRangedWeaponMode;
struct CInputProcessSetRangedWeaponThirdPersonMode;
struct CInputProcessSpecialAbilities;
struct CInputProcessStrafe;
struct CInputProcessTargetLockCycleTargets;
struct CInputProcessTargetLockRightStickTargetSelect;
struct CInputProcessTavernGame;
struct CInputProcessToggleViewHeroMode;
struct CInputProcessUseEnvironment;
struct CInputProcessUseRangedWeapon;
struct CInputProcessUseRangedWeaponZLock;
struct CInputProcessWatchForRangedWeaponThirdPersonModeTermination;
struct CInputProcessWatchForWillChargeUpThirdPersonModeTermination;
struct CInputProcessYesNoQuestion;
struct CInputProcessZTarget;
enum EPlayerMode { PLAYER_MODE_NONE };
struct SListNodeBase {
    SListNodeBase* _M_next;
    SListNodeBase* _M_prev;
};
struct SListNode : SListNodeBase {
    EPlayerMode _M_data;
};
struct CPlayerModeList {                        // STLport list<EPlayerMode> (4 bytes: the node proxy)
    SListNode* _M_node;
    CPlayerModeList() : _M_node(0)
    {
        SListNode* node = (SListNode*)malloc(sizeof(SListNode));
        node->_M_next = node;
        node->_M_prev = node;
        _M_node = node;
    }
};
struct CPlayerDef;
template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
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
    scoped_ptr<CInputProcessMain> InputProcessMain;           // +0x100
    scoped_ptr<CInputProcessControlCreature> InputProcessControlCreature; // +0x104
    scoped_ptr<CInputProcessZTarget> InputProcessZTarget;     // +0x108
    scoped_ptr<CInputProcessDead> InputProcessDead;           // +0x10c
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
    scoped_ptr<CInputProcessBlock> InputProcessBlock;         // +0x158
    scoped_ptr<CInputProcessDebugControls> InputProcessDebugControls; // +0x15c
    scoped_ptr<CInputProcessQuickAccessItems> InputProcessQuickAccessItems; // +0x160
    scoped_ptr<CInputProcessCombat> InputProcessCombat;       // +0x164
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
    scoped_ptr<CInputProcessCutScene> InputProcessCutScene;   // +0x1a4
    scoped_ptr<CInputProcessFishing> InputProcessFishing;     // +0x1a8
    scoped_ptr<CInputProcessDigging> InputProcessDigging;     // +0x1ac
    scoped_ptr<CInputProcessParalysed> InputProcessParalysed; // +0x1b0
    scoped_ptr<CInputProcessBoastUI> InputProcessBoastUI;     // +0x1b4
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
    scoped_ptr<CInputProcessBetting> InputProcessBetting;     // +0x1e0
    scoped_ptr<CInputProcessLightning> InputProcessLightning; // +0x1e4
    scoped_ptr<CInputProcessCreatureMovementWatchForControlAngleChange> InputProcessWatchForControlAngleChange; // +0x1e8
    scoped_ptr<CInputProcessOracleMinigame> InputProcessOracleMinigame; // +0x1ec
    scoped_ptr<CInputProcessFireheartMinigame> InputProcessFireheartMinigame; // +0x1f0
    scoped_ptr<CInputProcessStrafe> InputProcessStrafe;       // +0x1f4
    scoped_ptr<CInputProcessConsole> InputProcessConsole;     // +0x1f8
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
    bool DisallowModeChanges;                   // +0x215
    bool InitialPlayerModeSet;                  // +0x216
    bool AggressiveMode;                        // +0x217
    bool SpellMode;                             // +0x218
    bool ExpressionShiftMode;                   // +0x219
    bool PCProjectileWeaponThirdPersonAimingMode;   // +0x21a
    bool KillEverythingMode;                    // +0x21b
    CDefPointer<CPlayerDef> PlayerDef;          // +0x21c
    CPlayer();
    virtual void v00();
};

CPlayer::CPlayer()
{
}
