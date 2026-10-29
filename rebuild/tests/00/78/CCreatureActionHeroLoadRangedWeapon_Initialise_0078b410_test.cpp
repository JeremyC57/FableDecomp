// Behaviour test for CCreatureAction_HeroLoadRangedWeapon::Initialise (0x0078B410): base Initialise; with experience (0x68)
// and carrying (0x46), a stats def and the weapon's ability def, inserts (this, (max-min)*level+min) into the CTCHaste
// multipliers (adding CTCHaste when missing) and recalculates; all def references are released.
#include <cstdio>
#include <cstring>
#include <cmath>

class CTCBase;
static int g_baseInit = 0, g_inserts = 0, g_recalcs = 0, g_addTC = 0; static void* g_key = 0; static float g_value = 0;
static char g_addName[16]; static bool g_addActivate = true;
class CCharString { public: const char* s; CCharString(const char* t) : s(t) {} };
class CDefPointeeBase { public:
    mutable long m_refCount; mutable int Destroyed;
    void DestroyDef() const { ++Destroyed; }
    void Release() const { if (--m_refCount == 0) DestroyDef(); }
};
template<class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) {}
    CDefPointer(const CDefPointer& o) : Object(o.Object) { if (Object) ++Object->m_refCount; }
    ~CDefPointer() { if (Object) { Object->Release(); Object = 0; } }
    T* operator->() const { return Object; }
};
class CTCHeroExperience { public: float Level; };
struct CAbilityTable { float GetAbilityMultiplier(const CTCHeroExperience* e) const { return e->Level; } };
class CHeroStatsDef : public CDefPointeeBase { public: CAbilityTable Abilities; };
class CAbilityDef : public CDefPointeeBase { public: float MaxMultiplier, MinMultiplier; };
class CDefClassBase { public:
    long m_refCount; int Destroyed; const CAbilityDef* Ability;
    void Destroy() { ++Destroyed; }
    bool GetSubDef(CDefPointer<const CAbilityDef>* out) { if (!Ability) return false; ++Ability->m_refCount; out->Object = Ability; return true; }
};
struct CDefClassReference {
    CDefClassBase* m_p;
    CDefClassReference(CDefClassBase* p) : m_p(p) { if (m_p) ++m_p->m_refCount; }
    ~CDefClassReference() { if (m_p && --m_p->m_refCount == 0) m_p->Destroy(); }
    CDefClassBase* operator->() const { return m_p; }
};
struct CHasteMultiplier { void* first; float second; CHasteMultiplier(void* k, float v) : first(k), second(v) {} };
struct CHasteMap { void insert(const CHasteMultiplier& v) { g_key = v.first; g_value = v.second; ++g_inserts; } };
class CTCHaste { public: CHasteMap Multipliers; void RecalculateSpeed() { ++g_recalcs; } };
static CTCHaste g_addedHaste;
class CTCParamsBase;
class CTCCarrying;
class CThing { public:
    CDefClassBase* m_pDef; const CTCHeroExperience* Experience; const CTCCarrying* Carrying; CTCHaste* Haste; const CHeroStatsDef* Stats;
    CDefPointer<const CHeroStatsDef> GetHeroStatsDef() { CDefPointer<const CHeroStatsDef> p; p.Object = Stats; if (Stats) ++Stats->m_refCount; return p; }
    CTCBase* AddTC(const CCharString& n, bool activate, const CTCParamsBase*) { std::strcpy(g_addName, n.s); g_addActivate = activate; ++g_addTC; return (CTCBase*)&g_addedHaste; }
    bool GetInterface(const CTCHeroExperience** o, long) const { if (!Experience) return false; *o = Experience; return true; }
    bool GetInterface(const CTCCarrying** o, long) const { if (!Carrying) return false; *o = Carrying; return true; }
    bool GetInterface(CTCHaste** o, long) { if (!Haste) return false; *o = Haste; return true; }
};
static CThing* g_weapon = 0;
class CTCCarrying { public: CThing* GetThingInPrimarySlot() const { return g_weapon; } };
struct CPtr { CThing* m_pItem; CThing* GetPItem() const { return m_pItem; } };
class CCreatureActionBase { public:
    CPtr PCreature;
    void Initialise() { ++g_baseInit; }
    CThing* GetCreature() const { return PCreature.GetPItem(); }
};
class CCreatureAction_HeroLoadRangedWeapon : public CCreatureActionBase { public: void Initialise(); };

void CCreatureAction_HeroLoadRangedWeapon::Initialise()
{
    CCreatureActionBase::Initialise();
    const CTCHeroExperience* experience;
    if (((const CThing*)GetCreature())->GetInterface(&experience, 0x68)) {
        const CTCCarrying* carrying;
        if (((const CThing*)GetCreature())->GetInterface(&carrying, 0x46)) {
            CDefClassReference weaponDef(carrying->GetThingInPrimarySlot()->m_pDef);
            CDefPointer<const CHeroStatsDef> stats = GetCreature()->GetHeroStatsDef();
            CDefPointer<const CAbilityDef> ability;
            if (stats.Object && weaponDef->GetSubDef(&ability)) {
                float level = stats->Abilities.GetAbilityMultiplier(experience);
                float multiplier = (ability->MaxMultiplier - ability->MinMultiplier) * level + ability->MinMultiplier;
                CTCHaste* haste;
                if (!GetCreature()->GetInterface(&haste, 0x8c))
                    haste = (CTCHaste*)GetCreature()->AddTC("CTCHaste", false, 0);
                haste->Multipliers.insert(CHasteMultiplier(this, multiplier));
                haste->RecalculateSpeed();
            }
        }
    }
}

int main()
{
    int ok = 1;
    CHeroStatsDef stats; stats.m_refCount = 1; stats.Destroyed = 0;
    CAbilityDef ability; ability.m_refCount = 1; ability.Destroyed = 0; ability.MaxMultiplier = 3.0f; ability.MinMultiplier = 1.0f;
    CDefClassBase weaponDef; weaponDef.m_refCount = 1; weaponDef.Destroyed = 0; weaponDef.Ability = &ability;
    CThing weapon; weapon.m_pDef = &weaponDef; g_weapon = &weapon;
    CTCHeroExperience exp; exp.Level = 0.25f; CTCCarrying carrying; CTCHaste haste;
    CThing hero; hero.m_pDef = 0; hero.Experience = &exp; hero.Carrying = &carrying; hero.Haste = &haste; hero.Stats = &stats;
    CCreatureAction_HeroLoadRangedWeapon a; a.PCreature.m_pItem = &hero;
    a.Initialise();
    if (g_baseInit != 1 || g_inserts != 1 || g_key != &a || std::fabs(g_value - 1.5f) > 1e-5f || g_recalcs != 1 || g_addTC != 0) ok = 0;
    if (stats.m_refCount != 1 || ability.m_refCount != 1 || weaponDef.m_refCount != 1) ok = 0;
    // no haste: adds CTCHaste (inactive)
    hero.Haste = 0;
    a.Initialise();
    if (g_addTC != 1 || std::strcmp(g_addName, "CTCHaste") != 0 || g_addActivate || g_inserts != 2) ok = 0;
    // no stats def, no ability def, no experience: nothing inserted
    hero.Stats = 0; a.Initialise();
    hero.Stats = &stats; weaponDef.Ability = 0; a.Initialise();
    hero.Experience = 0; a.Initialise();
    if (g_inserts != 2 || g_baseInit != 5 || stats.m_refCount != 1 || weaponDef.m_refCount != 1) ok = 0;
    std::printf("0078b410_TEST %s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}