// _STL::_Rb_tree::erase(iterator position) -- unlink one node of the optimiser's contraction multimap (multimap<float, CCountedPointer<CVertexPairContraction>>), release its counted pointer, free the node, drop the count
// Leaf: ?erase@?$_Rb_tree@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AD4CA0, 0x67 bytes (+ int3 padding), thiscall, `ret 4` (the iterator by value).
// Identity: the 0x1C-byte node is { colour, parent, left, right } + pair<const float, CCountedPointer<
// QuadricOptimiserInternals::CVertexPairContraction>> (float key +0x10, object +0x14, count block +0x18): the
// COptimisedPrimitive::VertexPairContractions multimap (Ego_r.pdb; the float key is confirmed by the same tree's
// insert_equal 0x00AD54B0). Callers: multimap::erase(iterator) 0x00AD5CA0 (iterator copied into the argument
// slot) and three inlined erase calls in 0x00AC8xxx/0x00ACCxxx. The catalog's "CMemoryAllocator_Erase" comes from
// the propagated label of the rebalance helper.
// Callees: _Rb_global<bool>::_Rebalance_for_erase(z, root&, leftmost&, rightmost&) @ 0x0042A34E (__cdecl, manifest
// "CMemoryAllocatorVariableSize::Erase" is a propagation), operator delete @ 0x00BFE9BC (the count block),
// free @ 0x00BFEA14 (allocator<_Node>::deallocate, null-tested).
// Shape: STLport 4.x `erase(iterator)`: rebalance with the addresses of the header's parent/left/right links
// (pushed right to left), `_Destroy(&y->_M_value_field)` runs ~CCountedPointer (if a block: decrement, re-read
// the block, at zero call its fastcall destroy routine on the object and delete the block; then null both
// fields), `deallocate(y, 1)` frees behind a null test, `--_M_node_count`.
extern "C" void __cdecl free(void* block);                       // 0x00BFEA14
void __cdecl operator delete(void* block);                       // 0x00BFE9BC

struct SCountInfo {
    long  RefCount;                                              // +0x0
    void (__fastcall* DeleteFunc)(void*);                        // +0x4
    void* Data;                                                  // +0x8
};

template <class T> class CCountedPointer {
public:
    T*          Data;                                            // +0x0
    SCountInfo* Info;                                            // +0x4
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};

namespace QuadricOptimiserInternals { class CVertexPairContraction; }

namespace _STL {
template <class T> class allocator {
public:
    void deallocate(T* p, unsigned int) { if (p != 0) free(p); }
};
template <class T> class less { };
template <class T1, class T2> class pair {
public:
    T1 first;
    T2 second;
};
template <class P> struct _Select1st { };
struct _Rb_tree_node_base {
    bool                _M_color;                                // +0x00
    _Rb_tree_node_base* _M_parent;                               // +0x04
    _Rb_tree_node_base* _M_left;                                 // +0x08
    _Rb_tree_node_base* _M_right;                                // +0x0C
};
template <class V> struct _Rb_tree_node : public _Rb_tree_node_base {
    V _M_value_field;                                            // +0x10
};
template <class Dummy> class _Rb_global {
public:
    static _Rb_tree_node_base* __cdecl _Rebalance_for_erase(_Rb_tree_node_base* z, _Rb_tree_node_base*& root,
        _Rb_tree_node_base*& leftmost, _Rb_tree_node_base*& rightmost);   // 0x0042A34E
};
template <class T> inline void _Destroy(T* p) { p->~T(); }
template <class V> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
};
template <class V, class A> class _Rb_tree_base {
public:
    typedef _Rb_tree_node<V> _Node;
    struct _Header_proxy : public allocator<_Node> {
        _Node* _M_data;
    } _M_header;
};
template <class K, class V, class KeyOfValue, class Compare, class A> class _Rb_tree : public _Rb_tree_base<V, A> {
public:
    typedef _Rb_tree_node<V>       _Node;
    typedef _Rb_tree_iterator<V>   iterator;
    unsigned int _M_node_count;
    Compare      _M_key_compare;
    void erase(iterator __position);
};

template <class K, class V, class KeyOfValue, class Compare, class A>
void _Rb_tree<K, V, KeyOfValue, Compare, A>::erase(iterator __position)
{
    _Node* __y = (_Node*)_Rb_global<bool>::_Rebalance_for_erase(__position._M_node,
                                                                this->_M_header._M_data->_M_parent,
                                                                this->_M_header._M_data->_M_left,
                                                                this->_M_header._M_data->_M_right);
    _Destroy(&__y->_M_value_field);
    this->_M_header.deallocate(__y, 1);
    --_M_node_count;
}

typedef pair<const float, CCountedPointer<QuadricOptimiserInternals::CVertexPairContraction> > SContractionEntry;
template class _Rb_tree<float, SContractionEntry, _Select1st<SContractionEntry>, less<float>, allocator<SContractionEntry> >;
} // namespace _STL