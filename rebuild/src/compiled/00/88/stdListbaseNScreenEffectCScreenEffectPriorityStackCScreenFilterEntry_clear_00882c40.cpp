// std::_List_base::clear() -- (_List_base<NScreenEffect::CScreenEffectPriorityStack::CScreenFilterEntry, allocator<...> >) STLport 4.6 list clear: walk from the first node to the sentinel, destroy each node's element through its virtual destructor and free the node, then link the sentinel to itself
// Leaf: ?clear@?$_List_base@UCScreenFilterEntry@CScreenEffectPriorityStack@NScreenEffect@@V?$allocator@UCScreenFilterEntry@CScreenEffectPriorityStack@NScreenEffect@@@std@@@std@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00882C40, 58 bytes (+ int3 padding), thiscall, `ret`.
// Identity: the row was landed as the invented "CRetailList_00882c40::clear". Its only callers are the unreferenced list
//   destructors 0x00883360 / 0x00883CC0 of the CScreenEffectPriorityStack TU, each emitted right after the list constructor
//   that mallocs a 0x44-byte sentinel (0x00883340 / 0x00883CA0: 8 + 0x3C = Ego_r.pdb CScreenFilterEntry, 64 bytes there with
//   a 16-byte Dinkumware map, 12 bytes in STLport); Ego_r CScreenEffectPriorityStack::ScreenFilterParamStack is
//   list<CScreenFilterEntry>[3], after RadialParamStack (the landed sibling 0x00882B80) and DisplacementParamStack
//   (0x00882C00); the PDB name sets spell the entry a struct (UCScreenFilterEntry@CScreenEffectPriorityStack@NScreenEffect@@).
//   Family pass 2 (agent951 G073) agrees; retail has no ICF, so each byte-identical body is one element type.
// Callees: the element's virtual destructor (vtable slot 0, pushed 0 = destroy only, no delete), free 0x00BFEA14 (the node
//   allocator's deallocate: no null test).
// Body: STLport 4.6 list.c `_List_base<_Tp, _Alloc>::clear()`; the sentinel is re-read from `this` after every free.
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14

namespace NScreenEffect {
class CScreenEffectPriorityStack {
public:
    struct CStackEntry {                                     // 0x10 bytes (Ego_r.pdb; a struct in the PDB name sets)
        virtual ~CStackEntry();
        long curdurationcount;                               // +0x4
        long durationcount;                                  // +0x8
        bool fadingout;                                      // +0xC
    };
    struct CScreenFilterEntry : public CStackEntry {         // 0x3C bytes (Ego_r 0x40 with a 16-byte Dinkumware map)
        unsigned char param[0x20];                           // +0x10 CScreenFilterParams
        unsigned char reset[0x0C];                           // +0x30 STLport map<CThing*, CThingGraphicByPassFlagState>
    };
};
}

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
    typedef _STLP_alloc_proxy<_List_node_base*, _Node, allocator<_Node> > _AllocProxy;
    _AllocProxy _M_node;                                   // +0x0: the sentinel node
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

typedef NScreenEffect::CScreenEffectPriorityStack::CScreenFilterEntry _Elem;
template void _List_base<_Elem, allocator<_Elem> >::clear();

} // namespace std