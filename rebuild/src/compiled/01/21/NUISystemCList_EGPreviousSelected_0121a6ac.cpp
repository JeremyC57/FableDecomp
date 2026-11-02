// NUISystem::CList::$E_GPreviousSelected() -- dynamic initializer of the static std::list<NUISystem::CList*> NUISystem::CList::GPreviousSelected: the inline STLport 4.6 list() constructor (malloc the 0xc sentinel node, link it to itself), then its atexit destructor is registered
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0121A6AC (unaligned: the CList TU is size-optimised and packed), 31 bytes, in
//   `.text$yc`, reached from the .CRT$XCU table entry 0x01374300, plain `ret`: `push 0Ch ; call malloc ; mov [eax],eax ;
//   mov [eax+4],eax ; mov [0x013B8A98],eax ; mov dword ptr [esp],0x01228E44 ; call _atexit ; pop ecx`.
// Identity (proven, two sources): (1) Ego_r.pdb 0x0100DFCD (unaligned, 29 bytes) / 0x0100DFF0 names
//   `dynamic initializer for 'NUISystem::CList::GPreviousSelected'' then `...'NUISystem::CHoverable::GAlreadyHovered'', and its
//   atexit pair 0x0101BC4A / 0x0101BC70 the same two; retail runs this list at 0x013B8A98, then 0x0121A6D0 (the list at
//   0x013B8AD4); the atexit thunk this row registers, 0x01228E44 (`mov ecx,0x013B8A98 ; jmp 0x0053C9BC`), jumps into the
//   ~list of the CList TU; (2) FableWin.pdb's static accessor
//   ?GetPreviousSelected@CList@NUISystem@@SIAAV?$list@PAVCList@NUISystem@@V?$allocator@PAVCList@NUISystem@@@std@@@std@@XZ proves
//   the type list<NUISystem::CList*>, and the landed CTCQuestCompletionUI::~CTCQuestCompletionUI 0x005E41E0 clears 0x013B8A98
//   under this name. Catalog row "_gapscan::sub_0121a6ac" carried no name; the stored vtable-like constant 0x01228E44 the
//   catalog note calls a vtable is the atexit thunk (landed as the stand-in `GlobalMethodTailThunk01228e44`, label
//   `Static_OnReadFinished_on_013b8a98_01228e44`: NAME_FIXES.tsv / labels).
// Data symbol: the landed binding's spelling (005E41E0), kept so the address keeps one name:
//   ?GPreviousSelected@CList@NUISystem@@2V?$list@PAVCList@NUISystem@@V?$allocator@PAVCList@NUISystem@@@std@@@std@@A
//   (public static: access unproven -- no PDB data-symbol dump; the FableWin accessor suggests a non-public member, a
//   respell is a coordinator decision). NUISystem::CList is declared here for its static only (class key V).
// Callees: malloc 0x00BFEA0E (the STLport node allocator), _atexit 0x004012BC; the `_$E2` atexit thunk (~list, not this row).
// Model: STLport 4.6 stl/_list.h / _alloc.h, namespace std (as the landed list rows 0x0042BF06 / 0x0121A6D0): _List_base's
//   constructor allocates the sentinel into a local, links it, then stores it (`mov [eax],eax ; mov [eax+4],eax ; mov [g],eax`).
// Pragma: `#pragma optimize("s", on)` -- the retail TU is size-optimised (this row and the TU's ~list 0x0053C9BC start
//   unaligned): it reuses the malloc argument slot for the atexit argument (`mov [esp],thunk ; ... ; pop ecx`); the /O2 form
//   pushes it and ends `add esp,8` (probe 2, as the /O2 GAlreadyHovered twin 0x0121A6D0 does).
#pragma optimize("s", on)
extern "C" void* __cdecl malloc(unsigned int __size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* __p);                 // 0x00BFEA14
namespace std {
template <class _Tp> class allocator {
public:
    typedef _Tp value_type;
    allocator() { }
    allocator(const allocator<_Tp>&) { }
    template <class _Tp1> allocator(const allocator<_Tp1>&) { }
    ~allocator() { }
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
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
template <class _Tp, class _Alloc> class _List_base {
protected:
    typedef _List_node<_Tp> _Node;
    typedef allocator<_Node> _Node_allocator_type;
public:
    typedef _Alloc allocator_type;
    _List_base(const allocator_type& __a) : _M_node(_Node_allocator_type(__a), (_Node*)0)
    {
        _Node* __n = _M_node.allocate(1);
        __n->_M_next = __n;
        __n->_M_prev = __n;
        _M_node._M_data = __n;
    }
    ~_List_base() { clear(); _M_node.deallocate(_M_node._M_data, 1); }
    void clear();
protected:
    _STLP_alloc_proxy<_Node*, _Node, _Node_allocator_type> _M_node;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> {
    typedef _List_base<_Tp, _Alloc> _Base;
public:
    typedef typename _Base::allocator_type allocator_type;
    explicit list(const allocator_type& __a = allocator_type()) : _List_base<_Tp, _Alloc>(__a) { }
    ~list();
};
}
namespace NUISystem {
class CList {
public:
    static std::list<CList*> GPreviousSelected;   // 0x013B8A98
};
}
std::list<NUISystem::CList*> NUISystem::CList::GPreviousSelected;