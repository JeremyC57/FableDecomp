// CRetailList_ListOfCountedPointer::clear() -- destroy every node's inner list (+0xc: clear it, free its head node), free the node, re-link the head to itself
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A84110, length 80 bytes.
// Callees: the inner list's clear @ 0x00A80A20 (the landed CRetailList_CountedPointer::clear), free @ 0x00BFEA14 (the inner head, then the node). Shape: the landed list clear @ 0x00A80A20 with the inner list's
// inlined destructor (its address kept in edi for the clear call, its head re-read through edi and null-tested before the free). The chunk label "CGuiControlTreePane::SortTreeRecursively"
// is a propagated STL label.
extern "C" void __cdecl free(void* block);   // 0x00BFEA14
struct CRetailList_CountedPointer {                     // the inner list: one head-node pointer
    void* m_pHead;
    void clear();                                  // 0x00A80A20
    ~CRetailList_CountedPointer()
    {
        clear();
        if (m_pHead)
            free(m_pHead);
    }
};

struct CRetailList_ListOfCountedPointer {
    struct node {
        node* next;                                // +0
        node* prev;                                // +4
        long  m_key;                               // +8
        CRetailList_CountedPointer value;      // +0xc
    };
    node* head;
    void clear();
};

void CRetailList_ListOfCountedPointer::clear()
{
    node* n = head->next;
    while (n != head) {
        node* victim = n;
        n = n->next;
        victim->value.~CRetailList_CountedPointer();
        free(victim);
    }
    head->next = head;
    head->prev = head;
}