// std::__destroy_aux(list<CWeaponTrailEntry>* first, list<CWeaponTrailEntry>* last, const __false_type&) -- STLport range destroy of weapon-trail point lists: free every node of each list, relink its sentinel to itself, then free the sentinel
// Leaf: ??$__destroy_aux@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00770350, 80 bytes, /Gr `__fastcall` (first in ecx -> esi, last in edx ->
//   ebx, the empty tag reference on the stack: `ret 4`).
// Callee: free @ 0x00BFEA14 (STLport's malloc allocator).
// Element: a 4-byte STLport list<CEnginePrimitiveWeaponTrail::CWeaponTrailEntry> (CTCWeaponTrail::Trail is a vector of
//   them; the list model of the landed 0x007705A0 / 0x0076E150). Callers: the vector<list<CWeaponTrailEntry> > members
//   (erase 0x00770600, push_back/insert 0x007706A0.., reallocation 0x00770810, 0x00770990, the CTCWeaponTrail dtor 0x00770AC0).
// Label correction: catalogued as `LinkedList_DestroyRange`; the body is STLport `__destroy_aux<list<...>*>`.
// Shape: `for (; first != last; ++first) _Destroy(&*first);` with the list destructor inline: `_List_base::~_List_base()
//   { clear(); _M_node.deallocate(_M_node._M_data, 1); }` -- clear() frees each node (the next pointer read first) and
//   re-reads the sentinel pointer after every free, then self-links it; the sentinel's deallocate keeps the allocator's
//   null test, the node frees inside clear() do not need one.
extern "C" void __cdecl free(void* block);              // 0x00BFEA14

class CEnginePrimitiveWeaponTrail {
public:
    class CWeaponTrailEntry {                            // 28 bytes: two C3DVectors and a float
    public:
        float Data[7];
    };
};

namespace std {

struct __false_type { __false_type() { } };

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};

template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};

template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_List_node_base*, _Node, allocator<_Node> > _M_node;

    void clear()
    {
        _Node* __cur = (_Node*)_M_node._M_data->_M_next;
        while (__cur != _M_node._M_data) {
            _Node* __tmp = __cur;
            __cur = (_Node*)__cur->_M_next;
            free(__tmp);
        }
        _M_node._M_data->_M_next = _M_node._M_data;
        _M_node._M_data->_M_prev = _M_node._M_data;
    }
    ~_List_base()
    {
        clear();
        _M_node.deallocate((_Node*)_M_node._M_data, 1);
    }
};

template <class _Tp> class list : public _List_base<_Tp> {
};

template <class _Tp>
inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}

typedef list<CEnginePrimitiveWeaponTrail::CWeaponTrailEntry> CWeaponTrailList;

template void __fastcall __destroy_aux<CWeaponTrailList*>(CWeaponTrailList*, CWeaponTrailList*, const __false_type&);

}