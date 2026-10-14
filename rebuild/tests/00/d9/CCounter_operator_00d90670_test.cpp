#include <cstdio>
struct CThingManager_Calls {
    virtual long GetWorldFrame() = 0;
    virtual long GetWorldFramesPerSecond() = 0;
};
CThingManager_Calls* g_pThingManager_0143e8f8;
struct CCounter {
    long RelevantWorldFrame;
    long operator>(float seconds) const;
};
long CCounter::operator>(float seconds) const
{
    long elapsedFrames = g_pThingManager_0143e8f8->GetWorldFrame() - RelevantWorldFrame;
    if (elapsedFrames < 0)
        elapsedFrames = 0;
    float elapsed = (float)elapsedFrames;
    long rate = g_pThingManager_0143e8f8->GetWorldFramesPerSecond();
    if (elapsed > (float)rate * seconds)
        return 1;
    return 0;
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
    m.m_frame = 131;                      // 31 frames = 2.07s
    if ((c > 2.0f) != 1 || (c > 2.1f) != 0) { std::printf("00d90670_TEST FAIL\n"); return 1; }
    m.m_frame = 50;                       // negative delta clamps to 0
    if ((c > 0.0f) != 0) { std::printf("00d90670_TEST FAIL\n"); return 1; }
    std::printf("00d90670_TEST PASS\n");
    return 0;
}