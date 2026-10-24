// StdList_InitEmpty_006864c0(SListHead* list) -- give a bare STLport list head a fresh self-linked 12-byte sentinel node (the head word cleared first)
// Leaf: StdList_InitEmpty_006864c0
//
// Retail 0x006864C0, 28 bytes (+ int3 padding to 32), plain `ret`, the list in ecx, no return value.
// Callee: malloc @ 0x00BFEA0E (STLport's static malloc node allocator, no out-of-memory check).
// Identity: unreferenced -- no call, jmp or absolute reference reaches it (xref), so its class is not recoverable; it
//   sits in the CTCCreatureOpinionOfHero TU (the GetInterface helpers 0x006864F0.. and the class's slots follow).  The
//   body is STLport 4.x `_List_base(const allocator_type&) : _M_node(a, (_Node*)0) { _Node* n = _M_node.allocate(1);
//   n->_M_next = n; n->_M_prev = n; _M_node._M_data = n; }`, but without a constructor's `mov eax, this`, so it is
//   written as a descriptive __fastcall helper over the one-word list head (a 12-byte node = a 4-byte element).
// Shape: the head word is zeroed before the allocation (kept: the call could observe it), then the sentinel links to
//   itself both ways and becomes the head.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E

struct _List_node_base {
    _List_node_base* _M_next;                               // +0x00
    _List_node_base* _M_prev;                               // +0x04
};
struct _List_node : _List_node_base {
    long _M_data;                                           // +0x08 (a 4-byte element)
};
struct SListHead {                                          // retail STLport list: the bare sentinel pointer
    _List_node* _M_node;                                    // +0x00
};

void __fastcall StdList_InitEmpty_006864c0(SListHead* list)
{
    list->_M_node = 0;
    _List_node* n = (_List_node*)malloc(sizeof(_List_node));
    n->_M_next = n;
    n->_M_prev = n;
    list->_M_node = n;
}