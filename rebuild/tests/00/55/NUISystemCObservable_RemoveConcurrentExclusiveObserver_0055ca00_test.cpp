#include <cstdio>
#include <list>
#include <algorithm>
struct CObserver { int Ignored; CObserver() : Ignored(0) {} void IgnoreAllEvents() { ++Ignored; } };
struct CObservable {
    std::list<CObserver*> Observers; CObserver* m_pExclusiveObserver; std::list<CObserver*> ConcurrentExclusiveObservers;
    CObservable() : m_pExclusiveObserver(0) {}
    void RemoveConcurrentExclusiveObserver(CObserver* o) { std::list<CObserver*>::iterator it = std::find(ConcurrentExclusiveObservers.begin(), ConcurrentExclusiveObservers.end(), o); if (it != ConcurrentExclusiveObservers.end()) { (*it)->IgnoreAllEvents(); ConcurrentExclusiveObservers.erase(it); } }
};
int main()
{
    CObservable p; CObserver x, y; int bad = 0;
    p.ConcurrentExclusiveObservers.push_back(&x); p.ConcurrentExclusiveObservers.push_back(&y); p.RemoveConcurrentExclusiveObserver(&x); p.RemoveConcurrentExclusiveObserver(&x);
    if (p.ConcurrentExclusiveObservers.size() != 1 || p.ConcurrentExclusiveObservers.front() != &y || x.Ignored != 1 || y.Ignored != 0) bad = 1;
    if (bad) { std::printf("0055ca00_TEST FAIL\n"); return 1; }
    std::printf("0055ca00_TEST PASS\n");
    return 0;
}