// Behaviour test of CSalRender::CSalRender (0x00C6E6A0), linked against the row object: the update lock's critical section is
// initialised (an unowned section: LockCount -1, usable by Enter / Leave), the gain is 0 and the update flag false.
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
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(CRITICAL_SECTION*);
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(CRITICAL_SECTION*);
int main()
{
    static double buf[sizeof(TestRender) / sizeof(double) + 1];
    std::memset(buf, 0xCD, sizeof(buf));
    TestRender* r = ::new (buf) TestRender();
    unsigned char* b = r->Bytes();
    CRITICAL_SECTION* cs = reinterpret_cast<CRITICAL_SECTION*>(b + 8);
    bool ok = sizeof(CSalRender) == 0x28 && g_renderVtable != 0 && r->Gain() == 0.0f && !r->Update() && b[0x24] == 0
        && *reinterpret_cast<long*>(b + 0x20) == 0 && cs->LockCount == -1 && cs->RecursionCount == 0;
    EnterCriticalSection(cs); ok = ok && cs->RecursionCount == 1; LeaveCriticalSection(cs);
    if (!ok) { std::printf("00c6e6a0_TEST FAIL\n"); return 1; }
    std::printf("00c6e6a0_TEST PASS\n");
    return 0;
}
