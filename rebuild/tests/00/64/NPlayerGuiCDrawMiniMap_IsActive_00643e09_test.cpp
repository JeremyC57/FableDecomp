// Behaviour test for 0x00643E09 NPlayerGui::CDrawMiniMap::IsActive() const: the fade decides during a region change, else Active && Alpha > 0 (standalone copy of the logic)
#include <cstdio>
struct Fade { bool Active; };
struct MiniMap { bool Active, RegionChange; unsigned char Alpha; Fade FadeInOut;
    bool IsActive() const { if (RegionChange) return FadeInOut.Active; return Active && Alpha > 0; } };

int main()
{
    MiniMap m; m.Active = true; m.RegionChange = false; m.Alpha = 10; m.FadeInOut.Active = false;
    if (!m.IsActive()) { std::printf("00643e09_TEST FAIL\n"); return 1; }
    m.Alpha = 0; if (m.IsActive()) { std::printf("00643e09_TEST FAIL\n"); return 1; }
    m.Alpha = 5; m.Active = false; if (m.IsActive()) { std::printf("00643e09_TEST FAIL\n"); return 1; }
    m.RegionChange = true; m.FadeInOut.Active = true; if (!m.IsActive()) { std::printf("00643e09_TEST FAIL\n"); return 1; }
    m.FadeInOut.Active = false; m.Active = true; if (m.IsActive()) { std::printf("00643e09_TEST FAIL\n"); return 1; }
    std::printf("00643e09_TEST PASS\n");
    return 0;
}