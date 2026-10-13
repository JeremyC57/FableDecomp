// CRetailList_CValue_0065d5f8::clear() -- destroy every node's CValue_0065d5f8 value (@ 0x0065D5F8), free the node, re-link the head to itself
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0065FF2D, oracle length 49 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push edi ; mov edi,ecx ; mov eax,[edi] ; mov esi,[eax] ; cmp esi,eax ; je E ; push ebx ; L: mov ebx,esi ; mov esi,[esi] ; lea ecx,[ebx+0x8] ; call CValue_0065d5f8::~CValue_0065d5f8 ; push ebx ; call free ; cmp esi,[edi] ; pop ecx ; jne L ; pop ebx ; E: mov eax,[edi] ; mov [eax],eax ; mov edi,[edi] ; mov [edi+4],edi ; pop edi ; pop esi ; ret
// Retail std::list::clear for CValue_0065d5f8 values: walks from head->next to the head (re-read from
// `this` each iteration), advancing the cursor before the old node's value is destroyed
// (@ 0x0065D5F8) and the node freed (free @ 0x00BFEA14, the argument popped into ecx: a size
// TU), then links the head to itself. The manifest label is propagated. One of 11 bodies.
extern "C" void __cdecl free(void* block);

struct CValue_0065d5f8 {
    void* m_data;
    ~CValue_0065d5f8();                             // 0x0065D5F8
};

struct CRetailList_CValue_0065d5f8 {
    struct node {
        node* next;                        // +0
        node* prev;                        // +4
        CValue_0065d5f8 value;                      // +0x8
    };
    node* head;
    void clear();
};

void CRetailList_CValue_0065d5f8::clear()
{
    node* n = head->next;
    while (n != head) {
        node* victim = n;
        n = n->next;
        victim->value.~CValue_0065d5f8();
        free(victim);
    }
    head->next = head;
    head->prev = head;
}
