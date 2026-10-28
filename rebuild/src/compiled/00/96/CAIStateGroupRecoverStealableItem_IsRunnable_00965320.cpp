// CAIStateGroup_RecoverStealableItem::IsRunnable() -- when the creature may recover items, reset the state, find a target point and a stealable item outside its range; runnable when both are alive.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00965320, length 512 bytes (+ int3 padding). Vtable slot 11.
// Callees: CVectorMap::LowerBound const @ 0x004365B0 (inlined GetInterface: 0xaa = bit 10 of +0x34 on the creature, 0xb2
// = bit 18 of +0x34 on the target point; both miss paths read the never-written key slot), CBaseIntelligentPointer
// ctor/SetPItem/GetPItem/dtor @ 0x00A01B10/0x00A01B90/0x00A01B50/0x00A01C10 (CIntelligentPointer<CThing> vtable 0x01238C6C,
// CIntelligentPointer<const CThing> vtable 0x01237F1C), FindPTargetPoint @ 0x009652C0,
// FindPStealableItemOutsideTargetPointRange(const C3DVector&, float) @ 0x00965210, CThing::GetPos @ 0x004C73D0.
// Layout: engine/CAIStateGroup_RecoverStealableItem.h (PStealableItem +0x14, PTargetPoint +0x1c, PPreviouslyCarriedItem
// +0x24, GotActionPoint +0x2c, ActionPointIndex +0x30, ReachedDestination +0x34, PickUpActionID +0x38, NeedsCleanup +0x3c)
// over the landed local CAIStateGroupBase model (the generated header flattens the vptr).
// Shape: one nested `if` chain with a single `return false` (retail pushes edi before the first test; the early exits
// share the `xor al,al` epilogue); IS_THING_ALIVE is `__forceinline` (VC out-lines the fourth expansion otherwise); the
// target point is taken into a named pointer before the range read (retail calls GetPItem before pushing the range);
// the final `&&` returns through the full register (`mov eax,1` / `xor eax,eax`).
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const; // 0x004365B0
};
class C3DVector { public: float x, y, z; };
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPos() const;             // 0x004C73D0
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CBaseIntelligentPointer();                  // 0x00A01B10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {        // CIntelligentPointer<CThing> (vtable 0x01238C6C)
    virtual void v00();
    CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { CThing* item = other.GetPItem(); SetPItem(item); return *this; }
};
struct CIntelligentPointerConstThing : CBaseIntelligentPointer {   // CIntelligentPointer<const CThing> (vtable 0x01237F1C)
    virtual void v00();
    CIntelligentPointerConstThing(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerConstThing& operator=(const CIntelligentPointerConstThing& other) { CThing* item = other.GetPItem(); SetPItem(item); return *this; }
};
__forceinline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CTCStealer {                             // interface 0xaa
    unsigned char _pad[0x69];
    bool CanRecoverItems;                       // +0x69
};
struct CTCTargetPoint {                         // interface 0xb2
    unsigned char _pad[0x0c];
    float Range;                                // +0x0c
};
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Cleanup();
    virtual void OnReplace();
    virtual bool IsRunnable();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;
    unsigned char Enabled;
    unsigned char _pad_0x13;
};
struct CAIStateGroup_RecoverStealableItem : CAIStateGroupBase {
    CIntelligentPointerThing PStealableItem;    // +0x14
    CIntelligentPointerConstThing PTargetPoint; // +0x1c
    CIntelligentPointerThing PPreviouslyCarriedItem;   // +0x24
    bool GotActionPoint;                        // +0x2c
    long ActionPointIndex;                      // +0x30
    bool ReachedDestination;                    // +0x34
    long PickUpActionID;                        // +0x38
    bool NeedsCleanup;                          // +0x3c
    CThing* FindPTargetPoint();                 // 0x009652C0
    CThing* FindPStealableItemOutsideTargetPointRange(const C3DVector& pos, float range);   // 0x00965210
    virtual bool IsRunnable();
};

bool CAIStateGroup_RecoverStealableItem::IsRunnable()
{
    if (!NeedsCleanup) {
        CTCStealer* stealer;
        PBrain->PCreature->GetInterface(&stealer, 0xaa);
        if (stealer->CanRecoverItems) {
            ActionPointIndex = -1;
            GotActionPoint = false;
            ReachedDestination = false;
            PickUpActionID = -1;
            PStealableItem = CIntelligentPointerThing(0);
            PTargetPoint = CIntelligentPointerConstThing(FindPTargetPoint());
            if (IS_THING_ALIVE(PTargetPoint)) {
                CTCTargetPoint* target;
                PTargetPoint.GetPItem()->GetInterface(&target, 0xb2);
                CThing* point = PTargetPoint.GetPItem();
                PStealableItem = CIntelligentPointerThing(FindPStealableItemOutsideTargetPointRange(point->GetPos(), target->Range));
            }
            return IS_THING_ALIVE(PStealableItem) && IS_THING_ALIVE(PTargetPoint);
        }
    }
    return false;
}