// std::_Rb_tree<long,pair<const long,CIVCountedPointer<CCountedObject>>>::erase(iterator position) -- STLport map erase of one node: unlink and rebalance, release the value's counted pointer, free the node, decrement the count
// Leaf: erase
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004C43B0, length 78 bytes, `ret 4` (thiscall: the by-value iterator).
// Callees: _Rb_global<bool>::_Rebalance_for_erase @ 0x0042A34E (landed, __cdecl), free @ 0x00BFEA14; the pointee's
// virtual slot 1 (+4) when its last reference goes.
// Label: the catalog "StdList_Erase" is wrong (it is a tree, not a list). The bytes are those of the MeshEffects map's
// erase 0x004C4C70 (this chunk) except for the call displacements: a second instantiation, kept unfolded (the retail
// link does no identical-COMDAT folding), with no direct caller in the image. Only a 4-byte key at node +0x10 and a
// counted pointer at +0x14 (the first and only destroyed member of the value) are observable, so the key is modelled as
// `long` and the pointee as a descriptive CCountedObject.
// Shape: STLport 4.x `erase(iterator)` verbatim: `y = _Rebalance_for_erase(pos._M_node, root, leftmost, rightmost);
// _Destroy(&y->_M_value_field); deallocate(y); --_M_node_count;` -- the value's implicit destructor runs the counted
// pointer's `if (p) { p->Release(); p = 0; }`, deallocate is a null-tested free.
extern "C" void __cdecl free(void* block);   // 0x00BFEA14

class CCountedObject {
public:
    virtual void v000();
    virtual void Destroy();                                 // slot 1 (+0x04)
    long RefCount;                                          // +0x04
    __forceinline void Release()
    {
        if (--RefCount == 0)
            Destroy();
    }
};

template <class T> class CIVCountedPointer {
public:
    T* Object;
    __forceinline ~CIVCountedPointer()
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
    }
};

namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};

struct _Rb_tree_node_base {
    bool                _M_color;            // +0x00
    _Rb_tree_node_base* _M_parent;           // +0x04
    _Rb_tree_node_base* _M_left;             // +0x08
    _Rb_tree_node_base* _M_right;            // +0x0c
};
template <class V> struct _Rb_tree_node : _Rb_tree_node_base {
    V _M_value_field;                        // +0x10
};

template <class _Dummy> struct _Rb_global {
    static _Rb_tree_node_base* __cdecl _Rebalance_for_erase(_Rb_tree_node_base* z, _Rb_tree_node_base*& root,
                                                            _Rb_tree_node_base*& leftmost, _Rb_tree_node_base*& rightmost);   // 0x0042A34E
};

template <class T> __forceinline void _Destroy(T* p) { p->~T(); }

struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    __forceinline _Rb_tree_iterator(_Rb_tree_node_base* x) : _M_node(x) {}
    __forceinline _Rb_tree_iterator(const _Rb_tree_iterator& x) : _M_node(x._M_node) {}
};

template <class K, class V> class _Rb_tree {
public:
    typedef _Rb_tree_node<V> _Node;
    typedef _Rb_tree_iterator iterator;
    _Rb_tree_node_base* _M_header;           // +0x00 _STLP_alloc_proxy::_M_data
    unsigned int        _M_node_count;       // +0x04
    void erase(iterator position);
};

template <class K, class V> void _Rb_tree<K, V>::erase(iterator position)
{
    _Node* y = static_cast<_Node*>(_Rb_global<bool>::_Rebalance_for_erase(position._M_node, _M_header->_M_parent,
                                                                          _M_header->_M_left, _M_header->_M_right));
    _Destroy(&y->_M_value_field);
    if (y)
        free(y);
    --_M_node_count;
}

template class _Rb_tree<long, pair<const long, CIVCountedPointer<CCountedObject> > >;
}