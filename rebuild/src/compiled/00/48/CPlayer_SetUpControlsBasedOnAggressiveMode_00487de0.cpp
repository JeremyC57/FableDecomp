// CPlayer::SetUpControlsBasedOnAggressiveMode() -- pick the PC control scheme (normal or aggressive) from the global def and, when leaving aggressive mode while the controlled creature is alive and carrying a weapon, make it sheathe the weapon back into the inventory
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00487DE0, length 176 bytes, plain `ret`.
// Identity: Ego_r.pdb `?SetUpControlsBasedOnAggressiveMode@CPlayer@@AAEXXZ` (callers SetAggressiveMode 0x00487EB0,
// ToggleAggressiveMode 0x00487E90).
// Callees: CGamePlayerInterface::SetControlScheme(long) 0x00446EF0, CBaseIntelligentPointer::GetPItem 0x00A01B50 (x4),
// IsThingCarryingWeapon(const CThing*) 0x006A1540 (__fastcall), CCreatureAction_SheatheItemToInventory ctor
// 0x0069FFD0 / its out-of-line dtor 0x0048C950, CThingCreatureBase::SetCurrentAction 0x006644F0.
// Layout: PlayerInterface +0x0C, DefinitionManager +0x18 (its CDefPointer<CGlobalDef> at +0xD0), ControlledCreature
// +0x2C, AggressiveMode +0x217 (PDB +0x21F: the retail PlayerModes list at +0x210 is 4 bytes, so the engine header's
// offsets past it are 8 high and a local model is used). CGlobalDef ControlSchemePC +0x3C / ControlSchemePCAggressive
// +0x40 (PDB +0x40/+0x44, retail defs sit 4 bytes low).
// Shape: the global def is copied into a local CDefPointer (`inc [edi+4]` behind a null test; the release at the end is
// the def's inline Release() with no null test: `dec [edi+4] ; jne ; call [vtable+4]`); the sheathe action is a
// 0xb0-byte temporary passed straight from its constructor, the creature for `this` fetched first (ebx).
struct CBaseObject;

struct CThing {
    unsigned char _pad00[0x91];
    unsigned char m_stateFlags;                         // +0x91 (bit 0: dead / being deleted)
};
struct CCreatureActionBase;
struct CThingCreatureBase : CThing {
    bool SetCurrentAction(CCreatureActionBase* action); // 0x006644F0
};
struct CThingPlayerCreature : CThingCreatureBase { };

bool __fastcall IsThingCarryingWeapon(const CThing* thing);   // 0x006A1540

inline bool IsThingAlive(const CThing* thing)
{
    return thing && !(thing->m_stateFlags & 1);
}

struct CCreatureActionBase {                            // retail sizeof 0xa8
    virtual void v00();
    unsigned char _pad04[0xa8 - 0x04];
};
struct CCreatureAction_SheatheItemToInventory : CCreatureActionBase {   // retail sizeof 0xb0
    unsigned char PNextAction[8];                       // +0xa8 CCountedPointer<CCreatureActionBase>
    CCreatureAction_SheatheItemToInventory(CThingCreatureBase* owner, CCreatureActionBase* next, long kind);   // 0x0069FFD0
    ~CCreatureAction_SheatheItemToInventory();          // 0x0048C950 (out-of-line copy)
};

class CBaseIntelligentPointer {
public:
    virtual void v00();
    CBaseObject* PItem;                                 // +0x04
    CBaseObject* GetPItem() const;                      // 0x00A01B50
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};

struct CDefPointeeBase {
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long RefCount;                                      // +0x04
    void AddRef() { ++RefCount; }
    void Release() { if (--RefCount == 0) Destroy(); }
};
struct CGlobalDef : CDefPointeeBase {
    unsigned char _pad08[0x3c - 0x08];
    long ControlSchemePC;                               // +0x3c
    long ControlSchemePCAggressive;                     // +0x40
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) Object->AddRef(); }
    ~CDefPointer() { Object->Release(); }
    T* operator->() const { return Object; }
};

struct CGameDefinitionManager {
    unsigned char _pad00[0xd0];
    CDefPointer<CGlobalDef> GlobalDef;                  // +0xd0
};

struct CGamePlayerInterface {
    void SetControlScheme(long scheme);                 // 0x00446EF0
};

struct CPlayer {
    void*                   __vftable;
    unsigned char           _pad04[0x0c - 0x04];
    CGamePlayerInterface*   PlayerInterface;            // +0x0c
    unsigned char           _pad10[0x18 - 0x10];
    CGameDefinitionManager* DefinitionManager;          // +0x18
    unsigned char           _pad1c[0x2c - 0x1c];
    CIntelligentPointer<CThingPlayerCreature> ControlledCreature;   // +0x2c
    unsigned char           _pad34[0x217 - 0x34];
    bool                    AggressiveMode;             // +0x217
    void SetUpControlsBasedOnAggressiveMode();
};

void CPlayer::SetUpControlsBasedOnAggressiveMode()
{
    CDefPointer<CGlobalDef> globalDef = DefinitionManager->GlobalDef;
    PlayerInterface->SetControlScheme(AggressiveMode ? globalDef->ControlSchemePCAggressive : globalDef->ControlSchemePC);
    if (!AggressiveMode && IsThingAlive(ControlledCreature.GetPItem()) && IsThingCarryingWeapon(ControlledCreature.GetPItem()))
        ControlledCreature.GetPItem()->SetCurrentAction(&CCreatureAction_SheatheItemToInventory(ControlledCreature.GetPItem(), 0, 100));
}