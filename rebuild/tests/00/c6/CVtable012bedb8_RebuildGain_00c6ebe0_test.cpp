// Behaviour test (NF452, linked against the row object over engine/CSalRenderDisable*.h; replaces the HEAD test of the local CVtable_012bedb8
// model) of
// CSalRenderDisable::UpdateFilters 0x00C6EBE0: m_bUpdate = bNew; the new set (bNew) or the live set is rebuilt from m_fGain (Params3DInit
// expanded); the other set is untouched.
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
CSalRenderDisable::CSalRenderDisable() { m_paramsA.pan.gain = 0; m_paramsA.pan.gainDelta = 0; m_paramsB = m_paramsA; m_pParamsCurrent = &m_paramsA; m_pParamsNew = &m_paramsB; }
CSalRenderDisable::~CSalRenderDisable() { }
void CSalRenderDisable::Process(const short*, int*(&)[4]) { g_called = 0xe8f0; }   // 0x00C6E8F0 (no row)
void CSalRenderDisableSSE2::Process(const short*, int*(&)[4]) { g_called = 0xeef0; }
void CSalRenderDisableMMX::Process(const short*, int*(&)[4]) { g_called = 0xfe10; }
// The private helpers are reached through member stubs of the slots the unit does not define: SetGain stores the gain and calls UpdateFilters
// (the SDK body), SetPosition stores the gain and calls Params3DInit on the new (bNew) or current set.
void CSalRenderDisable::SetGain(float fGain, const float*, bool bNew) { m_fGain = fGain; UpdateFilters(bNew); }
void CSalRenderDisable::SetPosition(float, float, float, float fGain, const float*, bool bNew, bool)
{ m_fGain = fGain; Params3DInit(&(bNew ? m_pParamsNew : m_pParamsCurrent)->pan); }
int main()
{
    bool ok = true;
    CSalRender* r = new CSalRenderDisable;
    At<bool>(r, 0x24) = true;
    r->SetGain(0.5f, 0, false);
    ok = ok && !At<bool>(r, 0x24) && Pan(r, 0, 8192, 64) && Pan(r, 1, 0, 0);
    r->SetGain(1.0f, 0, true);
    ok = ok && At<bool>(r, 0x24) && Pan(r, 1, 16383, 127) && Pan(r, 0, 8192, 64);
    r->SetGain(4.0f, 0, false);
    ok = ok && !At<bool>(r, 0x24) && Pan(r, 0, 16383, 127) && Pan(r, 1, 16383, 127);
    r->SetGain(0.25f, 0, true);
    ok = ok && At<bool>(r, 0x24) && Pan(r, 1, 4096, 32) && Pan(r, 0, 16383, 127) && g_called == 0;
    delete r;
    if (!ok) { std::printf("00c6ebe0_TEST FAIL\n"); return 1; }
    std::printf("00c6ebe0_TEST PASS\n");
    return 0;
}
