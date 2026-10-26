// behaviour test for 0x00660894 CTCVillage::RemoveThingFromTaskList<CVillageTaskHorn>: involved tasks drop the thing, removable ones are erased
#include <cstdio>
class CThing { public: int Id; };
class CVillageTaskBase {
public:
    int Owner; int Users; int Calls;
    virtual ~CVillageTaskBase() { }
    virtual bool IsThingInvolvedWithTask(const CThing& t) const { return Owner == t.Id; }
    virtual void RemoveThingFromTask(const CThing& t, bool& remove_task) { ++Calls; --Users; remove_task = Users <= 0; }
};
class CVillageTaskHorn : public CVillageTaskBase { };
template <class T> class CCountedPointer {
public:
    T* Data; int* Info;
    CCountedPointer(T* p) : Data(p), Info(0) { }
    template <class U> CCountedPointer(const CCountedPointer<U>& o) : Data(o.Data), Info(o.Info) { }
    T* operator->() const { return Data; }
};
struct NodeBase { NodeBase* Next; NodeBase* Prev; };
template <class T> struct Node : public NodeBase { Node(T* p) : Value(p) { } CCountedPointer<T> Value; };
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
class CTCVillage {
public:
    template <class T> void RemoveThingFromTaskList(const CThing& thing, List<T>& tasks) const
    {
        Iter<T> it = tasks.begin();
        Iter<T> end = tasks.end();
        Iter<T> next = it;
        while (it != end)
        {
            ++next;
            bool involved;
            {
                CCountedPointer<CVillageTaskBase> task(*it);
                involved = task->IsThingInvolvedWithTask(thing);
            }
            if (involved)
            {
                bool remove_task;
                (*it)->RemoveThingFromTask(thing, remove_task);
                if (remove_task)
                    tasks.erase(it);
            }
            it = next;
        }
    }
};
int main()
{
    CVillageTaskHorn t[4];
    int owners[4] = { 1, 2, 1, 1 }, users[4] = { 1, 1, 2, 1 };
    NodeBase s; s.Next = &s; s.Prev = &s;
    Node<CVillageTaskHorn >* n[4];
    List<CVillageTaskHorn > l; l.S = &s; l.Size = 0;
    for (int i = 0; i < 4; ++i) {
        t[i].Owner = owners[i]; t[i].Users = users[i]; t[i].Calls = 0;
        n[i] = new Node<CVillageTaskHorn >(&t[i]);
        n[i]->Prev = s.Prev; n[i]->Next = &s; s.Prev->Next = n[i]; s.Prev = n[i]; ++l.Size;
    }
    CThing me; me.Id = 1;
    CTCVillage v;
    v.RemoveThingFromTaskList(me, l);
    if (l.Size != 2 || t[0].Calls != 1 || t[1].Calls != 0 || t[2].Calls != 1 || t[3].Calls != 1) { std::printf("00660894_TEST FAIL\n"); return 1; }
    if (s.Next != n[1] || n[1]->Next != n[2] || n[2]->Next != &s) { std::printf("00660894_TEST FAIL\n"); return 1; }
    v.RemoveThingFromTaskList(me, l);
    if (l.Size != 1 || s.Next != n[1] || t[2].Calls != 2) { std::printf("00660894_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 4; ++i) delete n[i];
    std::printf("00660894_TEST PASS\n");
    return 0;
}