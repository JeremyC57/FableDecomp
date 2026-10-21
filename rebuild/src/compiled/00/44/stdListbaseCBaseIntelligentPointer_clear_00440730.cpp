// std::_List_base<CBaseIntelligentPointer>::clear() -- destroy and free every node, leave the sentinel linked to itself
// Leaf: ?clear@?$_List_base@
// Retail: Fable.exe @ 0x00440730, 56 bytes, plain `ret`; callers 0x00440E43, 0x00441673, 0x00442663, 0x0082A313 and a
// tail jump from 0x0082A303. Catalog label CGuiControlTreePane::SortTreeRecursively is wrong: STLport _List_base::clear
// (as 0x0043FE40) over intelligent-pointer elements -- each node's value (+8) is destroyed by the out-of-line
// CBaseIntelligentPointer destructor (0x00A01C10, non-virtual: called directly), then free(node) (0x00BFEA14). The
// exact CIntelligentPointer<T> instantiation is not visible in the bytes, so the element is modelled as the base.
// Layout: STLport list -- one sentinel-node pointer; node {_M_next +0, _M_prev +4, _M_data +8}; CBaseIntelligentPointer
// 8 bytes (vptr, info pointer).
// Shape: /O2 (`lea ecx,[ecx]` loop-head pad, end re-read from the sentinel pointer every pass).
#include <stdlib.h>
class CBaseIntelligentPointer {
public:
    void* _vfptr_or_pad;                                 // +0x00
    void* PInfo;                                         // +0x04
    ~CBaseIntelligentPointer();                          // 0x00A01C10
};
namespace std {
struct _List_node_base {
    _List_node_base* _M_next;                            // +0x00
    _List_node_base* _M_prev;                            // +0x04
};
template <class T> struct _List_node : public _List_node_base {
    T _M_data;                                           // +0x08
};
template <class T> inline void _Destroy(T* p) { p->~T(); }
template <class T> class _List_base {
public:
    _List_node<T>* _M_node;                              // sentinel
    void clear()
    {
        _List_node<T>* cur = (_List_node<T>*)_M_node->_M_next;
        while (cur != _M_node) {
            _List_node<T>* tmp = cur;
            cur = (_List_node<T>*)cur->_M_next;
            _Destroy(&tmp->_M_data);
            free(tmp);
        }
        _M_node->_M_next = _M_node;
        _M_node->_M_prev = _M_node;
    }
};
}
template class std::_List_base<CBaseIntelligentPointer>;