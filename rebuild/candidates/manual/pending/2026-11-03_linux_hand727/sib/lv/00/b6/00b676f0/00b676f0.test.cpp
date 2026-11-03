// Behaviour test for 0x00B676F0 CEngineLandscapeRenderer::Clear() -- clear every map's landscape, skipping null maps / landscapes
#include <cstdio>
class CEngineLandscapeMap { public: int Cleared; void Clear() { ++Cleared; } };
class CEngineMap { public: CEngineLandscapeMap* LandscapeMap; };
struct TArray { CEngineMap** _M_start; CEngineMap** _M_finish; CEngineMap** _M_end;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    CEngineMap*& operator[](unsigned int n) { return _M_start[n]; } };
class CEngineWorldMap { public: TArray MapList; };
static CEngineWorldMap* g_pEngineWorldMap = 0;
class CEngineLandscapeRenderer { public:
    void Clear()
    {
        for (long i = 0; i < (long)g_pEngineWorldMap->MapList.size(); ++i) {
            CEngineMap* map = g_pEngineWorldMap->MapList[i];
            if (map != 0 && map->LandscapeMap != 0)
                map->LandscapeMap->Clear();
        }
    }
};
int main()
{
    CEngineLandscapeMap l0 = { 0 }, l2 = { 0 };
    CEngineMap m0, m1, m2; m0.LandscapeMap = &l0; m1.LandscapeMap = 0; m2.LandscapeMap = &l2;
    CEngineMap* maps[4] = { &m0, 0, &m1, &m2 };
    CEngineWorldMap w; w.MapList._M_start = maps; w.MapList._M_finish = maps; w.MapList._M_end = maps + 4;
    g_pEngineWorldMap = &w;
    CEngineLandscapeRenderer r;
    r.Clear();
    if (l0.Cleared != 0 || l2.Cleared != 0) { std::printf("00b676f0_TEST FAIL\n"); return 1; }
    w.MapList._M_finish = maps + 4;
    r.Clear();
    if (l0.Cleared != 1 || l2.Cleared != 1) { std::printf("00b676f0_TEST FAIL\n"); return 1; }
    std::printf("00b676f0_TEST PASS\n");
    return 0;
}