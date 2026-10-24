#include <cstdio>
#include <list>
#include <algorithm>
struct CObserver { int Ignored; CObserver() : Ignored(0) {} void IgnoreAllEvents() { ++Ignored; } };
struct CObservable {
    std::list<CObserver*> Observers; CObserver* m_pExclusiveObserver; std::list<CObserver*> ConcurrentExclusiveObservers;
    CObservable() : m_pExclusiveObserver(0) {}
    bool IsExclusivelyObserved() { if (m_pExclusiveObserver) return true; if (ConcurrentExclusiveObservers.size() != 0) return true; return false; }
};
int main()
{
    CObservable p; CObserver x; int bad = 0;
    if (p.IsExclusivelyObserved()) bad = 1;
    p.m_pExclusiveObserver = &x; if (!p.IsExclusivelyObserved()) bad = 1;
    p.m_pExclusiveObserver = 0; p.ConcurrentExclusiveObservers.push_back(&x); if (!p.IsExclusivelyObserved()) bad = 1;
    p.Observers.push_back(&x); p.ConcurrentExclusiveObservers.clear(); if (p.IsExclusivelyObserved()) bad = 1;
    if (bad) { std::printf("0055cae0_TEST FAIL\n"); return 1; }
    std::printf("0055cae0_TEST PASS\n");
    return 0;
}