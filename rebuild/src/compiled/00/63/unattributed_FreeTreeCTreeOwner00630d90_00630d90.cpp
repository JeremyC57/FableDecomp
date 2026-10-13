// CTreeOwner_00630d90::FreeTree(SNode*) -- recursive free of a child-linked (+0xc) / sibling-linked (+0x8) node list, CMember_0099eae0 member at +0x14 destroyed first
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00630D90, oracle length 59 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push ebx ; push esi ; mov esi,[esp+0Ch] ; test esi,esi ; mov ebx,ecx ; je E ; push edi ; L: mov eax,[esi+0xc] ; push eax ; mov ecx,ebx ; call FreeTree ; mov edi,[esi+0x8] ; lea ecx,[esi+0x14] ; call CMember_0099eae0::~CMember_0099eae0 ; push esi ; call free ; add esp,4 ; test edi,edi ; mov esi,edi ; jne L ; pop edi ; E: pop esi ; pop ebx ; ret 4
// Walks a sibling list: recurses into each node's child list at +0xc, remembers the sibling at
// +0x8, destroys the node's CMember_0099eae0 member at +0x14 (destructor @ 0x0099EAE0), frees the
// node (free @ 0x00BFEA14) and continues with the sibling. The manifest label
// (`LTextTreeWalkThrough::BuildTreeArray`) is propagated; the owner is named descriptively.
// One of 12 bodies of this shape.
extern "C" void __cdecl free(void* block);

struct CMember_0099eae0 {
    void* m_data;
    ~CMember_0099eae0();                             // 0x0099EAE0
};

struct SNode {
    unsigned char _pad[0x8];
    SNode* m_next;                       // +0x8
    SNode* m_child;                       // +0xc
    unsigned char _gap2[0x4];
    CMember_0099eae0 m_name;        // +0x14
};

struct CTreeOwner_00630d90 {
    void FreeTree_CTreeOwner_00630d90(SNode* node);
};

void CTreeOwner_00630d90::FreeTree_CTreeOwner_00630d90(SNode* node)
{
    while (node) {
        FreeTree_CTreeOwner_00630d90(node->m_child);
        SNode* next = node->m_next;
        node->m_name.~CMember_0099eae0();
        free(node);
        node = next;
    }
}