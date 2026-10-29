// NUISystem::CList::Die() -- leave the list registry (when the def asks for it), unregister and kill every scrolling child, then the component's Die
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
// Same-TU body of the STLport list find instantiation 0x0053C8D0 (template bodies are compiled in the caller's TU): walks
// the list comparing each entry with the value read through its const reference.
__declspec(noinline) _List_iterator<NUISystem::CList*> __fastcall find(NUISystem::CList* const& value, _List_iterator<NUISystem::CList*> first, _List_iterator<NUISystem::CList*> last)
{
    for (; first != last; first._M_node = first._M_node->_M_next)
        if (first._M_node->_M_data == value)
            return first;
    return last;
}
#pragma auto_inline(on)
extern list<NUISystem::CList*> g_ListRegistry;     // 0x013B8A98
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
struct CComponentRoot {                            // the primary vtable at +0
    virtual ~CComponentRoot();
};
class CComponent : public CComponentRoot, public CInputListener {
public:
    V16(a) V16(b) V16(c) V16(d) V16(e) V16(f) V4(g0) V4(g1) virtual void h0(); virtual void h1(); virtual void h2();
    virtual CDefPointer<CComponentDef> GetDef();   // slot 108 (+0x1b0) 0x00530EC0
    virtual void Die();                            // slot 109 (+0x1b4) 0x00530720 (CComponent's)
    unsigned char _pad18[0x164 - 0x18];
};
template <class T> struct vector {                 // STLport vector, 12 bytes
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    T& operator[](unsigned long i) { return _M_start[i]; }
};
class CList : public CComponent {
public:
    vector<CComponent*> ScrollingChildren;         // +0x164
    virtual void Die();
};

void CList::Die()
{
    bool registered = GetDef()->RegisterInListRegistry;
    if (registered) {
        _List_iterator<CList*> it = find(this, g_ListRegistry.begin(), g_ListRegistry.end());
        if (it != g_ListRegistry.end())
            it = g_ListRegistry.erase(it);
    }
    for (unsigned long i = 0; i < ScrollingChildren.size(); ++i) {
        CManager::GetInstance()->RemoveInputListener(ScrollingChildren[i]);
        ScrollingChildren[i]->Die();
    }
    CComponent::Die();
}
}