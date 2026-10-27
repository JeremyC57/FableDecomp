// std::_List_base::clear() -- (_List_base<CAtmosCopyInfo, allocator<CAtmosCopyInfo> >) STLport 4.6 list clear: destroy and free every node, then link the sentinel to itself
// Leaf: ?clear@?$_List_base@VCAtmosCopyInfo@@V?$allocator@VCAtmosCopyInfo@@@std@@@std@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004AC270, 70 bytes, thiscall, plain `ret`.
// Identity (proven): CWorld::~CWorld 0x004A6AB0 calls it on this+0x174 (`lea edi,[esi+174h]`), the retail slot of Ego_r.pdb CWorld::AtmosBanksWaitingToCopy (list<CAtmosCopyInfo>, Ego +392); the beside-it list destructors 0x004ACA50 / 0x004AD5A0 call it too; CAtmosCopyInfo is {CWideString DVDFileName, CWideString HDFileName, long EntryID} -- the CWideString destructors at +4 then +0.
// Callees: CWideString::~CWideString 0x0099B510 (twice: the element's implicit destructor), free 0x00BFEA14 (the node allocator's
//   deallocate: no null test).
// Shape (family M076, representative 0x004AC270): the landed STLport 4.6 list.c `_List_base<_Tp, _Alloc>::clear()` model
//   (0x00834F90) -- the sentinel re-read from `this` after every free; speed TU (the aligned loop's `nop`).
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
class CWideString {                                       // 4 bytes (PStringData)
public:
    ~CWideString();                                       // 0x0099B510
    void* PStringData;
};
class CAtmosCopyInfo {                                    // Ego_r.pdb, 12 bytes
public:
    CWideString DVDFileName;                              // +0x0
    CWideString HDFileName;                               // +0x4
    long EntryID;                                         // +0x8
};
namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
struct _List_node_base {
    _List_node_base* _M_next;                             // +0x0
    _List_node_base* _M_prev;                             // +0x4
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                          // +0x8
};
template <class _Tp, class _Alloc>
class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    typedef _STLP_alloc_proxy<_List_node_base*, _Node, allocator<_Node> > _AllocProxy;
    _AllocProxy _M_node;                                  // +0x0: the sentinel node
    void clear();
};
template <class _Tp, class _Alloc>
void _List_base<_Tp, _Alloc>::clear()
{
    _List_node<_Tp>* __cur = (_List_node<_Tp>*)this->_M_node._M_data->_M_next;
    while (__cur != this->_M_node._M_data) {
        _List_node<_Tp>* __tmp = __cur;
        __cur = (_List_node<_Tp>*)__cur->_M_next;
        _Destroy(&__tmp->_M_data);
        this->_M_node.deallocate(__tmp, 1);
    }
    this->_M_node._M_data->_M_next = this->_M_node._M_data;
    this->_M_node._M_data->_M_prev = this->_M_node._M_data;
}
template void _List_base<CAtmosCopyInfo, allocator<CAtmosCopyInfo > >::clear();
}