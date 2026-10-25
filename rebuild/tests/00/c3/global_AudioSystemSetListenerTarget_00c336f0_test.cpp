// Behaviour test for 00c336f0: AudioSystem_SetListenerTarget -- sink gets the target when present; the helper gets it and refreshes only when the device flag is set
#include <cstdio>
struct CAudioSink { void* got; int calls; virtual void SetTarget(void* t) { got = t; ++calls; } };
struct CAudioHelper { int refreshes; void* m_pTarget; virtual void Refresh() { ++refreshes; } };
struct CAudioDevice {
    CAudioHelper* m_pHelper; long m_helperActive; CAudioSink* m_pSink;
    void SetListenerTarget(void* target)
    {
        if (m_pSink) m_pSink->SetTarget(target);
        if (m_helperActive) { CAudioHelper* helper = m_pHelper; helper->m_pTarget = target; helper->Refresh(); }
    }
};
struct CAudioSystem { CAudioDevice* m_pDevice; };
static CAudioSystem g_audioSystem;
static void AudioSystem_SetListenerTarget(void* target) { g_audioSystem.m_pDevice->SetListenerTarget(target); }
int main()
{
    CAudioSink sink; sink.got = 0; sink.calls = 0;
    CAudioHelper helper; helper.refreshes = 0; helper.m_pTarget = 0;
    CAudioDevice dev; dev.m_pHelper = &helper; dev.m_helperActive = 0; dev.m_pSink = 0;
    g_audioSystem.m_pDevice = &dev;
    int t1 = 0, t2 = 0;
    AudioSystem_SetListenerTarget(&t1);
    if (sink.calls != 0 || helper.refreshes != 0 || helper.m_pTarget != 0) { std::printf("00c336f0_TEST FAIL\n"); return 1; }
    dev.m_pSink = &sink; dev.m_helperActive = 1;
    AudioSystem_SetListenerTarget(&t2);
    if (sink.got != &t2 || sink.calls != 1 || helper.m_pTarget != &t2 || helper.refreshes != 1) { std::printf("00c336f0_TEST FAIL\n"); return 1; }
    std::printf("00c336f0_TEST PASS\n");
    return 0;
}