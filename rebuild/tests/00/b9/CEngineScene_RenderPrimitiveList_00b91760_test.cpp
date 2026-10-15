// Behaviour test for 0x00B91760 (see src/00b91760.cpp): RenderPrimitiveList walking the layer lists and dispatching to the type managers.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#define CHECK(c) do { if (!(c)) { std::printf("00b91760_TEST FAIL\n"); return 1; } } while (0)

struct CEngineSubPrimitiveChain { void* f; void* l; CEngineSubPrimitiveChain() : f(0), l(0) { } };
struct CPrim;
struct CManager { int calls; int result; int lastLayer; int RenderPrimitive(CPrim&, int layer, CEngineSubPrimitiveChain& c) { ++calls; lastLayer = layer; return (c.f == 0 && c.l == 0) ? result : -1; } };
struct CPrim { int Type; unsigned int RenderLayerMask; unsigned char Enable : 1; bool visible; unsigned short LastFrameRendered; CPrim* NextPrimitive; CPrim* NextLayerMask;
    bool IsVisible(int flags) { return visible && flags == 0; } };
static CManager* g_managers[4];
static unsigned short g_frame = 77;
static void RenderList(CPrim* p, int layer)
{
    while (p) {
        if (p->RenderLayerMask & layer) {
            if (p->Enable && p->IsVisible(0)) {
                CManager* manager = g_managers[p->Type];
                CEngineSubPrimitiveChain chain;
                if (manager->RenderPrimitive(*p, layer, chain) == 2) p->LastFrameRendered = g_frame;
            }
            p = p->NextPrimitive;
        }
        else p = p->NextLayerMask;
    }
}
int main()
{
    CManager m0 = { 0, 2, 0 }, m1 = { 0, 1, 0 };
    g_managers[0] = &m0; g_managers[1] = &m1;
    CPrim d = { 0, 4, 1, true, 0, 0, 0 };
    CPrim c = { 1, 4, 1, true, 0, 0, 0 };
    CPrim b = { 0, 2, 1, true, 0, 0, &d };         // not on layer 4: follow NextLayerMask to d
    CPrim a = { 0, 4, 0, true, 0, &b, 0 };          // disabled
    a.NextPrimitive = &b; c.NextPrimitive = 0;
    RenderList(&a, 4);
    CHECK(m0.calls == 1 && m0.lastLayer == 4 && d.LastFrameRendered == 77 && a.LastFrameRendered == 0 && b.LastFrameRendered == 0);
    RenderList(&c, 4);
    CHECK(m1.calls == 1 && c.LastFrameRendered == 0);
    c.visible = false; RenderList(&c, 4);
    CHECK(m1.calls == 1);
    RenderList(0, 4);
    std::printf("00b91760_TEST PASS\n");
    return 0;
}