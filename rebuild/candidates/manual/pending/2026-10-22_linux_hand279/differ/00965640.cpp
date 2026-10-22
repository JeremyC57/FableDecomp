// Grade: DIFFER(612v612) -- this/&PStealableItem register swap (retail this=ebx, &PStealableItem=ebp; this restored into ebx after the carry-slot block) plus the temporaries' register picks in the GetActionID/FinishCurrentAction chains
// CAIStateGroup_RecoverStealableItem::OnReplace() -- clean up when the group is replaced: release the stealable item's action point; unless the destination was reached, finish the pick-up action, drop the item (leaving the carrying-goods mode 0x22) and put the previously carried item back in its active slot (out of its passive slot, re-adding mode 4 for a weapon); then reset the bookkeeping
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00965640, length 612 bytes (+ int3 padding), plain `ret` (virtual).
// Callees: CBaseIntelligentPointer ctor/GetPItem/SetPItem/dtor @ 0x00A01B10/0x00A01B50/0x00A01B90/0x00A01C10,
// CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface 0x2D CTCActionPoints, 0x31 CTCCreatureModeManager, 0x46
// CTCCarrying), CTCActionPoints::ReleaseActionPoint @ 0x007FEF40, CThingCreatureBase::IsPerformingAnyAction @ 0x006622A0,
// CCreatureActionBase::GetActionID @ 0x00692620, CThingCreatureBase::FinishCurrentAction @ 0x00662650,
// CTCCarrying::IsCarryingThing @ 0x006A1060 / RemoveThing @ 0x006A19B0 / GetThingInCarrySlot @ 0x006A11F0 /
// RemoveThingInCarrySlot @ 0x006A16A0 / PutThingInCarrySlot @ 0x006A1F80, CTCCreatureModeManager::RemoveMode @ 0x0073ABA0
// / AddMode(long) @ 0x0073AE10, CDefClassBase::GetSubDef<CCarryableDef> @ 0x005CC10E.
// Data: vtable 0x01238C6C (CIntelligentPointer<CThing>).
// Layout: engine/CAIStateGroup_RecoverStealableItem.h (PStealableItem +0x14, PPreviouslyCarriedItem +0x24, GotActionPoint
// +0x2c, ActionPointIndex +0x30, ReachedDestination +0x34, PickUpActionID +0x38, NeedsCleanup +0x3c) flattens the
// pointers to byte arrays, so the model is local; CCarryableDef from Ego_r.pdb.
// Shape: the landed PickUpGoods 0x00848730 model (GetSubDef + LowerBound same-TU stand-ins, unchecked lookups).
struct CThing;
struct CThingObject;
struct C3DVector { float x, y, z; };
struct CTCBase;
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);  // 0x0099EBF0
    ~CCharString();                                   // 0x0099EAE0
};
static __forceinline bool DecRefToZero(long& count) { return --count == 0; }
struct CCarryableDef {
    virtual void v00();
    virtual void Destroy();                           // slot 1
    long RefCount;                                    // +0x04
    unsigned char _pad08[0x28 - 0x08];
    long ActiveCarrySlot;                             // +0x28
    long SecondaryActiveCarrySlot;                    // +0x2c
    long PassiveCarrySlot;                            // +0x30
};
template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) { if (DecRefToZero(Object->RefCount)) Object->Destroy(); } }
    T* operator->() const { return Object; }
    CDefPointer& operator=(const CDefPointer& other)
    {
        if (Object != other.Object) {
            if (Object) {
                if (--Object->RefCount == 0)
                    Object->Destroy();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                ++Object->RefCount;
        }
        return *this;
    }
};
struct CDefinitionManager {
    CDefPointer<CCarryableDef> GetPDefFromGlobalIndex(long index);   // 0x009ADA10 (hidden result pointer)
};
struct CDefClassBase {
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c(); virtual void v10();
    virtual void v14(); virtual void v18(); virtual void v1c(); virtual void v20(); virtual void v24();
    virtual void v28(); virtual void v2c(); virtual void v30(); virtual void v34();
    virtual long* GetSubDefIndex(const CCharString& name);   // slot 14 (+0x38)
    unsigned char _pad04[0x1c - 4];
    CDefinitionManager* m_pManager;                   // +0x1c
    bool GetSubDef(CDefPointer<CCarryableDef>* out); // 0x005CC10E
};
#pragma auto_inline(off)
// Same-TU stand-ins (retail's LTCG knowledge): GetSubDef<CCarryableDef> 0x005CC10E (the landed GetSubDef body,
// 0x00485815's shape) and CVectorMap::LowerBound 0x0040F020 -- VC7.1 then knows neither keeps the address of the def
// pointer / lookup key, so the def stays in ebp and the frame words are shared like retail's.
bool CDefClassBase::GetSubDef(CDefPointer<CCarryableDef>* out)
{
    long* pIndex;
    {
        CCharString name("CCarryableDef");
        pIndex = GetSubDefIndex(name);
    }
    if (pIndex) {
        CDefPointer<CCarryableDef> result = m_pManager->GetPDefFromGlobalIndex(*pIndex);
        *out = result;
        return true;
    }
    return false;
}
#pragma auto_inline(on)
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);             // 0x0040F020
};
#pragma auto_inline(off)
// Same-TU stand-in for CVectorMap::LowerBound 0x0040F020 (its retail body: STLport lower_bound over the 8-byte
// {type, component} entries): compiled above the caller so VC7.1 knows the key's address does not escape, which lets
// the key share its frame word with the AddTC name string and the by-value target copy.
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                   // +0x20
    CInterfaceMap m_interfaces;                       // +0x44
    unsigned char _pad4c[0x70 - 0x4c];
    CDefClassBase* m_pDef;                            // +0x70
    unsigned char _pad74[0x91 - 0x74];
    unsigned char m_flags91;                          // +0x91 bit 0: dead
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CCreatureActionBase {
    long GetActionID() const;                         // 0x00692620
};
struct CThingCreatureBase : CThing {
    unsigned char _pad94[0x100 - 0x94];
    CCreatureActionBase* CurrentAction;               // +0x100
    bool IsPerformingAnyAction();                     // 0x006622A0
    void FinishCurrentAction();                       // 0x00662650
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                  // +4
    CBaseIntelligentPointer();                        // 0x00A01B10
    ~CBaseIntelligentPointer();                       // 0x00A01C10
    CThing* GetPItem() const;                         // 0x00A01B50
    void SetPItem(CThing* item);                      // 0x00A01B90
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {    // vtable 0x01238C6C
    virtual void v00();
    __forceinline CIntelligentPointerThing(CThing* item) { SetPItem(item); }
    __forceinline CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem(other.GetPItem()); return *this; }
};
struct CTCActionPoints {
    void ReleaseActionPoint(long index);              // 0x007FEF40
};
struct CTCCarrying {
    bool IsCarryingThing(CThing* thing);              // 0x006A1060
    void RemoveThing(CThing* thing);                  // 0x006A19B0
    CThing* GetThingInCarrySlot(long slot);           // 0x006A11F0
    void RemoveThingInCarrySlot(long slot, bool immediately);     // 0x006A16A0
    void PutThingInCarrySlot(CThing* thing, long slot, bool immediately);   // 0x006A1F80
};
struct CTCCreatureModeManager {
    void RemoveMode(long mode);                       // 0x0073ABA0
    void AddMode(long mode);                          // 0x0073AE10
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;                    // +0x18
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                                 // +0x04
    unsigned char _pad08[0x0c];
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
};
struct CAIStateGroup_RecoverStealableItem : CAIStateGroupBase {
    CIntelligentPointerThing PStealableItem;          // +0x14
    CIntelligentPointerThing PTargetPoint;            // +0x1c
    CIntelligentPointerThing PPreviouslyCarriedItem;  // +0x24
    bool GotActionPoint;                              // +0x2c
    long ActionPointIndex;                            // +0x30
    bool ReachedDestination;                          // +0x34
    long PickUpActionID;                              // +0x38
    bool NeedsCleanup;                                // +0x3c
    virtual void OnReplace();
};

void CAIStateGroup_RecoverStealableItem::OnReplace()
{
    if (PStealableItem.GetPItem() && !(PStealableItem.GetPItem()->m_flags91 & 1) && GotActionPoint) {
        CTCActionPoints* actionPoints;
        PStealableItem.GetPItem()->GetInterface(&actionPoints, 0x2d);
        actionPoints->ReleaseActionPoint(ActionPointIndex);
        if (!ReachedDestination) {
            if (PickUpActionID != -1 && GetCreature()->IsPerformingAnyAction()
                    && GetCreature()->CurrentAction->GetActionID() == PickUpActionID)
                GetCreature()->FinishCurrentAction();
            CThingCreatureBase* creature = GetCreature();
            CTCCreatureModeManager* modes;
            creature->GetInterface(&modes, 0x31);
            CTCCarrying* carrying;
            creature->GetInterface(&carrying, 0x46);
            if (carrying->IsCarryingThing(PStealableItem.GetPItem())) {
                carrying->RemoveThing(PStealableItem.GetPItem());
                modes->RemoveMode(0x22);
            }
            if (PPreviouslyCarriedItem.GetPItem() && !(PPreviouslyCarriedItem.GetPItem()->m_flags91 & 1)) {
                CDefPointer<CCarryableDef> carryDef;
                CThing* previous = PPreviouslyCarriedItem.GetPItem();
                previous->m_pDef->GetSubDef(&carryDef);
                long activeSlot = carryDef->ActiveCarrySlot;
                long passiveSlot = carryDef->PassiveCarrySlot;
                if (carrying->GetThingInCarrySlot(passiveSlot) == PPreviouslyCarriedItem.GetPItem())
                    carrying->RemoveThingInCarrySlot(passiveSlot, false);
                carrying->PutThingInCarrySlot(PPreviouslyCarriedItem.GetPItem(), activeSlot, true);
                if (PPreviouslyCarriedItem.GetPItem()->m_presentBits[2] & 0x100)
                    modes->AddMode(4);
            }
        }
    }
    ActionPointIndex = -1;
    GotActionPoint = false;
    PickUpActionID = -1;
    PPreviouslyCarriedItem = CIntelligentPointerThing(0);
    NeedsCleanup = false;
}
