// Behaviour test for CGameCameraManager::ResetFlags() (0x00699130): a model of the row -- every live camera point that has the
// camera point component gets Activated cleared (dead, null or component-less points are skipped), then the list is emptied.
#include <stdio.h>
struct Point { bool Activated; };
struct Thing { bool dead; Point* point; };
struct Manager {
    Thing* points[4]; int count;
    void ResetFlags()
    {
        for (int i = 0; i < count; ++i) {
            Thing* t = points[i];
            if (t && !t->dead) {
                if (t->point)
                    t->point->Activated = false;
            }
        }
        count = 0;
    }
};
int main()
{
    Point p1 = { true }, p2 = { true }, p3 = { true };
    Thing alive = { false, &p1 }, dead = { true, &p2 }, bare = { false, 0 }, alive2 = { false, &p3 };
    Manager m = { { &alive, &dead, 0, &alive2 }, 4 };
    m.ResetFlags();
    int ok = !p1.Activated && p2.Activated && !p3.Activated && m.count == 0;
    m.points[0] = &bare; m.count = 1; m.ResetFlags(); ok = ok && m.count == 0;
    printf(ok ? "00699130_TEST PASS\n" : "00699130_TEST FAIL\n");
    return ok ? 0 : 1;
}