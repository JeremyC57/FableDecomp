// Behaviour test of CSalRender::~CSalRender (0x00C6E630), linked against the row object: the destructor leaves the CSalMutex member
// under its own vtable (the inline ~CSalMutex ran on +4) and the object under the CCodaObjectBase root vtable.
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
    CCodaObjectBase root; CSalMutex mutex;
    void* rootVtable = *reinterpret_cast<void**>(&root); void* mutexVtable = *reinterpret_cast<void**>(&mutex);
    static double buf[sizeof(TestRender) / sizeof(double) + 1];
    TestRender* r = ::new (buf) TestRender();
    unsigned char* b = r->Bytes();
    bool constructed = *reinterpret_cast<void**>(b + 4) == mutexVtable && *reinterpret_cast<void**>(b) != rootVtable;
    r->CSalRender::~CSalRender();
    bool ok = constructed && *reinterpret_cast<void**>(b) == rootVtable && *reinterpret_cast<void**>(b + 4) == mutexVtable && g_frees == 0;
    if (!ok) { std::printf("00c6e630_TEST FAIL\n"); return 1; }
    std::printf("00c6e630_TEST PASS\n");
    return 0;
}
