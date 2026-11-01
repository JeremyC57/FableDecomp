// Behaviour test for 0x00EF8D30 NScript::CPersonalScript_EmptyScript::CVillager::Main: the villager fetches the crate home only when it strayed and nobody carries it
#include <cstdio>
#include <cstring>
static int gFrames = 0, gPickUps = 0, gMoves = 0, gDrops = 0, gUnsheathes = 0, gTerminateAt = 100;
static float gCrateDist = 0.0f;
static const char* gCarrier = "";
class CThing { public: const char* Name; const char* GetName() const { return Name; }
    bool IsBeingCarriedBy(const char* who) const { return std::strcmp(gCarrier, who) == 0; } };
static bool IsDistanceBetweenThingsOver(const CThing&, const CThing&, float d) { return gCrateDist > d; }
struct CObject {
    int Busy;
    void MoveToAndPickUpGenericBox(CThing&, int, bool) { ++gPickUps; gCarrier = "Villager"; Busy = 2; }
    void MoveToThing(const CThing&, float, int, bool, bool, bool, bool) { ++gMoves; gCrateDist = 0.0f; }
    void DropGenericBox() { ++gDrops; gCarrier = ""; Busy = 1; }
    void UnsheatheWeapons() { ++gUnsheathes; }
    bool IsPerformingScriptTask() { return Busy > 0 ? (--Busy, true) : false; }
};
static bool Term() { return gFrames >= gTerminateAt; }
static void Frame()
{
    ++gFrames;
    if (gFrames == 3) gCrateDist = 20.0f;                    // strayed, nobody carrying: fetch it
    if (gFrames == 20) { gCrateDist = 20.0f; gCarrier = "Hero"; }   // strayed but carried by the hero: leave it
}
static void Main(CThing& me, CThing& crate, CThing& home, CObject& object)
{
    Frame();
    if (Term()) return;
    if (Term()) return;
    while (!Term()) {
        if (!IsDistanceBetweenThingsOver(crate, home, 10.0f) || crate.IsBeingCarriedBy("")) { Frame(); continue; }
        if (Term()) return;
        object.MoveToAndPickUpGenericBox(crate, 1, false);
        while (object.IsPerformingScriptTask()) { Frame(); if (Term()) return; }
        if (Term()) return;
        if (crate.IsBeingCarriedBy(me.GetName())) {
            if (Term()) return;
            object.MoveToThing(home, 0.0f, 1, true, true, false, true);
            object.DropGenericBox();
            while (object.IsPerformingScriptTask()) { Frame(); if (Term()) return; }
            if (Term()) return;
            object.UnsheatheWeapons();
            while (object.IsPerformingScriptTask()) { Frame(); if (Term()) return; }
            if (Term()) return;
        }
        Frame();
    }
}
int main()
{
    CThing me = { "Villager" }, crate = { "ScrumpyCrate" }, home = { "CrateHomeTarget" };
    CObject object = { 0 };
    gTerminateAt = 40;
    Main(me, crate, home, object);
    bool ok = gPickUps == 1 && gMoves == 1 && gDrops == 1 && gUnsheathes == 1 && gFrames == 40;
    std::printf(ok ? "00ef8d30_TEST PASS\n" : "00ef8d30_TEST FAIL\n");
    return ok ? 0 : 1;
}