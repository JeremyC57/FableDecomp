// CCreatureAction_SummonCreatures::Initialise() -- play SUMMON_CREATURES, run the combat-action Initialise, spawn the summon def's effect at the summoner, then have its AI-summon component summon the creatures at the live target (SpawnOnTarget) or at the default spot
// Grade: DIFFER(236v236) -- register swap only: retail `this` in edi and the summon component / map in esi, ours the reverse (inline-map and named-creature forms move other registers)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008B7A20, length 240 bytes (plain `ret`, virtual).
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CCombatActionBase::SetAnimationName(const
// CCharString&) 0x008574D0, CCombatActionBase::Initialise 0x00857520 (direct base call), CBaseIntelligentPointer::GetPItem
// 0x00A01B50, CVectorMap::LowerBound 0x0040F020 (inline GetInterface 0xFA = CTCAISummon, bit 26 of the present-bits dword
// +0x3C), CThing::GetPosition 0x004C73D0, __fastcall CreateEffectThing(long, const C3DVector*, bool) 0x006E0880,
// CTCAISummon::SummonCreatures(const C3DVector*) 0x007FE040 (result dropped).
// Layout: CTCAISummon PSummonDef +0x0C (PDB); the CSummonDef effect read is retail +0x44 = PDB SummoneeEffect +0x48 (def
// layouts sit 4 bytes lower in retail; if the def base matches the PDB it is SummonEffect); SpawnOnTarget retail +0x114
// (PDB +0x12C); target CIntelligentPointer +0xA8.
// Shape: the summon component pointer is zero-initialised and filled by the unchecked out-parameter GetInterface (the
// absent path dereferences null, as retail does); the two SummonCreatures calls are not merged.
struct C3DVector { float X, Y, Z; };
struct CTCBase;

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

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_deleteFlags;                // +0x91 bit 0: being deleted
    const C3DVector& GetPosition() const;       // 0x004C73D0
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

CThing* __fastcall CreateEffectThing(long type, const C3DVector* position, bool flag);   // 0x006E0880

struct CSummonDef {
    unsigned char _pad0[0x44];
    long          SummoneeEffect;               // +0x44
};

struct CTCAISummon {
    unsigned char _pad0[0x0c];
    CSummonDef*   PSummonDef;                   // +0x0c CDefPointer<const CSummonDef>
    long SummonCreatures(const C3DVector* position);   // 0x007FE040
};

struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};

struct CCombatActionBase {
    virtual void v00();
    virtual void Initialise();                  // 0x00857520
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;          // +0x08
    unsigned char           _pad10[0xa8 - 0x10];
    CBaseIntelligentPointer PTarget;            // +0xa8
    unsigned char           _padb0[0x114 - 0xb0];
    void SetAnimationName(const CCharString& name);   // 0x008574D0
};

struct CCreatureAction_SummonCreatures : CCombatActionBase {
    bool SpawnOnTarget;                         // +0x114
    virtual void Initialise();
};

void CCreatureAction_SummonCreatures::Initialise()
{
    {
        CCharString name("SUMMON_CREATURES", -1);
        SetAnimationName(name);
    }
    CCombatActionBase::Initialise();
    CTCAISummon* summon = 0;
    PCreature.GetPItem()->GetInterface(&summon, 0xfa);
    long effect = summon->PSummonDef->SummoneeEffect;
    if (effect > 0)
        CreateEffectThing(effect, &PCreature.GetPItem()->GetPosition(), false);
    if (SpawnOnTarget && PTarget.GetPItem() && !(PTarget.GetPItem()->m_deleteFlags & 1))
        summon->SummonCreatures(&PTarget.GetPItem()->GetPosition());
    else
        summon->SummonCreatures(0);
}
