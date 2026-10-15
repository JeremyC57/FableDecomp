// CTCContainerRewardHero::HasReward() -- true (1) when the container holds items or the parent thing's gold component (interface 204) holds a positive amount; 0 otherwise
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0074E500, length 96 bytes.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x004365B0 (another instantiation of the
// 0x0040F020 search), CTCGold::GetGold @ 0x007CBDC0.
// Layout (retail): CTCBase::ParentThing +4, CTCContainer::ContainerItems +0xc (vector<pair<long,
// CCharString> >: start +0xc, finish +0x10, 8-byte elements). CThing: the interface presence bitset +0x20
// (type 204 -> word 6 bit 12: `mov edx,[ecx+38h] ; test dh,10h`), the sorted interface map +0x44.
// Shape: the gold is a zero-initialised local (`xor eax,eax` before the presence test) assigned the call
// result inside the landed out-parameter GetInterface template (0x007504E0; the key in the `push ecx`
// frame slot); `size() > 0 || gold > 0` is `sar ecx,3 ; test ; ja` then `test eax,eax ; jg`; a `long`
// result (`mov eax,1` / `xor eax,eax`).
struct CTCBase;

struct CInterfaceMap {                      // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);   // 0x004365B0
};

struct CTCGold;

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;             // +0x44
    bool GetInterface(long type, CTCGold** out)   // inline: the landed GetInterface_0x.. family shape
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
            long key = type;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            if (it == map->last || type < it->type)
                it = map->last;
            *out = (CTCGold*)it->component;
            return true;
        }
        return false;
    }
};

struct CTCGold {
    long GetGold();                         // 0x007CBDC0
};

struct SContainerItem {                     // pair<long, CCharString>: 8 bytes
    long  first;
    void* second;
};

struct CTCContainerRewardHero {
    void*           __vftable;              // +0x00
    CThing*         ParentThing;            // +0x04 (CTCBase)
    unsigned char   _pad08[4];              // +0x08
    SContainerItem* ItemsStart;             // +0x0c ContainerItems
    SContainerItem* ItemsFinish;            // +0x10
    SContainerItem* ItemsEndOfStorage;      // +0x14
    unsigned long ItemCount() const { return ItemsFinish - ItemsStart; }
    long HasReward();
};

long CTCContainerRewardHero::HasReward()
{
    long gold = 0;
    CTCGold* goldComponent;
    if (ParentThing->GetInterface(204, &goldComponent))
        gold = goldComponent->GetGold();
    if (ItemCount() > 0 || gold > 0)
        return 1;
    return 0;
}