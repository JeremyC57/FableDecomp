// CTCSpecialAbilities::SetupPhysicalShieldSpell() -- cancel an active physical shield on the creature (false), or add a fresh CTCPhysicalShieldSpell component (true)
// Leaf: SetupPhysicalShieldSpell
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0072B0D0, length 129 bytes (padded row 144).
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (inlined GetTC, interface 0x93 = bit 19 of CThing+0x30),
// CTCPhysicalShieldSpell 0x0079DCB0 (`[this+0x1c] != 0`, manifest label "C3DMeshInfo::HasPhysicsMesh" is a byte twin),
// CTCPhysicalShieldSpell::CancelMove @ 0x0079E300, CCharString::CCharString(const char*, long) @ 0x0099EBF0 ("CTCPhysicalShieldSpell"
// @ 0x0123C418), CThing::AddTC(const CCharString&, bool, CTCParamsBase*) @ 0x004C9D60, CCharString::~CCharString @ 0x0099EAE0.
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
    CTCBase* AddTC(const CCharString& name, bool activate, CTCParamsBase* params);   // 0x004C9D60
};
struct CTCPhysicalShieldSpell {
    bool IsShieldActive() const;                // 0x0079DCB0
    void CancelMove();                          // 0x0079E300
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
    bool SetupPhysicalShieldSpell();
};

bool CTCSpecialAbilities::SetupPhysicalShieldSpell()
{
    if (Creature->HasTC(0x93)) {
        CTCPhysicalShieldSpell* spell = (CTCPhysicalShieldSpell*)Creature->GetTC(0x93);
        if (spell->IsShieldActive())
            spell->CancelMove();
        return false;
    }
    {
        CCharString name("CTCPhysicalShieldSpell");
        Creature->AddTC(name, true, 0);
    }
    return true;
}
