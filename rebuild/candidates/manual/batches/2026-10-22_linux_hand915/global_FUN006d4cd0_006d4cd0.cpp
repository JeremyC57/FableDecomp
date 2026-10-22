// _global::FUN_006d4cd0() -- STLport list empty(): the sentinel node's next pointer points back at the sentinel
// Retail: Fable.exe (TLC, VC7.1) @ 0x006D4CD0, 14 bytes, `ret`. Unreferenced out-of-line copy of an inline
//   `std::list<T>::empty() const` (`_M_node->_M_next == _M_node`): `mov eax,[ecx] ; mov edx,[eax] ; cmp ; sete`.
//   The element type is unknown; modelled as a list of opaque nodes.
struct SListNodeBase {
    SListNodeBase* _M_next;                      // +0x00
    SListNodeBase* _M_prev;                      // +0x04
};

struct CList_006d4cd0 {
    SListNodeBase* _M_node;                      // +0x00 the sentinel
    bool FUN_006d4cd0() const;
};

bool CList_006d4cd0::FUN_006d4cd0() const
{
    return _M_node->_M_next == _M_node;
}
