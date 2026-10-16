// CTCSpecialAbilities::SetupSummonSpell() -- reset the creature's existing summon spell minion position, or add a fresh CTCSummonSpell component; always true
// Grade: DIFFER(116v118) -- component load: ours `mov ecx,[eax+4]`, retail `mov eax,[eax+4] ; mov ecx,eax` before ResetMinionPos
// Leaf: SetupSummonSpell
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0072B330, length 118 bytes (padded row 128).
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (inlined GetTC, interface 0x91 = bit 17 of CThing+0x30),
// CTCSummonSpell::ResetMinionPos @ 0x0079B6F0, CCharString::CCharString(const char*, long) @ 0x0099EBF0 ("CTCSummonSpell" @ 0x0123C378),
// CThing::AddTC(const CCharString&, bool, CTCParamsBase*) @ 0x004C9D60, CCharString::~CCharString @ 0x0099EAE0.
// Layout: PDB CTCSpecialAbilities (56 bytes) has Creature at +0x20, but retail's STLport list UndeadFollowers (+0x14) is one sentinel
// pointer, so Creature sits at +0x18 (local model). CThing: interface bitset +0x20, CVectorMap<type,component> +0x44.
// Shape: HasTC `if (bits & mask) return true; return false;`; the CCharString is a named local (retail `lea ecx ; push ecx`) in its own `{}`
// block so it shares the lookup key's frame word.
class CTCBase;
struct CTCParamsBase;
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
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
    CTCBase* AddTC(const CCharString& name, bool activate, CTCParamsBase* params);   // 0x004C9D60
};
struct CTCSummonSpell {
    void ResetMinionPos();                      // 0x0079B6F0
};
struct CTCSpecialAbilities {
    virtual void v00();
    CThing* ParentThing;                        // +0x04 (CTCBase)
    unsigned char _base08[4];
    bool AbilityMutexLock;                      // +0x0c
    bool UsingBed;                              // +0x0d
    long CurrentAbility;                        // +0x10
    void* UndeadFollowers;                      // +0x14 list sentinel
    CThing* Creature;                           // +0x18 (PDB +0x20)
    bool SetupSummonSpell();
};

bool CTCSpecialAbilities::SetupSummonSpell()
{
    CTCSummonSpell* spell;
    if (Creature->GetInterface(&spell, 0x91)) {
        spell->ResetMinionPos();
        return true;
    }
    {
        CCharString name("CTCSummonSpell");
        Creature->AddTC(name, true, 0);
    }
    return true;
}
