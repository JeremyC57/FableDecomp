// CTCCombat::SetMeleeFlourishValid(bool valid) -- when the flourish becomes valid for a creature whose hero-combat def allows it, count up and announce it once ("FLOURISH_AVAILABLE" on the thing's sound component); otherwise reset the counter; the block-counter timer is always reset
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00733E10, 208 bytes (`ret 4`).
// Callees: CThing::GetHeroCombatDef() const @ 0x00663340 (CDefPointer by hidden pointer into the `push ecx` word; manifest
// "CEventKilledCreature" is propagated), CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020, CCharString(const char*,
// long) @ 0x0099EBF0, CTCSound::PlaySound(const CCharString&, bool, long, long, long) @ 0x0066BD00 (catalog "CSoundInfo::
// PlaySound"), ~CCharString @ 0x0099EAE0, the def's virtual slot 1 on the last release. Literal "FLOURISH_AVAILABLE" @ 0x012670E8.
// Layout: local model -- engine/CTCCombat.h has PDB offsets; retail (three 12-byte vectors): ParentCreature +0x24, CurrentAbility
// Index +0x44 (PDB +0x4c, as the landed GetPMeleeFlourishAbility 0x00733480), BlockCounterTimer +0x50 (PDB +0x58); CHeroCombatDef
// flag at +0xe1, read as MeleeFlourishUsable (Ego_r.pdb +0xfd, the retail def sitting 0x1c lower -- inferred, not proven);
// CThing sound component = interface 0x10 (bit 16 of the +0x20 word).
// Shape: the def flag folds into a bool preset false, declared first so it lands in bl (`xor bl,bl` before the call); the
// lookup key and the block-scoped name string both live in the dead `valid` argument slot; `++index == 0` (`inc ; jne`) gates the
// announcement; the def pointer's dtor is null-tested and calls the def's inline ReleaseTemporary() over its ref-count base
// at +4 (`mov esi,[eax+4] ; mov ecx,eax ; add eax,4 ; dec esi ; mov edx,esi ; test edx,edx`).
struct CTCBase;
struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length);         // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                             // slot 1
};
struct CHeroCombatDef : CDefVirtual, CRefCounted {
    unsigned char _pad08[0xe1 - 0x08];
    bool MeleeFlourishUsable;                           // +0xe1
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
template <class T> struct CDefPointer {
    T* Object;
    ~CDefPointer()
    {
        if (Object)
            Object->ReleaseTemporary();
    }
    T* operator->() const { return Object; }
};
struct CTCSound {
    unsigned long PlaySound(const CCharString& name, bool positional, long a, long b, long c);   // 0x0066BD00
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                         // +0x44
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
    CDefPointer<CHeroCombatDef> GetHeroCombatDef() const;   // 0x00663340
};
struct CTCCombat {
    virtual void v00();
    CThing* ParentThing;                                // +0x04 (CTCBase)
    unsigned char _pad08[0x24 - 0x08];
    CThing* ParentCreature;                             // +0x24
    unsigned char _pad28[0x44 - 0x28];
    long CurrentAbilityIndex;                           // +0x44 (PDB +0x4c)
    unsigned char _pad48[0x50 - 0x48];
    long BlockCounterTimer;                             // +0x50 (PDB +0x58)
    void SetMeleeFlourishValid(bool valid);
};

void CTCCombat::SetMeleeFlourishValid(bool valid)
{
    bool usable = false;
    CDefPointer<CHeroCombatDef> def = ParentCreature->GetHeroCombatDef();
    if (def.Object && def->MeleeFlourishUsable)
        usable = true;
    if (valid && usable) {
        if (++CurrentAbilityIndex == 0 && ParentThing->HasTC(0x10)) {
            CTCSound* sound = (CTCSound*)ParentThing->GetTC(0x10);
            {
                CCharString name("FLOURISH_AVAILABLE", -1);
                sound->PlaySound(name, true, 1, 0, -1);
            }
        }
    } else {
        CurrentAbilityIndex = -1;
    }
    BlockCounterTimer = -1;
}
