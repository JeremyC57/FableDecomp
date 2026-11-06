// Behaviour test for NScript::CExpression_FishScript::Main (0x00EE95C0): the rod choice (last used target, else upgraded rod, else
// the plain rod), the first enabled fishing spot, the 10-unit test on the hero / water-height point (X*X + Y*Y + Z*Z < 100), the
// catch handling (disable the spot and give its items when caught at a spot, else give a live fish), and the terminate exits.
#include <cstdio>
#include <cstring>
struct Vec { float X, Y, Z; };
struct Sim {
    char log[1024]; int frame, termFrame, finishFrame, spotCount, enabledSpot; bool hasUpgraded, fishAlive; const char* target;
    Vec hero, spotPos[4]; float water;
    bool Term() const { return frame >= termFrame; }
    void Ev(const char* e) { std::strcat(log, e); std::strcat(log, ";"); }
    void Main()
    {
        if (Term()) return;
        if (target[0] != 0) { if (Term()) return; Ev(target); }
        else { if (Term()) return; Ev(hasUpgraded ? "OBJECT_FISHING_ROD_UPGRADED" : "OBJECT_FISHING_ROD"); }
        bool caught = false, atSpot = false; int spot = -1;
        if (spotCount > 0) {
            if (Term()) return;
            for (int i = 0; i < spotCount; ++i) { if (Term()) return; if (i == enabledSpot) { if (Term()) return; spot = i; break; } }
            if (Term()) return;
            if (spot >= 0) {
                Vec pos = spotPos[spot]; pos.Z = water;
                Vec d = { hero.X - pos.X, hero.Y - pos.Y, hero.Z - pos.Z };
                if (d.X * d.X + d.Y * d.Y + d.Z * d.Z < 10.0f * 10.0f) { if (Term()) return; atSpot = true; Ev("FACE"); }
            }
        }
        Ev(atSpot ? "GOFISH_SPOT" : "GOFISH");
        while (frame < finishFrame) { ++frame; if (Term()) return; }
        if (Term()) return;
        if (fishAlive) { caught = true; Ev("SUCCESS"); } else Ev("FAILURE");
        if (caught && atSpot) Ev("DISABLE");
        if (caught && atSpot) Ev("CONTAINER;WEIGH;REMOVE");
        else if (fishAlive) Ev("WEIGH;GIVE");
        Ev("DEACTIVATE");
    }
};
static void Make(Sim& s) { s.log[0] = 0; s.frame = 0; s.termFrame = 100; s.finishFrame = 3; s.spotCount = 2; s.enabledSpot = 1;
    s.hasUpgraded = false; s.fishAlive = true; s.target = ""; Vec h = { 0, 0, 0 }; s.hero = h; Vec a = { 50, 0, 0 }, b = { 3, 4, 9 };
    s.spotPos[0] = a; s.spotPos[1] = b; s.water = 5.0f; }
int main()
{
    Sim a; Make(a); a.Main();
    bool ok = std::strcmp(a.log, "OBJECT_FISHING_ROD;FACE;GOFISH_SPOT;SUCCESS;DISABLE;CONTAINER;WEIGH;REMOVE;DEACTIVATE;") == 0;
    Sim b; Make(b); b.target = "OBJECT_ROD_X"; b.water = 20.0f; b.Main();      // 3,4,-20: 9+16+400 > 100
    ok = ok && std::strcmp(b.log, "OBJECT_ROD_X;GOFISH;SUCCESS;WEIGH;GIVE;DEACTIVATE;") == 0;
    Sim c; Make(c); c.hasUpgraded = true; c.fishAlive = false; c.enabledSpot = 5; c.Main();
    ok = ok && std::strcmp(c.log, "OBJECT_FISHING_ROD_UPGRADED;GOFISH;FAILURE;DEACTIVATE;") == 0;
    Sim d; Make(d); d.termFrame = 2; d.Main();
    ok = ok && std::strcmp(d.log, "OBJECT_FISHING_ROD;FACE;GOFISH_SPOT;") == 0;
    std::printf(ok ? "00ee95c0_TEST PASS\n" : "00ee95c0_TEST FAIL\n");
    return ok ? 0 : 1;
}