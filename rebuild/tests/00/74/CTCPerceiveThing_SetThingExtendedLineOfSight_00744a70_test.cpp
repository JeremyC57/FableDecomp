#include <cstdio>
// Behaviour model of CTCPerceiveThing::SetThingExtendedLineOfSight (0x00744a70)
static int g_adds, g_removes;
class CPlayerGui { public: void AddPerceivingThing(void*) { ++g_adds; } void RemovePerceivingThing(void*) { ++g_removes; } };
static CPlayerGui g_gui; static CPlayerGui* g_pPlayerGui = &g_gui;
class CTCPerceiveThing { public:
    void* ParentThing; void* Perceived; long FramesSinceDirectlyPerceivedThing; bool ExtendedLineOfSight;
    void UpdateDrawPerceivers() { if (Perceived) g_pPlayerGui->AddPerceivingThing(ParentThing); else g_pPlayerGui->RemovePerceivingThing(ParentThing); }
    void Set(bool v) {
        if (ExtendedLineOfSight != v) { if (ExtendedLineOfSight) g_pPlayerGui->RemovePerceivingThing(ParentThing); ExtendedLineOfSight = v; if (v) UpdateDrawPerceivers(); }
        if (0 && ExtendedLineOfSight) FramesSinceDirectlyPerceivedThing = 0;
    } };
int main()
{
    int t, target;
    CTCPerceiveThing p = { &t, &target, 5, false };
    p.Set(true);
    if (!p.ExtendedLineOfSight || g_adds != 1 || g_removes != 0 || p.FramesSinceDirectlyPerceivedThing != (0 ? 0 : 5)) { std::printf("00744a70_TEST FAIL\n"); return 1; }
    p.Set(true);
    if (g_adds != 1) { std::printf("00744a70_TEST FAIL\n"); return 1; }
    p.Set(false);
    if (p.ExtendedLineOfSight || g_removes != 1) { std::printf("00744a70_TEST FAIL\n"); return 1; }
    p.Perceived = 0; p.Set(true);
    if (g_removes != 2 || g_adds != 1) { std::printf("00744a70_TEST FAIL\n"); return 1; }
    std::printf("00744a70_TEST PASS\n");
    return 0;
}