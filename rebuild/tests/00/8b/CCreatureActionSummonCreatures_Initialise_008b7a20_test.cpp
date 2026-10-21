// CCreatureAction_SummonCreatures::Initialise() -- play SUMMON_CREATURES, run the combat-action Initialise, spawn the summon def's effect at the summoner, then have its AI-summon component summon the creatures at the live target (SpawnOnTarget) or at the default spot
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
// GetInterface: the inline out-parameter lookup picks the entry with the end/hit ternary (`*out = (it == last || k <
//   it->type ? last : it)->component`), which loads the component straight into the register retail uses.
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
            *ppOut = (T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
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
// ---- harness ----
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static char g_anim[64]; static int g_strings = 0, g_baseInits = 0, g_effects = 0, g_summons = 0;
static long g_lastEffect = 0; static const C3DVector* g_lastEffectPos = 0; static bool g_lastFlag = true;
static const C3DVector* g_lastSummonPos = (const C3DVector*)1; static const void* g_summonThis = 0;
CCharString::CCharString(const char* text, long length) { m_pData = (void*)text; ++g_strings; (void)length; }
CCharString::~CCharString() { --g_strings; }
void CCombatActionBase::SetAnimationName(const CCharString& name) { strcpy(g_anim, (const char*)name.m_pData); }
void CCombatActionBase::v00() { }
void CCombatActionBase::Initialise() { ++g_baseInits; }
CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    while (it != last && it->type < *type)
        ++it;
    return it;
}
static C3DVector g_pos[2];
static CThing g_creature, g_target;
const C3DVector& CThing::GetPosition() const { return this == &g_creature ? g_pos[0] : g_pos[1]; }
CThing* __fastcall CreateEffectThing(long type, const C3DVector* position, bool flag)
{ ++g_effects; g_lastEffect = type; g_lastEffectPos = position; g_lastFlag = flag; return 0; }
long CTCAISummon::SummonCreatures(const C3DVector* position) { ++g_summons; g_lastSummonPos = position; g_summonThis = this; return 7; }
static CSummonDef g_def;
static CTCAISummon g_summon;
static CInterfaceMap::SEntry g_entries[3];
int main()
{
    g_def.SummoneeEffect = 42;
    g_summon.PSummonDef = &g_def;
    g_entries[0].type = 0x10; g_entries[0].component = 0;
    g_entries[1].type = 0xfa; g_entries[1].component = (CTCBase*)&g_summon;
    g_entries[2].type = 0x100; g_entries[2].component = 0;
    g_creature.m_interfaces.first = g_entries; g_creature.m_interfaces.last = g_entries + 3;
    g_creature.m_presentBits[0xfa >> 5] = 1UL << (0xfa & 31);
    CCreatureAction_SummonCreatures a;
    a.PCreature.m_pItem = &g_creature;
    a.PTarget.m_pItem = &g_target;
    a.SpawnOnTarget = true;
    a.Initialise();
    CHECK(strcmp(g_anim, "SUMMON_CREATURES") == 0 && g_strings == 0 && g_baseInits == 1);
    CHECK(g_effects == 1 && g_lastEffect == 42 && g_lastEffectPos == &g_pos[0] && g_lastFlag == false);
    CHECK(g_summons == 1 && g_lastSummonPos == &g_pos[1] && g_summonThis == &g_summon);
    g_target.m_deleteFlags = 1;                     // target being deleted: default spot
    a.Initialise();
    CHECK(g_summons == 2 && g_lastSummonPos == 0);
    g_target.m_deleteFlags = 0; a.PTarget.m_pItem = 0;   // no target
    a.Initialise();
    CHECK(g_summons == 3 && g_lastSummonPos == 0);
    a.PTarget.m_pItem = &g_target; a.SpawnOnTarget = false;
    g_def.SummoneeEffect = 0;                       // no effect
    a.Initialise();
    CHECK(g_effects == 3 && g_summons == 4 && g_lastSummonPos == 0 && g_baseInits == 4);
    if (fails == 0) printf("008b7a20_TEST PASS\n");
    return fails;
}