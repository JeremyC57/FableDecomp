// Behaviour test for 0x005367F3 NUISystem::CList::Update (RST2): model plus the body verbatim from final.cpp, the engine types stubbed.
// Checks: when the def is a front list and this list heads the component list, the layout refreshes run once, a scroll bar
// not targeting state 1 is sent there, and every scrolling child other than the selected one that is in and targeting state 3
// is sent to 4, loses states 2..5 and gets ten 0.2f updates; otherwise none of that happens. Then the base update always
// runs, and when the state change is finished: 6 -> ChangeState(0); 5 -> ChangeState(3) plus the selected child to 3;
// 0 / 3 run the inactive / selected actions. The temporary def is released.
#include <stdio.h>
#include <string.h>
#ifndef _MSC_VER
#define __forceinline inline
#define _snprintf snprintf
#endif
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail++; printf("FAIL line %d: %s\n", __LINE__, #c); } } while (0)
static int g_destroyed = 0, g_baseUpdates = 0, g_refreshes = 0;
static void* g_lastActions = 0; static int g_actionsRun = 0;
namespace std {
template <class T> struct _List_node { _List_node* _M_next; _List_node* _M_prev; T _M_data; };
template <class T> class list {
public:
    _List_node<T>* _M_node;
    bool empty() const { return _M_node->_M_next == _M_node; }
    T& front() { return _M_node->_M_next->_M_data; }
};
}
namespace NUISystem {
struct CRefCounted {
    long m_count;
    __forceinline void DecRef() { --m_count; }
    __forceinline long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00() { }
    virtual void DestroySelf() { ++g_destroyed; }
};
class CUIDef : public CDefVirtual, public CRefCounted { public:
    __forceinline void ReleaseTemporary() { DecRef(); if (GetRefCount() == 0) DestroySelf(); }
    bool IsFrontList;
};
template <class T> class CDefPointer {
public:
    T* Object;
    CDefPointer() : Object(0) {}
    CDefPointer(const CDefPointer& o) : Object(o.Object) { if (Object) const_cast<CUIDef*>(Object)->m_count++; }   // model: counted copy (no NRVO under /Od)
    T* operator->() const { return Object; }
    __forceinline ~CDefPointer() { if (Object) const_cast<CUIDef*>(Object)->ReleaseTemporary(); }
};
class CComponent;
std::list<CComponent*> g_ComponentList;
struct CActionList { int Id; };
struct CStateHolder {
    unsigned int States;
    CStateHolder() : States(0) { }
    virtual void AddState(long state) { States |= 1u << state; }
    virtual void RemoveState(long state) { States &= ~(1u << state); }
};
struct CComponentBase {
    long State, Target, Changes, LastChange, ChildUpdates; float LastTime;
    CUIDef* Def; bool Finished;
    CComponentBase() : State(0), Target(0), Changes(0), LastChange(-1), ChildUpdates(0), LastTime(0), Def(0), Finished(false) { }
    virtual void Update(float time) { ++ChildUpdates; LastTime = time; }
    virtual void ChangeState(long state) { ++Changes; LastChange = state; }
    virtual long GetState() { return State; }
    virtual long GetTargetState() { return Target; }
    virtual CDefPointer<const CUIDef> LoadAndReturnDef() const { CDefPointer<const CUIDef> p; Def->m_count++; p.Object = Def; return p; }
    virtual void ExecuteActions(CActionList* actions) { ++g_actionsRun; g_lastActions = actions; }
    virtual bool IsStateChangeFinished() { return Finished; }
    virtual void ComputeChildrenVisibility() { ++g_refreshes; }
    virtual void RefreshScrollBars() { g_refreshes += 10; }
    virtual void RefreshChildren() { g_refreshes += 100; }
};
class CComponent : public CComponentBase, public CStateHolder { public:
    long CurrentState;
    CComponent() : CurrentState(0) { }
    void Update(float time);
};
void CComponent::Update(float time) { (void)time; ++g_baseUpdates; }
struct TestChild : CComponent {
    void Update(float time) { CComponentBase::Update(time); }
};
template <class T> struct Vector {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    T& operator[](unsigned int n) { return *(_M_start + n); }
};
class CList : public CComponent { public:
    long                SelectedChild;
    float               LastInputTime;
    Vector<CComponent*> ScrollingChildren;
    Vector<CComponent*> NonScrollingChildren;
    CActionList*        PActionsOnInactive;
    CActionList*        PActionsOnSelected;
    CComponent*         PScrollBar;
    void Update(float time);
};

void CList::Update(float time)
{
    bool active = LoadAndReturnDef()->IsFrontList && !g_ComponentList.empty() && this == g_ComponentList.front();
    if (active) {
        ComputeChildrenVisibility();
        RefreshChildren();
        RefreshScrollBars();
        if (PScrollBar && PScrollBar->GetTargetState() != 1)
            PScrollBar->ChangeState(1);
        for (unsigned int i = 0; i < ScrollingChildren.size(); ++i) {
            if (i != (unsigned int)SelectedChild && ScrollingChildren[i]->GetState() == 3 && ScrollingChildren[i]->GetTargetState() == 3) {
                ScrollingChildren[i]->ChangeState(4);
                { CComponent* c = ScrollingChildren[i]; c->RemoveState(2); }
                { CComponent* c = ScrollingChildren[i]; c->RemoveState(3); }
                { CComponent* c = ScrollingChildren[i]; c->RemoveState(4); }
                { CComponent* c = ScrollingChildren[i]; c->RemoveState(5); }
                for (int k = 0; k < 10; ++k)
                    ScrollingChildren[i]->Update(0.2f);   // 0.2f @ 0x012305A0
            }
        }
    }
    CComponent::Update(time);
    if (IsStateChangeFinished()) {
        if (CurrentState == 6)
            ChangeState(0);
        if (CurrentState == 5) {
            ChangeState(3);
            if (ScrollingChildren.size() > 0)
                ScrollingChildren[SelectedChild]->ChangeState(3);
        }
        if (CurrentState == 0)
            ExecuteActions(PActionsOnInactive);
        if (CurrentState == 3)
            ExecuteActions(PActionsOnSelected);
    }
}
}
using namespace NUISystem;

int main()
{
    CUIDef def; def.m_count = 1; def.IsFrontList = true;
    TestChild kids[4]; CComponent* arr[4];
    for (int i = 0; i < 4; ++i) { arr[i] = &kids[i]; kids[i].State = 3; kids[i].Target = 3; kids[i].States = 0x3f; }
    kids[2].Target = 4;                                 // not targeting 3: left alone
    TestChild bar; bar.Target = 0;
    CList list; list.Def = &def; list.SelectedChild = 1;
    list.ScrollingChildren._M_start = arr; list.ScrollingChildren._M_finish = arr + 4; list.ScrollingChildren._M_end_of_storage = arr + 4;
    CActionList inactive = { 1 }, selected = { 2 };
    list.PActionsOnInactive = &inactive; list.PActionsOnSelected = &selected; list.PScrollBar = &bar;
    std::_List_node<CComponent*> head, node;
    head._M_next = &node; head._M_prev = &node; node._M_next = &head; node._M_prev = &head; node._M_data = &list;
    g_ComponentList._M_node = &head;

    list.Update(1.5f);                                  // active, state change not finished
    CHECK(g_refreshes == 111);
    CHECK(bar.Changes == 1 && bar.LastChange == 1);
    CHECK(kids[0].LastChange == 4 && kids[0].States == 0x03 && kids[0].ChildUpdates == 10 && kids[0].LastTime == 0.2f);
    CHECK(kids[3].LastChange == 4 && kids[3].States == 0x03 && kids[3].ChildUpdates == 10);
    CHECK(kids[1].Changes == 0 && kids[1].States == 0x3f && kids[1].ChildUpdates == 0);   // the selected child
    CHECK(kids[2].Changes == 0 && kids[2].States == 0x3f && kids[2].ChildUpdates == 0);
    CHECK(g_baseUpdates == 1 && g_actionsRun == 0 && list.Changes == 0);
    CHECK(def.m_count == 1 && g_destroyed == 0);

    bar.Target = 1; kids[0].Changes = 0; kids[0].State = 2;
    list.Update(1.0f);                                  // bar already targeting 1; child 0 no longer in 3
    CHECK(bar.Changes == 1 && kids[0].Changes == 0 && kids[3].Changes == 2);   // kids[3] still 3/3 so sent again
    CHECK(g_refreshes == 222 && g_baseUpdates == 2);

    def.IsFrontList = false; kids[3].Changes = 0;
    list.Update(1.0f);                                  // not a front list: nothing refreshed
    CHECK(g_refreshes == 222 && kids[3].Changes == 0 && g_baseUpdates == 3);
    def.IsFrontList = true; node._M_data = &bar;
    list.Update(1.0f);                                  // not the front component
    CHECK(g_refreshes == 222 && kids[3].Changes == 0 && g_baseUpdates == 4);
    head._M_next = &head;
    list.Update(1.0f);                                  // empty component list
    CHECK(g_refreshes == 222 && g_baseUpdates == 5);

    list.Finished = true;
    list.CurrentState = 6; list.Update(0); CHECK(list.LastChange == 0 && list.Changes == 1 && g_actionsRun == 0);
    list.CurrentState = 5; kids[1].Changes = 0; list.Update(0);
    CHECK(list.LastChange == 3 && list.Changes == 2 && kids[1].Changes == 1 && kids[1].LastChange == 3 && g_actionsRun == 0);
    list.CurrentState = 0; list.Update(0); CHECK(g_actionsRun == 1 && g_lastActions == &inactive && list.Changes == 2);
    list.CurrentState = 3; list.Update(0); CHECK(g_actionsRun == 2 && g_lastActions == &selected);
    list.CurrentState = 5; list.ScrollingChildren._M_finish = arr; kids[1].Changes = 0; list.Update(0);
    CHECK(list.Changes == 3 && kids[1].Changes == 0);   // no children: selected child untouched
    CHECK(def.m_count == 1 && g_destroyed == 0);
    def.m_count = 0; list.Update(0); CHECK(g_destroyed == 1);   // last reference released by the temporary
    char line[64];
    _snprintf(line, sizeof(line), "005367f3_TEST %s", g_fail ? "FAIL" : "PASS");
    puts(line);
    return g_fail ? 1 : 0;
}