// Behaviour test (NF452, linked against the row object over engine/CSalRenderPan*.h; replaces the HEAD test of the local CVtable_012bed90
// model) of
// CSalRenderPanSSE2::Process 0x00C6E410: the constant-gain path hands the input, each output and its entry's gain to the SSE2 kernel
// (outputs 2 / 3 only with four or more output channels, zero gains skipped); an update clears m_bUpdate, fades the current entries out,
// swaps the sets and fades the new entries in (the hand SSE2 ramp kernel of the row object runs for real).
#define FABLE_RETAIL_CSALPLATFORM_CREATE_MIXER_PASS23
#define FABLE_RETAIL_CSALOUTPUTKS_INLINE_MEMBERS
#define FABLE_RETAIL_CCODAOBJECTBASE_INLINE_DTOR
#define FABLE_RETAIL_CSALRENDER_MEMBERS
#define FABLE_RETAIL_CSALMUTEX_ENTER_LEAVE
#include "engine/CSalRenderPanSSE2.h"
#include "engine/CSalRenderPanMMX.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
static int g_frees = 0, g_baseCtors = 0, g_baseDtors = 0;
extern "C" void* __cdecl CodaMalloc(unsigned int n) { void* p = std::malloc(n); std::memset(p, 0x5a, n); return p; }
extern "C" void __cdecl CodaFree(void* p) { ++g_frees; std::free(p); }
CSalRender::CSalRender() : m_fGain(0.0f), m_bUpdate(false) { ++g_baseCtors; }
CSalRender::~CSalRender() { ++g_baseDtors; }
void CSalRender::ClearFilterHistory() { }
// Retail layout (engine/CSalRenderPan.h): the test reads protected / private members at their retail offsets.
template <class T> static T& At(void* o, unsigned off) { return *reinterpret_cast<T*>(static_cast<char*>(o) + off); }
static FilterPan* Params(void* o, unsigned which) { return At<FilterPan*>(o, which ? 0x5c : 0x58); }
static bool Pan(void* o, unsigned which, int i, unsigned short gain, short delta) { return Params(o, which)[i].gain == gain && Params(o, which)[i].gainDelta == delta; }
static int g_called = 0;
static int g_updates = 0; static bool g_lastNew = false;
struct CAudioMaster { unsigned char _pad[0x1bc]; unsigned long m_outputChannels; };   // the rows' view of g_pSal (+0x1BC output channels)
static CAudioMaster g_master;
CAudioMaster* g_pAudioMaster = &g_master;
CSalRenderPan::CSalRenderPan() { std::memset(&m_paramsA, 0, sizeof(m_paramsA)); std::memset(&m_paramsB, 0, sizeof(m_paramsB)); m_pParamsCurrent = &m_paramsA; m_pParamsNew = &m_paramsB; for (int i = 0; i < 6; ++i) m_fGainPan[i] = 0.0f; }
CSalRenderPan::~CSalRenderPan() { }
void CSalRenderPan::Process(const short*, int*(&)[4]) { g_called = 0xdba0; }   // 0x00C6DBA0 (no row)
void CSalRenderPan::SetGain(float, const float*, bool) { g_called = 0xe020; }
void CSalRenderPan::SetPosition(float, float, float, float, const float*, bool, bool) { g_called = 0xe050; }   // 0x00C6E050 (no row)
void CSalRenderPan::Params3DInit(FilterPan*, float) { g_called = 0xde60; }
void CSalRenderPan::UpdateFilters(const bool bNew) { ++g_updates; g_lastNew = bNew; }
void CSalRenderPanMMX::Process(const short*, int*(&)[4]) { g_called = 0xf990; }
// What the hand ramp kernel adds per sample (measured on the row object): the high word of sample * gain (pmulhw), times two.
static int Ramp(short sample, int gain0, int step, int i) { return ((sample * (short)(gain0 + step * i)) >> 16) * 2; }
static int g_kernels = 0; static const short* g_src[4]; static long* g_dst[4]; static short g_gain[4]; static unsigned long g_count[4];
namespace Row00c6e1f0 { void __cdecl ScaleAccumulate16(const short* src, unsigned long count, long* dst, short gain); }
void __cdecl Row00c6e1f0::ScaleAccumulate16(const short* src, unsigned long count, long* dst, short gain)   // 0x00C6E1F0 (landed kernel, not linked): records the call
{ if (g_kernels < 4) { g_src[g_kernels] = src; g_dst[g_kernels] = dst; g_gain[g_kernels] = gain; g_count[g_kernels] = count; } ++g_kernels; }
int main()
{
    bool ok = true;
    CSalRender* r = new CSalRenderPanSSE2;
    __declspec(align(16)) short src[128]; for (int i = 0; i < 128; ++i) src[i] = (short)(1000 + 8 * i);
    __declspec(align(16)) int o0[128]; __declspec(align(16)) int o1[128]; __declspec(align(16)) int o2[128]; __declspec(align(16)) int o3[128];
    std::memset(o0, 0, sizeof(o0)); std::memset(o1, 0, sizeof(o1)); std::memset(o2, 0, sizeof(o2)); std::memset(o3, 0, sizeof(o3));
    int* out[4] = { o0, o1, o2, o3 };
    FilterPan* cur = Params(r, 0); FilterPan* nw = Params(r, 1);
    cur[0].gain = 0x4000; cur[1].gain = 0x2000; cur[2].gain = 0x1000; cur[3].gain = 0;
    g_master.m_outputChannels = 2;
    r->Process(src, out);   // constant gain: one kernel call per output and entry, the whole 128-sample block (16 groups)
    ok = ok && g_kernels == 2 && g_src[0] == src && g_dst[0] == (long*)o0 && g_dst[1] == (long*)o1 && g_gain[0] == 0x4000 && g_gain[1] == 0x2000;
    ok = ok && g_count[0] == 16 && g_count[1] == 16;
    g_kernels = 0; g_master.m_outputChannels = 4;
    r->Process(src, out);   // entry 3 is silent
    ok = ok && g_kernels == 3 && g_dst[2] == (long*)o2 && g_gain[2] == 0x1000;
    // an update: fade the current entries out (silent here), swap the sets, fade the new entries in with the real ramp kernel
    for (int i = 0; i < 4; ++i) { cur[i].gain = 0; cur[i].gainDelta = 0; }
    nw[0].gain = 0x7000; nw[0].gainDelta = 0x40; nw[1].gain = 0x3000; nw[1].gainDelta = 0x20; nw[2].gain = 0x1000; nw[2].gainDelta = 0x10;
    g_master.m_outputChannels = 2; g_kernels = 0;
    At<bool>(r, 0x24) = true;
    r->Process(src, out);
    ok = ok && !At<bool>(r, 0x24) && Params(r, 0) == nw && Params(r, 1) == cur && g_kernels == 0;
    for (int i = 0; i < 128; ++i) ok = ok && o0[i] == Ramp(src[i], 0, 0x40, i) && o1[i] == Ramp(src[i], 0, 0x20, i) && o2[i] == 0;
    // the next update fades those entries out from their gains (the swapped-in set is silent)
    std::memset(o0, 0, sizeof(o0)); std::memset(o1, 0, sizeof(o1));
    At<bool>(r, 0x24) = true;
    r->Process(src, out);
    ok = ok && Params(r, 0) == cur && Params(r, 1) == nw && g_kernels == 0;
    for (int i = 0; i < 128; ++i) ok = ok && o0[i] == Ramp(src[i], 0x7000, -0x40, i) && o1[i] == Ramp(src[i], 0x3000, -0x20, i) && o2[i] == 0 && o3[i] == 0;
    delete r;
    if (!ok) { std::printf("00c6e410_TEST FAIL\n"); return 1; }
    std::printf("00c6e410_TEST PASS\n");
    return 0;
}
