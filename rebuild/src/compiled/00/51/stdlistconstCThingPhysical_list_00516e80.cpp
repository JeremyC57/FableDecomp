// std::list<const CThingPhysical*>::list() -- an empty list: the sentinel node linked to itself
// Leaf: ??0?$list@PBVCThingPhysical@@V?$allocator@PBVCThingPhysical@@@std@@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00516E80, 30 bytes, thiscall, plain `ret`.
// Identity (probable): unreferenced; it sits in CWorldMap's STLport block right after the three GetAllLeafThings<T, list<const CThingPhysical*>, ...> rows (0x005163B0..0x00516830, whose caller CWorldMap::GetMapNavigationAreaInit builds that list), and FableWin.pdb's CWorldMap translation unit holds ??0?$list@PBVCThingPhysical@@...@std@@QAE@XZ as its 4-byte-element list constructor.
// Callees: malloc 0x00BFEA0E (the 0xc-byte sentinel node).
// Shape (family M112, representative 0x00516E80): the STLport 4.6 `_List_base(const allocator_type&)` with the sentinel in a
//   local -- the null store first, the node linked to itself, then stored (writing through _M_node._M_data directly reloads
//   it after the first link: 32 bytes).
extern "C" void* __cdecl malloc(unsigned int size);      // 0x00BFEA0E
class CThingPhysical;
namespace std {
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Tp> class allocator {
public:
    allocator() { }
    template <class _Tp1> allocator(const allocator<_Tp1>&) { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _STLP_alloc_proxy(const _Alloc& __a, _Tp __p) : _Alloc(__a), _M_data(__p) { }
    _Tp _M_data;
};
struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
template <class _Tp> struct _List_node : public _List_node_base { _Tp _M_data; };
template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _List_base(const _Alloc& __a) : _M_node(allocator<_Node>(__a), (_Node*)0)
    {
        _Node* __n = _M_node.allocate(1);
        __n->_M_next = __n;
        __n->_M_prev = __n;
        _M_node._M_data = __n;
    }
    _STLP_alloc_proxy<_Node*, allocator<_Node> > _M_node;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _Alloc allocator_type;
    list() : _List_base<_Tp, _Alloc>(allocator_type()) { }
};
}
template std::list<const CThingPhysical* >::list();