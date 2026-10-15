// CTCChest::vfunc_4() -- while the chest is open, clears the flag byte (+0x10) of the parent thing's interface-9 component when the thing has that interface
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00750730, length 67 bytes (+ int3 padding). Vtable 0x0126957C
// (CTCChest) slot 4.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (landed callers).
// Layout: ChestOpen +0x30 (Ego_r.pdb +0x34 less the 4 bytes of the retail CTCContainer vector),
// ParentThing +4 (CTCBase).
// Shape: the thing's inline out-parameter GetInterface(type, &out) of the landed 0x007504E0 body (same
// module, same interface): the presence bitset at +0x20 is tested on the raw masked word (type 9 ->
// `test ch,2`), the sorted interface map at +0x44 is searched with LowerBound (end or a larger key ->
// the end entry), the component goes to the out pointer (eax); the type key lives in the `push ecx`
// frame slot and the map pointer is a named local (esi). The ChestOpen guard is the outer `if`.
struct CTCBase;

struct CInterfaceMap {                      // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);   // 0x0040F020
};

struct CTCBase {
    unsigned char _pad[0x10];
    bool          m_flag;                   // +0x10
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;             // +0x44
    __forceinline bool GetInterface(long type, CTCBase** out)   // the landed GetInterface_0x.. family shape (forced: under the ChestOpen guard VC calls it out of line)
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

struct CTCChest {                           // vtable 0x0126957C
    void*         __vftable;                // +0x00
    CThing*       ParentThing;              // +0x04 (CTCBase)
    unsigned char _pad08[0x28];             // +0x08 CTCContainerRewardHero, ChestOpener
    bool          ChestOpen;                // +0x30
    void vfunc_4();
};

void CTCChest::vfunc_4()
{
    if (ChestOpen) {
        CTCBase* component;
        if (ParentThing->GetInterface(9, &component))
            component->m_flag = false;
    }
}
