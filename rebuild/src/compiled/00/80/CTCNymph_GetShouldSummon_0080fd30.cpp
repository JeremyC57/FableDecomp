// CTCNymph::GetShouldSummon() const -- ready to summon and the parent's summon component (type 0xfa) reports a positive creature count
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0080FD30, length 85 bytes.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020, CTCAISummon::GetMaxCreatureCount() @ 0x007FDF60.
// Retail CTCNymph: ReadyToSummon +0x5a (PDB +0x5e: the spline's vector is 4 bytes narrower in retail).
// Shape: the inline out-parameter GetInterface(&summon, 0xfa) (bit 26 of the bitset word at +0x3c) inside the flag test, both
// false exits sharing `xor al,al`; the count test is a `return count > 0;` bool (`xor ecx,ecx ; setg cl ; mov al,cl`).
struct CTCAISummon {
    long GetMaxCreatureCount();                     // 0x007FDF60
};

struct CInterfaceMap {
    struct SEntry { long type; void* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);           // 0x0040F020
};

struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                 // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                     // +0x44
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

struct CTCBase_0080fd30 {
    virtual void v00();
    CThing*       ParentThing;                      // +0x04
    unsigned char _pad08[4];
};

struct CTCNymph : CTCBase_0080fd30 {
    unsigned char _pad0c[0x5a - 0x0c];
    bool          ReadyToSummon;                    // +0x5a
    bool GetShouldSummon();
};

bool CTCNymph::GetShouldSummon()
{
    if (ReadyToSummon) {
        CTCAISummon* summon;
        if (ParentThing->GetInterface(&summon, 0xfa))
            return summon->GetMaxCreatureCount() > 0;
    }
    return false;
}