// std::map::operator[](const long& k) -- STLport subscript of map<long, CEnginePrimitiveHandle> (size TU): lower_bound, insert (k, CEnginePrimitiveHandle()) when absent, return the handle
// Leaf: ??A?$map
// Retail: Fable.exe (TLC, VC7.1) @ 0x005BBFD6, 170 bytes, __fastcall `ret 4`, frame-pointer size TU.
// Identity: Ego_r.pdb CTCInventoryClothing::MorphPrimitiveHandles is map<long, CEnginePrimitiveHandle>; both callers sit in
//   CTCInventoryClothing vfunc 11 (0x005B9641: 0x005B9AAE, 0x005B9B6B). The manifest type agrees.
// Callees: _M_lower_bound @ 0x005BAC98 (landed; node pointer in eax), CBaseClass() @ 0x0099A2D0 / ~CBaseClass() @
//   0x0099A2E0 (the default handle temporary), pair(const long&, const CEnginePrimitiveHandle&) @ 0x005BB008 (out of
//   line), insert(iterator, const value_type&) @ 0x005BBF9D (result in its own frame word), ~pair @ 0x004DD728 (the
//   value_type temporary; ICF-shared COMDAT); the primitive's virtual slots 3 and 1 in the temporary's inline destructor.
// Shape: the landed map<NUISystem::CComponent*, long>::operator[] 0x0042D1B7 template (STLport 4.x, optimize("s", on))
//   with the handle model of 0x00485D30 (same `(Object ? Object->RefCount : 0) == 1` uniqueness test, `__forceinline`
//   ctor/dtor in this /Os TU).
#pragma optimize("s", on)

class CEnginePrimitiveHandle;

class CEnginePersistentPrimitiveBase {
public:
    virtual void v000();
    virtual void Destroy();                                          // slot 1 (+0x04)
    virtual void v002();
    virtual void OnLastHandleReleased(CEnginePrimitiveHandle* handle); // slot 3 (+0x0C)
    long RefCount;                                                   // +0x04
    __forceinline void DecRef() { --RefCount; }
    __forceinline long GetRefCount() const { return RefCount; }
    __forceinline void Release()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};

class CIVCountedPointer {
public:
    CEnginePersistentPrimitiveBase* Object;
    __forceinline CIVCountedPointer() : Object(0) { }
    __forceinline CEnginePersistentPrimitiveBase* operator->() const { return Object; }
    __forceinline long GetRefCount() const { return Object ? Object->RefCount : 0; }
    __forceinline ~CIVCountedPointer()
    {
        if (Object)
            Object->Release();
        Object = 0;
    }
};

class CBaseClass {                                 // vtable 0x01231710
public:
    virtual void v000();
    CBaseClass();                                  // 0x0099A2D0
    ~CBaseClass();                                 // 0x0099A2E0
};

class CEnginePrimitiveHandle : public CBaseClass { // vtable 0x0122F598, 8 bytes
public:
    CIVCountedPointer Data;                        // +0x04
    virtual void v001();
    __forceinline CEnginePrimitiveHandle() { }
    CEnginePrimitiveHandle(const CEnginePrimitiveHandle& x);   // (inside the out-of-line pair ctor)
    __forceinline ~CEnginePrimitiveHandle()
    {
        if (Data.GetRefCount() == 1)
            Data->OnLastHandleReleased(this);
    }
};

namespace std {
template <class T> struct less {
    bool operator()(const T& x, const T& y) const { return x < y; }
};
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b);                      // 0x005BB008
};
struct _Rb_tree_node_base {
    bool                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class V> struct _Rb_tree_node : _Rb_tree_node_base {
    V _M_value_field;                                    // +0x10
};
struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
    bool operator==(const _Rb_tree_base_iterator& y) const { return _M_node == y._M_node; }
};
template <class V> struct _Rb_tree_iterator : _Rb_tree_base_iterator {
    _Rb_tree_iterator(_Rb_tree_node_base* x) { _M_node = x; }
    _Rb_tree_iterator(const _Rb_tree_iterator& it) { _M_node = it._M_node; }
    V& operator*() const { return static_cast<_Rb_tree_node<V>*>(_M_node)->_M_value_field; }
};
template <class K, class T, class C = less<K> > class map {
public:
    typedef pair<const K, T> value_type;
    typedef _Rb_tree_iterator<value_type> iterator;
    _Rb_tree_node_base* _M_header;
    unsigned int        _M_node_count;
    C                   _M_key_compare;
    _Rb_tree_node_base* _M_lower_bound(const K& k) const;         // 0x005BAC98
    iterator insert(iterator position, const value_type& v);      // 0x005BBF9D
    iterator lower_bound(const K& k) { return iterator(_M_lower_bound(k)); }
    iterator end() { return iterator(_M_header); }
    C key_comp() const { return _M_key_compare; }
    T& operator[](const K& k)
    {
        iterator i = lower_bound(k);
        if (i == end() || key_comp()(k, (*i).first))
            i = insert(i, value_type(k, T()));
        return (*i).second;
    }
};
template CEnginePrimitiveHandle& map<long, CEnginePrimitiveHandle>::operator[](const long&);
}