// behaviour test for 0x00660062 CTCVillage::GetBestTaskForThing<CVillageTaskHorn>: the functor visits every task once, the best one comes back
#include <cstdio>
class CThing { public: int Id; };
class CVillageTaskHorn { public: int Score; int OwnerId; };
class CCPPointerInfo { public: int Count; };
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };
template <class T> class CGetBestTaskForThing {
public:
    const CThing* Thing;
    T*            PBestTask;
    CGetBestTaskForThing(const CThing& thing) : Thing(&thing), PBestTask(0) { }
    void operator()(const CCountedPointer<T>& task)
    {
        if (task.Data != 0 && task.Data->OwnerId == Thing->Id && (PBestTask == 0 || task.Data->Score > PBestTask->Score))
            PBestTask = task.Data;
    }
};
struct NodeBase { NodeBase* Next; NodeBase* Prev; };
template <class T> struct Node : public NodeBase { CCountedPointer<T> Value; };
template <class T> struct Iter {
    NodeBase* N;
    Iter(NodeBase* n) : N(n) { }
    Iter(const Iter& o) : N(o.N) { }
    bool operator!=(const Iter& o) const { return N != o.N; }
    CCountedPointer<T>& operator*() const { return ((Node<T>*)N)->Value; }
    Iter& operator++() { N = N->Next; return *this; }
};
template <class T> struct List {
    NodeBase* Sentinel;
    Iter<T> begin() { return Iter<T>(Sentinel->Next); }
    Iter<T> end() { return Iter<T>(Sentinel); }
};
template <class It, class F> F ForEach(It first, It last, F f) { for (; first != last; ++first) f(*first); return f; }
class CTCVillage {
public:
    template <class T> T* GetBestTaskForThing(const CThing& thing, List<T>& tasks) const
    {
        return ForEach(tasks.begin(), tasks.end(), CGetBestTaskForThing<T>(thing)).PBestTask;
    }
};
int main()
{
    CVillageTaskHorn t[4];
    t[0].Score = 3; t[0].OwnerId = 1;
    t[1].Score = 9; t[1].OwnerId = 2;
    t[2].Score = 7; t[2].OwnerId = 1;
    t[3].Score = 5; t[3].OwnerId = 1;
    NodeBase s; Node<CVillageTaskHorn > n[5];
    NodeBase* prev = &s;
    for (int i = 0; i < 5; ++i) { prev->Next = &n[i]; n[i].Prev = prev; prev = &n[i]; n[i].Value.Info = 0; n[i].Value.Data = i < 4 ? &t[i] : 0; }
    prev->Next = &s; s.Prev = prev;
    List<CVillageTaskHorn > l; l.Sentinel = &s;
    CThing me; me.Id = 1;
    CTCVillage v;
    if (v.GetBestTaskForThing(me, l) != &t[2]) { std::printf("00660062_TEST FAIL\n"); return 1; }
    me.Id = 2;
    if (v.GetBestTaskForThing(me, l) != &t[1]) { std::printf("00660062_TEST FAIL\n"); return 1; }
    me.Id = 3;
    if (v.GetBestTaskForThing(me, l) != 0) { std::printf("00660062_TEST FAIL\n"); return 1; }
    List<CVillageTaskHorn > e; e.Sentinel = &s; s.Next = &s;
    if (v.GetBestTaskForThing(me, e) != 0) { std::printf("00660062_TEST FAIL\n"); return 1; }
    std::printf("00660062_TEST PASS\n");
    return 0;
}