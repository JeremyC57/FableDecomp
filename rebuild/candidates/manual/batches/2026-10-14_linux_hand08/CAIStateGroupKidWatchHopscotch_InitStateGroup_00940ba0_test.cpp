#include <cstdio>
struct CWorld {}; struct CThingAICreature {};
static long g_frame = 1234; static long g_framesPerSecond = 15;
long GFGetCurrentFrame() { return g_frame; }
struct CAIStateGroupBase { virtual ~CAIStateGroupBase() {} unsigned char _pad[0x10]; };
struct CAIStateGroup_KidWatchHopscotch : CAIStateGroupBase {
    unsigned char _pad14[0x18]; long m_frameStarted; long _pad30; long m_framesToWatch; bool m_gameSeen; long m_gamesWatched; long m_lastGameFrame;
    virtual void InitStateGroup(const CWorld&, CThingAICreature&);
};
void CAIStateGroup_KidWatchHopscotch::InitStateGroup(const CWorld& world, CThingAICreature& creature)
{
    m_frameStarted = GFGetCurrentFrame(); m_framesToWatch = g_framesPerSecond * 60;
    m_gameSeen = false; m_gamesWatched = 0; m_lastGameFrame = 0;
}
int main()
{
    CAIStateGroup_KidWatchHopscotch g; CWorld w; CThingAICreature c;
    g.m_gameSeen = true; g.m_gamesWatched = 7; g.m_lastGameFrame = 9; g.m_frameStarted = 0; g.m_framesToWatch = 0;
    g.InitStateGroup(w, c);
    if (g.m_frameStarted != 1234 || g.m_framesToWatch != 900 || g.m_gameSeen || g.m_gamesWatched || g.m_lastGameFrame) { std::printf("00940ba0_TEST FAIL\n"); return 1; }
    std::printf("00940ba0_TEST PASS\n"); return 0;
}
