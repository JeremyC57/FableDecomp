// std::list<C2DLineF>::~list() -- STLport list destructor: clear() inline (free every node up to the sentinel, then
// self-link the sentinel) and deallocate the sentinel node (null-tested allocator deallocate -> free)
// Leaf: ??1?$list@VC2DLineF@@V?$allocator@VC2DLineF@@@std@@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005127A0, length 62 bytes; thiscall (this in ecx, kept in edi).
// Callees: MSVCR71 free (import thunk 0x00BFEA14), for the nodes and the sentinel.
// Identity: was landed as "CGuiControlTreePane::SortTreeRecursively" (a propagated label; the catalog said
//   StdList_Clear_005127a0). The body is the STLport list destructor for a list whose nodes need no element destructor;
//   its callers are the list<C2DLineF> code of the CNavigationLayer line lists (Ego_r struct_layouts: CNavigationLayer
//   LineLists = vector<list<C2DLineF> > +16, SwitchableLineLists = CVectorMap<unsigned __int64, list<C2DLineF> > +32):
//   the pair<u64, list<C2DLineF> > heap/sort/copy helpers 0x00A820E0, 0x00A81E20, 0x00A82960, 0x00A82B10, 0x00A83570,
//   0x00A852B0 and the list<C2DLineF> operator= / range-insert TU around 0x00517720 / 0x005152C0 (agent422 NOTES).
//   Retail does not fold identical COMDATs, so the 21 call sites share this one instance. Whether the out-of-line symbol is
//   ~list (with ~_List_base inlined) or ~_List_base itself is not recoverable from the bytes; ~list is used (as for the
//   landed std::list<long>::~list @ 0x0042AB73).
extern "C" void __cdecl free(void*);

class C2DLineF;

namespace std {
template <class _Tp> class allocator;

struct _List_node_base {
    _List_node_base* _M_next;                   // +0x00
    _List_node_base* _M_prev;                   // +0x04
};

template <class _Tp, class _Alloc = allocator<_Tp> > class list {
public:
    _List_node_base* _M_node;                   // +0x00 the sentinel node (STLport _List_base)
    ~list();
};

template <class _Tp, class _Alloc> list<_Tp, _Alloc>::~list()
{
    _List_node_base* cur = _M_node->_M_next;
    while (cur != _M_node) {
        _List_node_base* tmp = cur;
        cur = cur->_M_next;
        free(tmp);
    }
    _M_node->_M_next = _M_node;
    _M_node->_M_prev = _M_node;
    if (_M_node != 0)
        free(_M_node);
}

template list<C2DLineF>::~list();
}
