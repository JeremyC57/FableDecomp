// CAIStateGroup_MoveGoodsBase::FetchGoods(CIntelligentPointer<CThing>, float, vector<long>, CAIFilterBase*, CThing*) -- target the container (or the next crate of its crate stack), walk up to it and face it, then perform the fetch action with the good for the top crate, free the action point, take the crate off the stack and return true; false when the walk fails, the target dies or the action fails
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00915890, length 720 bytes (+ int3 padding), `ret 20h`, `and esp,-8` frame.
// Callees: CBaseIntelligentPointer ctor @ 0x00A01B10 / GetPItem @ 0x00A01B50 / SetPItem @ 0x00A01B90 / dtor @ 0x00A01C10,
// CVectorMap::LowerBound @ 0x0040F020 (GetInterface 0xB5 = CTCCrateStack, bit 21 of +0x34; 0x2D = CTCUserActionPointBase),
// CTCCrateStack::GetNextCrate @ 0x007B6750 / GetNumCrates @ 0x007B6660 / RemoveNextCrate @ 0x007B6790,
// AIAction_MoveToPossiblyBlockedObjectAndFace @ 0x00980770, the fetch action ctor @ 0x00916590 / ~CCreatureActionBase
// @ 0x00693EF0, CCreatureActionBase::GetActionID @ 0x00692620, AIAction_PerformActionAndWait @ 0x0097DB70,
// CTCUserActionPointBase::FreeActionPoint @ 0x007FEF40, free @ 0x00BFEA14 (the by-value vector's storage).
// Layout: local model (PBrain +4; brain PCreature +0x18; CurrentActionID +0x18; retail CThing bitset +0x20, map +0x44, dead bit +0x91;
// STLport vector 12 bytes; the fetch action is CCreatureActionBase (0xa8) + a CIntelligentPointer<CThing> at +0xa8).
// Shape: one shared false tail (success path nested); goods.Start read into a local after the walk; the target item hoisted into a local before the action ctor; the crate-stack lookup is the word-local HasTC in the first test and the masked test (null-checked) in the second.
extern "C" void __cdecl free(void* block);
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    bool HasTC(long interfaceType) const
    {
        unsigned long word = m_presentBits[interfaceType >> 5];
        return ((word >> (interfaceType & 31)) & 1) != 0;
    }
    template<class T> __forceinline bool GetInterfaceWord(T** ppOut, long interfaceType)
    {
        const bool& present = HasTC(interfaceType);
        if (present) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
        }
        return present != 0;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1u << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct CThingCreatureBase : CThing { };
struct CTCCrateStack {                          // interface 0xB5
    CThing* GetNextCrate();                     // 0x007B6750
    long GetNumCrates();                        // 0x007B6660
    CThing* RemoveNextCrate();                  // 0x007B6790
};
struct CTCUserActionPointBase {                 // interface 0x2D
    void FreeActionPoint(long index);           // 0x007FEF40
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(const CThing* item);          // 0x00A01B90
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {     // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    __forceinline CIntelligentPointerThing() : CBaseIntelligentPointer() { }
    __forceinline CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    __forceinline CIntelligentPointerThing(const CIntelligentPointerThing& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
    __forceinline CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem(other.GetPItem()); return *this; }
};
__forceinline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CLongVector {                            // STLport vector<long>, 12 bytes
    long* Start;
    long* Finish;
    long* EndOfStorage;
    ~CLongVector() { if (Start) free(Start); }
};
struct CAIFilterBase;
struct CCreatureActionBase {
    virtual void v00();
    unsigned char _body[0xa4];
    ~CCreatureActionBase();                     // 0x00693EF0
    long GetActionID() const;                   // 0x00692620
};
struct CCreatureAction_FetchGoods : CCreatureActionBase {
    CIntelligentPointerThing PTarget;           // +0xa8
    CCreatureAction_FetchGoods(CThingCreatureBase* creature, CThing* target, long good);   // 0x00916590
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x10];
    long CurrentActionID;                       // +0x18
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
    long AIAction_PerformActionAndWait(CCreatureActionBase* action, bool wait);   // 0x0097DB70
    long AIAction_MoveToPossiblyBlockedObjectAndFace(CIntelligentPointerThing target, long* actionPoint, float distance,
        float faceDistance, CAIFilterBase* filter, CThing* ignore, CThing* other, float speed);   // 0x00980770
};
struct CAIStateGroup_MoveGoodsBase : CAIStateGroupBase {
    bool FetchGoods(CIntelligentPointerThing container, float distance, CLongVector goods, CAIFilterBase* filter, CThing* ignore);
};

bool CAIStateGroup_MoveGoodsBase::FetchGoods(CIntelligentPointerThing container, float distance, CLongVector goods,
    CAIFilterBase* filter, CThing* ignore)
{
    CIntelligentPointerThing target;
    target.SetPItem(container.GetPItem());
    CTCCrateStack* stack = 0;
    if (container.GetPItem()->GetInterfaceWord(&stack, 0xb5))
        target = CIntelligentPointerThing(stack->GetNextCrate());
    long actionPoint;
    if (AIAction_MoveToPossiblyBlockedObjectAndFace(target, &actionPoint, distance, 10.0f, filter, ignore, 0, 1.0f) == 2
            && IS_THING_ALIVE(target)) {
        long* data = goods.Start;
        long index;
        CTCCrateStack* crates;
        if (container.GetPItem()->GetInterface(&crates, 0xb5) && crates)
            index = crates->GetNumCrates() - 1;
        else
            index = 0;
        CThing* item = target.GetPItem();
        CCreatureAction_FetchGoods action(GetCreature(), item, data[index]);
        CurrentActionID = action.GetActionID();
        if (AIAction_PerformActionAndWait(&action, false) == 2) {
            if (IS_THING_ALIVE(target)) {
                CTCUserActionPointBase* point;
                target.GetPItem()->GetInterface(&point, 0x2d);
                point->FreeActionPoint(actionPoint);
            }
            if (container.GetPItem() && stack)
                stack->RemoveNextCrate();
            return true;
        }
    }
    return false;
}
