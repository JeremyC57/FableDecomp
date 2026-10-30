// CAIStateGroup_StealStealableItem::Update() -- claim a free action point on the stealable item, walk to it (or near the item), face it, pick it up into the inventory (dropping whatever was carried), then carry it to the escape point and destroy it if the escape point says so
// Grade: RELOCATION_MATCH -- retry wave 2 (sec4): the +0x91 dead test through CThing::IsDead() const (Ego_r.pdb
//   ?IsDead@CThing@@QBE_NXZ, a bool accessor) instead of a direct bit test; the bool accessor keeps retail's register
//   choice / test-and-branch shape.
class CTCBase;
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    __forceinline C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    __forceinline C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
};
class CRightHandedSet { public:
    C3DVector Forward;
    C3DVector Up;
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
#pragma auto_inline(off)
// Same-TU stand-in for the binary search 0x0040F020 (reads the map and the key only).
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* f = first;
    long n = last - first;
    while (n > 0) {
        long half = n >> 1;
        if (f[half].type < *type) {
            f += half + 1;
            n -= half + 1;
        } else
            n = half;
    }
    return f;
}
#pragma auto_inline(on)
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91
    bool IsDead() const { return (m_flags91 & 1) != 0; }   // Ego_r.pdb ?IsDead@CThing@@QBE_NXZ
    const C3DVector& GetPosition() const;       // 0x004C73D0
    void Kill(bool instant);                    // 0x004C9B80
    __forceinline bool HasTC(long interfaceType)
    {
        unsigned long word = m_presentBits[interfaceType >> 5];
        return ((word >> (interfaceType & 31)) & 1) != 0;
    }
    __forceinline bool HasInterface(long interfaceType)
    {
        const bool& found = HasTC(interfaceType);
        return found;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        const bool& found = HasTC(interfaceType);
        if (found) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
        }
        return found;
    }
    __forceinline unsigned long HasTCWord(long interfaceType) const
    {
        unsigned long word = m_presentBits[interfaceType >> 5];
        return word & (1u << (interfaceType & 31));
    }
    template<class T> __forceinline void GetInterfaceC(T** ppOut, long interfaceType) const
    {
        if (HasTCWord(interfaceType)) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
        }
    }
};
class CThingCreatureBase : public CThing { public:
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {
    CIntelligentPointerThing(CThing* item);     // 0x004A96F0
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& o);   // 0x004A9760
};
struct CIntelligentPointerConstThing : CBaseIntelligentPointer {
};
bool __fastcall IS_THING_ALIVE(const CIntelligentPointerThing& p);        // 0x004CC340
bool __fastcall IS_THING_ALIVE(const CIntelligentPointerConstThing& p);   // 0x0069CE20
struct CTCUserActionPointBase {
    bool GetFreeActionPoint(CThingCreatureBase* creature, C3DVector* pos, CRightHandedSet* set, long* index);   // 0x007FEDE0
};
struct CTCStealable {                           // interface 0x47
    unsigned char _pad[0x10];
    void* POwnerAction;                         // +0x10
};
class CTCCarrying { public:                            // interface 0x46
    CThing* GetThingInPrimarySlot();            // 0x006A1440
    void RemoveThingFromPrimarySlot(bool drop); // 0x006A1E00
    bool IsCarryingThing(CThing* thing);        // 0x006A1060
};
struct CTCEscapeFlags {                         // interface 0xAA
    unsigned char _pad[0x6b];
    bool DestroyCarried;                        // +0x6B
};
class CCreatureActionBase { public:
    virtual ~CCreatureActionBase();             // Ego_r.pdb ??1CCreatureActionBase@@UAE@XZ (vptr at +0)
    long GetActionID() const;                   // 0x00692620 (Ego_r.pdb ?GetActionID@CCreatureActionBase@@QBEJXZ)
};
class CCreatureAction_PickUpGenericBox : public CCreatureActionBase { public:   // vtable 0x01274BD4 (RTTI), sizeof 0xb0
    unsigned char _data[0xb0 - 4];              // PThingToPickUp CIntelligentPointer<CThing> at +0xa8
    CCreatureAction_PickUpGenericBox(CThingCreatureBase& owner, CThing& thing);   // 0x00843700 (Ego_r.pdb ??0CCreatureAction_PickUpGenericBox@@QAE@AAVCThingCreatureBase@@AAVCThing@@@Z; "ST_PICK_UP_GENERIC_BOX")
    ~CCreatureAction_PickUpGenericBox();        // 0x007120D0 (destroys PThingToPickUp +0xa8, then ~CCreatureActionBase)
};
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
};
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
    void Terminate();                           // 0x008FCF40
    long AIAction_LookAtPos(long repeat, C3DVector pos, long flags, float speed);   // 0x0097D580
    long AIAction_PerformActionAndWait(CCreatureActionBase* action, bool b);        // 0x0097DB70
    long AIAction_MoveToPos(long repeat, C3DVector pos, bool* reached, long a, bool b, bool c, bool d, bool e, long f);   // 0x0097EC00
    long AIAction_MoveToPosProximity(long repeat, C3DVector pos, float prox, bool* reached, long a, bool b, bool c, bool d, long e, bool f, float g);   // 0x0097E890
};
struct CAIStateGroup_StealStealableItem : CAIStateGroupBase {
    CIntelligentPointerThing PStealableItem;            // +0x14
    CIntelligentPointerConstThing PEscapePoint;         // +0x1c
    CIntelligentPointerThing PPreviouslyCarriedItem;    // +0x24
    long ActionPointIndex;                      // +0x2c
    bool GotActionPoint;                        // +0x30
    bool ReachedDestination;                    // +0x31
    long PickUpActionID;                        // +0x34
    bool NeedsCleanup;                          // +0x38
    virtual void Update();
};

void CAIStateGroup_StealStealableItem::Update()
{
    if (!PStealableItem.GetPItem() || PStealableItem.GetPItem()->IsDead()) {
        Terminate();
        return;
    }
    CTCUserActionPointBase* actionPoint;
    PStealableItem.GetPItem()->GetInterface(&actionPoint, 0x2d);
    C3DVector pos;
    CRightHandedSet set;
    if (!actionPoint->GetFreeActionPoint(GetCreature(), &pos, &set, &ActionPointIndex)) {
        Terminate();
        return;
    }
    GotActionPoint = true;
    C3DVector facing = set.Up;
    CTCStealable* stealable;
    PStealableItem.GetPItem()->GetInterface(&stealable, 0x47);
    bool arrived;
    long result;
    do {
        if (!PStealableItem.GetPItem() || PStealableItem.GetPItem()->IsDead()) {
            Terminate();
            return;
        }
        if (stealable->POwnerAction != 0) {
            Terminate();
            return;
        }
        bool reached;
        result = AIAction_MoveToPos(2, pos, &reached, 5, true, true, false, true, 0);
    } while (result == 4);
    if (result == 3) {
        do {
            if (!PStealableItem.GetPItem() || PStealableItem.GetPItem()->IsDead()) {
                Terminate();
                return;
            }
            if (stealable->POwnerAction != 0) {
                Terminate();
                return;
            }
            CThing* item = PStealableItem.GetPItem();
            bool reached;
            result = AIAction_MoveToPosProximity(2, item->GetPosition(), 1.0f, &reached, 5, true, false, true, 0, true, 0.0f);
        } while (result == 4);
        if (IS_THING_ALIVE(PStealableItem)) {
            CThingCreatureBase* creature = GetCreature();
            CThing* item = PStealableItem.GetPItem();
            facing = item->GetPosition() - creature->GetPosition();
        }
    }
    if (!IS_THING_ALIVE(PStealableItem)) {
        Terminate();
        return;
    }
    if (stealable->POwnerAction != 0) {
        Terminate();
        return;
    }
    if (result != 2) {
        Terminate();
        return;
    }
    result = AIAction_LookAtPos(1, GetCreature()->GetPosition() + facing, 0x21, 0.0027777f);
    if (!IS_THING_ALIVE(PStealableItem)) {
        Terminate();
        return;
    }
    if (stealable->POwnerAction != 0) {
        Terminate();
        return;
    }
    if (result != 2) {
        Terminate();
        return;
    }
    CCreatureAction_PickUpGenericBox action(*GetCreature(), *PStealableItem.GetPItem());
    PickUpActionID = action.GetActionID();
    CTCCarrying* carrying;
    GetCreature()->GetInterface(&carrying, 0x46);
    {
        CIntelligentPointerThing previous(carrying->GetThingInPrimarySlot());
        PPreviouslyCarriedItem = previous;
    }
    carrying->RemoveThingFromPrimarySlot(false);
    if (IS_THING_ALIVE(PPreviouslyCarriedItem) && PPreviouslyCarriedItem.GetPItem()->HasInterface(0x48))
        PPreviouslyCarriedItem.GetPItem()->Kill(true);
    long done = AIAction_PerformActionAndWait(&action, false);
    if (IS_THING_ALIVE(PEscapePoint) && IS_THING_ALIVE(PStealableItem) && done == 2
        && carrying->IsCarryingThing(PStealableItem.GetPItem())) {
        C3DVector escapePos = PEscapePoint.GetPItem()->GetPosition();
        if (AIAction_MoveToPosProximity(1, escapePos, 1.0f, &arrived, 5, true, false, true, 0, true, 0.0f) == 2) {
            ReachedDestination = true;
            const CTCEscapeFlags* flags;
            static_cast<const CThing*>(GetCreature())->GetInterfaceC(&flags, 0xaa);
            if (IS_THING_ALIVE(PStealableItem) && flags->DestroyCarried)
                PStealableItem.GetPItem()->Kill(false);
        }
    }
    Terminate();
}