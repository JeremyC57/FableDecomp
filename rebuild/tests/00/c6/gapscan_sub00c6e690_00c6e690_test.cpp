// Behaviour test of CSalRender::ClearFilterHistory (0x00C6E690), linked against the row object: the base renderer keeps no filter
// history, so the call through the vtable (TestRender does not override it) changes nothing in the object.
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
    static double buf[sizeof(TestRender) / sizeof(double) + 1];
    TestRender* r = ::new (buf) TestRender();
    unsigned char before[sizeof(TestRender)]; std::memcpy(before, buf, sizeof(TestRender));
    static_cast<CSalRender*>(r)->ClearFilterHistory();
    bool ok = std::memcmp(before, buf, sizeof(TestRender)) == 0 && r->Gain() == 0.0f && !r->Update();
    if (!ok) { std::printf("FAIL_00c6e690\n"); return 1; }
    std::printf("PASS_00c6e690\n");
    return 0;
}
