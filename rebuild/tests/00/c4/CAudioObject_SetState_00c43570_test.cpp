// Behaviour test for 00c43570: CAudioObject::SetState -- voice counting / release on state changes, state store, callback
#include <cstdio>

struct CVoice { int id; };
static int g_counted = 0, g_uncounted = 0, g_released = 0, g_callbacks = 0;
static unsigned int g_lastFlags = 0;
struct CVoiceManager {
    void Count(CVoice*) { ++g_counted; }
    void Uncount(CVoice*) { ++g_uncounted; }
    void Release(CVoice*) { ++g_released; }
};
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
    bool KeepsVoice() const { return (m_flags >> 17) & 1; }
    void SetState(unsigned int state)
    {
        state &= 0x7f;
        unsigned int old = GetState();
        if (state == old)
            return;
        if (!IsVirtual()) {
            switch (state) {
            case 0:
                if (old != 8 && old != 1 && old != 0x10)
                    g_pVoiceManager->Uncount(m_pVoice);
                if (!KeepsVoice()) {
                    g_pVoiceManager->Release(m_pVoice);
                    m_pVoice = 0;
                }
                break;
            case 2:
                g_pVoiceManager->Count(m_pVoice);
                break;
            case 8:
                if (old != 0 && old != 1 && old != 0x10)
                    g_pVoiceManager->Uncount(m_pVoice);
                break;
            }
        }
        m_flags = (m_flags & ~0x7fU) | state;
        if (state == 0)
            m_flags &= ~0x100U;
        if (m_stateCallback)
            m_stateCallback(this, m_flags & 0xffff, m_stateCallbackData);
    }
};

int main()
{
    CVoice v = { 1 };
    CAudioObject o; o.m_flags = 0x10101; o.m_pVoice = &v; o.m_stateCallback = Callback; o.m_stateCallbackData = 0;
    int ok = 1;
    o.SetState(0x81);                       // masked to 1: unchanged
    ok &= g_callbacks == 0;
    o.SetState(2);                          // play: counted
    ok &= g_counted == 1 && (o.m_flags & 0x7f) == 2 && g_callbacks == 1 && g_lastFlags == 0x102;
    o.SetState(8);                          // stop from playing: uncounted
    ok &= g_uncounted == 1 && (o.m_flags & 0x7f) == 8;
    o.SetState(0);                          // idle from 8: no uncount, release, bit 8 cleared
    ok &= g_uncounted == 1 && g_released == 1 && o.m_pVoice == 0 && (o.m_flags & 0x1ff) == 0 && g_lastFlags == 0;
    o.m_flags = 0x20002; o.m_pVoice = &v;   // playing, keeps its voice
    o.SetState(0);
    ok &= g_uncounted == 2 && g_released == 1 && o.m_pVoice == &v;
    o.m_flags = 0x202;                      // virtual: no voice bookkeeping
    o.SetState(0);
    ok &= g_uncounted == 2 && g_released == 1 && (o.m_flags & 0x7f) == 0;
    if (!ok) { std::printf("00c43570_TEST FAIL\n"); return 1; }
    std::printf("00c43570_TEST PASS\n");
    return 0;
}