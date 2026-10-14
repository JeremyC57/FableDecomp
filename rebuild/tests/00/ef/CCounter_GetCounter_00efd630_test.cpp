#include <cstdio>
struct CThingManager_Calls {
    virtual long GetWorldFrame() = 0;
    virtual long GetWorldFramesPerSecond() = 0;
};
CThingManager_Calls* g_pThingManager_0143e8f8;
struct CCounter {
    long RelevantWorldFrame;
    float GetCounter() const;
};
float CCounter::GetCounter() const
{
    long elapsedFrames = g_pThingManager_0143e8f8->GetWorldFrame() - RelevantWorldFrame;
    if (elapsedFrames < 0)
        elapsedFrames = 0;
    float elapsed = (float)elapsedFrames;
    long rate = g_pThingManager_0143e8f8->GetWorldFramesPerSecond();
    return elapsed / (float)rate;
}
struct CTestManager : CThingManager_Calls {
    long m_frame;
    virtual long GetWorldFrame() { return m_frame; }
    virtual long GetWorldFramesPerSecond() { return 15; }
};
int main()
{
    CTestManager m; g_pThingManager_0143e8f8 = &m;
    CCounter c; c.RelevantWorldFrame = 100;
    m.m_frame = 130;
    if (c.GetCounter() != 2.0f) { std::printf("00efd630_TEST FAIL\n"); return 1; }
    m.m_frame = 40;
    if (c.GetCounter() != 0.0f) { std::printf("00efd630_TEST FAIL\n"); return 1; }
    std::printf("00efd630_TEST PASS\n");
    return 0;
}