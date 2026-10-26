#include <cstdio>
#include <cstdlib>
// behaviour test: the CTCHeroExperience ctor sets fishing level 1, zero experience, vectors of 3/10/10 zeroed longs, two
// self-linked list headers, AbleToGainExperience, and takes (and add-refs) the definition manager's hero-stats def
struct CDefBase { long m_count; int destroyed; void Destroy() { ++destroyed; } };
struct CDefPointer {
    CDefBase* Object;
    CDefPointer() : Object(0) {}
    CDefPointer(const CDefPointer& o) : Object(o.Object) { if (Object) ++Object->m_count; }
    ~CDefPointer() { if (Object) { if (--Object->m_count == 0) Object->Destroy(); } }
    CDefPointer& operator=(const CDefPointer& o)
    {
        if (Object != o.Object) {
            CDefBase* object = Object;
            if (object) { if (--object->m_count == 0) object->Destroy(); Object = 0; }
            Object = o.Object;
            if (Object) ++Object->m_count;
        }
        return *this;
    }
};
class CDefinitionManager { public: CDefPointer HeroStatsDef; CDefPointer GetHeroStatsDef() const { return HeroStatsDef; } };
static CDefinitionManager g_dm;
struct CVec {
    long* s; long* f; long* e;
    explicit CVec(unsigned long n) : s(0), f(0), e(0) { s = (long*)std::malloc(n * sizeof(long)); e = s + n; long* p = s; for (; n > 0; --n, ++p) *p = 0; f = p; }
};
struct CNode { CNode* next; CNode* prev; long a, b; };
struct CList { CNode* node; CList() : node(0) { CNode* n = (CNode*)std::malloc(sizeof(CNode)); n->next = n; n->prev = n; node = n; } };
class CTCHeroExperience { public:
    void* ParentThing;
    long FishingLevel, TotalExperiencePoints, ExperiencePointsAvailableToSpend;
    CVec StatExperiencePoints, TrainableStatLevels;
    float ExperienceMultiplierParam, LastExperienceMultiplierParam; long ExperienceAttenuationTimer; bool AbleToGainExperience;
    CList Pickups; CVec ExperienceSpentOnStats; CList AttackHistory;
    CDefPointer PDef, PHeroCombatDef, PHeroStatsDef;
    CDefinitionManager* GetDefinitionManager() const { return &g_dm; }
    CTCHeroExperience(void* parent)
        : ParentThing(parent), FishingLevel(1), TotalExperiencePoints(0), ExperiencePointsAvailableToSpend(0),
          StatExperiencePoints(3), TrainableStatLevels(10), ExperienceMultiplierParam(0.0f), LastExperienceMultiplierParam(0.0f),
          ExperienceAttenuationTimer(0), AbleToGainExperience(true), ExperienceSpentOnStats(10)
    {
        PHeroStatsDef = GetDefinitionManager()->GetHeroStatsDef();
    }
};
static bool zeroed(const CVec& v, long n) { if (v.f - v.s != n || v.e != v.f) return false; for (long i = 0; i < n; ++i) if (v.s[i]) return false; return true; }
int main()
{
    CDefBase stats = { 1, 0 };
    g_dm.HeroStatsDef.Object = &stats;
    int parent;
    CTCHeroExperience* h = new CTCHeroExperience(&parent);
    bool ok = h->ParentThing == &parent && h->FishingLevel == 1 && h->TotalExperiencePoints == 0 && h->ExperiencePointsAvailableToSpend == 0
        && zeroed(h->StatExperiencePoints, 3) && zeroed(h->TrainableStatLevels, 10) && zeroed(h->ExperienceSpentOnStats, 10)
        && h->ExperienceMultiplierParam == 0.0f && h->ExperienceAttenuationTimer == 0 && h->AbleToGainExperience
        && h->Pickups.node->next == h->Pickups.node && h->AttackHistory.node->prev == h->AttackHistory.node
        && h->PDef.Object == 0 && h->PHeroCombatDef.Object == 0 && h->PHeroStatsDef.Object == &stats
        && stats.m_count == 2 && stats.destroyed == 0;
    g_dm.HeroStatsDef.Object = 0;
    if (!ok) { std::printf("006d7170_TEST FAIL\n"); return 1; }
    std::printf("006d7170_TEST PASS\n");
    return 0;
}