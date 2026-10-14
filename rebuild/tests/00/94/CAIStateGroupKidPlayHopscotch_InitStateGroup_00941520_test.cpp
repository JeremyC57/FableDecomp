#include <cstdio>
// Behaviour model of CAIStateGroup_KidPlayHopscotch::InitStateGroup @ 0x00941520.
struct CWorld { int x; }; struct CThingAICreature { int x; };
static long g_frame = 1234; static long GFGetCurrentFrame() { return g_frame; } static long g_framesPerSecond = 30;
struct CAIStateGroup_KidPlayHopscotch { bool m_foundBoard; long m_startFrame, m_playDurationFrames; bool m_thrown; long m_hopCount, m_currentSquare;
    void InitStateGroup(const CWorld&, CThingAICreature&) { m_foundBoard = false; m_startFrame = GFGetCurrentFrame(); m_playDurationFrames = g_framesPerSecond * 60; m_hopCount = 0; m_currentSquare = 0; m_thrown = false; } };
int main()
{
    CWorld w; CThingAICreature c; CAIStateGroup_KidPlayHopscotch g; g.m_foundBoard = true; g.m_thrown = true; g.m_hopCount = 9; g.m_currentSquare = 9;
    g.InitStateGroup(w, c);
    bool ok = !g.m_foundBoard && g.m_startFrame == 1234 && g.m_playDurationFrames == 1800 && !g.m_thrown && g.m_hopCount == 0 && g.m_currentSquare == 0;
    if (!ok) { std::printf("00941520_TEST FAIL\n"); return 1; }
    std::printf("00941520_TEST PASS\n");
    return 0;
}