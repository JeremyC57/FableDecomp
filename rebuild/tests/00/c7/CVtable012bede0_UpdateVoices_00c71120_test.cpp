#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __cdecl
#define __stdcall
#define __fastcall
#define __declspec(x)
typedef long long __int64;
#endif
#define EnterCriticalSection TestEnterCriticalSection
#define LeaveCriticalSection TestLeaveCriticalSection
struct _RTL_CRITICAL_SECTION;
extern "C" void __stdcall EnterCriticalSection(_RTL_CRITICAL_SECTION* section);
extern "C" void __stdcall LeaveCriticalSection(_RTL_CRITICAL_SECTION* section);

struct SVoiceTap;                                      // tap descriptor (word at +0x32: the fractional delay)

struct SFilteredVoice {                                // 0x2a4 bytes
    unsigned char _pad000[0x2a1];
    bool          m_swapOutputs;                       // +0x2a1
    unsigned char _pad2a2[0x2];
};

struct CVtable_012beda4 {                              // vtable 0x012BEDA4, 0x28 bytes
    virtual void v00();
    void*          m_lockVtable;                       // +0x04
    _RTL_CRITICAL_SECTION* Lock() { return reinterpret_cast<_RTL_CRITICAL_SECTION*>(m_lockSection); }
    unsigned char  m_lockSection[0x18];                // +0x08
    float          m_volume;                           // +0x20
    bool           m_crossfade;                        // +0x24
    unsigned char  _pad25[0x3];
};

class CVtable_012bede0 : public CVtable_012beda4 {     // vtable 0x012BEDE0
public:
    unsigned char    _pad028[0x194];
    SFilteredVoice   m_sets[2][2];                     // +0x1bc
    SFilteredVoice*  m_pCurrent;                       // +0xc4c
    SFilteredVoice*  m_pPending;                       // +0xc50
    float            m_gains[2];                       // +0xc54
    float            m_front;                          // +0xc5c
    float            m_rear;                           // +0xc60
    const SVoiceTap* m_taps[2];                        // +0xc64

    virtual void v00();
    void SetupVoice(SFilteredVoice* voice, float gain, const SVoiceTap* tap);   // 0x00C70F80
    void UpdateVoices(bool crossfade);
};

void CVtable_012bede0::UpdateVoices(bool crossfade)
{
    EnterCriticalSection(Lock());
    m_crossfade = crossfade;
    if (crossfade) {
        m_pPending[0].m_swapOutputs = m_pPending[1].m_swapOutputs = m_rear > m_front;
        SetupVoice(&m_pPending[0], m_gains[0], m_taps[0]);
        SetupVoice(&m_pPending[1], m_gains[1], m_taps[1]);
    } else {
        m_pCurrent[0].m_swapOutputs = m_pCurrent[1].m_swapOutputs = m_rear > m_front;
        SetupVoice(&m_pCurrent[0], m_gains[0], m_taps[0]);
        SetupVoice(&m_pCurrent[1], m_gains[1], m_taps[1]);
    }
    LeaveCriticalSection(Lock());
}

static int fail() { std::printf("00c71120_TEST FAIL\n"); return 1; }
static bool nearly(float a, float b) { float d = a - b; if (d < 0) d = -d; return d < 1e-3f; }
static int g_enter = 0, g_leave = 0; static _RTL_CRITICAL_SECTION* g_lock = 0; static int g_lockMismatch = 0;
extern "C" void __stdcall TestEnterCriticalSection(_RTL_CRITICAL_SECTION* s) { ++g_enter; g_lock = s; }
extern "C" void __stdcall TestLeaveCriticalSection(_RTL_CRITICAL_SECTION* s) { ++g_leave; if (s != g_lock) g_lockMismatch = 1; }
void CVtable_012beda4::v00() { }
void CVtable_012bede0::v00() { }
static SFilteredVoice* g_voice[4]; static float g_gain[4]; static const SVoiceTap* g_tap[4]; static bool g_swap[4]; static int g_held[4]; static int g_calls = 0;
void CVtable_012bede0::SetupVoice(SFilteredVoice* voice, float gain, const SVoiceTap* tap)
{ g_voice[g_calls] = voice; g_gain[g_calls] = gain; g_tap[g_calls] = tap; g_swap[g_calls] = voice->m_swapOutputs; g_held[g_calls] = g_enter - g_leave; ++g_calls; }
static CVtable_012bede0 g_fx;
int main()
{
    static int tapA, tapB;
    CVtable_012bede0& fx = g_fx;
    fx.m_pCurrent = &fx.m_sets[0][0]; fx.m_pPending = &fx.m_sets[1][0];
    fx.m_gains[0] = 0.5f; fx.m_gains[1] = 0.75f;
    fx.m_taps[0] = reinterpret_cast<const SVoiceTap*>(&tapA); fx.m_taps[1] = reinterpret_cast<const SVoiceTap*>(&tapB);
    fx.m_front = 0.25f; fx.m_rear = 0.75f;                   // rear louder: the outputs swap
    fx.UpdateVoices(true);                                   // cross-fade: the pending set is rebuilt
    if (!fx.m_crossfade || g_enter != 1 || g_leave != 1 || g_lockMismatch || g_lock != fx.Lock()) return fail();
    if (g_calls != 2 || g_voice[0] != &fx.m_pPending[0] || g_voice[1] != &fx.m_pPending[1]) return fail();
    if (g_gain[0] != 0.5f || g_gain[1] != 0.75f || g_tap[0] != fx.m_taps[0] || g_tap[1] != fx.m_taps[1]) return fail();
    if (!g_swap[0] || !g_swap[1] || g_held[0] != 1 || g_held[1] != 1) return fail();
    if (fx.m_pCurrent[0].m_swapOutputs || fx.m_pCurrent[1].m_swapOutputs) return fail();
    fx.m_pCurrent[0].m_swapOutputs = fx.m_pCurrent[1].m_swapOutputs = true;
    fx.m_front = 0.5f; fx.m_rear = 0.5f;                     // balanced: no swap
    g_calls = 0;
    fx.UpdateVoices(false);                                  // in place: the current set is rebuilt
    if (fx.m_crossfade || g_enter != 2 || g_leave != 2 || g_calls != 2) return fail();
    if (g_voice[0] != &fx.m_pCurrent[0] || g_voice[1] != &fx.m_pCurrent[1] || g_swap[0] || g_swap[1]) return fail();
    if (fx.m_pCurrent[0].m_swapOutputs || fx.m_pCurrent[1].m_swapOutputs || !fx.m_pPending[0].m_swapOutputs) return fail();
    std::printf("00c71120_TEST PASS\n");
    return 0;
}