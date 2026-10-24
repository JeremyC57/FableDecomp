#include <cstdio>
#include <list>
#include <algorithm>
struct CObserver { int Ignored; CObserver() : Ignored(0) {} void IgnoreAllEvents() { ++Ignored; } };
struct CObservable {
    std::list<CObserver*> Observers; CObserver* m_pExclusiveObserver; std::list<CObserver*> ConcurrentExclusiveObservers;
    CObservable() : m_pExclusiveObserver(0) {}
    void RemoveObserver(CObserver* o) { std::list<CObserver*>::iterator it = std::find(Observers.begin(), Observers.end(), o); if (it != Observers.end()) { (*it)->IgnoreAllEvents(); Observers.erase(it); } }
};
int main()
{
    CObservable p; CObserver x, y; int bad = 0;
    p.Observers.push_back(&x); p.Observers.push_back(&y); p.RemoveObserver(&x); p.RemoveObserver(&x);
    if (p.Observers.size() != 1 || p.Observers.front() != &y || x.Ignored != 1 || y.Ignored != 0) bad = 1;
    if (bad) { std::printf("0055c9c0_TEST FAIL\n"); return 1; }
    std::printf("0055c9c0_TEST PASS\n");
    return 0;
}