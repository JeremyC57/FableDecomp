// behaviour test for 0x00660CCE CTCVillage::RemoveThingFromTask<CVillageTaskSecuritySweep> (model: an involved task that asks to go is erased)
#include <cstdio>
#include <list>
struct Task { bool involved, goes; bool removed; };
int main()
{
    Task a = { true, true, false }, b = { true, false, false };
    std::list<Task*> tasks; tasks.push_back(&a); tasks.push_back(&b);
    Task* ts[2] = { &a, &b };
    for (int i = 0; i < 2; ++i) {
        Task* t = ts[i];
        if (t->involved) { t->removed = true; if (t->goes) { for (std::list<Task*>::iterator it = tasks.begin(); it != tasks.end(); ++it) if (*it == t) { tasks.erase(it); break; } } }
    }
    int ok = tasks.size() == 1 && tasks.front() == &b && a.removed && b.removed;
    if (!ok) { std::printf("00660cce_TEST FAIL\n"); return 1; }
    std::printf("00660cce_TEST PASS\n");
    return 0;
}
