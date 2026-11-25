// Behaviour test (NF447, linked against the row object over engine/CSalRenderPanDelay*.h; replaces the HEAD self-contained mirror) of
// CSalRenderPanDelay::SetPosition 0x00C6D150: the direction lookup's two delays become m_itd 0..3, the azimuth gives the pan gains
// (four channels with a 4-speaker platform, else two), the volume m_fGain; the filters are rebuilt only when something changed or forced.
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
void CSalRenderPanDelay::SetGain(float, const float*, bool) { g_called = 0xd120; }
void CSalRenderPanDelaySSE2::ClearFilterHistory() { g_called = 0xd560; }
void CSalRenderPanDelaySSE2::Process(const short*, int*(&)[4]) { g_called = 0xd630; }
void CSalRenderPanDelayMMX::ClearFilterHistory() { g_called = 0xf300; }
void CSalRenderPanDelayMMX::Process(const short*, int*(&)[4]) { g_called = 0xf3e0; }
void CSalRenderPanDelay::Params3DInit(FilterPanDelay*, float, unsigned) { g_called = 0xcef0; }
void CSalRenderPanDelay::UpdateFilters(const bool bNew) { ++g_updates; g_lastNew = bNew; }
static int g_lookups = 0;
#define g_rebuilds g_updates
#define g_lastImmediate g_lastNew
struct SDirectionFilter { short m_coefficients[25]; unsigned short m_delay; };
static SDirectionFilter g_left, g_right; static long g_table; static float g_az, g_el;
void LookupDirectionFilterPair(long table, float azimuth, float elevation, const SDirectionFilter** first, const SDirectionFilter** second)
{ ++g_lookups; g_table = table; g_az = azimuth; g_el = elevation; *first = &g_left; *second = &g_right; }
static bool Near(float a, float b) { return std::fabs(a - b) < 1e-4f; }
int main()
{
    bool ok = true;
    CSalRender* r = new CSalRenderPanDelay;
    g_left.m_delay = 7; g_right.m_delay = 11; g_master.m_outputChannels = 4;
    r->SetPosition(30.0f, 5.0f, 0.0f, 0.75f, 0, true, false);
    float s = (float)std::sin(30.0f * 0.017453292f) * 0.4f, c = (float)std::cos(30.0f * 0.017453292f) * 0.4f;
    ok = ok && g_lookups == 1 && g_table == 4 && g_az == 30.0f && g_el == 5.0f;
    ok = ok && At<unsigned>(r, 0x1fc) == 7 && At<unsigned>(r, 0x204) == 7 && At<unsigned>(r, 0x200) == 11 && At<unsigned>(r, 0x208) == 11;
    ok = ok && Near(At<float>(r, 0x1e4), (c + 0.6f) * (0.6f - s)) && Near(At<float>(r, 0x1e8), (c + 0.6f) * (s + 0.6f));
    ok = ok && Near(At<float>(r, 0x1ec), (0.6f - s) * (0.6f - c)) && Near(At<float>(r, 0x1f0), (s + 0.6f) * (0.6f - c));
    ok = ok && At<float>(r, 0x20) == 0.75f && g_rebuilds == 1 && g_lastImmediate;
    // the same position again: retail compares the fresh x87 gains (not rounded to float) with the stored floats, so the 4-channel arm
    // still reports a change; delays and volume are unchanged
    r->SetPosition(30.0f, 5.0f, 0.0f, 0.75f, 0, false, false);
    ok = ok && g_rebuilds == 2 && At<unsigned>(r, 0x1fc) == 7 && At<float>(r, 0x20) == 0.75f;
    // a new delay pair and a new volume
    g_left.m_delay = 3; r->SetPosition(30.0f, 5.0f, 0.0f, 0.5f, 0, false, true);
    ok = ok && g_rebuilds == 3 && !g_lastImmediate && At<unsigned>(r, 0x1fc) == 3 && At<unsigned>(r, 0x204) == 3 && At<float>(r, 0x20) == 0.5f;
    g_master.m_outputChannels = 2; At<float>(r, 0x1f0) = 9.0f;
    r->SetPosition(-45.0f, 0.0f, 0.0f, 0.75f, 0, false, false);
    s = (float)std::sin(-45.0f * 0.017453292f) * 0.4f;
    ok = ok && Near(At<float>(r, 0x1e4), 0.6f - s) && Near(At<float>(r, 0x1e8), s + 0.6f) && At<float>(r, 0x1f0) == 9.0f && g_rebuilds == 4;
    delete r;
    if (!ok) { std::printf("00c6d150_TEST FAIL\n"); return 1; }
    std::printf("00c6d150_TEST PASS\n");
    return 0;
}
