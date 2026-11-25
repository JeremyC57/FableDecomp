// Behaviour test (NF447, linked against the row object over engine/CSalRenderPanDelay*.h; replaces the HEAD self-contained mirror) of
// CSalRenderPanDelay::UpdateFilters 0x00C6CF40: m_bUpdate = bNew; the new set (bNew) or the live set is rebuilt from m_fGainPan / m_itd and
// m_fGain: filters 0 / 1 always, 2 / 3 only with four or more output channels, 4 / 5 never; the other set is untouched.
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
void CSalRenderPanDelay::Process(const short*, int*(&)[4]) { g_called = 0xcc60; }
void CSalRenderPanDelaySSE2::ClearFilterHistory() { g_called = 0xd560; }
void CSalRenderPanDelaySSE2::Process(const short*, int*(&)[4]) { g_called = 0xd630; }
void CSalRenderPanDelayMMX::ClearFilterHistory() { g_called = 0xf300; }
void CSalRenderPanDelayMMX::Process(const short*, int*(&)[4]) { g_called = 0xf3e0; }
void CSalRenderPanDelay::SetGain(float fGain, const float*, bool bNew) { m_fGain = fGain; UpdateFilters(bNew); }
void CSalRenderPanDelay::SetPosition(float which, float fGainPan, float itd, float, const float*, bool bNew, bool)
{ Params3DInit(&(bNew ? m_pParamsNew : m_pParamsCurrent)->pan[(int)which], fGainPan, (unsigned)itd); }
static bool Filter(void* r, unsigned set, int i, unsigned itd, unsigned short gain, short delta)
{ FilterPanDelay* f = Params(r, set) + i; return f->itd == itd && f->gain == gain && f->gainDelta == delta; }
int main()
{
    bool ok = true;
    CSalRender* r = new CSalRenderPanDelay;
    float g[6] = { 1.0f, 0.25f, 3.0f, 0.1f, 1.0f, 1.0f }; unsigned d[6] = { 10, 20, 30, 40, 50, 60 };
    for (int i = 0; i < 6; ++i) { At<float>(r, 0x1e4 + 4 * i) = g[i]; At<unsigned>(r, 0x1fc + 4 * i) = d[i]; }
    g_master.m_outputChannels = 2; At<bool>(r, 0x24) = true;
    r->SetGain(0.5f, 0, false);
    ok = ok && !At<bool>(r, 0x24) && Filter(r, 0, 0, 10, 16384, 128) && Filter(r, 0, 1, 20, 4096, 32) && Filter(r, 0, 2, 0, 0, 0);
    ok = ok && Filter(r, 1, 0, 0, 0, 0);
    g_master.m_outputChannels = 4;
    r->SetGain(0.5f, 0, true);
    ok = ok && At<bool>(r, 0x24) && Filter(r, 1, 0, 10, 16384, 128) && Filter(r, 1, 1, 20, 4096, 32) && Filter(r, 1, 2, 30, 0x7fff, 0xff);
    ok = ok && Filter(r, 1, 3, 40, 1638, 12) && Filter(r, 1, 4, 0, 0, 0) && Filter(r, 0, 2, 0, 0, 0);
    r->SetGain(0.5f, 0, false);
    ok = ok && !At<bool>(r, 0x24) && Filter(r, 0, 2, 30, 0x7fff, 0xff) && Filter(r, 0, 3, 40, 1638, 12) && Filter(r, 0, 5, 0, 0, 0) && g_called == 0;
    delete r;
    if (!ok) { std::printf("00c6cf40_TEST FAIL\n"); return 1; }
    std::printf("00c6cf40_TEST PASS\n");
    return 0;
}
