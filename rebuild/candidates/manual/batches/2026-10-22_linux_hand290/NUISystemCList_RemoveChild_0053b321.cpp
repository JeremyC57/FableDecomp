// NUISystem::CList::RemoveChild(long index) -- remove child `index`: when it is a scrolling child, shift the rows after it,
//   keep the selection in range, drop its row, state map and visibility entry and re-arm the remaining children (states
//   0x19, 2-5 on the selection) before recomputing offsets; otherwise drop it from the non-scrolling children. Then the
//   base removal; an emptied list sends action 1, and when it heads the list registry it gives the head state 0 / 6,
//   pops it and wakes the next list (state 3); finally the up / down arrows get the new item counts.
// Leaf: ?RemoveChild@CList@NUISystem@
// Retail: Fable.exe @ 0x0053B321, 795 bytes, `ret 4`; frame-pointer size TU (optimize "s" on, "y" off) as the landed
//   CList::AddScrollingChildDynamically 0x0053BEAC / ScrollToFirstChild 0x005363EA.
// Callees: CCountedPointer<CComponent> copy 0x00428928 / dtor 0x004291DE, std::find over CComponent* 0x0053C91C (LTCG
//   register convention: first ecx, last edx, value by const&), vector<CComponent*>::erase 0x00485468, the state-map
//   vector's erase 0x0053CD05, the visibility vector's erase 0x0053C7EF, CManager::GetInstance 0x0041E5F2 (slot 2
//   AddInputListener on the +4 base), CChangingStateComponent::RemoveChild 0x00533BC0 (direct base call), CAction ctor
//   0x0042BED4 / copy 0x0042CD84 / list dtor 0x0042ABCA, the list registry 0x013B8A98 with pop_front 0x0053C7D9.
//   Component slots 48 ChangeState, 62 ProcessAction (CAction by value), 108 GetDef, 150 / 161 / 165 / 181 (descriptive
//   names); +4 base slot 3 AddState.
// Layout: Ego_r.pdb CList, shifted to retail by the landed CList models: SelectedChild +0x15c, ScrollingChildren +0x164,
//   NonScrollingChildren +0x170, ScrollingChildrenStates +0x17c (12-byte maps), ScrollingChildrenVisibility +0x1c8,
//   PListUp +0x1e0, PListDown +0x1e4 (PDB 548 / 552 less the three 4-byte-smaller STLport vectors and the 12-byte
//   vector<bool>); the base child vector of counted pointers +0xb0 as the landed AddScrollingChildDynamically. Def fields
//   +0x13c / +0x221 are named descriptively (the retail def does not follow Ego_r.pdb CUIDef).
// Shape: the scrolling search caches the end in a local (compared in ebx); the non-scrolling one passes a local end but
//   compares a fresh end() (else VC keeps &finish, not &vector, in ebx); the arrows are cached in locals; the down count
//   reads a named def pointer (in the dead `index` slot) so the release follows the call; `if (restore) ...(0); else
//   ...(6);` keeps the push 0 / push 6 branches (a ?: becomes sbb/and/add).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define V1(n) virtual void n();
#define V2(n) V1(n##0) V1(n##1)
#define V4(n) V2(n##0) V2(n##1)
#define V8(n) V4(n##0) V4(n##1)
#define V16(n) V8(n##0) V8(n##1)
#define V32(n) V16(n##0) V16(n##1)
#define V64(n) V32(n##0) V32(n##1)

struct CRefCounted {
    long m_count;
    __forceinline void DecRef() { --m_count; }
    __forceinline long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void d00();
    virtual void Destroy();                          // slot 1 (+0x04)
};
struct CDefBase : CDefVirtual, CRefCounted {
    __forceinline void ReleaseTemporary() { DecRef(); if (GetRefCount() == 0) Destroy(); }
};
struct CListDef : CDefBase {
    unsigned char _pad08[0x13c - 0x08];
    long          ScrollLimit;                       // +0x13c (descriptive)
    unsigned char _pad140[0x221 - 0x140];
    bool          RestoreRegistryState;              // +0x221 (descriptive)
};
struct CListDefPointer {
    CListDef* Object;
    __forceinline ~CListDefPointer() { if (Object) Object->ReleaseTemporary(); }
    CListDef* operator->() const { return Object; }
};
struct CCPPointerInfo;
template <class T> struct CCountedPointer {
    T*              m_pObject;
    CCPPointerInfo* m_pInfo;
    CCountedPointer(const CCountedPointer& other);   // 0x00428928
    ~CCountedPointer();                              // 0x004291DE
    T* GetPtr() const { return m_pObject; }
};
template <class T> struct vector {                   // STLport vector, 12 bytes
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    bool empty() const { return _M_start == _M_finish; }
    T& operator[](unsigned long i) { return _M_start[i]; }
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    T* erase(T* position);
};
template <class T> struct _List_node {
    _List_node* _M_next;
    _List_node* _M_prev;
    T           _M_data;
};
template <class T> struct list {
    _List_node<T>* _M_node;
    T& front() { return _M_node->_M_next->_M_data; }
    bool empty() const { return _M_node->_M_next == _M_node; }
    void pop_front();
};
namespace std {
template <class T> class allocator { public: allocator() {} };
template <class T, class A = allocator<T> > class list {
public:
    explicit list(const A& a = A());                 // 0x0042ABAF
    ~list();                                         // 0x0042ABCA
    void* _M_node;
};
}
namespace NUISystem {
class CActionParam;
enum EActionType { ACTION_NONE };
class CAction {                                      // 8 bytes
public:
    EActionType              ActionType;             // +0x00
    std::list<CActionParam*> UIActionParams;         // +0x04
    CAction();                                       // 0x0042BED4
    CAction(const CAction& other);                   // 0x0042CD84
};
struct CStateHolder {                                // the component's second base (+4)
    virtual void s00(); virtual void s01(); virtual void s02();
    virtual void AddState(long state);               // slot 3 (+0x0C)
};
struct CManager {
    virtual void m000(); virtual void m001();
    virtual void AddInputListener(CStateHolder* listener);   // slot 2 (+0x08)
    static CManager* __fastcall GetInstance();       // 0x0041E5F2
};
struct CComponent;
struct CComponentBase {
    V32(a) V16(b)                                    // slots 0..47
    virtual void ChangeState(long state);            // slot 48 (+0xC0)
    V8(c) V4(d) V1(e)                                // slots 49..61
    virtual void ProcessAction(CAction action);      // slot 62 (+0xF8)
    V32(f) V8(g) V4(h) V1(i)                         // slots 63..107
    virtual CListDefPointer GetDef();                // slot 108 (+0x1B0)
    V32(j) V8(k) V1(l)                               // slots 109..149
    virtual void SetItemCount(long count);           // slot 150 (+0x258)
    V8(m) V2(n)                                      // slots 151..160
    virtual void SetSelectedChild(long child);       // slot 161 (+0x284)
    V2(o) V1(p)                                      // slots 162..164
    virtual void ShiftChildren(long from, long to, bool a, bool b, bool c);   // slot 165 (+0x294)
    V8(q) V4(r) V2(s) V1(t)                          // slots 166..180
    virtual void RecomputeOffsets(bool now);         // slot 181 (+0x2D4)
};
struct CComponent : CComponentBase, CStateHolder {
};
struct CChangingStateComponent : CComponent {
    unsigned char _pad08[0xb0 - 0x08];
    vector<CCountedPointer<CComponent> > Children;   // +0xb0
    void RemoveChild(long index);                    // 0x00533BC0
};
struct CStateMap { void* _body[3]; };
class CList;
extern list<CList*> g_ListRegistry;                  // 0x013B8A98
CComponent** __fastcall find(CComponent** first, CComponent** last, CComponent* const& value);   // 0x0053C91C
class CList : public CChangingStateComponent {
public:
    unsigned char          _padbc[0x15c - 0xbc];
    long                   SelectedChild;            // +0x15c
    float                  LastInputTime;            // +0x160
    vector<CComponent*>    ScrollingChildren;        // +0x164
    vector<CComponent*>    NonScrollingChildren;     // +0x170
    vector<CStateMap>      ScrollingChildrenStates;  // +0x17c
    unsigned char          _pad188[0x1c8 - 0x188];
    vector<unsigned char>  ScrollingChildrenVisibility;   // +0x1c8
    unsigned char          _pad1d4[0x1e0 - 0x1d4];
    CComponent*            PListUp;                  // +0x1e0
    CComponent*            PListDown;                // +0x1e4
    void RemoveChild(long index);
};

void CList::RemoveChild(long index)
{
    CCountedPointer<CComponent> child = Children[index];
    CComponent** last = ScrollingChildren._M_finish;
    CComponent** it = find(ScrollingChildren._M_start, last, child.GetPtr());
    if (it != last) {
        long pos = it - ScrollingChildren._M_start;
        if (SelectedChild == 0) {
            ShiftChildren(pos, ScrollingChildren.size() - 1, false, false, true);
            SetSelectedChild(0);
        } else if (SelectedChild == ScrollingChildren.size() - 1) {
            ShiftChildren(pos, ScrollingChildren.size() - 1, false, false, true);
            SetSelectedChild(ScrollingChildren.size() - 2);
        } else {
            ShiftChildren(pos, ScrollingChildren.size() - 1, false, false, true);
        }
        ScrollingChildren.erase(it);
        ScrollingChildrenStates.erase(ScrollingChildrenStates._M_start + pos);
        ScrollingChildrenVisibility.erase(ScrollingChildrenVisibility._M_start + pos);
        if (!ScrollingChildren.empty()) {
            ScrollingChildren[SelectedChild]->ChangeState(3);
            for (unsigned long i = 0; i < ScrollingChildren.size(); ++i) {
                CManager::GetInstance()->AddInputListener(ScrollingChildren[i]);
                ScrollingChildren[i]->AddState(0x19);
            }
            ScrollingChildren[SelectedChild]->AddState(2);
            ScrollingChildren[SelectedChild]->AddState(3);
            ScrollingChildren[SelectedChild]->AddState(4);
            ScrollingChildren[SelectedChild]->AddState(5);
        }
        RecomputeOffsets(true);
    } else {
        CComponent** otherLast = NonScrollingChildren.end();
        CComponent** found = find(NonScrollingChildren.begin(), otherLast, child.GetPtr());
        if (found != NonScrollingChildren.end())
            NonScrollingChildren.erase(found);
    }
    CChangingStateComponent::RemoveChild(index);
    if (ScrollingChildren.empty()) {
        CAction action;
        action.ActionType = (EActionType)1;
        ProcessAction(action);
        if (this == g_ListRegistry.front()) {
            bool restore = GetDef()->RestoreRegistryState;
            if (restore)
                g_ListRegistry.front()->ChangeState(0);
            else
                g_ListRegistry.front()->ChangeState(6);
            g_ListRegistry.pop_front();
            if (!g_ListRegistry.empty())
                g_ListRegistry.front()->ChangeState(3);
        }
    }
    CComponent* up = PListUp;
    if (up)
        up->SetItemCount(ScrollingChildren.size());
    CComponent* down = PListDown;
    if (down) {
        CListDefPointer def = GetDef();
        down->SetItemCount(ScrollingChildren.size() - def->ScrollLimit - 1);
    }
}
}
