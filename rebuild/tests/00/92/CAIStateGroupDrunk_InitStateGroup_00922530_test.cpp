// Behaviour test for 0x00922530 CAIStateGroup_Drunk::InitStateGroup(const CWorld&, CThingAICreature&): clears FrameToNextTalkToSelf, ignores both arguments.
#include <cstdio>
struct CWorldModel { }; struct CCreatureModel { };
struct CDrunkModel { long FrameToNextTalkToSelf; void InitStateGroup(const CWorldModel&, CCreatureModel&) { FrameToNextTalkToSelf = 0; } };
int main()
{
    CWorldModel w; CCreatureModel c; CDrunkModel d; d.FrameToNextTalkToSelf = 77; d.InitStateGroup(w, c);
    if (d.FrameToNextTalkToSelf != 0) { std::printf("00922530_TEST FAIL\n"); return 1; }
    std::printf("OK_00922530\n"); return 0;
}
