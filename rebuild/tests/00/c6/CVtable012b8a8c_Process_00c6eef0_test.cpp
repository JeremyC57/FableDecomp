// Behaviour test (NF452, linked against the row object over engine/CSalRenderDisable*.h; replaces the HEAD test of the local CVtable_012bedb8
// model) of
// CSalRenderDisableSSE2::Process 0x00C6EEF0: the constant-gain path hands the input, each output and the current gain to the SSE2 kernel
// (outputs 2 / 3 only with four or more output channels, a zero gain skipped); an update clears m_bUpdate, fades the current record out,
// swaps the sets and fades the new record in (the hand SSE2 ramp kernel of the row object runs for real).
#define FABLE_RETAIL_CSALPLATFORM_CREATE_MIXER_PASS23
#define FABLE_RETAIL_CSALOUTPUTKS_INLINE_MEMBERS
#define FABLE_RETAIL_CCODAOBJECTBASE_INLINE_DTOR
#define FABLE_RETAIL_CSALRENDER_MEMBERS
#define FABLE_RETAIL_CSALMUTEX_ENTER_LEAVE
#include "engine/CSalRenderDisableSSE2.h"
#include "engine/CSalRenderDisableMMX.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
static int g_frees = 0, g_baseCtors = 0, g_baseDtors = 0;
extern "C" void* __cdecl CodaMalloc(unsigned int n) { void* p = std::malloc(n); std::memset(p, 0x5a, n); return p; }
extern "C" void __cdecl CodaFree(void* p) { ++g_frees; std::free(p); }
CSalRender::CSalRender() : m_fGain(0.0f), m_bUpdate(false) { ++g_baseCtors; }
CSalRender::~CSalRender() { ++g_baseDtors; }
void CSalRender::ClearFilterHistory() { }
// Retail layout (engine/CSalRenderDisable.h): the test reads protected / private members at their retail offsets.
template <class T> static T& At(void* o, unsigned off) { return *reinterpret_cast<T*>(static_cast<char*>(o) + off); }
static FilterPan* Params(void* o, unsigned which) { return At<FilterPan*>(o, which ? 0x34 : 0x30); }
static bool Pan(void* o, unsigned which, unsigned short gain, short delta) { return Params(o, which)->gain == gain && Params(o, which)->gainDelta == delta; }
static int g_called = 0;
static int g_updates = 0; static bool g_lastNew = false;
struct CAudioMaster { unsigned char _pad[0x1bc]; unsigned long m_outputChannels; };   // the rows' view of g_pSal (+0x1BC output channels)
static CAudioMaster g_master;
CAudioMaster* g_pAudioMaster = &g_master;
CSalRenderDisable::CSalRenderDisable() { m_paramsA.pan.gain = 0; m_paramsA.pan.gainDelta = 0; m_paramsB = m_paramsA; m_pParamsCurrent = &m_paramsA; m_pParamsNew = &m_paramsB; }
CSalRenderDisable::~CSalRenderDisable() { }
void CSalRenderDisable::Process(const short*, int*(&)[4]) { g_called = 0xe8f0; }   // 0x00C6E8F0 (no row)
void CSalRenderDisable::SetGain(float, const float*, bool) { g_called = 0xec50; }
void CSalRenderDisable::SetPosition(float, float, float, float, const float*, bool, bool) { g_called = 0xec80; }
void CSalRenderDisable::Params3DInit(FilterPan*) { g_called = 0xeba0; }
void CSalRenderDisable::UpdateFilters(const bool bNew) { ++g_updates; g_lastNew = bNew; }
void CSalRenderDisableMMX::Process(const short*, int*(&)[4]) { g_called = 0xfe10; }
// What the hand ramp kernel adds per sample (measured on the row object): the high word of sample * gain (pmulhw), times two.
static int Ramp(short sample, int gain0, int step, int i) { return ((sample * (short)(gain0 + step * i)) >> 16) * 2; }
static int g_kernels = 0; static const short* g_src[4]; static long* g_dst[4]; static short g_gain[4]; static unsigned long g_count[4];
namespace Row00c6ecd0 { void __cdecl ScaleAccumulate16(const short* src, unsigned long count, long* dst, short gain); }
void __cdecl Row00c6ecd0::ScaleAccumulate16(const short* src, unsigned long count, long* dst, short gain)   // 0x00C6ECD0 (landed kernel, not linked): records the call
{ if (g_kernels < 4) { g_src[g_kernels] = src; g_dst[g_kernels] = dst; g_gain[g_kernels] = gain; g_count[g_kernels] = count; } ++g_kernels; }
int main()
{
    bool ok = true;
    CSalRender* r = new CSalRenderDisableSSE2;
    __declspec(align(16)) short src[128]; for (int i = 0; i < 128; ++i) src[i] = (short)(1000 + 8 * i);
    __declspec(align(16)) int o0[128]; __declspec(align(16)) int o1[128]; __declspec(align(16)) int o2[128]; __declspec(align(16)) int o3[128];
    std::memset(o0, 0, sizeof(o0)); std::memset(o1, 0, sizeof(o1)); std::memset(o2, 0, sizeof(o2)); std::memset(o3, 0, sizeof(o3));
    int* out[4] = { o0, o1, o2, o3 };
    FilterPan* cur = Params(r, 0); FilterPan* nw = Params(r, 1);
    cur->gain = 0x4000; cur->gainDelta = 0x80;
    g_master.m_outputChannels = 2;
    r->Process(src, out);   // constant gain: one kernel call per output, the whole 128-sample block (16 groups)
    ok = ok && g_kernels == 2 && g_src[0] == src && g_dst[0] == (long*)o0 && g_dst[1] == (long*)o1 && g_gain[0] == 0x4000 && g_gain[1] == 0x4000;
    ok = ok && g_count[0] == 16 && g_count[1] == 16;
    g_kernels = 0; g_master.m_outputChannels = 4;
    r->Process(src, out);
    ok = ok && g_kernels == 4 && g_dst[2] == (long*)o2 && g_dst[3] == (long*)o3;
    g_kernels = 0; cur->gain = 0;
    r->Process(src, out);   // a silent record mixes nothing
    ok = ok && g_kernels == 0;
    // an update: fade the current record out (its gain is 0 here, so nothing), swap the sets, fade the new one in with the real ramp kernel
    nw->gain = 0x7000; nw->gainDelta = 0x40; g_master.m_outputChannels = 2;
    At<bool>(r, 0x24) = true;
    r->Process(src, out);
    ok = ok && !At<bool>(r, 0x24) && Params(r, 0) == nw && Params(r, 1) == cur && g_kernels == 0;
    for (int i = 0; i < 128; ++i) ok = ok && o0[i] == Ramp(src[i], 0, 0x40, i) && o1[i] == o0[i] && o2[i] == 0;
    // the fade-out of a record whose gain is set ramps down: the last sample gets less than the first group
    std::memset(o0, 0, sizeof(o0)); std::memset(o1, 0, sizeof(o1));
    At<bool>(r, 0x24) = true; cur->gain = 0; cur->gainDelta = 0;
    r->Process(src, out);
    ok = ok && Params(r, 0) == cur && Params(r, 1) == nw && g_kernels == 0;
    for (int i = 0; i < 128; ++i) ok = ok && o0[i] == Ramp(src[i], 0x7000, -0x40, i) && o1[i] == o0[i] && o2[i] == 0 && o3[i] == 0;
    delete r;
    if (!ok) { std::printf("00c6eef0_TEST FAIL\n"); return 1; }
    std::printf("00c6eef0_TEST PASS\n");
    return 0;
}
