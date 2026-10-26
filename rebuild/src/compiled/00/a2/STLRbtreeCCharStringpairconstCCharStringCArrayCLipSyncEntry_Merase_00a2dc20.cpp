// std::_Rb_tree<CCharString,pair<const CCharString,CArray<CLipSyncEntry> > >::_M_erase(_Rb_tree_node* x) -- STLport recursive subtree release of the mesh bank's lip-sync map: for each node erase its right subtree, destroy the value (the entry array, then the key string) and free the node, continuing with its left child
// Leaf: ?_M_erase@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A2DC20, 112 bytes, thiscall, `ret 4`; recursive (calls itself at 0x00A2DC39).
// Identity: the tree of CMeshDataBank::LipSyncAnimations (Ego_r.pdb `map<CCharString, CArray<CMeshDataBank::CLipSyncEntry> >`,
//   CLipSyncEntry = CCharString PhonemeName +0, ulong AnimationIndex +4: the 8-byte stride of the element destructor loop).
//   Callers: the map destructor 0x00A2E1B0 / clear 0x00A2E240 (landed, named by this callee), 0x00A2744A, 0x00A2DEC2.
//   The catalog's "Utility_DestroyMapNodeWithAllocator" is descriptive.
// Callees: ~CCharString 0x0099EAE0, free 0x00BFEA14.
// Shape: byte twin of the landed map<CCharString, vector<CCharString> >::_M_erase 0x007AE0E0 except the element stride:
//   STLport `while (x != 0) { _M_erase(_S_right(x)); y = _S_left(x); _Destroy(&x->_M_value_field); deallocate(x); x = y; }`;
//   the inlined ~CArray (= ~vector) destroys [start, finish) and frees the null-tested start, then the key is destroyed;
//   register pressure puts `this` in the `push ecx` word and y in x's dead argument slot.
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14

class CCharString {
public:
    void* PStringData;
    ~CCharString();                                         // 0x0099EAE0
};

namespace std {

template <class T> inline void _Destroy(T* p) { p->~T(); }
template <class T> inline void _Destroy(T* first, T* last)
{
    for (; first != last; ++first)
        _Destroy(first);
}

template <class T> class vector {
public:
    T* _M_start;                                            // +0
    T* _M_finish;                                           // +4
    T* _M_end_of_storage;                                   // +8
    ~vector()
    {
        _Destroy(_M_start, _M_finish);
        if (_M_start != 0)
            free(_M_start);
    }
};

template <class T1, class T2> class pair {
public:
    T1 first;
    T2 second;
};

struct _Rb_tree_node_base {
    bool                _M_color;                           // +0x00
    _Rb_tree_node_base* _M_parent;                          // +0x04
    _Rb_tree_node_base* _M_left;                            // +0x08
    _Rb_tree_node_base* _M_right;                           // +0x0c
};
template <class V> struct _Rb_tree_node : _Rb_tree_node_base {
    V _M_value_field;                                       // +0x10
};

template <class K, class V> class _Rb_tree {
public:
    typedef _Rb_tree_node<V>* _Link_type;
    _Rb_tree_node_base* _M_header;                          // +0x00
    unsigned long       _M_node_count;                      // +0x04
    static _Link_type _S_left(_Link_type x) { return (_Link_type)x->_M_left; }
    static _Link_type _S_right(_Link_type x) { return (_Link_type)x->_M_right; }
private:
    void _M_erase(_Link_type x);
};

template <class K, class V> void _Rb_tree<K, V>::_M_erase(_Link_type x)
{
    while (x != 0) {
        _M_erase(_S_right(x));
        _Link_type y = _S_left(x);
        _Destroy(&x->_M_value_field);
        free(x);
        x = y;
    }
}

}

class CMeshDataBank {
public:
    class CLipSyncEntry {                                   // 8 bytes
    public:
        CCharString   PhonemeName;                          // +0x00
        unsigned long AnimationIndex;                       // +0x04
    };
};

template <class T> class CArray : public std::vector<T> {   // retail CArray: the 12-byte STLport vector
};

template class std::_Rb_tree<CCharString, std::pair<const CCharString, CArray<CMeshDataBank::CLipSyncEntry> > >;