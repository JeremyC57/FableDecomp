#include <cstdio>
#include <cstring>
static char g_log[16]; static int g_n = 0;
struct Obj { char tag; virtual ~Obj() { g_log[g_n++] = tag; } };
template <class T> struct sp { T* ptr; void reset(T* p) { T* old = ptr; ptr = p; delete old; } };
struct Mgr { int released; Mgr() : released(0) { } void Release() { ++released; } };
static Mgr* g_inst = 0;
static Mgr& Instance() { if (g_inst == 0) g_inst = new Mgr; return *g_inst; }
static bool Run()
{
    sp<Obj> things; things.ptr = new Obj; things.ptr->tag = 'T';
    sp<Obj> camera; camera.ptr = new Obj; camera.ptr->tag = 'C';
    things.reset(0); camera.reset(0); Instance().Release(); Instance().Release();
    g_log[g_n] = 0; bool ok = std::strcmp(g_log, "TC") == 0 && things.ptr == 0 && camera.ptr == 0 && g_inst->released == 2;
    delete g_inst; return ok;
}
int main()
{
    if (!Run()) { std::printf("004a2e90_TEST FAIL\n"); return 1; }
    std::printf("004a2e90_TEST PASS\n");
    return 0;
}