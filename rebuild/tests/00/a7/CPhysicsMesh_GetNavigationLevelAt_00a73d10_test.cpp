#include <cstdio>
class C3DVector { public: float x, y, z; };
struct CContainmentVolume_a2e710 {
    float m_min, m_max;
    bool IsContaining(const C3DVector& p) const { return p.x >= m_min && p.x < m_max; }
};
struct SNavigationLevel { long m_level; CContainmentVolume_a2e710 m_volume; };
class CPhysicsMesh { public:
    SNavigationLevel* m_pLevelsFirst; SNavigationLevel* m_pLevelsLast;
    long GetNavigationLevelAt(const C3DVector& position) const
    {
        long count = m_pLevelsLast - m_pLevelsFirst;
        if (count != 0) {
            for (long i = 0; i < count; ++i) {
                const SNavigationLevel& level = m_pLevelsFirst[i];
                if (level.m_volume.IsContaining(position)) return level.m_level;
            }
        }
        return 0;
    }
};
int main()
{
    SNavigationLevel lv[3];
    lv[0].m_level = 5; lv[0].m_volume.m_min = 0.0f; lv[0].m_volume.m_max = 10.0f;
    lv[1].m_level = 7; lv[1].m_volume.m_min = 5.0f; lv[1].m_volume.m_max = 20.0f;
    lv[2].m_level = 9; lv[2].m_volume.m_min = 30.0f; lv[2].m_volume.m_max = 40.0f;
    CPhysicsMesh m; m.m_pLevelsFirst = lv; m.m_pLevelsLast = lv + 3;
    C3DVector p = { 6.0f, 0.0f, 0.0f };
    if (m.GetNavigationLevelAt(p) != 5) { std::printf("00a73d10_TEST FAIL\n"); return 1; }
    p.x = 15.0f; if (m.GetNavigationLevelAt(p) != 7) { std::printf("00a73d10_TEST FAIL\n"); return 1; }
    p.x = 35.0f; if (m.GetNavigationLevelAt(p) != 9) { std::printf("00a73d10_TEST FAIL\n"); return 1; }
    p.x = 25.0f; if (m.GetNavigationLevelAt(p) != 0) { std::printf("00a73d10_TEST FAIL\n"); return 1; }
    m.m_pLevelsLast = lv; p.x = 6.0f; if (m.GetNavigationLevelAt(p) != 0) { std::printf("00a73d10_TEST FAIL\n"); return 1; }
    std::printf("00a73d10_TEST PASS\n");
    return 0;
}