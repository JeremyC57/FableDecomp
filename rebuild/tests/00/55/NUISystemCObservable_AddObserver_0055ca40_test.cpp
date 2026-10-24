#include <cstdio>
#include <list>
#include <algorithm>
struct CObserver { int Ignored; CObserver() : Ignored(0) {} void IgnoreAllEvents() { ++Ignored; } };
struct CObservable {
    std::list<CObserver*> Observers; CObserver* m_pExclusiveObserver; std::list<CObserver*> ConcurrentExclusiveObservers;
    CObservable() : m_pExclusiveObserver(0) {}
    void AddObserver(CObserver* o) { std::list<CObserver*>::iterator it = std::find(Observers.begin(), Observers.end(), o); if (it == Observers.end()) Observers.push_back(o); }
};
int main()
{
    CObservable p; CObserver x, y; int bad = 0;
    p.AddObserver(&x); p.AddObserver(&y); p.AddObserver(&x);
    if (p.Observers.size() != 2 || p.Observers.front() != &x || p.Observers.back() != &y) bad = 1;
    if (bad) { std::printf("0055ca40_TEST FAIL\n"); return 1; }
    std::printf("0055ca40_TEST PASS\n");
    return 0;
}