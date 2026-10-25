// Behaviour test for 00c436c0: CAudioObject::Reset -- uncount / stop or release the voice, clear the low flags, callback, status 0
#include <cstdio>

static int g_uncounted = 0, g_released = 0, g_stopped = 0, g_callbacks = 0;
struct CVoice { void Stop(unsigned int, unsigned int) { ++g_stopped; } };
struct CVoiceManager {
    void Uncount(CVoice*) { ++g_uncounted; }
    void Release(CVoice*) { ++g_released; }
};
static CVoiceManager g_manager;
static CVoiceManager* g_pVoiceManager = &g_manager;
struct CAudioObject;
static void Callback(CAudioObject*, unsigned int, void*) { ++g_callbacks; }

struct CAudioObject {
    unsigned int m_flags;
    CVoice* m_pVoice;
    void (*m_stateCallback)(CAudioObject*, unsigned int, void*);
    void* m_stateCallbackData;
    int m_status;
    unsigned int GetState() const { return m_flags & 0x7f; }
    bool IsVirtual() const { return (m_flags >> 9) & 1; }
    bool KeepsVoice() const { return (m_flags >> 17) & 1; }
    void RaiseStateChange() { if (m_stateCallback) m_stateCallback(this, m_flags & 0xffff, m_stateCallbackData); }
    void Reset()
    {
        if (m_flags & 0xffff) {
            unsigned int state = GetState();
            if ((state != 8 && state != 0 && state != 1) || IsVirtual())
                g_pVoiceManager->Uncount(m_pVoice);
            if (m_pVoice) {
                if (KeepsVoice())
                    m_pVoice->Stop(0, 0);
                else {
                    g_pVoiceManager->Release(m_pVoice);
                    m_pVoice = 0;
                }
            }
            m_flags &= 0xffff0000;
            RaiseStateChange();
        }
        m_status = 0;
    }
};

int main()
{
    CVoice v;
    CAudioObject o; o.m_flags = 0x50000; o.m_pVoice = &v; o.m_stateCallback = Callback; o.m_stateCallbackData = 0; o.m_status = 7;
    int ok = 1;
    o.Reset();                              // no low bits: only the status
    ok &= o.m_status == 0 && g_callbacks == 0 && g_uncounted == 0 && o.m_pVoice == &v;
    o.m_flags = 0x50102; o.m_status = 3;    // playing, voice not kept (bit 17 clear)
    o.Reset();
    ok &= g_uncounted == 1 && g_released == 1 && o.m_pVoice == 0 && o.m_flags == 0x50000 && g_callbacks == 1 && o.m_status == 0;
    o.m_flags = 0x20008; o.m_pVoice = &v;   // stopped, voice kept
    o.Reset();
    ok &= g_uncounted == 1 && g_stopped == 1 && o.m_pVoice == &v && o.m_flags == 0x20000;
    o.m_flags = 0x201;                      // virtual ready object still uncounts
    o.Reset();
    ok &= g_uncounted == 2 && g_released == 2;
    if (!ok) { std::printf("00c436c0_TEST FAIL\n"); return 1; }
    std::printf("00c436c0_TEST PASS\n");
    return 0;
}