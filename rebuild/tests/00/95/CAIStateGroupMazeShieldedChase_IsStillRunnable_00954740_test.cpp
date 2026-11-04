// Behaviour test for CAIStateGroup_MazeShieldedChase::IsStillRunnable (0x00954740): a model of the row -- false when the target
// is missing or dead or the group is stopping; false (and the will-power indicator, if any, reset to -1) when the creature has
// lost its physical shield; true otherwise.
#include <stdio.h>
struct Thing { bool Dead; bool Shield; bool HasIndicator; float Indicator; };
struct Group { Thing* Target; bool Stopping; Thing* Creature;
    bool IsStillRunnable()
    {
        if (!(Target != 0 && !Target->Dead) || Stopping)
            return false;
        if (!Creature->Shield) {
            if (Creature->HasIndicator)
                Creature->Indicator = -1.0f;
            return false;
        }
        return true;
    }
};
int main()
{
    Thing target = { false, false, false, 0 }, creature = { false, true, true, 5.0f };
    Group g = { &target, false, &creature };
    int ok = g.IsStillRunnable() && creature.Indicator == 5.0f;
    creature.Shield = false; ok = ok && !g.IsStillRunnable() && creature.Indicator == -1.0f;
    creature.Shield = true; target.Dead = true; ok = ok && !g.IsStillRunnable();
    target.Dead = false; g.Stopping = true; ok = ok && !g.IsStillRunnable();
    g.Stopping = false; g.Target = 0; ok = ok && !g.IsStillRunnable();
    printf(ok ? "00954740_TEST PASS\n" : "00954740_TEST FAIL\n");
    return ok ? 0 : 1;
}