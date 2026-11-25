// Behaviour test (NF447, linked against the row object over engine/CSalRenderPanDelay*.h; replaces the HEAD self-contained mirror) of
// CSalRenderPanDelay::SetGain 0x00C6D120: a new gain is stored in m_fGain and rebuilds the filters (passing bImmediate); the same gain
// does nothing.
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
CSalRenderPanDelay::CSalRenderPanDelay() { m_pAudio = reinterpret_cast<short*>((reinterpret_cast<unsigned long>(m_audioBuffer + 32) + 15) & ~15UL); std::memset(&m_paramsA, 0, sizeof(m_paramsA)); std::memset(&m_paramsB, 0, sizeof(m_paramsB)); m_pParamsCurrent = &m_paramsA; m_pParamsNew = &m_paramsB; for (int i = 0; i < 6; ++i) { m_fGainPan[i] = 0.0f; m_itd[i] = 0; } }
CSalRenderPanDelay::~CSalRenderPanDelay() { }
void CSalRenderPanDelay::ClearFilterHistory() { g_called = 0xcad0; }
void CSalRenderPanDelay::Process(const short*, int*(&)[4]) { g_called = 0xcc60; }
void CSalRenderPanDelay::SetPosition(float, float, float, float, const float*, bool, bool) { g_called = 0xd150; }
void CSalRenderPanDelaySSE2::ClearFilterHistory() { g_called = 0xd560; }
void CSalRenderPanDelaySSE2::Process(const short*, int*(&)[4]) { g_called = 0xd630; }
void CSalRenderPanDelayMMX::ClearFilterHistory() { g_called = 0xf300; }
void CSalRenderPanDelayMMX::Process(const short*, int*(&)[4]) { g_called = 0xf3e0; }
void CSalRenderPanDelay::Params3DInit(FilterPanDelay*, float, unsigned) { g_called = 0xcef0; }
void CSalRenderPanDelay::UpdateFilters(const bool bNew) { ++g_updates; g_lastNew = bNew; }
#define g_rebuilds g_updates
#define g_lastImmediate g_lastNew
int main()
{
    bool ok = true;
    CSalRender* r = new CSalRenderPanDelay;
    float pan[6] = { 1, 1, 1, 1, 1, 1 };
    r->SetGain(0.5f, pan, true);
    ok = ok && At<float>(r, 0x20) == 0.5f && g_rebuilds == 1 && g_lastImmediate;
    r->SetGain(0.5f, pan, false);
    ok = ok && g_rebuilds == 1;
    r->SetGain(0.25f, 0, false);
    ok = ok && At<float>(r, 0x20) == 0.25f && g_rebuilds == 2 && !g_lastImmediate;
    delete r;
    if (!ok) { std::printf("00c6d120_TEST FAIL\n"); return 1; }
    std::printf("00c6d120_TEST PASS\n");
    return 0;
}
