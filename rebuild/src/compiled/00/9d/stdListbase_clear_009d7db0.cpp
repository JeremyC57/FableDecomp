// std::_List_base::clear() -- (_List_base<CCountedPointer<CBankFileAsyncData> >) STLport list clear: walk from the first node to the sentinel, release each node's counted pointer (deleter and count block freed at zero), free the node, then link the sentinel to itself
// Leaf: ?clear@?$_List_base@V?$CCountedPointer@VCBankFileAsyncData@@@@V?$allocator@V?$CCountedPointer@VCBankFileAsyncData@@@@@std@@@std@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D7DB0, 89 bytes (+ int3 padding), thiscall, `ret`.
// Identity (proven): std::_List_base<CCountedPointer<CBankFileAsyncData>, std::allocator<CCountedPointer<CBankFileAsyncData> > >::clear();
//   evidence: ~CBankFileAsync 0x009D6490 (called by the deleting destructor 0x009D81A0, slot 0 of vtable 0x0129B8D4, RTTI .?AVCBankFileAsync@@) clears this+0x144 = Ego_r.pdb CBankFileAsync::DisposalList (list<CCountedPointer<CBankFileAsyncData> >, retail offset); the list wrappers 0x009D7FA0 / 0x009D80E0 are its other callers.
// Label: the manifest's "remove_if<NEntityEvents::CConvFilterToCP<NEntityEvents::CIsEventOfType>_>" is propagated (family J111: five byte-identical counted-pointer list clears; retail has no
//   identical-code folding).
// Callees: the count block's deleter (CCPPointerInfo slot +4, __fastcall on the object), operator delete 0x00BFE9BC (the block),
//   free 0x00BFEA14 (the node allocator's deallocate: no null test).
// Shape: agent951's STLport `_List_base<_Tp, _Alloc>::clear()` (G073, 0x004A9CF0; the out-of-line member list::clear() and
//   ~_List_base() call) with CCountedPointer's destructor inline (J110's form: test Info, decrement, at zero DeleteFunc(Data)
//   and delete Info, then null Data and Info); the sentinel is re-read from `this` after every free.
// Layout: STLport list node {next +0, prev +4, CCountedPointer<T> {Data +8, Info +0xC}}; CCPPointerInfo {RefCount, DeleteFunc,
//   Data} (Ego_r.pdb, 12 bytes).
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14

class CBankFileAsyncData;
class CCPPointerInfo {                                     // Ego_r.pdb 12 bytes
public:
    unsigned long RefCount;                                // +0x00
    void (__fastcall* DeleteFunc)(void* data);             // +0x04
    void* Data;                                            // +0x08
};
template <class T> class CCountedPointer {               // 8 bytes: the object, the shared count block
public:
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
    T*              Data;                                  // +0x0
    CCPPointerInfo* Info;                                  // +0x4
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

typedef CCountedPointer<CBankFileAsyncData> _Elem;
template void _List_base<_Elem, allocator<_Elem> >::clear();
}   // namespace std