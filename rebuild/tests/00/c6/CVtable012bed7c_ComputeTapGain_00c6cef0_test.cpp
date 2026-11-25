// Behaviour test (NF447, linked against the row object over engine/CSalRenderPanDelay*.h; replaces the HEAD self-contained mirror) of
// CSalRenderPanDelay::Params3DInit 0x00C6CEF0: delay as given, Q15 gain = round(fGainPan * m_fGain * 32767) capped at 0x7fff (negative
// products wrap high and cap too), ramp step gain >> 7.
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
    At<float>(r, 0x20) = 0.5f;
    r->SetPosition(0, 1.0f, 10, 0, 0, false, false);  ok = ok && Filter(r, 0, 0, 10, 16384, 128);
    r->SetPosition(1, 0.25f, 20, 0, 0, false, false); ok = ok && Filter(r, 0, 1, 20, 4096, 32);
    r->SetPosition(2, 3.0f, 30, 0, 0, true, false);   ok = ok && Filter(r, 1, 2, 30, 0x7fff, 0xff);
    r->SetPosition(3, 0.1f, 40, 0, 0, true, false);   ok = ok && Filter(r, 1, 3, 40, 1638, 12);
    r->SetPosition(4, -1.0f, 50, 0, 0, false, false); ok = ok && Filter(r, 0, 4, 50, 0x7fff, 0xff);
    ok = ok && Filter(r, 0, 5, 0, 0, 0) && Filter(r, 1, 0, 0, 0, 0) && g_called == 0;
    delete r;
    if (!ok) { std::printf("00c6cef0_TEST FAIL\n"); return 1; }
    std::printf("00c6cef0_TEST PASS\n");
    return 0;
}
