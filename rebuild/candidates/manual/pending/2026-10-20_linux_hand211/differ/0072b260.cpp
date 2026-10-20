// CTCSpecialAbilities::SetupDrainLifeSpell() -- when the creature targets a live, drainable thing (an enemy, or anything while the creature is a player's), make sure it has a CTCDrainLife component and report success
// Leaf: SetupDrainLifeSpell
// Grade: DIFFER(198v198) -- scheduling only: retail loads the creature into ecx right after the string constructor (before the AddTC pushes, `lea eax` for the name); the lane loads it after the pushes (`lea ecx`); a creature local and an inline accessor give the same
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0072B260, 198 bytes (plain `ret`).
// Callees: the const interface LowerBound @ 0x004365B0 (inlined GetTC, interface 8 = the targeting component), its virtual
// slot 33 GetPTargetedThing (+0x84; CTCTargetingPlayer 0x00802610), AreThingsEnemies(CThing*, CThing*) @ 0x0076D2F0
// (__fastcall), CThing::GetPlayer @ 0x004C7A30 (the player's byte at +0x20a), IsThingLifeDrainable(CThing*) @ 0x007E44E0
// (__fastcall), CCharString(const char*, long) @ 0x0099EBF0 ("CTCDrainLife" @ 0x0123D18C) / ~CCharString @ 0x0099EAE0,
// CThing::AddTC(const CCharString&, bool, CTCParamsBase*) @ 0x004C9D60.
// Layout: as the landed SetupPhysicalShieldSpell 0x0072B0D0 (Creature at retail +0x18); CThing interface bitset +0x20
// (8 = bit 8 of the first word, 0x97 = bit 23 of the fifth), map +0x44, flag byte +0x6c (bit 3), dead flag bit 0 of +0x91.
// Shape: nested tests with one shared `xor al,al` exit; __forceinline HasTC (the second probe is the raw bitset word test)
// and the one-ternary GetTC (the component lands in ecx for the virtual call); the component name is a named local in its own block.
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
    const SEntry* LowerBound(const long* type) const;  // 0x004365B0
};
struct CPlayer {
    unsigned char _pad00[0x20a];
    bool m_flag20a;                                    // +0x20a
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                    // +0x20
    CInterfaceMap m_interfaces;                        // +0x44
    unsigned char _pad4c[0x6c - 0x4c];
    unsigned char m_flags6c;                           // +0x6c
    unsigned char _pad6d[0x91 - 0x6d];
    unsigned char m_flags91;                           // +0x91 bit 0: dead
    __forceinline bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    __forceinline CTCBase* GetTC(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (it == map->last || interfaceType < it->type ? map->last : it)->component;
    }
    CPlayer* GetPlayer() const;                        // 0x004C7A30 (const; 0x004C7A10 is the non-const twin)
    CTCBase* AddTC(const CCharString& name, bool activate, CTCParamsBase* params);   // 0x004C9D60
};
bool __fastcall AreThingsEnemies(CThing* a, CThing* b);   // 0x0076D2F0
bool __fastcall IsThingLifeDrainable(CThing* thing);      // 0x007E44E0
struct CTCTargetingBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32();
    virtual CThing* GetPTargetedThing();               // slot 33 (+0x84)
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
    bool SetupDrainLifeSpell();
};

bool CTCSpecialAbilities::SetupDrainLifeSpell()
{
    if (Creature->HasTC(8)) {
        CThing* target = ((CTCTargetingBase*)Creature->GetTC(8))->GetPTargetedThing();
        if (target && !(target->m_flags91 & 1) && (target->m_flags6c & 8)) {
            if (AreThingsEnemies(Creature, target) || Creature->GetPlayer()->m_flag20a) {
                if (IsThingLifeDrainable(target)) {
                    if (!Creature->HasTC(0x97)) {
                        CCharString name("CTCDrainLife");
                        Creature->AddTC(name, true, 0);
                    }
                    return true;
                }
            }
        }
    }
    return false;
}
