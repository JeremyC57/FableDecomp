// Behaviour test (NF445, linked against the row object over engine/CSalRender.h; replaces the HEAD self-contained mirror) of the scalar
// deleting destructor 00c70a30 of CVtable_012bedcc (slot 0 of its vtable): delete through a base pointer runs the derived destructor, then the
// CSalRender base destructor, each once and in that order, then frees the object once through the class allocator (CodaFree).
#define FABLE_RETAIL_CSALPLATFORM_CREATE_MIXER_PASS23
#define FABLE_RETAIL_CSALOUTPUTKS_INLINE_MEMBERS
#define FABLE_RETAIL_CCODAOBJECTBASE_INLINE_DTOR
#define FABLE_RETAIL_CSALRENDER_MEMBERS
#include "engine/CSalRender.h"
#include <cstdio>
#include <cstdlib>
static int g_baseCtors = 0, g_baseDtors = 0, g_dtors = 0, g_frees = 0, g_order = 0, g_baseAt = 0, g_derivedAt = 0, g_clear = 0;
static void* g_lastFree = 0;
extern "C" void* __cdecl CodaMalloc(unsigned int n) { return std::malloc(n); }
extern "C" void __cdecl CodaFree(void* p) { ++g_frees; g_lastFree = p; std::free(p); }
CSalRender::CSalRender() : m_fGain(0.0f), m_bUpdate(false) { ++g_baseCtors; }
CSalRender::~CSalRender() { ++g_baseDtors; g_baseAt = ++g_order; }
void CSalRender::ClearFilterHistory() { }

class CVtable_012bedcc : public CSalRender {     // vtable 0x012BEDCC
public:
    CVtable_012bedcc() { }                             // out of line at 0x00C70120 in retail
    virtual ~CVtable_012bedcc();                       // 0x00C701D0
    virtual void ClearFilterHistory();                // slot 1: own override (retail 0x012BEDCC slot 1)
    virtual void Process(const short*, int*(&)[4]);   // slots 2-4: own overrides in retail 0x012BEDCC (SDK signatures;
    virtual void SetGain(float, const float*, bool);  //   the landed method rows still carry their stand-in spellings)
    virtual void SetPosition(float, float, float, float, const float*, bool, bool);
};

CVtable_012bedcc::~CVtable_012bedcc() { ++g_dtors; g_derivedAt = ++g_order; }
void CVtable_012bedcc::ClearFilterHistory() { ++g_clear; }
void CVtable_012bedcc::Process(const short*, int*(&)[4]) { }
void CVtable_012bedcc::SetGain(float, const float*, bool) { }
void CVtable_012bedcc::SetPosition(float, float, float, float, const float*, bool, bool) { }
int main()
{
    CVtable_012bedcc* r = new CVtable_012bedcc;
    CCodaObjectBase* p = r; delete p;
    bool ok = g_baseCtors == 1 && g_dtors == 1 && g_baseDtors == 1 && g_derivedAt == 1 && g_baseAt == 2 && g_frees == 1 && g_lastFree == (void*)r;
    if (!ok) { std::printf("00c70a30_TEST FAIL\n"); return 1; }
    std::printf("00c70a30_TEST PASS\n");
    return 0;
}
