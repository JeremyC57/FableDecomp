// CThingObject::vfunc_22() -- false when the thing has interface 26 and that component's +0x10 link is set; otherwise true unless the thing was CreatedByOwnerThingCreation
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00703470, length 85 bytes (vtable 0x01264DCC slot 22).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x004365B0.
// Layout (retail CThing): the interface presence bitset at +0x20, the sorted interface map
// {first, last} at +0x44, the flag byte at +0x91 (bit 0 Dead, bit 1 CreatedByOwnerThingCreation:
// PDB +0xa1). The CThing slot-22 default (0x004C7870) forwards to the driver component.
// Shape: the landed 0x007504E0 out-parameter GetInterface template (presence word tested raw,
// the key in the `push ecx` frame slot, the map in esi, end or a larger key -> the end entry,
// the component through the out pointer); `!bitfield` gives `xor eax,eax ; mov al ; shr eax,1 ;
// not eax ; and eax,1`.
struct CTCBase {
    unsigned char _pad00[0x10];
    void*         m_pLink;                  // +0x10
};
struct CInterfaceMap {                      // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);   // 0x004365B0
};
struct CThing {
    virtual void v00();
    unsigned char _pad04[0x1c];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;             // +0x44
    unsigned char _pad4c[0x45];
    unsigned char Dead : 1;                 // +0x91 bit 0
    unsigned char CreatedByOwnerThingCreation : 1;   // bit 1
    bool GetInterface(long type, CTCBase** out)
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
            long key = type;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            if (it == map->last || type < it->type)
                it = map->last;
            *out = it->component;
            return true;
        }
        return false;
    }
};
struct CThingObject : CThing {
    virtual bool vfunc_22();
};

bool CThingObject::vfunc_22()
{
    CTCBase* component;
    if (GetInterface(26, &component) && component->m_pLink != 0)
        return false;
    return !CreatedByOwnerThingCreation;
}
