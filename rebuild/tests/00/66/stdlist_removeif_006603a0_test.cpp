// behaviour test for 0x006603A0 list<CCountedPointer<CVillageTaskTorch> >::remove_if<CIsNotValid<...> >: every node whose task reports
// IsValid() == false is erased, valid ones stay in order, an empty list is untouched
#include <cstdio>
#include <list>
struct Task { int valid; int id; };
static void RemoveIfNotValid(std::list<Task*>& l)            // the function under test
{
    std::list<Task*>::iterator first = l.begin(), last = l.end();
    while (first != last) {
        std::list<Task*>::iterator next = first; ++next;
        if (!(*first)->valid) l.erase(first);
        first = next;
    }
}
int main()
{
    int fail = 0;
    Task a = { 1, 1 }, b = { 0, 2 }, c = { 1, 3 }, d = { 0, 4 };
    std::list<Task*> l; l.push_back(&a); l.push_back(&b); l.push_back(&c); l.push_back(&d);
    RemoveIfNotValid(l);
    if (l.size() != 2u || l.front()->id != 1 || l.back()->id != 3) fail |= 1;
    std::list<Task*> e; RemoveIfNotValid(e);
    if (!e.empty()) fail |= 2;
    if (sizeof(Task) != (unsigned int)8) fail |= 4;
    if (fail) { std::printf("006603a0_TEST FAIL %d\n", fail); return 1; }
    std::printf("006603a0_TEST PASS\n");
    return 0;
}