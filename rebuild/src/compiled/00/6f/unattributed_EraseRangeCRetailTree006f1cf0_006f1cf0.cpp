// CRetailTree_006f1cf0::EraseRange(SNode*, SNode*) -- the whole tree is cleared through the recursive Erase @ 0x006EFCE0 (head reset, size zeroed); otherwise each node from first to last is unlinked through the __cdecl helper @ 0x0042A34E with the successor computed inline, freed and the size decremented
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006F1CF0, oracle length 189 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push ebx ; mov ebx,[last] ; push esi ; mov esi,[first] ; push edi ; mov edi,ecx ; mov eax,[edi] ; cmp esi,[eax+8] ; jne L ; cmp ebx,eax ; jne L ; mov ecx,[edi+4] ; test ecx,ecx ; je E ; push [eax+4] ; mov ecx,edi ; call Erase ; (head->left = head ; head->parent = 0 ; head->right = head ; size = 0) ; pop edi ; pop esi ; pop ebx ; ret 8 ; L: cmp esi,ebx ; je E ; N: (successor of esi into esi, victim in ecx) ; mov eax,[edi] ; lea edx,[eax+0Ch] ; push edx ; lea edx,[eax+8] ; push edx ; add eax,4 ; push eax ; push ecx ; call Unlink ; add esp,10h ; test eax,eax ; je S ; push eax ; call free ; add esp,4 ; S: mov ecx,[edi+4] ; dec ecx ; cmp esi,ebx ; mov [edi+4],ecx ; jne N ; E: pop edi ; pop esi ; pop ebx ; ret 8
// STLport-style `erase(first, last)`: when the range is [begin, end) the tree is cleared (the
// recursive node release @ 0x006EFCE0 from the root, the header re-linked to itself and the
// size zeroed); otherwise `erase(first++)` repeats -- the in-order successor is computed
// inline (right subtree's leftmost, else climb while the node is a right child), the victim
// is unlinked through the __cdecl helper @ 0x0042A34E with the addresses of the header's
// parent/left/right links (the last formed with `add eax,4`), freed when non-null (free @
// 0x00BFEA14) and the size decremented. Values are trivially destructible here. Labels are
// propagated; names are descriptive. One of 11 bodies of this shape.
extern "C" void __cdecl free(void* block);

struct SNode {
    long m_color;                          // +0
    SNode* m_parent;                       // +4
    SNode* m_left;                         // +8
    SNode* m_right;                        // +0xC
};

extern "C" SNode* __cdecl Unlink_0042a34e(SNode* node, SNode** root, SNode** leftmost, SNode** rightmost);   // 0x0042A34E

struct CRetailTree_006f1cf0 {
    SNode* m_header;                       // +0
    unsigned long m_size;                  // +4
    void Erase(SNode* root);               // 0x006EFCE0
    static SNode* Successor(SNode* node)
    {
        if (node->m_right != 0) {
            node = node->m_right;
            while (node->m_left != 0)
                node = node->m_left;
        }
        else {
            SNode* y = node->m_parent;
            while (node == y->m_right) {
                node = y;
                y = y->m_parent;
            }
            if (node->m_right != y)
                node = y;
        }
        return node;
    }
    void EraseRange_CRetailTree_006f1cf0(SNode* first, SNode* last);
};

void CRetailTree_006f1cf0::EraseRange_CRetailTree_006f1cf0(SNode* first, SNode* last)
{
    if (first == m_header->m_left && last == m_header) {
        if (m_size != 0) {
            Erase(m_header->m_parent);
            m_header->m_left = m_header;
            m_header->m_parent = 0;
            m_header->m_right = m_header;
            m_size = 0;
        }
    }
    else {
        while (first != last) {
            SNode* victim = first;
            first = Successor(first);
            SNode* node = Unlink_0042a34e(victim, &m_header->m_parent, &m_header->m_left, &m_header->m_right);
            if (node)
                free(node);
            --m_size;
        }
    }
}