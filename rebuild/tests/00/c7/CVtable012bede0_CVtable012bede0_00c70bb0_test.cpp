// Behaviour test (NF445, linked against the row object over engine/CSalRender.h; replaces the HEAD self-contained mirror) of the HRTF
// renderer destructor 0x00C70BB0 (CVtable_012bede0): destroying the renderer through a base pointer runs the derived destructor (the row) and
// then the CSalRender base destructor (0x00C6E630) once, and frees the object once.
#define FABLE_RETAIL_CSALPLATFORM_CREATE_MIXER_PASS23
#define FABLE_RETAIL_CSALOUTPUTKS_INLINE_MEMBERS
#define FABLE_RETAIL_CCODAOBJECTBASE_INLINE_DTOR
#include "engine/CSalRender.h"
#include <cstdio>
#include <cstdlib>
static int g_baseCtors = 0, g_baseDtors = 0, g_dtors = 0, g_frees = 0, g_order = 0, g_baseAt = 0, g_derivedAt = 0, g_clear = 0;
static void* g_lastFree = 0;
extern "C" void* __cdecl CodaMalloc(unsigned int n) { return std::malloc(n); }
extern "C" void __cdecl CodaFree(void* p) { ++g_frees; g_lastFree = p; std::free(p); }
CSalRender::~CSalRender() { ++g_baseDtors; g_baseAt = ++g_order; }
void CSalRender::ClearFilterHistory() { }
class CVtable_012bede0 : public CSalRender {        // vtable 0x012BEDE0
public:
    virtual ~CVtable_012bede0();
    virtual void ClearFilterHistory();                // slot 1: own override (retail 0x012BEDE0 slot 1)
    virtual void Process(const short*, int*(&)[4]);   // slots 2-4: own overrides in retail 0x012BEDE0 (SDK signatures;
    virtual void SetGain(float, const float*, bool);  //   the landed method rows still carry their stand-in spellings)
    virtual void SetPosition(float, float, float, float, const float*, bool, bool);
};
void CVtable_012bede0::ClearFilterHistory() { ++g_clear; }
void CVtable_012bede0::Process(const short*, int*(&)[4]) { }
void CVtable_012bede0::SetGain(float, const float*, bool) { }
void CVtable_012bede0::SetPosition(float, float, float, float, const float*, bool, bool) { }
int main()
{
    CVtable_012bede0* r = new CVtable_012bede0;
    r->ClearFilterHistory();
    CCodaObjectBase* p = r; delete p;
    bool ok = g_clear == 1 && g_baseDtors == 1 && g_frees == 1 && g_lastFree == (void*)r;
    if (!ok) { std::printf("00c70bb0_TEST FAIL\n"); return 1; }
    std::printf("00c70bb0_TEST PASS\n");
    return 0;
}
