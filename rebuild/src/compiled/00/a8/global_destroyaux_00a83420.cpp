// _global::__destroy_aux(PairU64LineList* first, PairU64LineList* last, const __false_type&) -- SGI destroy over a range of (unsigned __int64, std::list<C2DLineF>) pairs: run each element's destructor, which clears and frees the list
// Leaf: __destroy_aux
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A83420, length 92 bytes, `ret 4` (the __false_type
//   reference on the stack; first/last in ecx/edx -- the SGI algorithm TUs are /Gr). Byte twin in another TU: 0x00519EF0
//   (different `free` rel32 only). 14 callers each (vector destruction / erase / reallocation paths).
// Identity: the element is 16 bytes with an STLport list (one header-node pointer, 4 bytes) at +8: the 8-aligned pair
//   { unsigned __int64 first +0 ; list second +8 } (+0xC padding). Ego_r.pdb (Xbox Dinkumware list, 12 bytes) names
//   `_Pair_base<unsigned __int64, std::list<C2DLineF> >` as the only pair whose list sits at +8; with the PC STLport 4-byte list
//   it is exactly 16 bytes. The node value type does not show in the bytes (C2DLineF has a trivial destructor). The row's
//   `_Move_backward<pair<unsigned long,CSoundPair>*,...>` label is propagated (wrong function and wrong element).
// Callee: free @ 0x00BFEA14 (the CRT import thunk; STLport's malloc allocator).
// Shape: STLport
//     for ( ; __first != __last; ++__first) _Destroy(&*__first);
//   with the pair's implicit destructor reducing to the list destructor, inlined: clear() frees every node after the header
//   (`tmp = cur; cur = cur->_M_next; free(tmp)`, the header pointer re-read after each call), relinks the header to itself,
//   then the base destructor frees the header behind a null test.
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14

struct C2DLineF;

namespace std {
struct _List_node_base {
    _List_node_base* _M_next;                           // +0x00
    _List_node_base* _M_prev;                           // +0x04
};

template <class T> class list {                         // STLport list: one header-node pointer
public:
    _List_node_base* _M_node;                           // +0x00

    void clear()
    {
        _List_node_base* cur = _M_node->_M_next;
        while (cur != _M_node) {
            _List_node_base* tmp = cur;
            cur = cur->_M_next;
            free(tmp);
        }
        _M_node->_M_next = _M_node;
        _M_node->_M_prev = _M_node;
    }
    ~list()
    {
        clear();
        if (_M_node)
            free(_M_node);
    }
};
}

struct PairU64LineList {                                // std::pair<unsigned __int64, std::list<C2DLineF> >, 16 bytes
    unsigned __int64       first;                       // +0x00
    std::list<C2DLineF>    second;                      // +0x08
};

struct __false_type { };

template <class T> inline void _Destroy(T* p) { p->~T(); }

void __fastcall __destroy_aux(PairU64LineList* first, PairU64LineList* last, const __false_type&)
{
    for (; first != last; ++first)
        _Destroy(&*first);
}