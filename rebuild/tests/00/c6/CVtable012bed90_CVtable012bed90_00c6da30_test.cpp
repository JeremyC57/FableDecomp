// Behaviour test (NF452, linked against the row object over engine/CSalRenderPan*.h; replaces the HEAD test of the local CVtable_012bed90
// model) of
// CSalRenderPan::~CSalRenderPan 0x00C6DA30: the base destructor runs once, after which the storage is freed through the class operator
// delete; the SIMD classes' implicit destructors reach it too.
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
CSalRenderPan::CSalRenderPan() { std::memset(&m_paramsA, 0, sizeof(m_paramsA)); std::memset(&m_paramsB, 0, sizeof(m_paramsB)); m_pParamsCurrent = &m_paramsA; m_pParamsNew = &m_paramsB; for (int i = 0; i < 6; ++i) m_fGainPan[i] = 0.0f; }
void CSalRenderPan::Process(const short*, int*(&)[4]) { g_called = 0xdba0; }   // 0x00C6DBA0 (no row)
void CSalRenderPan::SetGain(float, const float*, bool) { g_called = 0xe020; }
void CSalRenderPan::SetPosition(float, float, float, float, const float*, bool, bool) { g_called = 0xe050; }   // 0x00C6E050 (no row)
void CSalRenderPan::Params3DInit(FilterPan*, float) { g_called = 0xde60; }
void CSalRenderPan::UpdateFilters(const bool bNew) { ++g_updates; g_lastNew = bNew; }
void CSalRenderPanSSE2::Process(const short*, int*(&)[4]) { g_called = 0xe410; }
void CSalRenderPanMMX::Process(const short*, int*(&)[4]) { g_called = 0xf990; }
int main()
{
    bool ok = true;
    CSalRender* r = new CSalRenderPan;
    g_called = 0;
    delete r;
    ok = ok && g_baseDtors == 1 && g_frees == 1 && g_called == 0;   // the destructor runs no virtual of the class
    r = new CSalRenderPanSSE2; delete r;
    r = new CSalRenderPanMMX; delete r;
    ok = ok && g_baseDtors == 3 && g_frees == 3;
    if (!ok) { std::printf("00c6da30_TEST FAIL\n"); return 1; }
    std::printf("00c6da30_TEST PASS\n");
    return 0;
}
