// Behaviour test for CAIStateGroup_ScreamerPursueTarget::OnReplace (0x0096DDB0): same control flow over mocks.
#include <cstdio>
struct CThing { bool dead; bool hasStealth; int dangerSetBy; int dangerStatus; };
struct CState {
    CThing* creature; CThing* target; bool inMode44; int outputs; unsigned long lastFlags; CThing* actionTarget;
    void OnReplace()
    {
        if (target && !target->dead) {
            if (target->hasStealth) { target->dangerSetBy = 1; target->dangerStatus = 0; }
        }
        if (target && !target->dead && !inMode44) {
            unsigned long flags = 0;
            flags |= 8;
            actionTarget = target;
            lastFlags = flags; ++outputs;
        }
    }
};
int main()
{
    CThing me = { false, false, 0, -1 };
    CThing t = { false, true, 0, -1 };
    CState s; s.creature = &me; s.target = &t; s.inMode44 = false; s.outputs = 0; s.lastFlags = 0; s.actionTarget = 0;
    s.OnReplace();
    bool ok = t.dangerSetBy == 1 && t.dangerStatus == 0 && s.outputs == 1 && s.lastFlags == 8 && s.actionTarget == &t;
    s.inMode44 = true; t.dangerSetBy = 0; s.OnReplace();
    ok = ok && t.dangerSetBy == 1 && s.outputs == 1;
    t.dead = true; t.dangerSetBy = 0; s.inMode44 = false; s.OnReplace();
    ok = ok && t.dangerSetBy == 0 && s.outputs == 1;
    s.target = 0; s.OnReplace();
    ok = ok && s.outputs == 1;
    if (!ok) { std::printf("0096ddb0_TEST FAIL\n"); return 1; }
    std::printf("0096ddb0_TEST PASS\n");
    return 0;
}
