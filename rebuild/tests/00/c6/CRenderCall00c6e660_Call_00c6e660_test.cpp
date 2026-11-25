// Behaviour test of CSalRender::Initialize (0x00C6E660), linked against the unit object: one call of SetPosition (slot 4) on `this` with
// azimuth / elevation / distance / gain 0, a two-float macro-FX gain record holding zeroes, starting false and update true; nothing else.
#define FABLE_RETAIL_CSALPLATFORM_CREATE_MIXER_PASS23
#define FABLE_RETAIL_CSALOUTPUTKS_INLINE_MEMBERS
#define FABLE_RETAIL_CCODAOBJECTBASE_INLINE_DTOR
#define FABLE_RETAIL_CSALRENDER_MEMBERS
#include "engine/CSalRender.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
static int g_mallocs = 0, g_frees = 0; static void* g_lastFree = 0;
extern "C" void* __cdecl CodaMalloc(unsigned int n) { ++g_mallocs; return std::malloc(n); }
extern "C" void __cdecl CodaFree(void* p) { ++g_frees; g_lastFree = p; std::free(p); }
static int g_testDtors = 0;
static void* g_renderVtable = 0;                        // CSalRender's own vtable, captured between the base and derived vptr stores
struct VtableSpy { explicit VtableSpy(CSalRender* r) { g_renderVtable = *reinterpret_cast<void**>(r); } };
// A concrete renderer over the shared header (CSalRender is abstract); it keeps the base's ClearFilterHistory.
static int g_setPos = 0; static float g_args[4]; static float g_macro[2]; static bool g_starting, g_update; static void* g_this;
// A concrete renderer over the shared header (CSalRender is abstract); its SetPosition records the call.
class TestRender : public CSalRender {
public:
    virtual ~TestRender() { ++g_testDtors; }
    virtual void Process(const short*, int*(&)[4]) { }
    virtual void SetGain(float, const float*, bool) { }
    virtual void SetPosition(float a, float e, float d, float g, const float* m, bool s, bool u)
    { ++g_setPos; g_this = this; g_args[0] = a; g_args[1] = e; g_args[2] = d; g_args[3] = g; g_macro[0] = m[0]; g_macro[1] = m[1]; g_starting = s; g_update = u; }
};
int main()
{
    static double buf[sizeof(TestRender) / sizeof(double) + 1];
    TestRender* r = ::new (buf) TestRender();
    g_args[0] = g_args[1] = g_args[2] = g_args[3] = 9.0f; g_macro[0] = g_macro[1] = 9.0f; g_starting = true; g_update = false;
    static_cast<CSalRender*>(r)->Initialize();
    bool ok = g_setPos == 1 && g_this == r && g_args[0] == 0.0f && g_args[1] == 0.0f && g_args[2] == 0.0f && g_args[3] == 0.0f
              && g_macro[0] == 0.0f && g_macro[1] == 0.0f && !g_starting && g_update;
    if (!ok) { std::printf("00c6e660_TEST FAIL\n"); return 1; }
    std::printf("00c6e660_TEST PASS\n");
    return 0;
}
