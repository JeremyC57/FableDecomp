// NUISystem::CList::~CList() -- leave the list registry, unregister the input listener, delete the action lists, then the members and the component
// Leaf: ??1CList
// Retail: Fable.exe (TLC, VC7.1) @ 0x0053B7F0, 302 bytes, frame-pointer size TU (optimize "y" off, "s" on).
// Callees: CComponent::GetDef (slot 108) 0x00530EC0, the STLport list find 0x0053C8D0 (same-TU body, the landed 0x00536A30
// Die model) and erase 0x0053C7B9 on the registry 0x013B8A98, CFrontEndManager::GetInstance 0x0041E5F2 + slot 5,
// ~list<CCountedPointer<CAction> > 0x005357E0, operator delete 0x00BFE9BC, free 0x00BFEA14, ~CCountedPointer 0x00429160,
// the states vector dtor 0x0053CD46, ~CComponent 0x0052CCF0.
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
template <class T> struct CDefPointer {
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
class CFrontEndManager {
public:
    static CFrontEndManager* __fastcall GetInstance();   // 0x0041E5F2
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void RemoveInputListener(CInputListener* listener);   // slot 5 (+0x14)
};
namespace NUISystem {
struct CAction;
struct CComponentRoot {                            // the primary vtable at +0
    virtual ~CComponentRoot();
};
struct CThirdBase { virtual void v00(); };         // the vtable at +0x18
class CComponent : public CComponentRoot, public CInputListener, public CThirdBase {
public:
    ~CComponent();                                 // 0x0052CCF0
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
class CList : public CComponent {
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
    CFrontEndManager::GetInstance()->RemoveInputListener(this);
    if (CActionList* p = PActionsOnInactive) { p->CActionList::~CActionList(); operator delete(p); }
    if (CActionList* p = PActionsOnSelected) { p->CActionList::~CActionList(); operator delete(p); }
}
}