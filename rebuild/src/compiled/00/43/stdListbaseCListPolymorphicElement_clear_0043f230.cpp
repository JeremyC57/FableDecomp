// std::_List_base<CListPolymorphicElement>::clear() -- destroy (virtually) and free every node, leave the sentinel linked to itself
// Leaf: ?clear@?$_List_base@
// Renamed (rename1, 2026-10-24) from CGuiControlTreePane::SortTreeRecursively (catalog name only; this source and its
//   ps1 Module already say std::_List_base<CListPolymorphicElement>::clear).
// Retail: Fable.exe @ 0x0043F230, 58 bytes, plain `ret`; callers 0x0043FCD3, 0x004406A3, 0x00441C29, 0x004AC817.
// Catalog label CGuiControlTreePane::SortTreeRecursively is wrong: STLport _List_base::clear (as 0x0043FE40) over
// elements with a virtual destructor -- _Destroy's `p->~T()` on each node value (+8) is the vtable slot-0 deleting
// destructor called with flags 0 (`push 0 / call [eax]`), then free(node) (0x00BFEA14). The element class is not
// identifiable from the bytes, hence the behavioural name.
// Layout: STLport list -- one sentinel-node pointer; node {_M_next +0, _M_prev +4, _M_data +8}.
// Shape: /O2 (`lea ecx,[ecx]` loop-head pad, end re-read from the sentinel pointer every pass).
#include <stdlib.h>
class CListPolymorphicElement {
public:
    virtual ~CListPolymorphicElement();
    long Value;                                          // +0x04
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
template class std::_List_base<CListPolymorphicElement>;