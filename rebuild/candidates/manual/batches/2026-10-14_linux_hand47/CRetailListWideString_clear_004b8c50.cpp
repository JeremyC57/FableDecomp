// CRetailList_WideString::clear() -- destroy every node's CWideString (+8), free the node, re-link the head to itself
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004B8C50, length 56 bytes.
// Callees: ~CWideString @ 0x0099B510, free @ 0x00BFEA14. Shape: the landed list clear @ 0x00A80A20 with a one-word value (`lea ecx,[edi+8]`). The chunk label
// "CGuiControlTreePane::SortTreeRecursively" is a propagated STL label.
extern "C" void __cdecl free(void* block);   // 0x00BFEA14
struct CWideString { void* m_pData; ~CWideString(); };   // 0x0099B510

struct CRetailList_WideString {
    struct node {
        node* next;                                // +0
        node* prev;                                // +4
        CWideString value;                         // +8
    };
    node* head;
    void clear();
};

void CRetailList_WideString::clear()
{
    node* n = head->next;
    while (n != head) {
        node* victim = n;
        n = n->next;
        victim->value.~CWideString();
        free(victim);
    }
    head->next = head;
    head->prev = head;
}
