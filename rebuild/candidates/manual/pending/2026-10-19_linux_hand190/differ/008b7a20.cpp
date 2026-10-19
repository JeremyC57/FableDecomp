// CCreatureAction_SummonCreatures::Initialise() -- animation SUMMON_CREATURES, base Initialise, spawn the summon def's effect at the owner, then let the owner's CTCAISummon summon its creatures (at the live target when SpawnOnTarget)
// Grade: DIFFER(236v236) -- register swap only: retail this in edi / summon component in esi, ours the reverse (plain, accessor and self-local forms)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008B7A20, length 236 bytes (+ int3 padding), plain `ret` (virtual).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, CCombatActionBase::SetAnimationName @ 0x008574D0,
// CCharString::~CCharString @ 0x0099EAE0, CCombatActionBase::Initialise @ 0x00857520, CBaseIntelligentPointer::GetPItem
// @ 0x00A01B50, CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface 0xFA = CTCAISummon, bit 26 of +0x3c),
// CThing::GetPosition @ 0x004C73D0, Create(long defIndex, const C3DVector&, long) @ 0x006E0880 (__fastcall),
// CTCAISummon::SummonCreatures(C3DVector*) @ 0x007FE040.
// Shape: the summon component is null-initialised before the out-parameter lookup (`xor esi,esi`), then read
// unconditionally; the def's effect index (+0x44 of the def at +0xc) is a `long` (`jle`); the two SummonCreatures calls
// (target position or null) are separate if/else statements with their own epilogues.
// Layout: owner CIntelligentPointer +8, target +0xa8; Ego_r.pdb SpawnOnTarget +0x12c, retail +0x114 (CCombatActionBase).
struct C3DVector { float X, Y, Z; };
struct CTCBase;
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
    unsigned char _pad4c[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
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

CThing* __fastcall Create(long defIndex, const C3DVector& position, long flags);   // 0x006E0880

struct CAISummonDef {
    unsigned char _pad[0x44];
    long          SummonEffect;                 // +0x44
};
struct CTCAISummon {
    unsigned char _pad[0xc];
    CAISummonDef* PDef;                         // +0x0c
    long SummonCreatures(C3DVector* position);  // 0x007FE040
};

struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};

struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};

class CCombatActionBase {
public:
    virtual void Initialise();                  // 0x00857520
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;          // +0x08
    unsigned char           _pad10[0xa8 - 0x10];
    CBaseIntelligentPointer PTarget;            // +0xa8
    unsigned char           _padb0[0x114 - 0xb0];
    void SetAnimationName(const CCharString& name);   // 0x008574D0
    __forceinline CTCAISummon* GetSummon() const
    {
        CTCAISummon* summon = 0;
        PCreature.GetPItem()->GetInterface(&summon, 0xfa);
        return summon;
    }
};

class CCreatureAction_SummonCreatures : public CCombatActionBase {
public:
    bool SpawnOnTarget;                         // +0x114 (PDB +0x12c)
    virtual void Initialise();
};

void CCreatureAction_SummonCreatures::Initialise()
{
    {
        CCharString name("SUMMON_CREATURES", -1);
        SetAnimationName(name);
    }
    CCombatActionBase::Initialise();
    CTCAISummon* summon = GetSummon();
    long effect = summon->PDef->SummonEffect;
    if (effect > 0)
        Create(effect, PCreature.GetPItem()->GetPosition(), 0);
    if (SpawnOnTarget && PTarget.GetPItem() && !(PTarget.GetPItem()->m_flags91 & 1))
        summon->SummonCreatures((C3DVector*)&PTarget.GetPItem()->GetPosition());
    else
        summon->SummonCreatures(0);
}
