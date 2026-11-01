// Test for CEngineSubPrimitiveRenderer::~CEngineSubPrimitiveRenderer (0x00B84390): the global frame-pool pointer is
// cleared, SortBuffer and every one of the 15 sub-primitive lists free their (non-null) buffers, then the CEngineComponent
// base and the primary base run, in that order.
#include <cstdio>
#include <cstdlib>
static char g_log[8]; static int g_n = 0, g_frees = 0;
static void* g_pool = 0;
struct SortElement { unsigned long SortValue; void* SubPrimitive; };
struct Array { SortElement* start; SortElement* finish; SortElement* eos; ~Array() { if (start) { std::free(start); ++g_frees; } } };
struct SubPrimitiveList { Array Elements; int RenderPass, PreRenderPass; bool b[4]; };
struct Primary { virtual ~Primary() { g_log[g_n++] = 'P'; } };
struct EngineComponent { virtual ~EngineComponent() { g_log[g_n++] = 'C'; } void** g; bool r; unsigned char k; };
struct Renderer : Primary, EngineComponent {
    float MaxSortDistance, SortDistanceMultiplier;
    SubPrimitiveList Lists[15];
    Array SortBuffer;
    bool EnableSolidPass, EnableAlphaPass;
    ~Renderer() { g_pool = 0; }
};
int main()
{
    int pool = 0; g_pool = &pool;
    Renderer* r = new Renderer;
    for (int i = 0; i < 15; ++i) { r->Lists[i].Elements.start = (i % 2) ? (SortElement*)std::malloc(8) : 0; }
    r->SortBuffer.start = (SortElement*)std::malloc(16);
    delete r;
    if (g_pool != 0 || g_frees != 7 + 1 || g_n != 2 || g_log[0] != 'C' || g_log[1] != 'P') { std::printf("00b84390_TEST FAIL\n"); return 1; }
    std::printf("00b84390_TEST PASS\n");
    return 0;
}