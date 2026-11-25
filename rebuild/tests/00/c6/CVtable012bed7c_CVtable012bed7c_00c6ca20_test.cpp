// Behaviour test (NF447, linked against the row object over engine/CSalRenderPanDelay*.h; replaces the HEAD self-contained mirror) of
// CSalRenderPanDelay::CSalRenderPanDelay 0x00C6CA20: base constructed once, m_pAudio 16-byte aligned 32..47 samples into m_audioBuffer,
// both parameter sets zeroed with the current / new pointers on A / B, the six pan gains and delays zeroed; the vtable is the class's.
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
CSalRenderPanDelay::~CSalRenderPanDelay() { }
void CSalRenderPanDelay::ClearFilterHistory() { g_called = 0xcad0; }
void CSalRenderPanDelay::Process(const short*, int*(&)[4]) { g_called = 0xcc60; }
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
    short* lo = reinterpret_cast<short*>(reinterpret_cast<char*>(r) + 0x28);
    short* a = Audio(r);
    ok = ok && g_baseCtors == 1 && (reinterpret_cast<unsigned long>(a) & 15) == 0 && a >= lo + 32 && a < lo + 40;
    ok = ok && Params(r, 0) == &At<FilterPanDelay>(r, 0x17c) && Params(r, 1) == &At<FilterPanDelay>(r, 0x1ac);
    for (unsigned o = 0x17c; o < 0x1dc; o += 4) ok = ok && At<unsigned>(r, o) == 0;
    for (unsigned o = 0x1e4; o < 0x214; o += 4) ok = ok && At<unsigned>(r, o) == 0;
    ok = ok && At<unsigned>(r, 0x28) == 0x5a5a5a5a;
    g_called = 0; r->ClearFilterHistory(); ok = ok && g_called == 0xcad0;
    int* out[4] = { 0, 0, 0, 0 }; r->Process(0, out); ok = ok && g_called == 0xcc60;
    delete r;
    ok = ok && g_frees == 1;
    if (!ok) { std::printf("00c6ca20_TEST FAIL\n"); return 1; }
    std::printf("00c6ca20_TEST PASS\n");
    return 0;
}
