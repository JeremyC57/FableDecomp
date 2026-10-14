#include <cstdio>
long g_FramesPerSecond_01375550 = 15;
struct CCreatureActionBase {
    virtual float GetActionTimeSeconds() = 0;
    unsigned char _pad[0x3c];
    float RunningTimeSeconds;
    float LastRunningTimeSeconds;
    long  StartFrame;
    void InitialiseActionTime();
};
void CCreatureActionBase::InitialiseActionTime()
{
    if (GetActionTimeSeconds() > 0.0f) {
        long framesPerSecond = g_FramesPerSecond_01375550;
        float seconds = GetActionTimeSeconds();
        RunningTimeSeconds = seconds;
        LastRunningTimeSeconds = seconds;
        StartFrame = (long)(seconds * (float)framesPerSecond);
    }
}
struct CTestAction : CCreatureActionBase {
    float m_duration;
    virtual float GetActionTimeSeconds() { return m_duration; }
};
int main()
{
    CTestAction a; a.m_duration = 2.5f; a.RunningTimeSeconds = 0; a.LastRunningTimeSeconds = 0; a.StartFrame = -1;
    a.InitialiseActionTime();
    if (a.RunningTimeSeconds != 2.5f || a.LastRunningTimeSeconds != 2.5f || a.StartFrame != 37) { std::printf("00692400_TEST FAIL\n"); return 1; }
    CTestAction b; b.m_duration = -1.0f; b.RunningTimeSeconds = 9; b.LastRunningTimeSeconds = 9; b.StartFrame = 9;
    b.InitialiseActionTime();
    if (b.RunningTimeSeconds != 9 || b.StartFrame != 9) { std::printf("00692400_TEST FAIL\n"); return 1; }
    std::printf("00692400_TEST PASS\n");
    return 0;
}