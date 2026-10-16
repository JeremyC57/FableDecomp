// behaviour test for 0x007B0800 CTCFishing::CTCFishing(CThing*)
#include <cstdio>
#include <cstring>
struct FishingDef { int id; };
struct ParentDef { FishingDef* sub; bool GetSubDef(FishingDef** out) { *out = sub; return true; } };
struct CThing { ParentDef* PDef; };
struct Fishing {
    CThing* ParentThing; void* PFish; long Stage; bool StageInitialised; long FrameStageFinishes, HookPresses;
    bool Pressed; long LastFramePressed; float Distance, MaxDistance, Strain; long FrameReelFinishes; bool Pulling;
    long Score; char Text[4]; long TextTimer; bool InstructionsShown; unsigned int Reel, Escape; FishingDef* PDef;
    Fishing(CThing* t) : ParentThing(t), PFish(0), Stage(0), StageInitialised(false), FrameStageFinishes(0), HookPresses(0),
        Pressed(false), LastFramePressed(0), Distance(0.0f), MaxDistance(0.0f), Strain(0.0f), FrameReelFinishes(0),
        Pulling(false), Score(0), TextTimer(0), InstructionsShown(false), Reel(0), Escape(0), PDef(0)
    { std::strcpy(Text, ""); ParentThing->PDef->GetSubDef(&PDef); }
};
int main()
{
    FishingDef fd = { 1 }; ParentDef pd = { &fd }; CThing t = { &pd };
    Fishing f(&t);
    bool ok = f.PDef == &fd && f.Stage == 0 && !f.StageInitialised && f.Distance == 0.0f && f.Score == 0 && f.Text[0] == 0
        && f.Reel == 0 && f.Escape == 0 && !f.InstructionsShown;
    if (!ok) { std::printf("007b0800_TEST FAIL\n"); return 1; }
    std::printf("007b0800_TEST PASS\n");
    return 0;
}
