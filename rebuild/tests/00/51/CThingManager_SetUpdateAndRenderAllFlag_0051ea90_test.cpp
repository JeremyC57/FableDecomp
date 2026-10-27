// Behaviour test for 0051ea90 (plain C++98 model of the reconstructed logic).
#include <cstdio>
#include <new>
class CThingManager { public:
    bool UpdateAndRenderAllFlag; int Refreshes;
    void FillRenderListsWithAllEntities() { ++Refreshes; }
    void SetUpdateAndRenderAllFlag(bool f) { UpdateAndRenderAllFlag = f; FillRenderListsWithAllEntities(); } };
int main() {
    CThingManager m; m.UpdateAndRenderAllFlag = false; m.Refreshes = 0;
    m.SetUpdateAndRenderAllFlag(true); if (!m.UpdateAndRenderAllFlag || m.Refreshes != 1) { std::printf("0051ea90_TEST FAIL\n"); return 1; }
    m.SetUpdateAndRenderAllFlag(false); if (m.UpdateAndRenderAllFlag || m.Refreshes != 2) { std::printf("0051ea90_TEST FAIL\n"); return 1; }
    { std::printf("0051ea90_TEST PASS\n"); return 0; }
}