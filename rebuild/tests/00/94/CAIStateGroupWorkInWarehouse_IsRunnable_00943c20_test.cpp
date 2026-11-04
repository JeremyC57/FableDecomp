// Behaviour test for CAIStateGroup_WorkInWarehouse::IsRunnable() (0x00943C20): a model of the row -- runnable only for a village
// member in working hours whose village returns a crate task for the creature; the task is remembered (cleared when none).
#include <stdio.h>
struct Task { int id; };
struct Village { Task* best; int asked; Task* GetBestCrateTaskForThing() { ++asked; return best; } };
struct Group {
    bool isMember, workHours; Village* village; Task* PTask;
    bool IsRunnable()
    {
        if (isMember && workHours) {
            Village* v = village;
            if (v != 0) {
                PTask = v->GetBestCrateTaskForThing();
                if (PTask != 0)
                    return true;
            }
        }
        return false;
    }
};
int main()
{
    Task t = { 7 }; Village v = { &t, 0 };
    Group g = { true, true, &v, 0 };
    int ok = g.IsRunnable() && g.PTask == &t && v.asked == 1;
    v.best = 0; ok = ok && !g.IsRunnable() && g.PTask == 0 && v.asked == 2;
    v.best = &t; g.workHours = false; ok = ok && !g.IsRunnable() && v.asked == 2;
    g.workHours = true; g.isMember = false; ok = ok && !g.IsRunnable();
    g.isMember = true; g.village = 0; g.PTask = &t; ok = ok && !g.IsRunnable() && g.PTask == &t;
    printf(ok ? "00943c20_TEST PASS\n" : "00943c20_TEST FAIL\n");
    return ok ? 0 : 1;
}