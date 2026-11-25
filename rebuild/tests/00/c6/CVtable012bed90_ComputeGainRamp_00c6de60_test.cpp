// Behaviour test (NF452, linked against the row object over engine/CSalRenderPan*.h; replaces the HEAD test of the local CVtable_012bed90
// model) of
// CSalRenderPan::Params3DInit 0x00C6DE60: Q15 gain = round(fGainPan * m_fGain * 32767) capped at 0x7fff (negative products wrap high and
// cap too), ramp step gain >> 7; only the given entry changes.
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
void CSalRenderPanSSE2::Process(const short*, int*(&)[4]) { g_called = 0xe410; }
void CSalRenderPanMMX::Process(const short*, int*(&)[4]) { g_called = 0xf990; }
// The private helpers are reached through member stubs of the slots the unit does not define: SetGain stores the gain and calls UpdateFilters
// (the SDK body), SetPosition(which, fGainPan, ...) calls Params3DInit on entry `which` of the new (bNew) or current set.
void CSalRenderPan::SetGain(float fGain, const float*, bool bNew) { m_fGain = fGain; UpdateFilters(bNew); }
void CSalRenderPan::SetPosition(float which, float fGainPan, float, float, const float*, bool bNew, bool)
{ Params3DInit(&(bNew ? m_pParamsNew : m_pParamsCurrent)->pan[(int)which], fGainPan); }
int main()
{
    bool ok = true;
    CSalRender* r = new CSalRenderPan;
    At<float>(r, 0x20) = 0.5f;
    r->SetPosition(0, 1.0f, 0, 0, 0, false, false);  ok = ok && Pan(r, 0, 0, 16384, 128);
    r->SetPosition(1, 0.25f, 0, 0, 0, false, false); ok = ok && Pan(r, 0, 1, 4096, 32);
    r->SetPosition(2, 3.0f, 0, 0, 0, true, false);   ok = ok && Pan(r, 1, 2, 0x7fff, 0xff);
    r->SetPosition(3, 0.1f, 0, 0, 0, true, false);   ok = ok && Pan(r, 1, 3, 1638, 12);
    r->SetPosition(5, -1.0f, 0, 0, 0, false, false); ok = ok && Pan(r, 0, 5, 0x7fff, 0xff);
    ok = ok && Pan(r, 0, 4, 0, 0) && Pan(r, 1, 0, 0, 0) && !At<bool>(r, 0x24) && g_called == 0;
    delete r;
    if (!ok) { std::printf("00c6de60_TEST FAIL\n"); return 1; }
    std::printf("00c6de60_TEST PASS\n");
    return 0;
}
