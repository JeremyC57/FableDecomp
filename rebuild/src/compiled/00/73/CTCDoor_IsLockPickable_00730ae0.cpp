// CTCDoor::IsLockPickable() const -- a door can be lock-picked when its def allows it, it is closed (or moving), no interface-0xC2 component holds it, and its owner does not belong to a village that forbids it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00730AE0, 286 bytes (+ int3 padding).
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound const @ 0x004365B0 (three inlined GetInterface lookups:
//   0xC2 = word +0x38 bit 2, 0x33 = CTCOwnedEntity, word +0x24 bit 19, 0x23 = CTCVillageMember, word +0x24 bit 3),
//   CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (x3), CTCVillageMember::GetPVillage @ 0x007EE540 (manifest
//   "IS_THING_ALIVE<...>" is propagated; landed 0x00833710 names it).
// Layout: engine/CTCDoor.h (retail offsets): CurrentOpen +0x10, Opening +0x12, Closing +0x13, PDef +0x20; CDoorDef
//   (Ego_r.pdb) LockPickable +0x39; CThing presence bitset +0x20, interface map +0x44; CTCOwnedEntity POwner at the odd
//   offset +0x0d (landed 0x006A8C60); the 0xC2 component's flag +0x0c and thing pointer +0x20, the village's flag +0xb3
//   (names unverified). Local model: the header is a flat pack(1) struct with an opaque def pointer.
// Shape: nested ifs over HasTC + GetTC (the end/larger-key pick as one ternary); the false exits inside the esi region
//   share one `pop esi ; xor al,al` tail, the def/state false exits the outer one; the owner pointer is tested
//   through the member, then fetched again into a named local for the village lookup (two GetPItem calls; a third
//   call appears when the lookup re-reads the member).
class CTCBase;
struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const; // 0x004365B0
};
class CThing;
struct CBaseIntelligentPointer {
    void* __vftable;
    void* m_pItem;
    CThing* GetPItem() const;                   // 0x00A01B50
};
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned long HasTC(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
    template<class T> __forceinline T* GetTC(long wanted) const
    {
        long type = wanted;
        const CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
    }
};
struct CTCInterfaceC2 {                         // interface type 0xC2 component
    unsigned char _pad00[0x0c];
    bool m_active;                              // +0x0c
    unsigned char _pad0d[0x20 - 0x0d];
    CBaseIntelligentPointer m_pThing;           // +0x20
};
#pragma pack(push, 1)
class CTCOwnedEntity { public:                         // TC 0x33
    unsigned char _pad00[0x0d];
    CBaseIntelligentPointer POwner;             // +0x0d
};
#pragma pack(pop)
class CTCVillage { public:
    unsigned char _pad00[0xb3];
    bool m_flagB3;                              // +0xb3
};
class CTCVillageMember { public:                       // TC 0x23
    CTCVillage* GetPVillage() const;            // 0x007EE540
};
class CDoorDef { public:
    unsigned char _pad00[0x39];
    bool LockPickable;                          // +0x39
};
class CTCDoor { public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04 (CTCBase)
    unsigned char _pad08[0x10 - 0x08];
    bool CurrentOpen;                           // +0x10
    bool OldOpen;                               // +0x11
    bool Opening;                               // +0x12
    bool Closing;                               // +0x13
    long LastCreatureFoundWF;                   // +0x14
    long UpdateID;                              // +0x18
    long TriggerType;                           // +0x1c EDoorTriggerType
    const CDoorDef* PDef;                       // +0x20 CDefPointer<CDoorDef const>::Object
    bool IsLockPickable() const;
};

bool CTCDoor::IsLockPickable() const
{
    if (!PDef || !PDef->LockPickable)
        return false;
    if (!Opening && !Closing && CurrentOpen)
        return false;
    if (ParentThing->HasTC(0xc2)) {
        CTCInterfaceC2* tc = ParentThing->GetTC<CTCInterfaceC2>(0xc2);
        if (tc->m_active && tc->m_pThing.GetPItem())
            return false;
    }
    if (ParentThing->HasTC(0x33)) {
        CTCOwnedEntity* owned = ParentThing->GetTC<CTCOwnedEntity>(0x33);
        if (owned->POwner.GetPItem()) {
            CThing* owner = owned->POwner.GetPItem();
            if (owner->HasTC(0x23)) {
                CTCVillage* village = owner->GetTC<CTCVillageMember>(0x23)->GetPVillage();
                if (village && village->m_flagB3)
                    return false;
            }
        }
    }
    return true;
}