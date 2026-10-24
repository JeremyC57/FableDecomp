#include <cstdio>
#include <list>
#include <algorithm>
struct CObserver { int Ignored; CObserver() : Ignored(0) {} void IgnoreAllEvents() { ++Ignored; } };
struct CObservable {
    std::list<CObserver*> Observers; CObserver* m_pExclusiveObserver; std::list<CObserver*> ConcurrentExclusiveObservers;
    CObservable() : m_pExclusiveObserver(0) {}
    void AddConcurrentExclusiveObserver(CObserver* o) { std::list<CObserver*>::iterator it = std::find(ConcurrentExclusiveObservers.begin(), ConcurrentExclusiveObservers.end(), o); if (it == ConcurrentExclusiveObservers.end()) ConcurrentExclusiveObservers.push_back(o); }
};
int main()
{
    CObservable p; CObserver x, y; int bad = 0;
    p.AddConcurrentExclusiveObserver(&x); p.AddConcurrentExclusiveObserver(&y); p.AddConcurrentExclusiveObserver(&x);
    if (p.ConcurrentExclusiveObservers.size() != 2 || p.ConcurrentExclusiveObservers.front() != &x || p.ConcurrentExclusiveObservers.back() != &y) bad = 1;
    if (bad) { std::printf("0055ca90_TEST FAIL\n"); return 1; }
    std::printf("0055ca90_TEST PASS\n");
    return 0;
}