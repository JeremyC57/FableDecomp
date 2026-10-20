#include <cstdio>
struct Init { long a, b, c, d; };
struct Registry { int count; void* last; void Add(void* p) { ++count; last = p; } };
static Registry g_list = { 0, 0 };
struct MainGameComponent {
    void* parent; void* textBank; void* lua; bool forceUpdateTick; float speedMul; float desiredFps; long failed;
    bool firstWorldFrame; long serverFrame; long inputFrame; long lastForceRendered; double lastRenderStart;
    bool initialised; bool allowRender; bool rendered; long unableToRender; Init init;
    MainGameComponent(void* p, const Init& i)
        : parent(p), textBank(0), lua(0), forceUpdateTick(false), speedMul(3.0f), desiredFps(30.0f), failed(0),
          firstWorldFrame(true), serverFrame(0), inputFrame(0), lastRenderStart(0.0),
          initialised(false), allowRender(true), rendered(false), init(i)
    {
        lastForceRendered = 0;
        g_list.Add(this);
        unableToRender = 0;
    }
};
int main()
{
    Init i = { 1, 2, 3, 4 };
    int parent = 0;
    MainGameComponent c(&parent, i);
    bool ok = c.parent == &parent && c.speedMul == 3.0f && c.desiredFps == 30.0f && c.firstWorldFrame && c.allowRender
        && !c.initialised && c.init.c == 3 && g_list.count == 1 && g_list.last == &c && c.unableToRender == 0 && c.lastRenderStart == 0.0;
    if (!ok) { std::printf("00418dca_TEST FAIL\n"); return 1; }
    std::printf("00418dca_TEST PASS\n");
    return 0;
}