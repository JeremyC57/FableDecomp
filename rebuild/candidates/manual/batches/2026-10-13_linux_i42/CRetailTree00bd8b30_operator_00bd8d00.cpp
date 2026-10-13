// CRetailTree_00bd8b30::operator=(const CRetailTree_00bd8b30&) -- inlined clear (Erase @ 0x00BD8A10, head reset, size zeroed), Copy @ 0x00BD8B30 under the head, leftmost/rightmost walks, size copied
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BD8D00, oracle length 178 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push ebp ; mov ebp,[other] ; push esi ; mov esi,ecx ; cmp esi,ebp ; je R ; mov eax,[esi+4] ; push ebx ; xor ebx,ebx ; cmp eax,ebx ; je S ; mov eax,[esi] ; mov ecx,[eax+4] ; push ecx ; mov ecx,esi ; call Erase ; (head re-read) mov [eax+8],eax ; mov [edx+4],ebx ; mov [eax+0Ch],eax ; mov [esi+4],ebx ; S: mov [esi+4],ebx ; mov eax,[ebp] ; mov eax,[eax+4] ; cmp eax,ebx ; jne C ; (empty source: head->parent = 0, head->left = head, head->right = head) ; ret 4 ; C: push edi ; mov edi,[esi] ; push edi ; push eax ; mov ecx,esi ; call Copy ; mov [edi+4],eax ; (leftmost walk) ; (rightmost walk) ; mov edx,[ebp+4] ; mov [esi+4],edx ; pop ebx ; R: mov eax,esi ; pop esi ; pop ebp ; ret 4
// VC6-era Dinkumware `_Tree::operator=` with `erase(begin(), end())` inlined: unless
// self-assigned, a non-empty tree is erased through the recursive Erase @ 0x00BD8A10 and its
// head re-linked to itself (the head pointer re-read per store), the count is zeroed, and
// either the head is reset for an empty source or the source's root is cloned under the head
// (Copy @ 0x00BD8B30, with a named head local across the call), the leftmost/rightmost links
// re-derived and the count copied. The manifest label is propagated. One of 14 bodies.
struct CRetailTree_00bd8b30 {
    struct node {
        unsigned char color;               // +0
        unsigned char _pad[3];
        node* parent;                      // +4
        node* left;                        // +8
        node* right;                       // +0xC
    };
    node* head;                            // +0
    unsigned long size;                    // +4
    void Erase(node* root);                // 0x00BD8A10
    node* Copy(node* src, node* parent);   // 0x00BD8B30
    void Clear()
    {
        if (size) {
            Erase(head->parent);
            head->left = head;
            head->parent = 0;
            head->right = head;
            size = 0;
        }
    }
    CRetailTree_00bd8b30& operator=(const CRetailTree_00bd8b30& other);
};

CRetailTree_00bd8b30& CRetailTree_00bd8b30::operator=(const CRetailTree_00bd8b30& other)
{
    if (this != &other) {
        Clear();
        size = 0;
        if (other.head->parent == 0) {
            head->parent = 0;
            head->left = head;
            head->right = head;
        }
        else {
            node* h = head;                // named: retail keeps the head in edi across the Copy call
            h->parent = Copy(other.head->parent, h);
            node* n = head->parent;
            while (n->left)
                n = n->left;
            head->left = n;
            n = head->parent;
            while (n->right)
                n = n->right;
            head->right = n;
            size = other.size;
        }
    }
    return *this;
}
