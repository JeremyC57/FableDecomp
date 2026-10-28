// Behaviour test for 0x0065FFCA std::find_if over the village horn tasks with CIsHornTaskWithThisHorn: the first task using the horn, else last (standalone copy of the logic)
#include <cstdio>
#include <list>
struct Task { int Horn; };
struct IsHornTaskWithThisHorn { int PHorn; bool operator()(const Task* t) const { return t->Horn == PHorn; } };
template <class It, class P> It FindIf(It first, It last, P pred) { while (first != last && !pred(*first)) ++first; return first; }

int main()
{
    Task a = { 1 }, b = { 2 }, c = { 2 }; std::list<Task*> l; l.push_back(&a); l.push_back(&b); l.push_back(&c);
    IsHornTaskWithThisHorn p; p.PHorn = 2;
    std::list<Task*>::iterator it = FindIf(l.begin(), l.end(), p);
    if (it == l.end() || *it != &b) { std::printf("0065ffca_TEST FAIL\n"); return 1; }
    p.PHorn = 5; if (FindIf(l.begin(), l.end(), p) != l.end()) { std::printf("0065ffca_TEST FAIL\n"); return 1; }
    std::printf("0065ffca_TEST PASS\n");
    return 0;
}