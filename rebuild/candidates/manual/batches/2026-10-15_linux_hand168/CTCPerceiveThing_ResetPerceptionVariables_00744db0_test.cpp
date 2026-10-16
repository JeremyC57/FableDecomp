#include <cstdio>
// Behaviour model of CTCPerceiveThing::ResetPerceptionVariables (0x00744DB0)
static int g_removes;
struct CPerceivedThingDef { long StartWithoutTarget; };
struct CTCPerceiveThing {
    void* ParentThing; void* Perceived; const CPerceivedThingDef* PDef;
    float AwarenessTimer, DistanceFromPerceivedThingSquared; long FrameHitByPerceivedThing, FramesSinceDirectlyPerceivedThing;
    bool Perceiving, Smelt, Heard, PerceptionCombat, SearchingForThingToPerceive, LineOfSight, ExtendedLineOfSight, IgnoreFOVLineOfSight;
    void SetPerceivedThing(void* t) { Perceived = t; }
    void Drop(bool& flag) { if (flag) { ++g_removes; flag = false; } }
    void ResetPerceptionVariables() {
        AwarenessTimer = 0;
        if (PDef->StartWithoutTarget == 0) SearchingForThingToPerceive = true;
        Perceiving = false; Smelt = false; Heard = false; DistanceFromPerceivedThingSquared = 0; PerceptionCombat = false; FrameHitByPerceivedThing = 0;
        SetPerceivedThing(0);
        Drop(LineOfSight); Drop(ExtendedLineOfSight); Drop(IgnoreFOVLineOfSight);
    } };
int main()
{
    int t; CPerceivedThingDef d = { 0 };
    CTCPerceiveThing p = { &t, &t, &d, 3, 4, 5, 6, true, true, true, true, false, true, false, true };
    p.ResetPerceptionVariables();
    if (p.Perceived || p.AwarenessTimer || p.DistanceFromPerceivedThingSquared || p.FrameHitByPerceivedThing || p.Perceiving || p.Smelt || p.Heard
        || p.PerceptionCombat || !p.SearchingForThingToPerceive || p.LineOfSight || p.IgnoreFOVLineOfSight || g_removes != 2)
    { std::printf("00744db0_TEST FAIL\n"); return 1; }
    std::printf("00744db0_TEST PASS\n");
    return 0;
}
