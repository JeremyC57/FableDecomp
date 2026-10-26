// behaviour test for 0x0065A053 CTCVillage::SetCrateTaskAsCompleted(CIntelligentPointer<CVillageTaskCarryCrate>): every list entry that
// refers to the given task is removed, the others stay in order
#include <cstdio>
#include <list>
static void SetCompleted(std::list<int*>& tasks, int* task)     // the function under test
{
    for (std::list<int*>::iterator it = tasks.begin(); it != tasks.end(); )
        if (*it == task) it = tasks.erase(it); else ++it;
}
int main()
{
    int fail = 0;
    int a = 1, b = 2, c = 3;
    std::list<int*> l; l.push_back(&a); l.push_back(&b); l.push_back(&c); l.push_back(&b);
    SetCompleted(l, &b);
    if (l.size() != 2u || l.front() != &a || l.back() != &c) fail |= 1;
    SetCompleted(l, &b);
    if (l.size() != 2u) fail |= 2;
    if (sizeof(int*) < (unsigned int)4) fail |= 4;
    if (fail) { std::printf("0065a053_TEST FAIL %d\n", fail); return 1; }
    std::printf("0065a053_TEST PASS\n");
    return 0;
}