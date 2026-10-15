// CTCPerceiveThing::Piece_007460f0() -- STLport list<T>::back(): the payload of the node before the sentinel
// Retail: Fable.exe (TLC, VC7.1) @ 0x007460F0, 9 bytes; no call sites or absolute references (an inlined-elsewhere
// COMDAT kept between CTCPerceiveThing's vtable bodies; catalogue row 007460d0 held it, skipped ledger, chunk 902).
// STLport _List_base keeps only the sentinel node pointer (the allocator proxy is empty) at +0; a node is
// { _M_next +0, _M_prev +4, _M_data +8 }, so back() == *(--end()) == &_M_node->_M_prev->_M_data.
// Shape: `mov eax,[ecx] ; mov eax,[eax+4] ; add eax,8` -- address of the data member of the previous node.
// Leaf: back
struct PerceiveListNode {
    PerceiveListNode* _M_next;                     // +0x00
    PerceiveListNode* _M_prev;                     // +0x04
    long              _M_data;                     // +0x08 element payload
};

struct PerceiveList {
    PerceiveListNode* _M_node;                     // +0x00 sentinel
    long& back();
};

long& PerceiveList::back()
{
    return _M_node->_M_prev->_M_data;
}
