// std::_List_base::clear() -- (list<CEngineInternalAttachedPrimitiveInfo>) STLport list clear: destroy every node's attached-primitive info through its virtual destructor, free the node, re-link the sentinel to itself
// Leaf: ?clear@?$_List_base@VCEngineInternalAttachedPrimitiveInfo@@V?$allocator@VCEngineInternalAttachedPrimitiveInfo@@@std@@@std@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B3A760, length 58 bytes, thiscall, plain `ret`.
// Identity: element from the Ego_r.pdb name sets ?clear@?$list@VCEngineInternalAttachedPrimitiveInfo@@V?$allocator@
//   VCEngineInternalAttachedPrimitiveInfo@@@std@@@std@@QAEXXZ and ??1CEngineInternalAttachedPrimitiveInfo@@UAE@XZ (virtual
//   destructor: the `push 0 ; call [eax]` scalar-deleting-destructor call with flag 0) and struct_layouts_egor
//   CEnginePrimitiveAttachmentManager::AttachedPrimitives (list<CEngineInternalAttachedPrimitiveInfo>, +0x10); the value
//   sits at node+8 (12 bytes: vptr + CEnginePrimitiveHandle). Retail is STLport 4.6, whose list::clear() forwards to the
//   out-of-line _List_base::clear() in _list.c, also called by ~_List_base (agent428 NOTES); callers 0x00B39F70,
//   0x00B3A850, 0x00B3A8C0 and the CEnginePrimitiveAttachmentManager deleting destructor 0x00B3ADC0. The manifest label
//   CGuiControlTreePane::SortTreeRecursively and the landed CRetailList_00b3a760 were a propagation and a shape name.
// Shape: STLport 4.6 _list.c: `__cur = _M_node->_M_next; while (__cur != _M_node) { __tmp = __cur; __cur = __cur->_M_next;
//   _Destroy(&__tmp->_M_data); _M_node.deallocate(__tmp, 1); }` then the sentinel's next/prev self-link; the sentinel is
//   re-read from `this` each pass, the cursor advances before the old value is destroyed, and the node goes to free
//   @ 0x00BFEA14 with no null test.
extern "C" void __cdecl free(void* block);              // 0x00BFEA14

class CEnginePrimitiveHandle {                          // 8 bytes (Ego_r: vptr, CIVCountedPointer Data)
public:
    void* _vtable;                                      // +0x00
    void* Data;                                         // +0x04
};

class CEngineInternalAttachedPrimitiveInfo {            // 12 bytes
public:
    virtual ~CEngineInternalAttachedPrimitiveInfo();    // slot 0: scalar deleting destructor
    CEnginePrimitiveHandle PrimitiveHandle;             // +0x04
};

namespace std {
template <class _Tp> class allocator { };

struct _List_node_base {
    _List_node_base* _M_next;                           // +0x00
    _List_node_base* _M_prev;                           // +0x04
};

template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                        // +0x08
};

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

template <class _Tp, class _Alloc> class _List_base {
public:
    _List_node<_Tp>* _M_node;                           // +0x00 the sentinel (the node-allocator proxy's data word)
    void clear();
};

template <class _Tp, class _Alloc> void _List_base<_Tp, _Alloc>::clear()
{
    _List_node<_Tp>* __cur = (_List_node<_Tp>*)this->_M_node->_M_next;
    while (__cur != this->_M_node) {
        _List_node<_Tp>* __tmp = __cur;
        __cur = (_List_node<_Tp>*)__cur->_M_next;
        _Destroy(&__tmp->_M_data);
        free(__tmp);
    }
    this->_M_node->_M_next = this->_M_node;
    this->_M_node->_M_prev = this->_M_node;
}
}

template void std::_List_base<CEngineInternalAttachedPrimitiveInfo,
    std::allocator<CEngineInternalAttachedPrimitiveInfo> >::clear();
