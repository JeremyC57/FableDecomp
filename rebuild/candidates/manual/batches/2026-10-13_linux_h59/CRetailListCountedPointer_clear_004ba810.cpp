// CRetailList_CountedPointer::clear() -- release every node's CCountedPointer (count block destroy + delete at zero), free the node, re-link the head to itself
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004BA810, oracle length 89 bytes.
// Grade: candidate -- still DIFFER(89v89) in the Linux lane (Wine + DDK 13.10.4035 cl.exe); generated for a
//        repeated body shape, see the batch README for the open question.
//
// Retail bytes:  push ebx ; mov ebx,ecx ; mov eax,[ebx] ; push edi ; mov edi,[eax] ; cmp edi,eax ; je E ; push ebp ; push esi ; xor ebp,ebp ; L: mov esi,edi ; mov eax,[esi+0x14] ; cmp eax,ebp ; mov edi,[edi] ; je F ; dec dword ptr [eax] ; mov eax,[esi+0x14] ; cmp [eax],ebp ; jne F ; mov ecx,[eax+8] ; call [eax+4] ; mov eax,[esi+0x14] ; push eax ; call operator delete ; add esp,4 ; F: push esi ; mov [esi+0x10],ebp ; mov [esi+0x14],ebp ; call free ; mov eax,[ebx] ; add esp,4 ; cmp edi,eax ; jne L ; pop esi ; pop ebp ; E: mov eax,[ebx] ; mov [eax],eax ; mov ebx,[ebx] ; mov [ebx+4],ebx ; pop ebx ; ret
// Retail std::list::clear for CCountedPointer values: walks from head->next to the head,
// remembering each node's successor, runs the inlined CCountedPointer destructor (decrement the
// count block; at zero call its destroy function @ slot +4 with the object in ecx and delete the
// block through the flag-tested operator delete @ 0x00BFE9BC; then null both fields), frees
// the node (free @ 0x00BFEA14) and finally links the head to itself. The head is re-read from
// `this` on every iteration (retail reloads [ebx]). One of 14 bodies of this shape.
extern "C" void __cdecl free(void* block);

struct SCountBlock {
    long m_references;                     // +0
    void (__fastcall* m_pfnDestroy)(void*);// +4
    void* m_pObject;                       // +8
};

struct CCountedPointer {
    void* m_pObject;                       // +0
    SCountBlock* m_pCount;                 // +4
    ~CCountedPointer()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }
        }
        m_pObject = 0;
        m_pCount = 0;
    }
};

struct CRetailList_CountedPointer {
    struct node {
        node* next;                        // +0
        node* prev;                        // +4
        CCountedPointer value;             // +0x10
    };
    node* head;
    void clear();
};

void CRetailList_CountedPointer::clear()
{
    node* n = head->next;
    while (n != head) {
        node* victim = n;                  // retail advances the cursor first, then destroys the old node
        n = n->next;
        victim->value.~CCountedPointer();
        free(victim);
    }
    head->next = head;
    head->prev = head;
}
