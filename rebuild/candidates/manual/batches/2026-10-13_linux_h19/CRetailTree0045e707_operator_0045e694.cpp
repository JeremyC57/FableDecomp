// CRetailTree_0045e707::operator=(const CRetailTree_0045e707&) -- Clear @ 0x004511A6, Copy @ 0x0045E707 of the source root under the head, leftmost/rightmost walks, size copied; self-assignment guarded
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0045E694, oracle length 115 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push ebx ; mov ebx,[esp+8] ; push esi ; mov esi,ecx ; cmp esi,ebx ; je R ; call Clear ; and dword ptr [esi+4],0 ; mov eax,[ebx] ; mov eax,[eax+4] ; test eax,eax ; jne C ; mov eax,[esi] ; and dword ptr [eax+4],0 ; mov eax,[esi] ; mov [eax+8],eax ; mov eax,[esi] ; mov [eax+0Ch],eax ; jmp R ; C: push edi ; mov edi,[esi] ; push edi ; push eax ; mov ecx,esi ; call Copy ; mov [edi+4],eax ; mov ecx,[esi] ; mov eax,[ecx+4] ; pop edi ; jmp T1 ; L1: mov eax,edx ; T1: mov edx,[eax+8] ; test edx,edx ; jne L1 ; mov [ecx+8],eax ; mov ecx,[esi] ; mov eax,[ecx+4] ; jmp T2 ; L2: mov eax,edx ; T2: mov edx,[eax+0Ch] ; test edx,edx ; jne L2 ; mov [ecx+0Ch],eax ; mov eax,[ebx+4] ; mov [esi+4],eax ; R: mov eax,esi ; pop esi ; pop ebx ; ret 4
// VC6-era Dinkumware `_Tree::operator=`: unless self-assigned, erase everything (Clear @
// 0x004511A6), zero the count, and either reset the head to an empty tree (null root, head
// linked to itself; the head pointer is re-read from `this` for each store) or clone the source's
// root under the head (Copy @ 0x0045E707), re-derive the leftmost/rightmost links by walking
// the new root, and copy the count. Size TU (`and dword ptr [..],0`). The manifest label
// (`CTCCarryable::operator=`) is propagated. One of 34 bodies of this shape.
#pragma optimize("s", on)
struct CRetailTree_0045e707 {
    struct node {
        unsigned char color;               // +0
        unsigned char _pad[3];
        node* parent;                      // +4
        node* left;                        // +8
        node* right;                       // +0xC
    };
    node* head;                            // +0
    unsigned long size;                    // +4
    void Clear();                          // 0x004511A6: erase(begin(), end())
    node* Copy(node* src, node* parent);   // 0x0045E707
    CRetailTree_0045e707& operator=(const CRetailTree_0045e707& other);
};

CRetailTree_0045e707& CRetailTree_0045e707::operator=(const CRetailTree_0045e707& other)
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
