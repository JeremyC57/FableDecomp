#include <cstdio>
// Behaviour model: removing the task object clears it and reports true; any other thing clears the assignee.
struct Task { const void* obj; const void* creature; };
static void remove(Task& t, const void* thing, bool& removed)
{
    if (t.obj == thing) { t.obj = 0; removed = true; } else { t.creature = 0; removed = false; }
}
int main()
{
    int a, b, c; Task t = { &a, &b }; bool r = false;
    remove(t, &c, r);
    if (r || t.creature || t.obj != &a) { std::printf("008607a0_TEST FAIL\n"); return 1; }
    remove(t, &a, r);
    if (!r || t.obj) { std::printf("008607a0_TEST FAIL\n"); return 1; }
    std::printf("008607a0_TEST PASS\n");
    return 0;
}