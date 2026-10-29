// Behaviour test for 0053b7f0: src/0053b7f0.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. NUISystem::CList::~CList()
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// NUISystem::CList::~CList() -- leave the list registry, unregister the input listener, delete the action lists, then the members and the component
// Leaf: ??1CList
// Retail: Fable.exe (TLC, VC7.1) @ 0x0053B7F0, 302 bytes, frame-pointer size TU (optimize "y" off, "s" on).
// Callees: CComponent::GetDef (slot 108) 0x00530EC0, the STLport list find 0x0053C8D0 (same-TU body, the landed 0x00536A30
// Die model) and erase 0x0053C7B9 on the registry 0x013B8A98, CManager::GetInstance 0x0041E5F2 + slot 5,
// ~list<CCountedPointer<CAction> > 0x005357E0, operator delete 0x00BFE9BC, free 0x00BFEA14, ~CCountedPointer 0x00429160,
// the states vector dtor 0x0053CD46, ~CChangingStateComponent 0x0052CCF0 (RTTI: CList : CChangingStateComponent : CComponent).
// Shape: the find body in the TU puts the `this` temporary's store after the argument pushes and the erase result in the
// iterator's own slot; the action lists are destroyed with a QUALIFIED explicit destructor call plus operator delete
// (`p->CActionList::~CActionList(); operator delete(p);`): under optimize("s") a plain `delete p` calls a generated
// scalar deleting destructor (`push 1 ; call ??_G`) and an unqualified `p->~CActionList()` routes through it with flag 0.
#pragma optimize("y", off)
#pragma optimize("s", on)
#define V4(n) virtual void n##0(); virtual void n##1(); virtual void n##2(); virtual void n##3();
#define V16(n) V4(n##a) V4(n##b) V4(n##c) V4(n##d)
struct CRefCounted {                               // reference count base at +4 of a def
    long Count;
    void DecRef() { --Count; }
    long GetRefCount() const { return Count; }
};
struct CComponentDef {
    virtual void v00();
    virtual void Destroy();                        // slot 1 (+0x04)
    CRefCounted   Ref;                             // +0x04
    unsigned char _pad08[0x20a - 0x08];
    bool          RegisterInListRegistry;          // +0x20a
    void ReleaseTemporary() { Ref.DecRef(); if (Ref.GetRefCount() == 0) Destroy(); }
};
template <class T> class CDefPointer { public:
    T* Object;
    ~CDefPointer() { if (Object) Object->ReleaseTemporary(); }
    T* operator->() const { return Object; }
};
template <class T> struct _List_node {
    _List_node* _M_next;
    _List_node* _M_prev;
    T           _M_data;
};
template <class T> struct _List_iterator {
    _List_node<T>* _M_node;
    _List_iterator(_List_node<T>* node) : _M_node(node) { }
    _List_iterator(const _List_iterator& other) : _M_node(other._M_node) { }
    bool operator!=(const _List_iterator& other) const { return _M_node != other._M_node; }
};
template <class T> struct list {
    _List_node<T>* _M_node;
    _List_iterator<T> begin() { return _List_iterator<T>(_M_node->_M_next); }
    _List_iterator<T> end() { return _List_iterator<T>(_M_node); }
    _List_iterator<T> erase(_List_iterator<T> position);   // 0x0053C7B9
};
namespace NUISystem {
class CList;
class CComponent;
}
_List_iterator<NUISystem::CList*> __fastcall find(NUISystem::CList* const& value, _List_iterator<NUISystem::CList*> first,
                                                  _List_iterator<NUISystem::CList*> last);   // 0x0053C8D0
#pragma auto_inline(off)
// Same-TU body of the STLport list find instantiation 0x0053C8D0 (the landed 0x00536A30 Die model): walks the list
// comparing each entry with the value read through its const reference.
__declspec(noinline) _List_iterator<NUISystem::CList*> __fastcall find(NUISystem::CList* const& value, _List_iterator<NUISystem::CList*> first, _List_iterator<NUISystem::CList*> last)
{
    for (; first != last; first._M_node = first._M_node->_M_next)
        if (first._M_node->_M_data == value)
            return first;
    return last;
}
#pragma auto_inline(on)
extern list<NUISystem::CList*> g_ListRegistry;     // 0x013B8A98
extern "C" void __cdecl free(void* p);             // 0x00BFEA14
void operator delete(void* p);                     // 0x00BFE9BC
struct CInputListener {                            // the input-listener base at +4
    virtual void v00();
    unsigned char _pad04[0x14 - 0x04];
};
namespace NUISystem { class CManager { public:
public:
    static CManager* __fastcall GetInstance();   // 0x0041E5F2
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void RemoveInputListener(CInputListener* listener);   // slot 5 (+0x14)
}; }
namespace NUISystem {
class CAction;
struct CComponentRoot {                            // the primary vtable at +0
    virtual ~CComponentRoot();
};
struct CThirdBase { virtual void v00(); };         // the vtable at +0x18
class CComponent : public CComponentRoot, public CInputListener, public CThirdBase {
public:
    V16(a) V16(b) V16(c) V16(d) V16(e) V16(f) V4(g0) V4(g1) virtual void h0(); virtual void h1(); virtual void h2();
    virtual CDefPointer<CComponentDef> GetDef();   // slot 108 (+0x1b0) 0x00530EC0
    unsigned char _pad1c[0x164 - 0x1c];
};
template <class T> struct vector {                 // STLport vector, 12 bytes
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    ~vector() { if (_M_start) free(_M_start); }
};
struct CBitVector {                                // STLport vector<bool>: two bit iterators + the end word
    unsigned int* _M_start_p;
    unsigned int  _M_start_offset;
    unsigned int* _M_finish_p;
    unsigned int  _M_finish_offset;
    unsigned int* _M_end_of_storage;
    ~CBitVector() { if (_M_start_p) free(_M_start_p); }
};
struct CStatesVector {                             // vector<map<unsigned long, CUIState> >
    void* _M_start; void* _M_finish; void* _M_end_of_storage;
    ~CStatesVector();                              // 0x0053CD46
};
struct CActionList {                               // list<CCountedPointer<CAction> >
    void* _M_node;
    ~CActionList();                                // 0x005357E0
};
struct CCountedPointer_Action {
    CAction* Data;
    void*    Info;
    ~CCountedPointer_Action();                     // 0x00429160
};
class CChangingStateComponent : public CComponent {   // RTTI direct base of CList
public:
    ~CChangingStateComponent();                    // 0x0052CCF0
};
class CList : public CChangingStateComponent {
public:
    vector<CComponent*>    ScrollingChildren;      // +0x164
    vector<CComponent*>    NonScrollingChildren;   // +0x170
    CStatesVector          ScrollingChildrenStates;   // +0x17c
    float                  PositionOffset[2];      // +0x188
    float                  PositionOffsetToUse[2]; // +0x190
    float                  AlphaOffset;            // +0x198
    float                  AlphaOffsetToUse;       // +0x19c
    float                  NextScrollingChildAlpha;   // +0x1a0
    float                  NextScrollingChildPosition[2];   // +0x1a4
    CActionList*           PActionsOnInactive;     // +0x1ac
    CActionList*           PActionsOnSelected;     // +0x1b0
    CCountedPointer_Action CPActionOnScrollUp;     // +0x1b4
    CCountedPointer_Action CPActionOnScrollDown;   // +0x1bc
    long                   DefaultChild;           // +0x1c4
    CBitVector             ScrollingChildrenVisibility;   // +0x1c8
    virtual ~CList();
};

CList::~CList()
{
    bool registered = GetDef()->RegisterInListRegistry;
    if (registered) {
        _List_iterator<CList*> it = find(this, g_ListRegistry.begin(), g_ListRegistry.end());
        if (it != g_ListRegistry.end())
            it = g_ListRegistry.erase(it);
    }
    CManager::GetInstance()->RemoveInputListener(this);
    if (CActionList* p = PActionsOnInactive) { p->CActionList::~CActionList(); operator delete(p); }
    if (CActionList* p = PActionsOnSelected) { p->CActionList::~CActionList(); operator delete(p); }
}
}

// ---- stubs and driver ----
#include <cstdlib>
#include <new>
using namespace NUISystem;
#define DEF4(n) void CComponent::n##0() { } void CComponent::n##1() { } void CComponent::n##2() { } void CComponent::n##3() { }
#define DEF16(n) DEF4(n##a) DEF4(n##b) DEF4(n##c) DEF4(n##d)
DEF16(a) DEF16(b) DEF16(c) DEF16(d) DEF16(e) DEF16(f) DEF4(g0) DEF4(g1)
void CComponent::h0() { } void CComponent::h1() { } void CComponent::h2() { }
void CInputListener::v00() { }
void CThirdBase::v00() { }
static int g_log[64]; static int g_n = 0;
static void Log(int v) { if (g_n < 64) g_log[g_n++] = v; }
struct CTestDef : CComponentDef { };
void CComponentDef::v00() { }
static int g_defDestroyed = 0;
void CComponentDef::Destroy() { ++g_defDestroyed; }
static CTestDef* g_def;
CDefPointer<CComponentDef> CComponent::GetDef() { Log(1); g_def->Ref.Count += 2; CDefPointer<CComponentDef> d; d.Object = g_def; return d; }   // +2: the stub's own local releases once
CComponentRoot::~CComponentRoot() { }
static int g_componentDtor = 0;
CChangingStateComponent::~CChangingStateComponent() { Log(9); ++g_componentDtor; }
list<CList*> g_ListRegistry;
static int g_erased = 0;
template <> _List_iterator<CList*> list<CList*>::erase(_List_iterator<CList*> position)
{
    Log(2); ++g_erased;
    _List_node<CList*>* n = position._M_node; _List_node<CList*>* next = n->_M_next;
    n->_M_prev->_M_next = next; next->_M_prev = n->_M_prev; free(n);
    return _List_iterator<CList*>(next);
}
static NUISystem::CManager g_fe; static const void* g_removed = 0;
NUISystem::CManager* __fastcall NUISystem::CManager::GetInstance() { return &g_fe; }
void NUISystem::CManager::v00() { } void NUISystem::CManager::v01() { } void NUISystem::CManager::v02() { } void NUISystem::CManager::v03() { } void NUISystem::CManager::v04() { }
void NUISystem::CManager::RemoveInputListener(CInputListener* listener) { Log(3); g_removed = listener; }
static const void* g_actionListDtor[16]; static int g_actionLists = 0;
CActionList::~CActionList() { Log(4); g_actionListDtor[g_actionLists++] = this; }
static const void* g_cpDtor[16]; static int g_cps = 0;
CCountedPointer_Action::~CCountedPointer_Action() { Log(5); g_cpDtor[g_cps++] = this; }
static const void* g_statesDtor = 0;
CStatesVector::~CStatesVector() { Log(6); g_statesDtor = this; }
static void AddToRegistry(CList* l)
{
    _List_node<CList*>* n = (_List_node<CList*>*)malloc(sizeof(_List_node<CList*>));
    n->_M_data = l; n->_M_next = g_ListRegistry._M_node; n->_M_prev = g_ListRegistry._M_node->_M_prev;
    g_ListRegistry._M_node->_M_prev->_M_next = n; g_ListRegistry._M_node->_M_prev = n;
}
static CList* MakeList(bool withActions)
{
    CList* l = (CList*)calloc(1, sizeof(CList));
    l->ScrollingChildren._M_start = (CComponent**)malloc(16);
    l->NonScrollingChildren._M_start = (CComponent**)malloc(16);
    l->ScrollingChildrenVisibility._M_start_p = (unsigned int*)malloc(8);
    if (withActions) {
        l->PActionsOnInactive = (CActionList*)operator new(sizeof(CActionList));
        l->PActionsOnSelected = (CActionList*)operator new(sizeof(CActionList));
    }
    return l;
}
int main()
{
    g_def = (CTestDef*)calloc(1, sizeof(CTestDef)); new (g_def) CTestDef; g_def->Ref.Count = 1; g_def->RegisterInListRegistry = true;
    g_ListRegistry._M_node = (_List_node<CList*>*)malloc(sizeof(_List_node<CList*>));
    g_ListRegistry._M_node->_M_next = g_ListRegistry._M_node; g_ListRegistry._M_node->_M_prev = g_ListRegistry._M_node;
    CList* other = (CList*)calloc(1, sizeof(CList));
    CList* a = MakeList(true);
    AddToRegistry(other); AddToRegistry(a);
    CActionList* inactive = a->PActionsOnInactive; CActionList* selected = a->PActionsOnSelected;
    a->CList::~CList();
    CHECK(g_erased == 1);
    CHECK(g_ListRegistry._M_node->_M_next->_M_data == other && g_ListRegistry._M_node->_M_next->_M_next == g_ListRegistry._M_node);
    CHECK(g_removed == (CInputListener*)a);
    CHECK(g_actionLists == 2 && g_actionListDtor[0] == inactive && g_actionListDtor[1] == selected);
    CHECK(g_cps == 2 && g_cpDtor[0] == &a->CPActionOnScrollDown && g_cpDtor[1] == &a->CPActionOnScrollUp);
    CHECK(g_statesDtor == &a->ScrollingChildrenStates && g_componentDtor == 1);
    CHECK(g_def->Ref.Count == 1 && g_defDestroyed == 0);
    // order: GetDef, erase, RemoveInputListener, two action lists, the counted pointers, the states vector, ~CComponent
    int expect[] = { 1, 2, 3, 4, 4, 5, 5, 6, 9 };
    CHECK(g_n == 9);
    for (int i = 0; i < 9 && i < g_n; ++i) CHECK(g_log[i] == expect[i]);
    // not registered by the def / not in the registry / no action lists: no erase, no list destructors
    g_n = 0; g_def->RegisterInListRegistry = false;
    CList* b = MakeList(false); AddToRegistry(b);
    b->CList::~CList();
    CHECK(g_erased == 1 && g_actionLists == 2 && g_componentDtor == 2 && g_removed == (CInputListener*)b);
    g_def->RegisterInListRegistry = true;
    CList* c = MakeList(false);
    c->CList::~CList();
    CHECK(g_erased == 1 && g_componentDtor == 3);
    // the last temporary reference destroys the def
    g_def->Ref.Count = 0;
    CList* d = MakeList(false);
    d->CList::~CList();
    CHECK(g_defDestroyed == 1);
    if (fails == 0) printf("0053b7f0_TEST PASS\n");
    return fails;
}
