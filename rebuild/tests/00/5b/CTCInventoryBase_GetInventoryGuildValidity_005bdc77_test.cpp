#include <cstdio>
class CThing { public: long tc; };
static CThing g_things[2]; static int g_n = 0;
class CThingFilter_HasTC { public: long m_tc; CThingFilter_HasTC(long tc) : m_tc(tc) {} };
class CThingSearchTools { public:
    bool GetPFirstLeafThing(const CThingFilter_HasTC& filter, CThing** out)
    {
        for (int i = 0; i < g_n; ++i) if (g_things[i].tc == filter.m_tc) { *out = &g_things[i]; return true; }
        return false;
    }
};
static CThingSearchTools g_tools;
CThingSearchTools* GetThingSearchTools() { return &g_tools; }
struct Inv {
    CThing* pBuildingThing;
    bool GetInventoryGuildValidity()
    {
        CThing* building = 0;
        GetThingSearchTools()->GetPFirstLeafThing(CThingFilter_HasTC(0x6A), &building);
        if (building) { pBuildingThing = building; return true; }
        return false;
    }
};
int main()
{
    Inv inv; CThing sentinel; inv.pBuildingThing = &sentinel;
    g_things[0].tc = 5; g_n = 1;
    if (inv.GetInventoryGuildValidity() || inv.pBuildingThing != &sentinel) { std::printf("005bdc77_TEST FAIL\n"); return 1; }
    g_things[1].tc = 0x6A; g_n = 2;
    if (!inv.GetInventoryGuildValidity() || inv.pBuildingThing != &g_things[1]) { std::printf("005bdc77_TEST FAIL\n"); return 1; }
    std::printf("005bdc77_TEST PASS\n");
    return 0;
}