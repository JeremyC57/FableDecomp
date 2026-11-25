// Behaviour test (NF445, linked against the row object over engine/CSalRender.h; replaces the HEAD self-contained mirror) of the
// disabled-renderer constructor 0x00C6E760 (CVtable_012bedb8): the CSalRender base is constructed once and the current / previous
// gain-ramp pointers point at the first / second record; the object carries this class's vtable (Process override).
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
struct SGainRamp {                                     // 4 bytes: a Q15 gain and its per-sample step
    unsigned short m_gain;                             // +0x00
    short          m_step;                             // +0x02
};
struct CVtable_012bedb8 : CSalRender {           // vtable 0x012BEDB8
    SGainRamp  m_first;                                // +0x28
    SGainRamp  m_second;                               // +0x2c
    SGainRamp* m_pCurrent;                             // +0x30
    SGainRamp* m_pPrevious;                            // +0x34
    CVtable_012bedb8();
    virtual ~CVtable_012bedb8();                      // 0x00C6E780 (slot 0 of the vtable is its scalar deleting destructor)
    virtual void Process(const short*, int*(&)[4]);   // slots 2-4: own overrides in retail 0x012BEDB8 (SDK signatures;
    virtual void SetGain(float, const float*, bool);  //   the landed method rows still carry their stand-in spellings)
    virtual void SetPosition(float, float, float, float, const float*, bool, bool);
};
CVtable_012bedb8::~CVtable_012bedb8() { ++g_dtors; g_derivedAt = ++g_order; }
void CVtable_012bedb8::Process(const short*, int*(&)[4]) { ++g_clear; }
void CVtable_012bedb8::SetGain(float, const float*, bool) { }
void CVtable_012bedb8::SetPosition(float, float, float, float, const float*, bool, bool) { }
int main()
{
    CVtable_012bedb8* e = new CVtable_012bedb8;
    int* out[4] = { 0, 0, 0, 0 };
    e->Process(0, out);
    bool ok = g_baseCtors == 1 && e->m_pCurrent == &e->m_first && e->m_pPrevious == &e->m_second && (void*)&e->m_second > (void*)&e->m_first
              && g_clear == 1;
    CSalRender* base = e; delete base;
    ok = ok && g_dtors == 1 && g_baseDtors == 1 && g_lastFree == (void*)e;
    if (!ok) { std::printf("00c6e760_TEST FAIL\n"); return 1; }
    std::printf("00c6e760_TEST PASS\n");
    return 0;
}
