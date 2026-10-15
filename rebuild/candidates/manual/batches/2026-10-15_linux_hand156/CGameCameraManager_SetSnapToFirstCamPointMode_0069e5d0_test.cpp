// Behaviour test for 0x0069E5D0: camera manager: build a CSnapToPoint mode on the first available camera point and hand it to SetCameraMode as a fresh counted pointer
#include <cstdio>
#include <cstring>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %%d\n", __LINE__); } } while (0)
struct Thing { int id; };
struct Block { long refs; void* object; };
struct Mode { int kind; void* manager; Thing* point; };
struct CountedMode {
    Mode* data; Block* info;
    CountedMode(Mode* p) : data(p) { if (data) { info = new Block; info->refs = 1; info->object = data; } else info = 0; }
};
struct Manager {
    Thing* firstCamPoint; Mode* current; long refsSeen;
    void SetCameraMode(CountedMode m) { current = m.data; refsSeen = m.info ? m.info->refs : 0; if (m.info) delete m.info; }
};
static void Run()
{
    Thing p; p.id = 9;
    Manager m; m.firstCamPoint = &p; m.current = 0; m.refsSeen = -1;
    Mode* mode = new Mode; mode->kind = 2; mode->manager = &m; mode->point = m.firstCamPoint;
    m.SetCameraMode(CountedMode(mode));
    CHECK(m.current == mode && m.current->kind == 2 && m.current->point == &p && m.current->manager == &m && m.refsSeen == 1);
    m.SetCameraMode(CountedMode(0));
    CHECK(m.current == 0 && m.refsSeen == 0);
    delete mode;
}

int main()
{
    Run();
    if (g_fail) { std::printf("0069e5d0_TEST FAIL\n"); return 1; }
    std::printf("0069e5d0_TEST PASS\n");
    return 0;
}
