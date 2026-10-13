// CRetailTree_00462411::Copy(node*, node*) -- clone a subtree: Buynode @ 0x00462411 per node, right subtrees by recursion, the left spine iteratively with `parent` re-used as the running predecessor
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00462381, oracle length 144 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push ebp ; mov ebp,esp ; push ecx ; push ebx ; push esi ; mov esi,[ebp+8] ; push edi ; lea eax,[esi+10h] ; push eax ; mov [ebp-4],ecx ; call Buynode ; mov ebx,eax ; mov al,[esi] ; and dword ptr [ebx+8],0 ; and dword ptr [ebx+0Ch],0 ; mov [ebx],al ; mov eax,[ebp+0Ch] ; mov [ebx+4],eax ; mov eax,[esi+0Ch] ; test eax,eax ; je A ; mov ecx,[ebp-4] ; push ebx ; push eax ; call Copy ; mov [ebx+0Ch],eax ; A: mov edi,[esi+8] ; mov [ebp+0Ch],ebx ; jmp T ; L: mov ecx,[ebp-4] ; lea eax,[edi+10h] ; push eax ; call Buynode ; mov esi,eax ; mov al,[edi] ; and dword ptr [esi+8],0 ; and dword ptr [esi+0Ch],0 ; mov [esi],al ; mov eax,[ebp+0Ch] ; mov [eax+8],esi ; mov [esi+4],eax ; mov eax,[edi+0Ch] ; test eax,eax ; je B ; mov ecx,[ebp-4] ; push esi ; push eax ; call Copy ; mov [esi+0Ch],eax ; B: mov edi,[edi+8] ; mov [ebp+0Ch],esi ; T: test edi,edi ; jne L ; pop edi ; pop esi ; mov eax,ebx ; pop ebx ; leave ; ret 8
// VC6-era Dinkumware `_Tree::_Copy(_X, _P)`: the first node is bought from the source value at
// +0x10 (Buynode @ 0x00462411, an out-of-line member taking the value by reference), its links
// zeroed, colour copied and parent set; its right subtree is cloned recursively; then the left
// spine is walked iteratively, each new node hung under the previous one through the `parent`
// parameter (retail keeps the running predecessor in the parameter's own slot) with its right
// subtree cloned recursively. `this` is only needed for the calls (spilled to [ebp-4]). The TU
// keeps frame pointers and optimises for size (`and dword ptr [..],0` stores). The manifest label
// (`CTCCarryable_CloneTree`) is propagated. One of 34 bodies of this shape.
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CRetailTree_00462411 {
    struct node {
        unsigned char color;               // +0
        unsigned char _pad[3];
        node* parent;                      // +4
        node* left;                        // +8
        node* right;                       // +0xC
        unsigned char value[4];            // +0x10 (size not recoverable from this body)
    };
    node* head;
    unsigned long size;
    node* Buynode(const unsigned char (&value)[4]);   // 0x00462411
    node* Copy(node* src, node* parent);
};

CRetailTree_00462411::node* CRetailTree_00462411::Copy(node* src, node* parent)
{
    node* first = Buynode(src->value);
    first->left = 0;
    first->right = 0;
    first->color = src->color;
    first->parent = parent;
    if (src->right)
        first->right = Copy(src->right, first);
    parent = first;
    for (node* s = src->left; s; s = s->left) {
        node* n = Buynode(s->value);
        n->left = 0;
        n->right = 0;
        n->color = s->color;
        parent->left = n;
        n->parent = parent;
        if (s->right)
            n->right = Copy(s->right, n);
        parent = n;
    }
    return first;
}