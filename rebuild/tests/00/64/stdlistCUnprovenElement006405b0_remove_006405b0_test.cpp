// behaviour test for 0x006405B0 list<CUnprovenElement_006405b0*>::remove (model: every node equal to the value is erased)
#include <cstdio>
#include <list>
int main()
{
    int a = 1, b = 2;
    std::list<int*> l; l.push_back(&a); l.push_back(&b); l.push_back(&a);
    l.remove(&a);
    int ok = l.size() == 1 && l.front() == &b;
    if (!ok) { std::printf("006405b0_TEST FAIL\n"); return 1; }
    std::printf("006405b0_TEST PASS\n");
    return 0;
}