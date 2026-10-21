// std::_List_base<CListRecordWithWideStringAt24>::clear() -- destroy and free every node, leave the sentinel linked to itself
// Leaf: ?clear@?$_List_base@
// Retail: Fable.exe @ 0x00440770, 56 bytes, plain `ret`; callers 0x00440EC3, 0x004422F3, 0x00443149, 0x00646573,
// 0x0065424C. Catalog label CGuiControlTreePane::SortTreeRecursively is wrong: STLport _List_base::clear (as 0x0043FE40)
// over a record whose only non-trivial member is a CWideString at value +0x24 (node +0x2c, ~CWideString 0x0099B510),
// then free(node) (0x00BFEA14). The record's real name is not recoverable from the bytes, hence the behavioural name.
// Layout: STLport list -- one sentinel-node pointer; node {_M_next +0, _M_prev +4, _M_data +8}.
// Shape: /O2 (`lea ecx,[ecx]` loop-head pad, end re-read from the sentinel pointer every pass).
#include <stdlib.h>
class CWideString {
public:
    void* PData;
    ~CWideString();                                      // 0x0099B510
};
struct CListRecordWithWideStringAt24 {
    long        Fields[9];                               // +0x00
    CWideString Name;                                    // +0x24
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
template class std::_List_base<CListRecordWithWideStringAt24>;