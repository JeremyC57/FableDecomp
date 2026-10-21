// Behaviour test for CCombatAction_GenericResponseToDrain::Initialise (0x008676A0): with a live drainer the creature
// faces directly away from it in the ground plane and receives a drained mode naming the drainer; base Initialise always.
#include <cstdio>
#include <cmath>
#include <string>

struct C3DVector { float X, Y, Z; };
struct CThing { C3DVector Pos; bool Dead; C3DVector Up, Forward; std::string Mode; CThing* Drainer; };
struct CAction {
    CThing* PCreature; CThing* PTarget; std::string Anim; int BaseInit;
    void Initialise()
    {
        Anim = "GENERIC_RESPONSE_DRAIN";
        if (PTarget && !PTarget->Dead) {
            const C3DVector& d = PTarget->Pos; const C3DVector& p = PCreature->Pos;
            C3DVector away = { p.X - d.X, p.Y - d.Y, 0.0f };
            float l = std::sqrt(away.X * away.X + away.Y * away.Y);
            away.X /= l; away.Y /= l;
            C3DVector up = { 0, 0, 1 };
            PCreature->Up = up; PCreature->Forward = away;
            PCreature->Mode = "DRAINED"; PCreature->Drainer = PTarget;
        }
        ++BaseInit;
    }
};

int main()
{
    CThing me = { { 0, 0, 0 }, false, { 0, 0, 0 }, { 0, 0, 0 }, "", 0 };
    CThing foe = { { 3, 4, 9 }, false, { 0, 0, 0 }, { 0, 0, 0 }, "", 0 };
    CAction a = { &me, &foe, "", 0 };
    a.Initialise();
    bool ok = a.Anim == "GENERIC_RESPONSE_DRAIN" && a.BaseInit == 1 && me.Mode == "DRAINED" && me.Drainer == &foe
        && std::fabs(me.Forward.X + 0.6f) < 1e-5f && std::fabs(me.Forward.Y + 0.8f) < 1e-5f && me.Forward.Z == 0 && me.Up.Z == 1;
    CThing me2 = { { 0, 0, 0 }, false, { 0, 0, 0 }, { 0, 0, 0 }, "", 0 };
    foe.Dead = true; a.PCreature = &me2;
    a.Initialise();
    ok = ok && a.BaseInit == 2 && me2.Mode.empty();
    if (!ok) { std::printf("008676a0_TEST FAIL\n"); return 1; }
    std::printf("008676a0_TEST PASS\n");
    return 0;
}