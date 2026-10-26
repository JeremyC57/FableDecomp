// Behaviour test for CEnginePrimitiveRenderer::CollectPrimitiveStats @ 0x00B321B0
#include <cstdio>
class C2DBoxF { public: float TLX, TLY, BRX, BRY; };
struct CPrimitiveStatMap { int total; };
static int g_baseDestroyed = 0;
namespace N_0099a300 { struct CBase { virtual void v00() {} ~CBase() { ++g_baseDestroyed; } }; } using N_0099a300::CBase;
struct CEngineScenePrimitiveStatsEnumerator : CBase {
    CPrimitiveStatMap* Destination; C2DBoxF Area; long StatIndex; unsigned int StatFlags;
    CEngineScenePrimitiveStatsEnumerator(CPrimitiveStatMap* destination, const C2DBoxF& area, long statIndex, unsigned int statFlags)
        : Destination(destination), Area(area), StatIndex(statIndex), StatFlags(statFlags) {}
};
class CEngineScene { public:
    int m_primitives;
    void Enumerate(CBase& enumerator)
    {
        CEngineScenePrimitiveStatsEnumerator& e = static_cast<CEngineScenePrimitiveStatsEnumerator&>(enumerator);
        if (e.StatIndex == 2 && e.StatFlags == 7 && e.Area.BRX == 10.0f) e.Destination->total += m_primitives;
    }
};
template <class T> struct CRetailVector {
    T* m_start; T* m_finish; T* m_endOfStorage;
    T* begin() const { return m_start; }
    unsigned int size() const { return (unsigned int)(m_finish - m_start); }
    T& operator[](unsigned int n) { return *(begin() + n); }
};
class CEnginePrimitiveRenderer { public:
    CRetailVector<CEngineScene*> SceneList;
    void CollectPrimitiveStats(CPrimitiveStatMap* destination, const C2DBoxF& area, long statIndex, unsigned int statFlags)
    {
        CEngineScenePrimitiveStatsEnumerator enumerator(destination, area, statIndex, statFlags);
        for (unsigned int i = 0; i < SceneList.size(); ++i)
            SceneList[i]->Enumerate(enumerator);
    }
};
int main()
{
    CEngineScene s[3]; s[0].m_primitives = 1; s[1].m_primitives = 10; s[2].m_primitives = 100;
    CEngineScene* list[3] = { &s[0], &s[1], &s[2] };
    CEnginePrimitiveRenderer r; r.SceneList.m_start = list; r.SceneList.m_finish = list + 3; r.SceneList.m_endOfStorage = list + 3;
    CPrimitiveStatMap stats; stats.total = 0;
    C2DBoxF area = { 0, 0, 10, 10 };
    r.CollectPrimitiveStats(&stats, area, 2, 7);
    if (stats.total != 111 || g_baseDestroyed != 1) { std::printf("00b321b0_TEST FAIL\n"); return 1; }
    std::printf("00b321b0_TEST PASS\n");
    return 0;
}