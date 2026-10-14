// CRetailList_StringRecord::clear() -- destroy every node's record (two CWideStrings and a CCharString after three plain words), free the node, re-link the head to itself
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004B8C00, length 80 bytes.
// Callees: ~CWideString @ 0x0099B510 (twice: +0x1c then +0x18), ~CCharString @ 0x0099EAE0 (+0x14), free @ 0x00BFEA14. Shape: the landed CRetailList_CountedPointer::clear @ 0x00A80A20
// (cursor advanced before the old node is destroyed, the head re-read from `this` every iteration, `push esi` inside the non-empty path). The chunk label
// "CGuiControlTreePane::SortTreeRecursively" is a propagated STL label; the value type is reconstructed from the destructor calls.
extern "C" void __cdecl free(void* block);   // 0x00BFEA14
struct CCharString { void* m_pData; ~CCharString(); };   // 0x0099EAE0
struct CWideString { void* m_pData; ~CWideString(); };   // 0x0099B510

struct SStringRecord_004b8c00 {                    // 24 bytes: three words, then the three strings
    long        m_a;                               // +0x00
    long        m_b;                               // +0x04
    long        m_c;                               // +0x08
    CCharString m_name;                            // +0x0c
    CWideString m_text;                            // +0x10
    CWideString m_tooltip;                         // +0x14
};

struct CRetailList_StringRecord {
    struct node {
        node* next;                                // +0
        node* prev;                                // +4
        SStringRecord_004b8c00 value;              // +8
    };
    node* head;
    void clear();
};

void CRetailList_StringRecord::clear()
{
    node* n = head->next;
    while (n != head) {
        node* victim = n;                          // retail advances the cursor first, then destroys the old node
        n = n->next;
        victim->value.~SStringRecord_004b8c00();
        free(victim);
    }
    head->next = head;
    head->prev = head;
}
