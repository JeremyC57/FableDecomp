// Behaviour test (NF447, linked against the row object over engine/CSalRenderPanDelay*.h; replaces the HEAD self-contained mirror) of
// CSalRenderPanDelay::Process 0x00C6CC60: each output channel gets the input delayed by its itd at its Q15 gain (channels 2 / 3 only
// on a 4-speaker platform); an update fades the current set out, swaps the sets and clears m_bUpdate; the last 32 samples become the history.
#define FABLE_RETAIL_CSALPLATFORM_CREATE_MIXER_PASS23
#define FABLE_RETAIL_CSALOUTPUTKS_INLINE_MEMBERS
#define FABLE_RETAIL_CCODAOBJECTBASE_INLINE_DTOR
#define FABLE_RETAIL_CSALRENDER_MEMBERS
#define FABLE_RETAIL_CSALMUTEX_ENTER_LEAVE
#include "engine/CSalRenderPanDelaySSE2.h"
#include "engine/CSalRenderPanDelayMMX.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
static int g_frees = 0, g_baseCtors = 0, g_baseDtors = 0;
extern "C" void* __cdecl CodaMalloc(unsigned int n) { void* p = std::malloc(n); std::memset(p, 0x5a, n); return p; }
extern "C" void __cdecl CodaFree(void* p) { ++g_frees; std::free(p); }
CSalRender::CSalRender() : m_fGain(0.0f), m_bUpdate(false) { ++g_baseCtors; }
CSalRender::~CSalRender() { ++g_baseDtors; }
void CSalRender::ClearFilterHistory() { }
// Retail layout (engine/CSalRenderPanDelay.h): the test reads protected / private members at their retail offsets.
template <class T> static T& At(void* o, unsigned off) { return *reinterpret_cast<T*>(static_cast<char*>(o) + off); }
static short*& Audio(void* o) { return At<short*>(o, 0x178); }
static FilterPanDelay* Params(void* o, unsigned which) { return At<FilterPanDelay*>(o, which ? 0x1e0 : 0x1dc); }
static int g_called = 0;
static int g_updates = 0; static bool g_lastNew = false;
struct CAudioMaster { unsigned char _pad[0x1bc]; unsigned long m_outputChannels; };
static CAudioMaster g_master;
CAudioMaster* g_pAudioMaster = &g_master;
CSalRenderPanDelay::CSalRenderPanDelay() { m_pAudio = reinterpret_cast<short*>((reinterpret_cast<unsigned long>(m_audioBuffer + 32) + 15) & ~15UL); std::memset(&m_paramsA, 0, sizeof(m_paramsA)); std::memset(&m_paramsB, 0, sizeof(m_paramsB)); m_pParamsCurrent = &m_paramsA; m_pParamsNew = &m_paramsB; for (int i = 0; i < 6; ++i) { m_fGainPan[i] = 0.0f; m_itd[i] = 0; } }
CSalRenderPanDelay::~CSalRenderPanDelay() { }
void CSalRenderPanDelay::ClearFilterHistory() { g_called = 0xcad0; }
void CSalRenderPanDelay::SetGain(float, const float*, bool) { g_called = 0xd120; }
void CSalRenderPanDelay::SetPosition(float, float, float, float, const float*, bool, bool) { g_called = 0xd150; }
void CSalRenderPanDelaySSE2::ClearFilterHistory() { g_called = 0xd560; }
void CSalRenderPanDelaySSE2::Process(const short*, int*(&)[4]) { g_called = 0xd630; }
void CSalRenderPanDelayMMX::ClearFilterHistory() { g_called = 0xf300; }
void CSalRenderPanDelayMMX::Process(const short*, int*(&)[4]) { g_called = 0xf3e0; }
void CSalRenderPanDelay::Params3DInit(FilterPanDelay*, float, unsigned) { g_called = 0xcef0; }
void CSalRenderPanDelay::UpdateFilters(const bool bNew) { ++g_updates; g_lastNew = bNew; }
int main()
{
    bool ok = true;
    CSalRender* r = new CSalRenderPanDelay;
    short* a = Audio(r);
    short src[128]; for (int i = 0; i < 128; ++i) src[i] = (short)(1000 + i);
    for (int i = -32; i < 0; ++i) a[i] = 2000;
    int o0[128], o1[128], o2[128], o3[128];
    std::memset(o0, 0, sizeof(o0)); std::memset(o1, 0, sizeof(o1)); std::memset(o2, 0, sizeof(o2)); std::memset(o3, 0, sizeof(o3));
    int* out[4] = { o0, o1, o2, o3 };
    FilterPanDelay* cur = Params(r, 0);
    cur[0].itd = 0; cur[0].gain = 0x4000; cur[1].itd = 2; cur[1].gain = 0x2000; cur[2].itd = 0; cur[2].gain = 0x4000; cur[3].gain = 0;
    g_master.m_outputChannels = 2;
    r->Process(src, out);
    ok = ok && o0[5] == (1005 * 0x4000) >> 15 && o1[0] == (2000 * 0x2000) >> 15 && o1[5] == (1003 * 0x2000) >> 15 && o2[5] == 0;
    for (int i = 0; i < 32; ++i) ok = ok && a[i - 32] == src[96 + i];
    g_master.m_outputChannels = 4;
    r->Process(src, out);
    ok = ok && o2[5] == (1005 * 0x4000) >> 15 && o3[5] == 0;
    FilterPanDelay* nw = Params(r, 1); nw[0].itd = 0; nw[0].gain = 0; nw[0].gainDelta = 0x100;
    std::memset(o0, 0, sizeof(o0));
    At<bool>(r, 0x24) = true;
    r->Process(src, out);
    ok = ok && !At<bool>(r, 0x24) && Params(r, 0) == nw && Params(r, 1) == cur && o0[0] == (1000 * 0x4000) >> 15;
    delete r;
    if (!ok) { std::printf("00c6cc60_TEST FAIL\n"); return 1; }
    std::printf("00c6cc60_TEST PASS\n");
    return 0;
}
