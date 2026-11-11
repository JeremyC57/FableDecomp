// Behaviour test for 0x006CEAE0 CTCCreatureOpinionOfHero::ResetLastOpinionReactionFrame (re-identified; was
// CTCHeroExperience::AddSuccessfulBlockParry): the world frame is stored to LastOpinionReactionFrame (+0x0C).
#include <cstdio>
#include <cstddef>
class CWorld { public: static long GetFrame(); };
static long g_frame = 0x1234;
static int g_getWorldCalls = 0;
long CWorld::GetFrame() { return g_frame; }
static CWorld g_world;
class CTCCreatureOpinionOfHero {
public:
    char  base[0xc];                                       // CTCBase
    int   LastOpinionReactionFrame;                        // +0x0c
    float NumberOfTimesHit;                                // +0x10
    CWorld& GetWorld() const { ++g_getWorldCalls; return g_world; }
    void ResetLastOpinionReactionFrame() { LastOpinionReactionFrame = GetWorld().GetFrame(); }
};
int main()
{
    CTCCreatureOpinionOfHero opinion;
    opinion.LastOpinionReactionFrame = -1;
    opinion.NumberOfTimesHit = 2.0f;
    opinion.ResetLastOpinionReactionFrame();
    bool ok = opinion.LastOpinionReactionFrame == 0x1234 && opinion.NumberOfTimesHit == 2.0f && g_getWorldCalls == 1
              && offsetof(CTCCreatureOpinionOfHero, LastOpinionReactionFrame) == 0xc;
    if (ok) { std::printf("CTCHeroExperience_006ceae0_TEST PASS\n"); return 0; }
    std::printf("FAIL\n");
    return 1;
}
