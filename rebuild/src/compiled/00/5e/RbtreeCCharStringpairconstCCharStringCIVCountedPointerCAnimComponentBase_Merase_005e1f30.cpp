// _Rb_tree<CCharString, pair<const CCharString, CIVCountedPointer<CAnimComponentBase> > >::_M_erase(_Rb_tree_node* x) -- STLport recursive subtree free of the named anim-component map: release each node's component, destroy its name, free the node
// Leaf: ?_M_erase
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005E1F30, 86 bytes (+ padding to 96), `ret 4` (__fastcall; `this`
//   kept in ebp for the recursive call only).
// Ego_r.pdb: map<CCharString, CIVCountedPointer<CAnimComponentBase>, less<CCharString> > (the Dinkumware-spelt donor
//   `_Tree::_Erase` @ 0x00640AE0 is the same map; retail is STLport 4.x with an allocated header node).
// Callees: itself (right subtree), 0x0099EAE0 ~CCharString, 0x00BFEA14 free; the component's virtual slot 1 (+4) on
//   its last reference.
// Callers: _Rb_tree::clear 0x005E2460 (`_M_erase(_M_root())` then the header re-link), 0x005E2992, 0x005E30D2.
// Node: color +0, parent +4, left +8, right +0xC, value +0x10 = { CCharString name +0x10, counted pointer +0x14 }.
// Shape: STLport `while (x != 0) { _M_erase(_S_right(x)); y = _S_left(x); _Destroy(&x->_M_value_field);
//   _M_header.deallocate(x, 1); x = y; }`; the pair's implicit destructor releases the pointer (second, with its zero
//   store) before ~CCharString (first); the malloc allocator's `if (p) free(p)` test folds away (x != 0).
// Label correction: catalogued as `BinaryTree_DestroyRecursive_WithRefcount`; the body is STLport `_Rb_tree::_M_erase`.
extern "C" void __cdecl free(void* p);

struct CCharString {
    void* PStringData;
    ~CCharString();                                        // 0x0099EAE0
};

struct CAnimComponentBase {                                // CIVCountedPointeeBase-derived: vptr, reference count at +4
    virtual void v00();
    virtual void Delete();                                 // slot 1 (+0x04): destroy on the last reference
    long IVRefCount;                                       // +0x04
};

template <class T> struct CIVCountedPointer {              // one word: the intrusively counted object
    T* Object;
    __forceinline void Release()
    {
        T* object = Object;
        if (object) {
            if (--object->IVRefCount == 0)
                object->Delete();
            Object = 0;
        }
    }
    ~CIVCountedPointer() { Release(); }
};

template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};

template <class T> class allocator {                       // STLport allocator over malloc / free
public:
    void deallocate(T* p, unsigned int) { if (p != 0) free(p); }
};

template <class T> inline void _Destroy(T* p) { p->~T(); }

struct _Rb_tree_node_base {
    char                _M_color;                          // +0x0
    _Rb_tree_node_base* _M_parent;                         // +0x4
    _Rb_tree_node_base* _M_left;                           // +0x8
    _Rb_tree_node_base* _M_right;                          // +0xC
};
template <class V> struct _Rb_tree_node : _Rb_tree_node_base {
    V _M_value_field;                                      // +0x10
};

template <class K, class V> class _Rb_tree {
public:
    typedef _Rb_tree_node<V> _Node;
    _Node*        _M_header;                               // +0x0
    unsigned long _M_node_count;                           // +0x4
    static _Node* _S_left(_Node* x) { return (_Node*)x->_M_left; }
    static _Node* _S_right(_Node* x) { return (_Node*)x->_M_right; }
    void _M_erase(_Node* x);
};

template <class K, class V> void _Rb_tree<K, V>::_M_erase(_Node* x)
{
    while (x != 0) {
        _M_erase(_S_right(x));
        _Node* y = _S_left(x);
        _Destroy(&x->_M_value_field);
        allocator<_Node>().deallocate(x, 1);
        x = y;
    }
}

template class _Rb_tree<CCharString, pair<const CCharString, CIVCountedPointer<CAnimComponentBase> > >;