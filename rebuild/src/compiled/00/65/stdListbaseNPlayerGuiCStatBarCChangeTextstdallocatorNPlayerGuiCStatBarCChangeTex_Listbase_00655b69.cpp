// std::_List_base<NPlayerGui::CStatBar::CChangeText, allocator<...> >::_List_base(const allocator_type&) -- STLport list
// base constructor: allocate the sentinel node and link it to itself
// Leaf: ??0?$_List_base@VCChangeText@CStatBar@NPlayerGui@@
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00655B69, 27 bytes, thiscall `ret 4` (the allocator reference is unused):
//   `and [this],0 ; push 2Ch ; call malloc ; mov [eax],eax ; mov [eax+4],eax ; mov [this],eax`.
// Renamed from the descriptive std_list_allocate_sentinel_0x2c (FINDINGS chunk 382: the model was wrong -- a __fastcall
//   free function with two unused parameters).
// Identity: NPlayerGui::CStatBar::CStatBar 0x0065431D (landed) calls it on this+0x64 = ChangeTexts (its `ChangeTexts()`
//   member initialiser, the default allocator temporary passed by address); struct_layouts_egor CStatBar::ChangeTexts is
//   list<NPlayerGui::CStatBar::CChangeText> (PDB +0x6C) and CChangeText is 36 bytes, so the node is 8 + 0x24 = 0x2C bytes,
//   the malloc size here. Ego_r.pdb mangles CChangeText as the class NPlayerGui::CStatBar::CChangeText.
// Shape: STLport 4.x `_List_base(a) : _M_node(a, (_Node*)0) { _Node* n = allocate(1); n->next = n->prev = n; _M_node._M_data = n; }`;
//   the zero store of the proxy comes out as `and dword ptr [esi],0` in this size TU; allocate(1) is malloc(0x2C)
//   without a null test. `list(const allocator_type& = allocator_type()) : _List_base(a) {}` is inlined into the caller
//   while the base constructor stays out of line: the CStatBar constructor calls this body directly with a temporary
//   allocator (`lea eax,[ebp+0Bh] ; lea ecx,[esi+64h] ; call 0x655B69`), and the uncalled out-of-line
//   list(const allocator_type&) at 0x006561E2 is the 18-byte forwarder `push esi ; push [esp+8] ; mov esi,ecx ; call
//   0x655B69 ; mov eax,esi ; pop esi ; ret 4` (the same bytes VC7.1 emits for that list constructor here).
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E

namespace NPlayerGui {
class CStatBar {
public:
    class CChangeText {                                // 36 bytes (Ego_r.pdb)
    public:
        unsigned char _body[0x24];
    };
};
}

namespace std {

template <class _Tp> class allocator {
public:
    allocator() {}
    allocator(const allocator<_Tp>&) {}
    template <class _Tp1> allocator(const allocator<_Tp1>&) {}
    _Tp* allocate(unsigned int n) { return (_Tp*)malloc(n * sizeof(_Tp)); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) {}
};

struct _List_node_base {
    _List_node_base* _M_next;           // +0x00
    _List_node_base* _M_prev;           // +0x04
};

template <class _Tp> struct _List_node : _List_node_base {
    _Tp _M_data;                        // +0x08
};

template <class _Tp, class _Alloc>
class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    typedef _Alloc allocator_type;
    typedef allocator<_Node> _Node_allocator_type;
    _List_base(const allocator_type& __a);
    _STLP_alloc_proxy<_Node*, _Node, _Node_allocator_type> _M_node;   // +0x00 the sentinel
};

template <class _Tp, class _Alloc>
_List_base<_Tp, _Alloc>::_List_base(const allocator_type& __a) : _M_node(_Node_allocator_type(__a), (_Node*)0)
{
    _Node* __n = _M_node.allocate(1);
    __n->_M_next = __n;
    __n->_M_prev = __n;
    _M_node._M_data = __n;
}

template class _List_base<NPlayerGui::CStatBar::CChangeText, allocator<NPlayerGui::CStatBar::CChangeText> >;

}
