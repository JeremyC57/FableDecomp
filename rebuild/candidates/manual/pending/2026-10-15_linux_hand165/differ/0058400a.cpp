// CTCInventory::OnItemSelectedInHand(CThing*, bool*) -- lets the selected item's carried-use component act for the owner, or, for an item without one, sheathes the creature's held item into the inventory
// Leaf: OnItemSelectedInHand
// Grade: DIFFER(156v156) -- register choice only: retail reuses ebx (this, dead in the sheathe arm) for the owner, ours takes esi (the dead item); tried a named owner local, a static __forceinline SheatheIntoInventory(owner) helper, and the inverted if/else (breaks the block layout)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0058400A, 156 bytes; frame-pointer size TU
// (`#pragma optimize("y", off)` + `optimize("s", on)`), `ret 8` (the bool* argument is unused), no EH frame.
// Callees: std::bitset::test @ 0x00410DE0 and CVectorMap::LowerBound @ 0x0040F020 (the item's inline GetInterface,
//          interface 0x0E), CThingCreatureBase::IsPerformingAnyAction() @ 0x006622A0, the component's vtable slots 30
//          (+0x78) and 32 (+0x80, called with the owner), CCreatureAction_SheatheItemToInventory::
//          CCreatureAction_SheatheItemToInventory(CThingCreatureBase*, CCreatureActionBase*, long) @ 0x0069FFD0,
//          CThingCreatureBase::SetCurrentAction(CCreatureActionBase*) @ 0x006644F0, and the action's out-of-line
//          destructor @ 0x0048C950 (manifest "_Dest_val<...>").
// Shape: the item stays in esi so the GetInterface key reuses its dead argument slot; the component is used without
// a null test: `if (!owner->IsPerformingAnyAction() || component->CanUseNow()) component->Use(owner);`; for a
// creature owner (type byte 2) the 0xB0-byte action is a temporary built in the frame and passed straight from the
// ctor's eax.
// Layout: CTCBase ParentThing +0x04; retail CThing: type byte +0x10, interface bitset +0x20, map +0x44.
#pragma optimize("y", off)
#pragma optimize("s", on)

struct CTCBase;

struct CPresenceBits {
    bool test(long bit);                             // 0x00410DE0 (std::bitset::test)
};

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;                                   // +0x00
    SEntry* last;                                    // +0x04
    SEntry* LowerBound(const long* type);            // 0x0040F020
};

struct CThing {
    unsigned char _pad_0x00[0x10];
    unsigned char ThingType;                         // +0x10 (2 = creature)
    unsigned char _pad_0x11[0xF];
    CPresenceBits m_present;                         // +0x20
    unsigned char _pad_0x21[0x23];
    CInterfaceMap m_interfaces;                      // +0x44
    template<class T> __forceinline bool GetInterface(T** out, long wanted)
    {
        if (m_present.test(wanted)) {
            long type = wanted;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || wanted < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
};

struct CCreatureActionBase;

struct CThingCreatureBase : CThing {
    bool IsPerformingAnyAction() const;              // 0x006622A0
    bool SetCurrentAction(CCreatureActionBase* action);   // 0x006644F0
};

struct CCreatureActionBase {                         // retail sizeof 0xA8
    unsigned char _pad_0x00[0xA8];
};

struct CCreatureAction_SheatheItemToInventory : CCreatureActionBase {   // retail sizeof 0xB0
    unsigned char _pad_0xa8[0x8];                    // +0xA8 follow-up action counted pointer
    CCreatureAction_SheatheItemToInventory(CThingCreatureBase* owner, CCreatureActionBase* next, long kind);   // 0x0069FFD0
    ~CCreatureAction_SheatheItemToInventory();       // 0x0048C950
};

struct CTCCarriedUse {                               // interface type 0x0E
    virtual void v00();
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10();
    virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20();
    virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25();
    virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual bool CanUseWhileBusy();                  // slot 30 (+0x78)
    virtual void v31();
    virtual void UseFromInventory(CThingCreatureBase* owner);   // slot 32 (+0x80)
};

struct CTCInventory {
    void*               __vftable;                   // +0x00
    CThingCreatureBase* ParentThing;                 // +0x04 CTCBase

    void OnItemSelectedInHand(CThing* item, bool* handled);
};

void CTCInventory::OnItemSelectedInHand(CThing* item, bool* handled)
{
    CTCCarriedUse* use;
    if (item->GetInterface(&use, 0x0E)) {
        if (!ParentThing->IsPerformingAnyAction() || use->CanUseWhileBusy())
            use->UseFromInventory(ParentThing);
    } else {
        CThingCreatureBase* owner = ParentThing;
        if (owner->ThingType == 2)
            owner->SetCurrentAction(&CCreatureAction_SheatheItemToInventory(owner, 0, 100));
    }
}
