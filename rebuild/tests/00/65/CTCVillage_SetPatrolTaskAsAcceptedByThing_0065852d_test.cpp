// behaviour test for 0x0065852D CTCVillage::SetPatrolTaskAsAcceptedByThing(CVillageTaskPatrol&, const CThing&): an available task is
// assigned without asking the suitability test; an unavailable one is assigned only when the creature is more appropriate
#include <cstdio>
struct Task { int available; int better; int asked; int assigned; int who; };
static bool Set(Task& task, int thing)                       // the function under test
{
    if (task.available || (++task.asked, task.better)) {
        task.assigned++; task.who = thing;
        return true;
    }
    return false;
}
int main()
{
    int fail = 0;
    Task a = { 1, 0, 0, 0, 0 };
    if (!Set(a, 7) || a.asked != 0 || a.assigned != 1 || a.who != 7) fail |= 1;
    Task b = { 0, 1, 0, 0, 0 };
    if (!Set(b, 9) || b.asked != 1 || b.assigned != 1 || b.who != 9) fail |= 2;
    Task c = { 0, 0, 0, 0, 0 };
    if (Set(c, 5) || c.asked != 1 || c.assigned != 0) fail |= 4;
    if (sizeof(Task) != (unsigned int)20) fail |= 8;
    if (fail) { std::printf("0065852d_TEST FAIL %d\n", fail); return 1; }
    std::printf("0065852d_TEST PASS\n");
    return 0;
}