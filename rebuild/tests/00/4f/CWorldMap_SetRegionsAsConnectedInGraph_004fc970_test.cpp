// Behaviour test (0x004fc970): CWorldMap::SetRegionsAsConnectedInGraph connects two named regions (both numbers > 0) and refreshes the minimap markers
#include <cstdio>
#include <cstring>
class CCharString { public: const char* Text; CCharString(const char* t) : Text(t) { } };
static int g_connects = 0, g_r1 = 0, g_r2 = 0, g_refresh = 0; static bool g_conn = false, g_force = true;
class CRegionConnectionGraph {
public:
    void SetRegionsAsConnected(long region1, long region2, bool connected) { ++g_connects; g_r1 = region1; g_r2 = region2; g_conn = connected; }
};
class CPlayerGui { public: void UpdateMiniMapMarkers(bool force) { ++g_refresh; g_force = force; } };
static CPlayerGui g_gui;
static CPlayerGui* g_pPlayerGui = &g_gui;
class CWorldMap {
public:
    CRegionConnectionGraph* PRegionGraph;
    long GetRegionNumberFromName(const CCharString& name) const
    {
        if (std::strcmp(name.Text, "Oakvale") == 0) return 3;
        if (std::strcmp(name.Text, "Bowerstone") == 0) return 7;
        return 0;
    }
    void SetRegionsAsConnectedInGraph(const CCharString& region1, const CCharString& region2, bool connected)
    {
        CRegionConnectionGraph* graph = PRegionGraph;
        if (graph)
        {
            long number1 = GetRegionNumberFromName(region1);
            long number2 = GetRegionNumberFromName(region2);
            if (number1 > 0 && number2 > 0)
            {
                graph->SetRegionsAsConnected(number1, number2, connected);
                g_pPlayerGui->UpdateMiniMapMarkers(false);
            }
        }
    }
};
int main()
{
    CRegionConnectionGraph graph;
    CWorldMap wm; wm.PRegionGraph = 0;
    wm.SetRegionsAsConnectedInGraph(CCharString("Oakvale"), CCharString("Bowerstone"), true);   // no graph
    if (g_connects != 0 || g_refresh != 0) { std::printf("004fc970_TEST FAIL\n"); return 1; }
    wm.PRegionGraph = &graph;
    wm.SetRegionsAsConnectedInGraph(CCharString("Oakvale"), CCharString("Nowhere"), true);      // unknown region
    if (g_connects != 0 || g_refresh != 0) { std::printf("004fc970_TEST FAIL\n"); return 1; }
    wm.SetRegionsAsConnectedInGraph(CCharString("Bowerstone"), CCharString("Oakvale"), true);
    if (g_connects != 1 || g_r1 != 7 || g_r2 != 3 || !g_conn || g_refresh != 1 || g_force) { std::printf("004fc970_TEST FAIL\n"); return 1; }
    std::printf("004fc970_TEST PASS\n");
    return 0;
}