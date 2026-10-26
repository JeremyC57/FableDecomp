#include <cstdio>
#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif
// Behaviour model of CTCPreCalculatedNavigationRoute::OnInitialActivate (0x00800460)
class CThing { public: int id; };
static CThing g_thing = { 5 };
class CThingManager { public: CThing* GetThingByUID(u64 uid) { return uid == 77 ? &g_thing : 0; } };
static CThingManager g_tm;
struct CPtr { CThing* item; void SetPItem(CThing* p) { item = p; } CThing* GetPItem() const { return item; } };
class CTCPreCalculatedNavigationRoute { public: CPtr PThingToCalculateRouteTo; u64 ThingToCalculateRouteToUID;
    void OnInitialActivate() {
        if (ThingToCalculateRouteToUID) { CPtr tmp; tmp.SetPItem(g_tm.GetThingByUID(ThingToCalculateRouteToUID)); PThingToCalculateRouteTo.SetPItem(tmp.GetPItem()); }
    } };
int main()
{
    CTCPreCalculatedNavigationRoute r = { { 0 }, 77 };
    r.OnInitialActivate();
    if (r.PThingToCalculateRouteTo.GetPItem() != &g_thing) { std::printf("00800460_TEST FAIL\n"); return 1; }
    int dummy; CTCPreCalculatedNavigationRoute z = { { (CThing*)&dummy }, 0 };
    z.OnInitialActivate();
    if (z.PThingToCalculateRouteTo.GetPItem() != (CThing*)&dummy) { std::printf("00800460_TEST FAIL\n"); return 1; }
    std::printf("00800460_TEST PASS\n");
    return 0;
}