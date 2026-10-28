// CPlayer::PlayerInit(const CPlayerInit* init) -- base initialisation, pushes the null mode, copies the init block, resolves the player def, clears the controlled creature and mode flags, builds the hero log book and (local players only) the interface, joystick device, input processes, modes and GUI
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0048A210, length 410 bytes, `ret 4`; returns true.
// Identity: Ego_r.pdb `?PlayerInit@CPlayer@@QAE_NPBVCPlayerInit@@@Z` (the parameter is a pointer).
// Callees: CInitBaseClass::Init 0x0099A350 (label "CXMVPlaybackInfo" propagated), malloc 0x00BFEA0E (the list node),
//   CGameDefinitionManager::GetDef 0x0044BA90 (label GetDef<CThingPhysicalSwitchDef>: shared template body),
//   CBaseIntelligentPointer ctor/SetPItem/GetPItem/dtor 0x00A01B10/0x00A01B90/0x00A01B50/0x00A01C10 (vtable 0x01231C4C),
//   InitPlayerHeroLogBook 0x0048A1A0, CGameJoystickManager::Get 0x0049BB90 / GetMainPlayerJoystickDeviceNumber 0x0049BB40,
//   CPlayerManager::GetMainPlayerNumber 0x00449700, InitInterfaces 0x00488D10, InitModes 0x00636E50, InitPlayerGui 0x00487FB0.
// Shape: the null mode is push_front'ed (inserted before begin(), not end()); InitJoystickDeviceNumber (out of line at
//   0x00487AA0) is inlined here with the landed body; ControlledCreature = CIntelligentPointer(0) temporary.
// Layout: local retail view of CPlayer -- engine/CPlayer.h is right below +0x210 but the retail PlayerModes list is
//   4 bytes (PDB 12), so every member from SuppressFullModeRemoval (+0x214) on sits 8 bytes below the PDB/header offset.
#include <stddef.h>
extern "C" void* __cdecl malloc(size_t size);    // 0x00BFEA0E
inline void* __cdecl operator new(size_t, void* place) { return place; }

class CMainGameComponent;
class CWorld;
class CThingManager;
class CWorldMap;
class CGamePlayerInterface;
class CBaseObject;
class CThingPlayerCreature;
class CDefPointeeBase;
class CPlayerDef;

template <class T> class CDefPointer { public:
    CDefPointeeBase* Object;
};
class CGameDefinitionManager { public:
    bool GetDef(long type, CDefPointer<CPlayerDef>* out);    // 0x0044BA90 (GetDef<T> shared body)
};
class CPlayerManager { public:
    long GetMainPlayerNumber() const;                        // 0x00449700
};
class CGameJoystickManager { public:
    static CGameJoystickManager& __fastcall Get();          // 0x0049BB90
    long GetMainPlayerJoystickDeviceNumber() const;         // 0x0049BB40
};
class CPlayerInit { public:
    CMainGameComponent*     Component;          // +0x00
    CWorld*                 World;              // +0x04
    CGameDefinitionManager* DefinitionManager;  // +0x08
    long                    Number;             // +0x0c
    long                    DefType;            // +0x10
    bool                    Local;              // +0x14
    CPlayerManager*         PlayerManager;      // +0x18
    CGamePlayerInterface*   PlayerInterface;    // +0x1c
    bool                    PlayerCharacter;    // +0x20
};

class CBaseIntelligentPointer {
public:
    virtual void v00();
    CBaseObject* PItem;
    CBaseIntelligentPointer();                              // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                             // 0x00A01C10
    void SetPItem(CBaseObject* item);                       // 0x00A01B90
    CBaseObject* GetPItem() const;                          // 0x00A01B50
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01231C4C
public:
    virtual void v00();
    CIntelligentPointer(T* item) { SetPItem((CBaseObject*)item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};

enum EPlayerMode { PLAYER_MODE_NULL = 0 };
struct SPlayerModeNode {                         // STLport _List_node<EPlayerMode> (12 bytes)
    SPlayerModeNode* _M_next;
    SPlayerModeNode* _M_prev;
    EPlayerMode      _M_data;
};
struct CPlayerModeList {                         // std::list<EPlayerMode>: retail 4-byte header pointer
    SPlayerModeNode* _M_node;
    void push_front(const EPlayerMode& value)
    {
        SPlayerModeNode* position = _M_node->_M_next;
        SPlayerModeNode* node = (SPlayerModeNode*)malloc(sizeof(SPlayerModeNode));
        new (&node->_M_data) EPlayerMode(value);
        SPlayerModeNode* prev = position->_M_prev;
        node->_M_next = position;
        node->_M_prev = prev;
        prev->_M_next = node;
        position->_M_prev = node;
    }
};

class CInitBaseClass { public:
    virtual void v00();
    bool Valid;                                  // +0x04
    bool Init();                                 // 0x0099A350
};

class CPlayer : public CInitBaseClass { public:
    bool                     DrawingFreeCamDebug;      // +0x08
    bool                     PlayerIsCharacter;        // +0x09
    CGamePlayerInterface*    PlayerInterface;          // +0x0c
    CPlayerManager*          PlayerManager;            // +0x10
    CMainGameComponent*      Component;                // +0x14
    CGameDefinitionManager*  DefinitionManager;        // +0x18
    CWorld*                  World;                    // +0x1c
    CThingManager*           ThingManager;             // +0x20
    CWorldMap*               PWorldMap;                // +0x24
    long                     Number;                   // +0x28
    CIntelligentPointer<CThingPlayerCreature> ControlledCreature;   // +0x2c
    unsigned char            PlayerCharacter[8];       // +0x34
    long                     PlayerRespawnDelay;       // +0x3c
    unsigned char            _pad40[0x208 - 0x40];
    bool                     Local;                    // +0x208
    bool                     ShowWorldThing;           // +0x209
    bool                     ZTargeting;               // +0x20a
    bool                     ProjectileTargetLocked;   // +0x20b
    bool                     RightTriggerTargetLocked; // +0x20c
    CPlayerModeList          PlayerModes;              // +0x210 (PDB 12 bytes)
    bool                     SuppressFullModeRemoval;  // +0x214
    bool                     DisallowModeChanges;      // +0x215
    bool                     InitialPlayerModeSet;     // +0x216
    bool                     AggressiveMode;           // +0x217
    bool                     SpellMode;                // +0x218
    bool                     ExpressionShiftMode;      // +0x219
    bool                     PCProjectileWeaponThirdPersonAimingMode; // +0x21a
    bool                     KillEverythingMode;       // +0x21b
    CDefPointer<CPlayerDef>  PlayerDef;                // +0x21c
    bool                     UsingFreeCamera;          // +0x220
    bool                     FreeCameraTrackingPlayer; // +0x221
    bool                     FreeCameraTrackingPlayerYZ; // +0x222
    bool                     ControllingFreeCamera;    // +0x223
    bool                     KeepAbilitiesDuringCutscenes; // +0x224
    long                     JoystickDeviceNumber;     // +0x228

    bool IsMainPlayer() const
    {
        long number = Number;
        return number == PlayerManager->GetMainPlayerNumber();
    }
    void InitJoystickDeviceNumber()
    {
        long mainDevice = CGameJoystickManager::Get().GetMainPlayerJoystickDeviceNumber();
        if (IsMainPlayer())
            JoystickDeviceNumber = mainDevice;
        else if (Number == mainDevice)
            JoystickDeviceNumber = 0;
        else
            JoystickDeviceNumber = Number;
    }
    void InitPlayerHeroLogBook();                // 0x0048A1A0
    void InitInterfaces();                       // 0x00488D10
    void InitModes();                            // 0x00636E50
    void InitPlayerGui();                        // 0x00487FB0
    bool PlayerInit(const CPlayerInit* init);
};

bool CPlayer::PlayerInit(const CPlayerInit* init)
{
    Init();
    PlayerModes.push_front(PLAYER_MODE_NULL);
    KeepAbilitiesDuringCutscenes = false;
    ControllingFreeCamera = false;
    UsingFreeCamera = false;
    PlayerIsCharacter = init->PlayerCharacter;
    Component = init->Component;
    World = init->World;
    Number = init->Number;
    Local = init->Local;
    PlayerManager = init->PlayerManager;
    DefinitionManager = init->DefinitionManager;
    FreeCameraTrackingPlayer = false;
    FreeCameraTrackingPlayerYZ = false;
    DrawingFreeCamDebug = false;
    JoystickDeviceNumber = -1;
    DefinitionManager->GetDef(init->DefType, &PlayerDef);
    ThingManager = 0;
    PWorldMap = 0;
    ControlledCreature = CIntelligentPointer<CThingPlayerCreature>(0);
    ZTargeting = false;
    ProjectileTargetLocked = false;
    RightTriggerTargetLocked = false;
    ShowWorldThing = false;
    AggressiveMode = false;
    SpellMode = false;
    ExpressionShiftMode = false;
    PCProjectileWeaponThirdPersonAimingMode = false;
    KillEverythingMode = false;
    PlayerRespawnDelay = 0;
    InitPlayerHeroLogBook();
    SuppressFullModeRemoval = false;
    DisallowModeChanges = false;
    InitialPlayerModeSet = true;
    PlayerInterface = 0;
    if (Local) {
        PlayerInterface = init->PlayerInterface;
        InitJoystickDeviceNumber();
        InitInterfaces();
        InitModes();
        InitPlayerGui();
    }
    return true;
}