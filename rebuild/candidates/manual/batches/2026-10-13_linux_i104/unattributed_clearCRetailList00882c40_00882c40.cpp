// CRetailList_00882c40::clear() -- destroy every node's polymorphic value at +0x8 through its virtual destructor (slot 0, flag 0), free the node, re-link the head to itself
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00882C40, oracle length 58 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push ebx ; mov ebx,ecx ; mov eax,[ebx] ; push esi ; mov esi,[eax] ; cmp esi,eax ; je E ; push edi ; L: mov edi,esi ; mov eax,[edi+0x8] ; mov esi,[esi] ; lea ecx,[edi+0x8] ; push 0 ; call [eax] ; push edi ; call free ; mov eax,[ebx] ; add esp,4 ; cmp esi,eax ; jne L ; pop edi ; E: mov eax,[ebx] ; mov [eax],eax ; mov ebx,[ebx] ; mov [ebx+4],ebx ; pop ebx ; ret
// STLport-style list::clear for values with a virtual destructor: walks from head->next to the
// head (re-read from `this` each iteration), advancing the cursor before the old node's value
// is destroyed through vtable slot 0 (the scalar deleting destructor called with flag 0: an
// explicit destructor call) and the node freed (free @ 0x00BFEA14, no null test), then links
// the head to itself. The manifest label (`CGuiControlTreePane::SortTreeRecursively`) is
// propagated across 9 identical bodies; the list is named descriptively.
extern "C" void __cdecl free(void* block);

struct CNodeValue_00882c40 {
    virtual ~CNodeValue_00882c40();                     // slot 0: scalar deleting destructor
};

struct CRetailList_00882c40 {
    struct SNode {
        SNode* m_next;                     // +0
        SNode* m_prev;                     // +4
        CNodeValue_00882c40 m_value;                    // +0x8
    };
    SNode* m_head;                         // +0 sentinel
    void clear_CRetailList_00882c40();
};

void CRetailList_00882c40::clear_CRetailList_00882c40()
{
    SNode* cur = m_head->m_next;
    while (cur != m_head) {
        SNode* tmp = cur;
        cur = cur->m_next;
        tmp->m_value.~CNodeValue_00882c40();
        free(tmp);
    }
    m_head->m_next = m_head;
    m_head->m_prev = m_head;
}
