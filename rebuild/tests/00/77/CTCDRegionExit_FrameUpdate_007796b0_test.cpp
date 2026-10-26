// Behaviour test for CTCDRegionExit::FrameUpdate (0x007796B0): inside the radius the hero is moved through the exit once
// (until it leaves); inside the message radius the letterbox starts; leaving it ends the letterbox.
#include <cstdio>

static int g_moves = 0, g_starts = 0, g_ends = 0, g_names = 0, g_finishes = 0;
static const float kEps = 1e-4f;
class C3DVector { public: float X, Y, Z; };
class CThing { public: C3DVector Pos; bool Dead; float Health; };
struct CRegionExit {
    CThing* Parent; CThing* Hero; bool Connected; bool ShowNames; bool GuiBusy;
    bool Active, InLetterBoxMode, WaitingForActivationOrPlayerToLeave, WaitingForPlayerToLeaveMessageArea;
    float Radius, MessageRadius;
    void MoveHeroThroughExit() { ++g_moves; }
    void FrameUpdate()
    {
        if (!Active || !Connected) return;
        CThing* hero = Hero;
        if (!hero || hero->Dead) return;
        bool moveHero = false;
        const C3DVector& exitPos = Parent->Pos;
        const C3DVector& heroPos = hero->Pos;
        float dx = heroPos.X - exitPos.X, dy = heroPos.Y - exitPos.Y, dz = heroPos.Z - exitPos.Z;
        float distSq = dz * dz + dy * dy + dx * dx;
        bool inside = distSq < Radius * Radius;
        if (!WaitingForActivationOrPlayerToLeave && inside) moveHero = true;
        else if (MessageRadius > kEps && kEps + Radius < MessageRadius && distSq < MessageRadius * MessageRadius) {
            if (!WaitingForPlayerToLeaveMessageArea) {
                if (!InLetterBoxMode) { ++g_starts; if (ShowNames) ++g_names; }
                InLetterBoxMode = true;
            }
        } else if (InLetterBoxMode && !GuiBusy) {
            ++g_ends; if (ShowNames) ++g_finishes;
            InLetterBoxMode = false;
        }
        if (!inside) WaitingForActivationOrPlayerToLeave = false;
        if (distSq > MessageRadius * MessageRadius) WaitingForPlayerToLeaveMessageArea = false;
        if (moveHero && hero->Health > kEps) { MoveHeroThroughExit(); WaitingForActivationOrPlayerToLeave = true; }
    }
};

int main()
{
    CThing exitThing = { { 0, 0, 0 }, false, 1.0f };
    CThing hero = { { 8, 0, 0 }, false, 50.0f };
    CRegionExit e = { &exitThing, &hero, true, true, false, true, false, false, false, 2.0f, 10.0f };
    e.FrameUpdate();                                  // in the message area
    if (g_starts != 1 || g_names != 1 || !e.InLetterBoxMode) { std::printf("007796b0_TEST FAIL\n"); return 1; }
    hero.Pos.X = 1.0f; e.FrameUpdate();               // inside: move once
    e.FrameUpdate();                                  // still inside: waiting, no second move
    if (g_moves != 1 || !e.WaitingForActivationOrPlayerToLeave) { std::printf("007796b0_TEST FAIL\n"); return 1; }
    hero.Pos.X = 20.0f; e.FrameUpdate();              // outside everything: letterbox ends, flags re-armed
    if (g_ends != 1 || g_finishes != 1 || e.InLetterBoxMode || e.WaitingForActivationOrPlayerToLeave) { std::printf("007796b0_TEST FAIL\n"); return 1; }
    hero.Dead = true; hero.Pos.X = 0.0f; e.FrameUpdate();
    if (g_moves != 1) { std::printf("007796b0_TEST FAIL\n"); return 1; }
    std::printf("007796b0_TEST PASS\n");
    return 0;
}