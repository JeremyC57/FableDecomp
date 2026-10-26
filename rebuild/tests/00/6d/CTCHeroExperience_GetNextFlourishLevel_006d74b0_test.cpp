// Behaviour test for 006d74b0: src/006d74b0.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CTCHeroExperience::GetNextFlourishLevel() const
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCHeroExperience::GetNextFlourishLevel() const -- with flourishes enabled in the hero-combat def, the next multiple of the flourish step above the current experience-multiplier level (level = (long)(param / def divisor + 1.0), at least 1); otherwise the next fallback multiplier
// Leaf: GetNextFlourishLevel
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006D74B0, length 146 bytes (+14 int3 padding).
// Ego_r.pdb: ?GetNextFlourishLevel@CTCHeroExperience@@ABEJXZ
// Callees: __ftol2 @ 0x00BFEA70 (the `(long)` of the double), CTCHeroExperience::GetNextExperienceFallbackMultiplier()
// const @ 0x006D7430 (manifest "Combat_GetNextFlourishLevelAsFloat"; tail-jumped), the def's slot 1 on release.
// Shape: the hero-combat def is read through an inline by-value getter, so each use copies the CDefPointer (null-tested AddRef)
// and releases the copy with NO null test (`dec [r+4] ; jne`): one temporary for the enabled test, one named copy for the
// multiplier, one temporary for the divisor. The level adds the double 1.0 (0x01237F00) before the truncation.
// Layout: retail ExperienceMultiplierParam +0x30, PDef +0x54 (CHeroExperienceDef: the level divisor float at +0x64),
// PHeroCombatDef +0x58 (CHeroCombatDef: flourish-enabled byte +0xd0, flourish level step +0xd4).
static __forceinline bool DecRefToZero(long& count) { return --count == 0; }
struct CRefCounted {
    long m_count;
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                     // slot 1
};
struct CDefObject : CDefVirtual, CRefCounted {};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer(const CDefPointer& o) : Object(o.Object) { if (Object) ++Object->m_count; }
    ~CDefPointer() { if (DecRefToZero(Object->m_count)) Object->Destroy(); }
    T* operator->() const { return Object; }
};
class CHeroExperienceDef : public CDefObject { public:
    unsigned char _pad08[0x64 - 0x08];
    float ExperienceMultiplierParamPerLevel;    // +0x64
};
class CHeroCombatDef : public CDefObject { public:
    unsigned char _pad08[0xd0 - 0x08];
    bool  FlourishesEnabled;                    // +0xd0
    unsigned char _padd1[3];
    long  FlourishLevelStep;                    // +0xd4
};
class CThing;
class CTCHeroExperience {
public:
    virtual ~CTCHeroExperience();
    CThing*       ParentThing;                  // +0x04
    unsigned char _pad08[0x30 - 0x08];
    float         ExperienceMultiplierParam;    // +0x30
    unsigned char _pad34[0x54 - 0x34];
    CDefPointer<CHeroExperienceDef> PDef;       // +0x54
    CDefPointer<CHeroCombatDef>     PHeroCombatDef;   // +0x58
    CDefPointer<CHeroCombatDef> GetHeroCombatDef() const { return PHeroCombatDef; }
    long GetNextExperienceFallbackMultiplier() const;     // 0x006D7430
    long GetNextFlourishLevel() const;
};

long CTCHeroExperience::GetNextFlourishLevel() const
{
    if (GetHeroCombatDef()->FlourishesEnabled) {
        CDefPointer<CHeroCombatDef> combatDef = GetHeroCombatDef();
        long level = (long)(ExperienceMultiplierParam / PDef->ExperienceMultiplierParamPerLevel + 1.0);
        if (level < 1)
            level = 1;
        return (level / GetHeroCombatDef()->FlourishLevelStep + 1) * combatDef->FlourishLevelStep;
    }
    return GetNextExperienceFallbackMultiplier();
}

// ---- stubs and driver ----
static int g_destroyed = 0; static long g_fallback = 77; static int g_fallbackCalls = 0;
void CDefVirtual::v00() {}
void CDefVirtual::Destroy() { ++g_destroyed; }
CTCHeroExperience::~CTCHeroExperience() {}
long CTCHeroExperience::GetNextExperienceFallbackMultiplier() const { ++g_fallbackCalls; return g_fallback; }
int main()
{
    static CHeroExperienceDef ed; ed.m_count = 10; ed.ExperienceMultiplierParamPerLevel = 2.0f;
    static CHeroCombatDef cd; cd.m_count = 10; cd.FlourishesEnabled = true; cd.FlourishLevelStep = 3;
    unsigned char raw[sizeof(CTCHeroExperience)]; memset(raw, 0, sizeof(raw));
    CTCHeroExperience* e = (CTCHeroExperience*)raw;   // not constructed: the tested member is non-virtual
    e->PDef.Object = &ed; e->PHeroCombatDef.Object = &cd;
    e->ExperienceMultiplierParam = 9.0f;   // level = 9/2 + 1 = 5 -> (5/3 + 1) * 3 = 6
    CHECK(e->GetNextFlourishLevel() == 6); CHECK(cd.m_count == 10); CHECK(g_fallbackCalls == 0);
    e->ExperienceMultiplierParam = -8.0f;  // level clamps to 1 -> (1/3 + 1) * 3 = 3
    CHECK(e->GetNextFlourishLevel() == 3);
    cd.FlourishesEnabled = false; CHECK(e->GetNextFlourishLevel() == 77); CHECK(g_fallbackCalls == 1); CHECK(cd.m_count == 10);
    CHECK(g_destroyed == 0);
    e->PDef.Object = 0; e->PHeroCombatDef.Object = 0;
    if (fails == 0) printf("006d74b0_TEST PASS\n"); else printf("006d74b0_TEST FAIL\n");
    return fails;
}