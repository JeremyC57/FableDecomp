// Behaviour test of CSalRender's scalar deleting destructor (0x00C6E6D0, slot 0 of vtable 0x012BEDA4), linked against the row
// object: an object under CSalRender's own vtable (captured between the base and the derived vptr stores) is destroyed through slot 0
// (root vtable restored, the derived destructor never runs) and freed through CodaFree only when bit 0 of the flags is set.
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
class TestRender : public CSalRender, public VtableSpy {
public:
    TestRender() : CSalRender(), VtableSpy(this) { }
    virtual ~TestRender() { ++g_testDtors; }
    virtual void Process(const short*, int*(&)[4]) { }
    virtual void SetGain(float, const float*, bool) { }
    virtual void SetPosition(float, float, float, float, const float*, bool, bool) { }
    float Gain() const { return m_fGain; }
    bool Update() const { return m_bUpdate; }
    unsigned char* Bytes() { return reinterpret_cast<unsigned char*>(static_cast<CSalRender*>(this)); }
};
int main()
{
    CCodaObjectBase root; void* rootVtable = *reinterpret_cast<void**>(&root);
    // flag 0 (virtual destructor call through slot 0): destroy in place, no free
    static double buf[sizeof(TestRender) / sizeof(double) + 1];
    TestRender* r = ::new (buf) TestRender();
    CSalRender* base = r;
    bool ok = g_renderVtable != 0 && g_renderVtable != *reinterpret_cast<void**>(base);
    *reinterpret_cast<void**>(base) = g_renderVtable;
    base->~CSalRender();
    ok = ok && *reinterpret_cast<void**>(buf) == rootVtable && g_frees == 0 && g_testDtors == 0;
    // flag 1 (delete through slot 0): destroyed and freed with CodaFree
    TestRender* h = new TestRender();
    CSalRender* hb = h;
    *reinterpret_cast<void**>(hb) = g_renderVtable;
    delete hb;
    ok = ok && g_mallocs == 1 && g_frees == 1 && g_lastFree == static_cast<void*>(hb) && g_testDtors == 0;
    if (!ok) { std::printf("FAIL_00c6e6d0\n"); return 1; }
    std::printf("PASS_00c6e6d0\n");
    return 0;
}
