// LHNetworkLib::LSocket::`dynamic initializer for 'GlobalList''() -- construct the static socket list LSocket::GlobalList (STLport 4.6 sentinel node from malloc, linked to itself) and register its destructor with atexit [PDB display name: `dynamic initializer for 'LHNetworkLib::LSocket::GlobalList'']
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0121F850, 31 bytes, .CRT$XCU entry 0x013746A0: `push 0Ch ; call malloc 0x00BFEA0E ;
//   [eax] = [eax+4] = eax ; push 0x0122A200 ; mov [0x013BB000],eax ; call atexit 0x004012BC ; add esp,8 ; ret`. Whole function:
//   it ends in `ret` and the next initializer 0x0121F86F (.CRT$XCU 0x013746A4) starts right after it.
// Identity (proven): Ego_r.pdb 0x01017230 / FableWin.pdb 0x03FFFE40 `dynamic initializer for 'LHNetworkLib::LSocket::GlobalList''
//   (+ the `dynamic atexit destructor for` entries 0x0101D810 / 0x04011B20); 0x013BB000 is bound by landed code as
//   ?GlobalList@LSocket@LHNetworkLib@@2V?$list@PAVLSocket@LHNetworkLib@@V?$allocator@PAVLSocket@LHNetworkLib@@@std@@@std@@A (the same
//   symbol this object emits) and read only by LSocket code (0x008F84AA ..); the PDB's LSocket::CheckForActivity takes
//   `list<LSocket*>&`. The atexit thunk 0x0122A200 is this TU's `_$E2` (`mov ecx,offset GlobalList ; jmp 0x008F9F50`): the compiled
//   `_$E2` jumps to ??1?$_List_base@PAVLSocket@LHNetworkLib@@..., so 0x008F9F50 (sentinel freed at the end, `jmp` thunk 0x008FA060 =
//   ~list) is STLport `_List_base<LSocket*>::~_List_base`, not the bound ??1?$list (NAME_FIXES.tsv).
// Shape: the real STLport 4.6 `_List_base(const allocator_type&)` (sentinel allocated, linked to itself, stored last) with
//   allocator<_Node>::allocate(1) = malloc(12) folded; `~list() {}` over `~_List_base() { clear(); deallocate(sentinel); }`.
// Layout: LSocket's instance members are not used by this TU (only its static member is defined; the class is RTTI-less and
//   its full layout is in the landed LSocket ctor 0x008F8A80).
extern "C" void* __cdecl malloc(unsigned int size);        // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14

namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
struct _List_node_base {
    _List_node_base* _M_next;                              // +0x0
    _List_node_base* _M_prev;                              // +0x4
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                           // +0x8
};
template <class _Tp, class _Alloc>
class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    typedef allocator<_Node> _Node_allocator_type;
    typedef allocator<_Tp> allocator_type;
    _List_base(const allocator_type&) : _M_node(_Node_allocator_type(), (_Node*)0)
    {
        _Node* __n = _M_node.allocate(1);
        __n->_M_next = __n;
        __n->_M_prev = __n;
        _M_node._M_data = __n;
    }
    ~_List_base() { clear(); _M_node.deallocate(_M_node._M_data, 1); }
    void clear();
protected:
    _STLP_alloc_proxy<_Node*, _Node, _Node_allocator_type> _M_node;   // +0x0: the sentinel node
};
template <class _Tp, class _Alloc>
void _List_base<_Tp, _Alloc>::clear()
{
    _List_node<_Tp>* __cur = (_List_node<_Tp>*)this->_M_node._M_data->_M_next;
    while (__cur != this->_M_node._M_data) {
        _List_node<_Tp>* __tmp = __cur;
        __cur = (_List_node<_Tp>*)__cur->_M_next;
        this->_M_node.deallocate(__tmp, 1);
    }
    this->_M_node._M_data->_M_next = this->_M_node._M_data;
    this->_M_node._M_data->_M_prev = this->_M_node._M_data;
}
template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> {
public:
    typedef typename _List_base<_Tp, _Alloc>::allocator_type allocator_type;
    explicit list(const allocator_type& __a = allocator_type()) : _List_base<_Tp, _Alloc>(__a) { }
    ~list() { }
};
}   // namespace std

namespace LHNetworkLib {
class LSocket {
public:
    static std::list<LSocket*> GlobalList;                 // 0x013BB000
};
std::list<LSocket*> LSocket::GlobalList;
}