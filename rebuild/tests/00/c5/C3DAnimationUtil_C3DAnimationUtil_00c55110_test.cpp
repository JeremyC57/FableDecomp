// Behaviour test of the option S thin row 0x00C55110 (CSalRenderDisableMMX::CSalRenderDisableMMX, the out-of-line COMDAT copy of the inline
// constructor that the CreateRenderer translation unit calls): the unit's own test, linked against the unit object. The C_HRTF_DISABLED
// checks construct a CSalRenderDisableMMX through that constructor (base constructor 0x00C6E760, then the class's own Process through
// the vtable 0x012B8AC8 it stores). Replaces the HEAD self-contained C3DAnimationUtil mirror.
// Behaviour test of CSalSw3DVoice::CreateRenderer (0x00C55270), linked against the row object, over the shared headers: the CPU is
// detected once; with SSE2 the full-HRTF / HRTF-delay / volume-pan / disabled renderers are the SSE2 classes, without it the MMX / C ones;
// each `new` asks CodaMalloc for the class size (0xC6C / 0x214 / 0x78 / 0x38, 0x66C for the MMX HRTF); a failed allocation gives null
// without running a constructor. The four-tap SIMD constructors are expanded in the row: the test sees the PanDelay base constructor and
// then the SIMD class's own Process / ClearFilterHistory through the vtable the row stored.
#define FABLE_RETAIL_PASS22_CSALSW_CALCPARAMS
#define FABLE_RETAIL_PASS22_CCODA3D_CREATE
#define FABLE_RETAIL_CSALPLATFORM_CREATE_SW_PASS23
#define FABLE_RETAIL_CSALSW3D_CREATE
#define FABLE_RETAIL_CSALPLATFORM_CREATE_MIXER_PASS23
#define FABLE_RETAIL_CSALOUTPUTKS_INLINE_MEMBERS
#define FABLE_RETAIL_CCODAOBJECTBASE_INLINE_DTOR
#define FABLE_RETAIL_CSALRENDER_MEMBERS
#include "engine/CSalSw3DVoice.h"
#include "engine/CSalRender.h"
#include "engine/CSalRenderPanDelaySSE2.h"   // NF447: the HRTF-delay panners (were local models)
#include "engine/CSalRenderPanDelayMMX.h"
#include "engine/CSalRenderDisableSSE2.h"    // NF452: the plain-gain renderers (were local models)
#include "engine/CSalRenderDisableMMX.h"
#include "engine/CSalRenderPanSSE2.h"        // NF452: the volume panners (were local models)
#include "engine/CSalRenderPanMMX.h"
#include <cstdio>
#include <cstdlib>
bool g_cpuDetected = false;
bool g_cpuHasSSE2 = false;
static int g_detects = 0, g_mallocs = 0; static unsigned int g_lastSize = 0; static bool g_failAlloc = false;
static unsigned long g_ctor = 0, g_call = 0;
void DetectCpu() { ++g_detects; g_cpuDetected = true; }
extern "C" void* __cdecl CodaMalloc(unsigned int n) { ++g_mallocs; g_lastSize = n; return g_failAlloc ? 0 : std::malloc(n); }
extern "C" void __cdecl CodaFree(void* p) { std::free(p); }
// The derived renderer classes: CSalRenderPanDelay / SSE2 / MMX (NF447) and CSalRenderDisable / SSE2 / MMX and CSalRenderPan / SSE2 / MMX (NF452) from their headers, the others declared as in the row (local
// models over CSalRender.h, no header yet); their out-of-line members are stubs that record which one ran (the ids are the retail addresses).
class CVtable_012bede0 : public CSalRender {                                // full HRTF renderer (SSE2), 0xC6C bytes
public:
    CVtable_012bede0();                                                     // 0x00C70B00
    virtual ~CVtable_012bede0();                                            // 0x00C70BB0
    virtual void ClearFilterHistory();
    virtual void Process(const short*, int*(&)[4]);
    virtual void SetGain(float, const float*, bool);
    virtual void SetPosition(float, float, float, float, const float*, bool, bool);
    unsigned char m_state[0xc6c - 0x28];
};
class CVtable_012bedcc : public CSalRender {                                // full HRTF renderer (MMX / C), 0x66C bytes
public:
    CVtable_012bedcc();                                                     // 0x00C70120
    virtual ~CVtable_012bedcc();                                            // 0x00C701D0
    virtual void ClearFilterHistory();
    virtual void Process(const short*, int*(&)[4]);
    virtual void SetGain(float, const float*, bool);
    virtual void SetPosition(float, float, float, float, const float*, bool, bool);
    unsigned char m_state[0x66c - 0x28];
};

CSalRender::CSalRender() : m_fGain(0.0f), m_bUpdate(false) { }
CSalRender::~CSalRender() { }
void CSalRender::ClearFilterHistory() { }
#define STUB_CTOR(C, id) C::C() { g_ctor = id; }
#define STUB_DTOR(C) C::~C() { }
#define STUB_PROC(C, id) void C::Process(const short*, int*(&)[4]) { g_call = id; }
#define STUB_GAINPOS(C) void C::SetGain(float, const float*, bool) { } void C::SetPosition(float, float, float, float, const float*, bool, bool) { }
STUB_CTOR(CSalRenderPanDelay, 0x00C6CA20) STUB_DTOR(CSalRenderPanDelay) STUB_PROC(CSalRenderPanDelay, 0x00C6CC60) STUB_GAINPOS(CSalRenderPanDelay)
void CSalRenderPanDelay::ClearFilterHistory() { g_call = 0x00C6CAD0; }
STUB_CTOR(CSalRenderPan, 0x00C6D9D0) STUB_DTOR(CSalRenderPan) STUB_PROC(CSalRenderPan, 0x00C6DBA0) STUB_GAINPOS(CSalRenderPan)
STUB_CTOR(CSalRenderDisable, 0x00C6E760) STUB_DTOR(CSalRenderDisable) STUB_PROC(CSalRenderDisable, 0x00C6E8F0) STUB_GAINPOS(CSalRenderDisable)
STUB_CTOR(CVtable_012bede0, 0x00C70B00) STUB_DTOR(CVtable_012bede0) STUB_PROC(CVtable_012bede0, 0x00C70BC0) STUB_GAINPOS(CVtable_012bede0)
void CVtable_012bede0::ClearFilterHistory() { }
STUB_CTOR(CVtable_012bedcc, 0x00C70120) STUB_DTOR(CVtable_012bedcc) STUB_PROC(CVtable_012bedcc, 0x00C701E0) STUB_GAINPOS(CVtable_012bedcc)
void CVtable_012bedcc::ClearFilterHistory() { }
STUB_PROC(CSalRenderPanDelaySSE2, 0x00C6D630) void CSalRenderPanDelaySSE2::ClearFilterHistory() { g_call = 0x00C6D560; }
STUB_PROC(CSalRenderPanDelayMMX, 0x00C6F3E0) void CSalRenderPanDelayMMX::ClearFilterHistory() { g_call = 0x00C6F300; }
STUB_PROC(CSalRenderPanSSE2, 0x00C6E410)
STUB_PROC(CSalRenderPanMMX, 0x00C6F990)
STUB_PROC(CSalRenderDisableSSE2, 0x00C6EEF0)
STUB_PROC(CSalRenderDisableMMX, 0x00C6FE10)
// CreateRenderer is private: the test reaches it through CSalSw3DVoice::Create (row 0x00C556E0, not linked here), defined as a test entry.
static CHRTFMODE g_entryMode; static CSalRender* g_result;
int CSalSw3DVoice::Create(const CCodaSource*) { g_result = CreateRenderer(g_entryMode); return 0; }
static unsigned char g_voice[0x248];
static CSalRender* Make(int mode)
{
    g_ctor = 0; g_call = 0; g_lastSize = 0; g_entryMode = static_cast<CHRTFMODE>(mode);
    reinterpret_cast<CSalSw3DVoice*>(g_voice)->CSalSw3DVoice::Create(0);
    return g_result;
}
static bool Check(int mode, unsigned int size, unsigned long ctor, unsigned long process, unsigned long clear)
{
    CSalRender* r = Make(mode);
    if (!r || g_lastSize != size || g_ctor != ctor) return false;
    int* out[4] = { 0, 0, 0, 0 };
    r->Process(0, out);
    bool ok = g_call == process;
    g_call = 0; r->ClearFilterHistory(); ok = ok && g_call == clear;
    std::free(r);
    return ok;
}
int main()
{
    bool ok = true;
    // MMX / C (no SSE2); the CPU is detected on the first call only
    ok = ok && Check(C_HRTF_FULL, 0x66c, 0x00C70120, 0x00C701E0, 0) && g_detects == 1;
    ok = ok && Check(C_HRTF_DELAY, 0x214, 0x00C6CA20, 0x00C6F3E0, 0x00C6F300) && g_detects == 1;
    ok = ok && Check(C_HRTF_VOLPAN, 0x78, 0x00C6D9D0, 0x00C6F990, 0);
    ok = ok && Check(C_HRTF_DISABLED, 0x38, 0x00C6E760, 0x00C6FE10, 0);
    ok = ok && Check(7, 0x38, 0x00C6E760, 0x00C6FE10, 0);
    // SSE2
    g_cpuHasSSE2 = true;
    ok = ok && Check(C_HRTF_FULL, 0xc6c, 0x00C70B00, 0x00C70BC0, 0);
    ok = ok && Check(C_HRTF_DELAY, 0x214, 0x00C6CA20, 0x00C6D630, 0x00C6D560);
    ok = ok && Check(C_HRTF_VOLPAN, 0x78, 0x00C6D9D0, 0x00C6E410, 0);
    ok = ok && Check(C_HRTF_DISABLED, 0x38, 0x00C6E760, 0x00C6EEF0, 0) && g_detects == 1;
    // a failed allocation: null, no constructor
    g_failAlloc = true; int before = g_mallocs;
    ok = ok && Make(C_HRTF_DELAY) == 0 && g_ctor == 0 && g_mallocs == before + 1 && g_lastSize == 0x214;
    if (!ok) { std::printf("00c55110_TEST FAIL\n"); return 1; }
    std::printf("00c55110_TEST PASS\n");
    return 0;
}
