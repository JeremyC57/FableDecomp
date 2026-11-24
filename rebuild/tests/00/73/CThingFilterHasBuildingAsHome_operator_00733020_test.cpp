// Behaviour test for 00733020: NEntityEvents::CIsHitEventHitBy::operator() (NF382 name fix, was CThingFilter_HasBuildingAsHome::operator()),
// GetPHitBy stubbed.
#include <cstdio>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
class CThing { public: int id; };
static CThing* g_hitBy; static int g_calls;
namespace NEntityEvents {
class CEventHitBase { public:
    CThing* GetPHitBy() const { ++g_calls; return g_hitBy; }
};
class CIsHitEventHitBy { public:
    CThing* HitBy;
    bool operator()(const CEventHitBase* event) const;
};
bool CIsHitEventHitBy::operator()(const CEventHitBase* event) const
{
    const CThing* hitBy = event->GetPHitBy();
    return hitBy == HitBy;
}
}
int main()
{
    CThing a, b; NEntityEvents::CEventHitBase event;
    NEntityEvents::CIsHitEventHitBy f; f.HitBy = &a;
    // 1. hit by the filter's thing: true
    g_hitBy = &a; g_calls = 0; CHECK(f(&event) == true); CHECK(g_calls == 1);
    // 2. hit by another thing: false
    g_hitBy = &b; CHECK(f(&event) == false);
    // 3. no hitter: false
    g_hitBy = 0; CHECK(f(&event) == false);
    // 4. a functor with no thing matches the hitter-less event
    f.HitBy = 0; CHECK(f(&event) == true);
    CHECK(g_calls == 4);
    if (fails == 0) printf("00733020_TEST PASS\n");
    else printf("00733020_TEST FAIL (%d)\n", fails);
    return fails;
}
