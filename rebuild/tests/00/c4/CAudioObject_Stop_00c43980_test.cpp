// Behaviour test for 00c43980: CAudioObject::Stop -- stop the voice; ready -> idle, stopped stays, else uncount and state 8
#include <cstdio>

static int g_uncounted = 0, g_setStates = 0, g_callbacks = 0;
static unsigned int g_lastFlags = 0;
struct CVoice {
    bool m_result; unsigned int m_a, m_b;
    bool Stop(unsigned int a, unsigned int b) { m_a = a; m_b = b; return m_result; }
};
struct CVoiceManager { void Uncount(CVoice*) { ++g_uncounted; } };
static CVoiceManager g_manager;
static CVoiceManager* g_pVoiceManager = &g_manager;
struct CAudioObject;
static void Callback(CAudioObject*, unsigned int flags, void*) { ++g_callbacks; g_lastFlags = flags; }

struct CAudioObject {
    unsigned int m_flags;
    CVoice* m_pVoice;
    void (*m_stateCallback)(CAudioObject*, unsigned int, void*);
    void* m_stateCallbackData;
    unsigned int GetState() const { return m_flags & 0x7f; }
    bool IsVirtual() const { return (m_flags >> 9) & 1; }
    void RaiseStateChange() { if (m_stateCallback) m_stateCallback(this, m_flags & 0xffff, m_stateCallbackData); }
    void SetState(unsigned int state) { ++g_setStates; m_flags = (m_flags & ~0x7fU) | state; }
    void Stop(unsigned int param1, unsigned int param2)
    {
        if (!m_pVoice->Stop(param1, param2))
            return;
        unsigned int state = GetState();
        if (state == 1) {
            SetState(0);
            return;
        }
        if (state == 8)
            return;
        if (!IsVirtual() && state != 0 && state != 0x10)
            g_pVoiceManager->Uncount(m_pVoice);
        m_flags = (m_flags & ~0x7fU) | 8;
        RaiseStateChange();
    }
};

int main()
{
    CVoice v = { false, 0, 0 };
    CAudioObject o; o.m_flags = 0x10002; o.m_pVoice = &v; o.m_stateCallback = Callback; o.m_stateCallbackData = 0;
    int ok = 1;
    o.Stop(5, 6);                           // voice refuses: nothing
    ok &= v.m_a == 5 && v.m_b == 6 && (o.m_flags & 0x7f) == 2 && g_callbacks == 0;
    v.m_result = true;
    o.Stop(1, 2);                           // playing -> stopped, uncounted
    ok &= g_uncounted == 1 && (o.m_flags & 0x7f) == 8 && g_callbacks == 1 && g_lastFlags == 0x0008;
    o.Stop(1, 2);                           // already stopped
    ok &= g_uncounted == 1 && g_callbacks == 1;
    o.m_flags = 1;
    o.Stop(0, 0);                           // ready -> SetState(0)
    ok &= g_setStates == 1 && (o.m_flags & 0x7f) == 0 && g_callbacks == 1;
    o.m_flags = 0x10;
    o.Stop(0, 0);                           // state 0x10: no uncount, becomes 8
    ok &= g_uncounted == 1 && (o.m_flags & 0x7f) == 8 && g_callbacks == 2;
    if (!ok) { std::printf("00c43980_TEST FAIL\n"); return 1; }
    std::printf("00c43980_TEST PASS\n");
    return 0;
}