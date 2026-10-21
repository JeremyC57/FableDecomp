// CThingCreatureBase::WorldUpdate() -- per-frame world update of a creature: refresh the turn speed from the physics component, run the game-object update (stop when it asks to), update the body reorientation, forget the last attacker after 300 seconds, update components 0x77 and 0xCD, and once no pause mode is active start unsheathing the item queued for after the cutscene (then clear it)
// Leaf: ?WorldUpdate@CThingCreatureBase
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006666A0, length 383 bytes, plain `ret`, bool in al.
//   Name from Ego_r.pdb `?WorldUpdate@CThingCreatureBase@@UAE_NXZ`.
// Callees: the physics component's float query @ 0x006AFF40 (two fpatan; name unproven), CThingGameObject::WorldUpdate
//   @ 0x00838BA0, CThingCreatureBase::UpdateBodyReorientation @ 0x006622D0, CBaseIntelligentPointer::GetPItem
//   @ 0x00A01B50 / SetPItem @ 0x00A01B90 / ctor @ 0x00A01B10 / dtor @ 0x00A01C10 (CIntelligentPointer<CThing> vtable
//   0x01238C6C), CThing::PeekWorld @ 0x004C7AB0, CWorld::GetWorldFrame @ 0x0049D870, CThingCreatureBase::
//   ClearLastAttackedByCreature @ 0x006691C0, LowerBound @ 0x0040F020, component 0x77's update @ 0x007C21A0 (engine_api
//   CTCShotDefinitionBase::Update) and component 0xCD's update @ 0x008650E0 (names unproven),
//   CCreatureAction_UnsheatheItemFromInventory(CThingCreatureBase*, CThing*, CCreatureActionBase*, long) @ 0x006A0150 and
//   its folded destructor @ 0x005CC091, CThingCreatureBase::SetCurrentAction(const CCreatureActionBase&) @ 0x006644F0.
// Globals: g_framesPerSecond @ 0x01375550, g_pThingManager @ 0x013B8A1C (NonScriptedEntitesPauseMode +0x54,
//   AllEntitesPauseMode +0x55).
// Layout (retail): CThing bitset +0x20 (0x77 = bit 23 of +0x2c, 0xCD = bit 13 of +0x38), map +0x44, PhysicsTC +0x60
//   (PDB +0x70), Dead bit 0 of +0x91; CThingCreatureBase PLastAttackedByCreature +0xf4, WFLastAttackedByCreature +0xfc,
//   TurnSpeed +0x128, PItemToUnsheatheAfterCutscene +0x150 (PDB +0x110/+0x118/+0x14c/+0x178).
// Shape: the world is a named local and the last-attacked frame is read into a local after it (retail loads it into
//   edi between PeekWorld and the static GetWorldFrame); the action is a temporary destroyed before the pointer reset;
//   the CIntelligentPointer converting ctor and operator= are `__forceinline` (VC keeps them out of line otherwise).
struct CTCBase;
struct CBaseObject;

struct CThing;
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* PItem;                                      // +0x04
    CBaseIntelligentPointer();                          // 0x00A01B10
    ~CBaseIntelligentPointer();                         // 0x00A01C10
    CThing* GetPItem() const;                           // 0x00A01B50
    void SetPItem(CThing* item);                        // 0x00A01B90
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    __forceinline CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    __forceinline CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem(other.GetPItem()); return *this; }
};

struct CTCPhysicsBase {
    float GetTurnSpeed();                               // 0x006AFF40
};
struct CTCInterface77 { void Update(); };               // 0x007C21A0
struct CTCInterfaceCD { void Update(); };               // 0x008650E0

struct CWorld {
    static unsigned long GetWorldFrame();               // 0x0049D870 (reads the global frame; called through the world)
};
struct CThingManager {
    unsigned char _pad00[0x54];
    bool          NonScriptedEntitesPauseMode;          // +0x54
    bool          AllEntitesPauseMode;                  // +0x55
};
extern CThingManager* g_pThingManager;                  // 0x013B8A1C
extern long g_framesPerSecond;                          // 0x01375550

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};

struct CThing {
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];                    // +0x20
    CInterfaceMap  m_interfaces;                        // +0x44
    unsigned char  _pad4c[0x60 - 0x4c];
    CTCPhysicsBase* PhysicsTC;                          // +0x60
    unsigned char  _pad64[0x91 - 0x64];
    unsigned char  Dead : 1;                            // +0x91 bit 0
    const CWorld* PeekWorld() const;                    // 0x004C7AB0
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    __forceinline CTCBase* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        CInterfaceMap::SEntry* end = map->last;
        return (it == end || type < it->type ? end : it)->component;
    }
};

struct CThingGameObject : CThing {
    bool WorldUpdate();                                 // 0x00838BA0
};

struct CThingCreatureBase;
struct CCreatureActionBase {
    virtual void v00();
    unsigned char _pad04[0xa8 - 0x04];
};
struct CCreatureAction_UnsheatheItemFromInventory : CCreatureActionBase {
    unsigned char _body[0x10];
    CCreatureAction_UnsheatheItemFromInventory(CThingCreatureBase* owner, CThing* item, CCreatureActionBase* next,
                                               long timeout);   // 0x006A0150
    ~CCreatureAction_UnsheatheItemFromInventory();      // 0x005CC091
};

struct CThingCreatureBase : CThingGameObject {
    unsigned char _pad94[0xf4 - 0x94];
    CIntelligentPointerThing PLastAttackedByCreature;   // +0xf4
    unsigned long WFLastAttackedByCreature;             // +0xfc
    unsigned char _pad100[0x128 - 0x100];
    float         TurnSpeed;                            // +0x128
    unsigned char _pad12c[0x150 - 0x12c];
    CIntelligentPointerThing PItemToUnsheatheAfterCutscene;   // +0x150
    void UpdateBodyReorientation();                     // 0x006622D0
    bool SetCurrentAction(const CCreatureActionBase& action);   // 0x006644F0
    void ClearLastAttackedByCreature();                 // 0x006691C0
    bool WorldUpdate();
};

bool CThingCreatureBase::WorldUpdate()
{
    TurnSpeed = PhysicsTC->GetTurnSpeed();
    if (CThingGameObject::WorldUpdate())
        return true;
    UpdateBodyReorientation();
    if (PLastAttackedByCreature.GetPItem()) {
        const CWorld* world = PeekWorld();
        unsigned long lastAttacked = WFLastAttackedByCreature;
        if (world->GetWorldFrame() - lastAttacked > (unsigned long)(g_framesPerSecond * 300))
            ClearLastAttackedByCreature();
    }
    if (HasTC(0x77))
        ((CTCInterface77*)GetTC(0x77))->Update();
    if (HasTC(0xcd))
        ((CTCInterfaceCD*)GetTC(0xcd))->Update();
    if (PItemToUnsheatheAfterCutscene.GetPItem() && !PItemToUnsheatheAfterCutscene.GetPItem()->Dead
        && !g_pThingManager->NonScriptedEntitesPauseMode && !g_pThingManager->AllEntitesPauseMode) {
        SetCurrentAction(CCreatureAction_UnsheatheItemFromInventory(this, PItemToUnsheatheAfterCutscene.GetPItem(), 0, 150));
        PItemToUnsheatheAfterCutscene = CIntelligentPointerThing(0);
    }
    return false;
}