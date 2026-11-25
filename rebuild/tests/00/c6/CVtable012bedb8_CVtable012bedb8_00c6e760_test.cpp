// Behaviour test (NF452, linked against the row object over engine/CSalRenderDisable*.h; replaces the HEAD test of the local CVtable_012bedb8
// model) of
// CSalRenderDisable::CSalRenderDisable 0x00C6E760: the base is constructed once, the current / new pointers aim at m_paramsA (+0x28) /
// m_paramsB (+0x2C), the records themselves are left alone, and the vtable is the class's (its own Process; the SIMD classes' constructors
// reach it too).
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
CSalRenderDisable::~CSalRenderDisable() { }
void CSalRenderDisable::Process(const short*, int*(&)[4]) { g_called = 0xe8f0; }   // 0x00C6E8F0 (no row)
void CSalRenderDisable::SetGain(float, const float*, bool) { g_called = 0xec50; }
void CSalRenderDisable::SetPosition(float, float, float, float, const float*, bool, bool) { g_called = 0xec80; }
void CSalRenderDisable::Params3DInit(FilterPan*) { g_called = 0xeba0; }
void CSalRenderDisable::UpdateFilters(const bool bNew) { ++g_updates; g_lastNew = bNew; }
void CSalRenderDisableSSE2::Process(const short*, int*(&)[4]) { g_called = 0xeef0; }
void CSalRenderDisableMMX::Process(const short*, int*(&)[4]) { g_called = 0xfe10; }
int main()
{
    bool ok = true;
    CSalRender* r = new CSalRenderDisable;
    ok = ok && g_baseCtors == 1 && Params(r, 0) == &At<FilterPan>(r, 0x28) && Params(r, 1) == &At<FilterPan>(r, 0x2c);
    ok = ok && At<unsigned>(r, 0x28) == 0x5a5a5a5a && At<unsigned>(r, 0x2c) == 0x5a5a5a5a;
    int* out[4] = { 0, 0, 0, 0 };
    g_called = 0; r->Process(0, out); ok = ok && g_called == 0xe8f0;
    g_called = 0; r->SetGain(1.0f, 0, false); ok = ok && g_called == 0xec50;
    delete r;
    r = new CSalRenderDisableSSE2; g_called = 0; r->Process(0, out);
    ok = ok && g_baseCtors == 2 && g_called == 0xeef0 && Params(r, 1) == &At<FilterPan>(r, 0x2c); delete r;
    r = new CSalRenderDisableMMX; g_called = 0; r->Process(0, out);
    ok = ok && g_baseCtors == 3 && g_called == 0xfe10 && Params(r, 0) == &At<FilterPan>(r, 0x28); delete r;
    ok = ok && g_frees == 3;
    if (!ok) { std::printf("00c6e760_TEST FAIL\n"); return 1; }
    std::printf("00c6e760_TEST PASS\n");
    return 0;
}
