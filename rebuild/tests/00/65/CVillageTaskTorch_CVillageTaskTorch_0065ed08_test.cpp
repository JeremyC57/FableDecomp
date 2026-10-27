// behaviour test for 0x0065ED08: the task remembers its thing and starts unassigned
#include <cstdio>
struct Thing { int id; };
struct Task { Thing* thing; const Thing* assigned; Task(Thing& t) : thing(&t), assigned(0) { } };
int main()
{
    Thing t = { 3 }; Task task(t);
    if (task.thing != &t || task.assigned != 0) { std::printf("0065ed08_TEST FAIL\n"); return 1; }
    std::printf("0065ed08_TEST PASS\n");
    return 0;
}