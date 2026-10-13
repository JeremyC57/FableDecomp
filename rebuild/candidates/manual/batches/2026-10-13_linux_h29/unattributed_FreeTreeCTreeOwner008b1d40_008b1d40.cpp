// CTreeOwner_008b1d40::FreeTree(SNode*) -- recursive free of a child-linked (+0xc) / sibling-linked (+0x8) node list
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008B1D40, oracle length 51 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push ebx ; push esi ; mov esi,[esp+0Ch] ; test esi,esi ; mov ebx,ecx ; je E ; push edi ; L: mov eax,[esi+0xc] ; push eax ; mov ecx,ebx ; call FreeTree ; mov edi,[esi+0x8] ; push esi ; call free ; add esp,4 ; test edi,edi ; mov esi,edi ; jne L ; pop edi ; E: pop esi ; pop ebx ; ret 4
// Walks a sibling list: recurses into each node's child list at +0xc, remembers the sibling at
// +0x8, frees the node (free @ 0x00BFEA14) and continues with the sibling. `this` is only
// needed for the recursive call. The manifest label (`LTextTreeWalkThrough::BuildTreeArray`) is
// propagated; the owner is named descriptively. One of 26 bodies of this shape.
extern "C" void __cdecl free(void* block);

struct SNode {
    unsigned char _pad[0x8];
    SNode* m_next;                       // +0x8
    SNode* m_child;                       // +0xc
};

struct CTreeOwner_008b1d40 {
    void FreeTree_CTreeOwner_008b1d40(SNode* node);
};

void CTreeOwner_008b1d40::FreeTree_CTreeOwner_008b1d40(SNode* node)
{
    while (node) {
        FreeTree_CTreeOwner_008b1d40(node->m_child);
        SNode* next = node->m_next;
        free(node);
        node = next;
    }
}
