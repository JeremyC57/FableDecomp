// Behaviour test of CSalSw3DVoice::Initialize (0x00C55680), linked against the row object, over the shared headers: the renderer for the
// mode is created and stored (+0x220); a null renderer returns false and leaves the rest of the voice alone; otherwise the renderer is
// initialized and azimuth / elevation (+0x210 / +0x214) become 0, the macro-FX gains (+0x218 / +0x21C) 1.0, the environment gains
// (+0x23C / +0x23E) 0 and the update flag (+0x240) false; every other byte of the voice is untouched.
#define FABLE_RETAIL_PASS22_CSALSW_CALCPARAMS
#define FABLE_RETAIL_PASS22_CCODA3D_CREATE
#define FABLE_RETAIL_CSALPLATFORM_CREATE_SW_PASS23
#define FABLE_RETAIL_CSALSW3D_CREATE
#include "engine/CSalSw3DVoice.h"
#include "engine/CSalRender.h"
#include <cstdio>
#include <cstring>
static int g_createCalls; static CHRTFMODE g_mode; static CSalRender* g_next; static void* g_createThis;
static int g_initCalls; static CSalRender* g_initThis;
// A concrete renderer over the shared header (CSalRender is abstract).
class TestRender : public CSalRender {
public:
    virtual void Process(const short*, int*(&)[4]) { }
    virtual void SetGain(float, const float*, bool) { }
    virtual void SetPosition(float, float, float, float, const float*, bool, bool) { }
};
// Initialize is private: the test reaches it through its real caller's slot, CSalSw3DVoice::Create (row 0x00C556E0, not linked here),
// defined below as a test entry that passes the mode through.
static CHRTFMODE g_entryMode;
int CSalSw3DVoice::Create(const CCodaSource*) { return Initialize(g_entryMode) ? 1 : 0; }
static bool CallInitialize(CSalSw3DVoice* v, CHRTFMODE mode) { g_entryMode = mode; return v->CSalSw3DVoice::Create(0) == 1; }
CSalRender* CSalSw3DVoice::CreateRenderer(CHRTFMODE mode) { ++g_createCalls; g_mode = mode; g_createThis = this; return g_next; }
void CSalRender::Initialize() { ++g_initCalls; g_initThis = this; }
CSalRender::~CSalRender() { }
void CSalRender::ClearFilterHistory() { }
CCodaObjectBase::~CCodaObjectBase() { }
extern "C" void __cdecl CodaFree(void*) { }
static unsigned char g_voice[0x248], g_before[0x248];
template <class T> static T At(unsigned int off) { T v; std::memcpy(&v, g_voice + off, sizeof(T)); return v; }
static bool OnlyChanged(unsigned int lo, unsigned int hi)   // bytes outside [lo, hi) and outside the written members are unchanged
{
    for (unsigned int i = 0; i < sizeof(g_voice); ++i) {
        bool written = (i >= 0x210 && i < 0x224) || (i >= 0x23c && i < 0x241);
        if (i >= lo && i < hi) continue;
        if (!written && g_voice[i] != g_before[i]) return false;
    }
    return true;
}
int main()
{
    bool ok = sizeof(CSalSw3DVoice) == 0x248;
    CSalSw3DVoice* v = reinterpret_cast<CSalSw3DVoice*>(g_voice);
    TestRender render;
    // 1. no renderer: false, only m_pRender (+0x220) written
    std::memset(g_voice, 0xAB, sizeof(g_voice)); std::memcpy(g_before, g_voice, sizeof(g_voice));
    g_next = 0;
    ok = ok && !CallInitialize(v, C_HRTF_FULL) && g_createCalls == 1 && g_mode == C_HRTF_FULL && g_createThis == v && g_initCalls == 0
            && At<void*>(0x220) == 0 && std::memcmp(g_voice, g_before, 0x220) == 0 && std::memcmp(g_voice + 0x224, g_before + 0x224, 0x248 - 0x224) == 0;
    // 2. a renderer: stored, initialized, the 3D state reset
    std::memset(g_voice, 0xAB, sizeof(g_voice)); std::memcpy(g_before, g_voice, sizeof(g_voice));
    g_next = &render;
    ok = ok && CallInitialize(v, C_HRTF_VOLPAN) && g_createCalls == 2 && g_mode == C_HRTF_VOLPAN && g_initCalls == 1 && g_initThis == &render
            && At<CSalRender*>(0x220) == &render && At<float>(0x210) == 0.0f && At<float>(0x214) == 0.0f
            && At<float>(0x218) == 1.0f && At<float>(0x21c) == 1.0f && At<unsigned short>(0x23c) == 0 && At<unsigned short>(0x23e) == 0
            && At<unsigned char>(0x240) == 0 && OnlyChanged(0, 0);
    if (!ok) { std::printf("00c55680_TEST FAIL\n"); return 1; }
    std::printf("00c55680_TEST PASS\n");
    return 0;
}
