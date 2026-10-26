// behaviour test for 0x00660CCE CTCVillage::RemoveThingFromTask<CVillageTaskSecuritySweep>: a task asking to be removed is found by identity and erased
#include <cstdio>
class CThing { public: int Id; };
class CVillageTaskBase {
public:
    int Owner; int Users;
    virtual ~CVillageTaskBase() { }
    virtual bool IsThingInvolvedWithTask(const CThing& t) const { return Owner == t.Id; }
    virtual void RemoveThingFromTask(const CThing&, bool& remove_task) { --Users; remove_task = Users <= 0; }
};
class CVillageTaskSecuritySweep : public CVillageTaskBase { };
template <class T> class CIntelligentPointer {
public:
    T* P;
    CIntelligentPointer(T* p) : P(p) { }
    T* operator->() const { return P; }
    T& operator*() const { return *P; }
};
template <class T> class CCountedPointer { public: T* Data; int* Info; };
class CIsTask {
public:
    CVillageTaskBase* Task;
    CIsTask(const CVillageTaskBase& task) : Task((CVillageTaskBase*)&task) { }
    template <class T> bool operator()(const CCountedPointer<T>& p) const { return (CVillageTaskBase*)p.Data == Task; }
};
struct NodeBase { NodeBase* Next; NodeBase* Prev; };
template <class T> struct Node : public NodeBase { CCountedPointer<T> Value; };
template <class T> struct Iter {
    NodeBase* N;
    Iter(NodeBase* n) : N(n) { }
    bool operator!=(const Iter& o) const { return N != o.N; }
    CCountedPointer<T>& operator*() const { return ((Node<T>*)N)->Value; }
    Iter& operator++() { N = N->Next; return *this; }
};
template <class T> struct List {
    NodeBase* S; int Size;
    Iter<T> begin() { return Iter<T>(S->Next); }
    Iter<T> end() { return Iter<T>(S); }
    Iter<T> erase(Iter<T> p) { NodeBase* n = p.N->Next; p.N->Prev->Next = n; n->Prev = p.N->Prev; --Size; return Iter<T>(n); }
};
template <class It, class P> It FindIf(It first, It last, P pred) { while (first != last && !pred(*first)) ++first; return first; }
class CTCVillage {
public:
    template <class T> void RemoveThingFromTask(CIntelligentPointer<CVillageTaskBase> task, const CThing& thing, List<T>& tasks) const
    {
        if (task->IsThingInvolvedWithTask(thing))
        {
            bool remove_task;
            task->RemoveThingFromTask(thing, remove_task);
            if (remove_task)
            {
                Iter<T> it = FindIf(tasks.begin(), tasks.end(), CIsTask(*task));
                tasks.erase(it);
            }
        }
    }
};
int main()
{
    CVillageTaskSecuritySweep t[3];
    NodeBase s; s.Next = &s; s.Prev = &s;
    Node<CVillageTaskSecuritySweep > n[3];
    List<CVillageTaskSecuritySweep > l; l.S = &s; l.Size = 0;
    for (int i = 0; i < 3; ++i) {
        t[i].Owner = 1; t[i].Users = 2; n[i].Value.Data = &t[i]; n[i].Value.Info = 0;
        n[i].Prev = s.Prev; n[i].Next = &s; s.Prev->Next = &n[i]; s.Prev = &n[i]; ++l.Size;
    }
    CThing me; me.Id = 1; CThing other; other.Id = 2;
    CTCVillage v;
    v.RemoveThingFromTask(CIntelligentPointer<CVillageTaskBase>(&t[1]), other, l);
    if (l.Size != 3 || t[1].Users != 2) { std::printf("00660cce_TEST FAIL\n"); return 1; }
    v.RemoveThingFromTask(CIntelligentPointer<CVillageTaskBase>(&t[1]), me, l);
    if (l.Size != 3 || t[1].Users != 1) { std::printf("00660cce_TEST FAIL\n"); return 1; }
    v.RemoveThingFromTask(CIntelligentPointer<CVillageTaskBase>(&t[1]), me, l);
    if (l.Size != 2 || s.Next != &n[0] || n[0].Next != &n[2]) { std::printf("00660cce_TEST FAIL\n"); return 1; }
    std::printf("00660cce_TEST PASS\n");
    return 0;
}
