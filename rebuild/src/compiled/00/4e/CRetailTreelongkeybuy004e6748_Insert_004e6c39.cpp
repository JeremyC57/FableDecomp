// CRetailTree_longkey_buy004e6748::_Insert(trynode, wherenode, const value_type&, forceRight) -- link a new node (Buynode @ 0x004E6748) under wherenode, rebalance @ 0x0042971D, bump the count
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E6C39, oracle length 136 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push ebp ; mov ebp,esp ; mov eax,[ebp+14h] ; push ebx ; mov ebx,[ebp+10h] ; push esi ; push edi ; mov edi,ecx ; cmp ebx,[edi] ; je L ; cmp dword ptr [ebp+18h],0 ; jne R ; cmp dword ptr [ebp+0Ch],0 ; jne L ; mov ecx,[eax] ; cmp ecx,[ebx+10h] ; jl L ; R: push eax ; mov ecx,edi ; call Buynode ; mov esi,eax ; mov [ebx+0Ch],esi ; mov eax,[edi] ; cmp ebx,[eax+0Ch] ; jne K ; mov [eax+0Ch],esi ; jmp K ; L: push eax ; mov ecx,edi ; call Buynode ; mov esi,eax ; mov [ebx+8],esi ; mov eax,[edi] ; cmp ebx,eax ; jne M ; mov [eax+4],esi ; mov eax,[edi] ; jmp (head->right = Z) ; M: cmp ebx,[eax+8] ; jne K ; mov [eax+8],esi ; K: and dword ptr [esi+8],0 ; and dword ptr [esi+0Ch],0 ; mov [esi+4],ebx ; mov eax,[edi] ; add eax,4 ; push eax ; push esi ; call Rebalance ; inc dword ptr [edi+4] ; mov eax,[ebp+8] ; pop ecx ; pop ecx ; pop edi ; mov [eax],esi ; pop esi ; pop ebx ; pop ebp ; ret 14h
// The retail (VC6-era Dinkumware) `_Tree::_Insert`: the new node goes LEFT of wherenode when
// wherenode is the head, or (unless the fourth argument forces the right side) when a try-node was
// supplied or the key is less than wherenode's; the head's parent/left/right links are fixed up
// for a new root, leftmost or rightmost node; the node is detached, parented, rebalanced by the
// __cdecl red-black fix-up @ 0x0042971D (node, &root) and counted. Returned through the hidden
// iterator pointer (`ret 14h`: hidden + four arguments). Frame pointer, `and [..],0` and shared
// call blocks: this TU is `optimize("y",off)` + `("s",on)`. One of 40 bodies.
#pragma optimize("y", off)
#pragma optimize("s", on)

struct CRetailTree_longkey_buy004e6748 {
    struct node { unsigned char flag; unsigned char _pad[3]; node* parent; node* left; node* right; long key; };
    struct iterator { node* current; iterator(node* n) : current(n) { } };
    node* head;
    unsigned long size;
    node* Buynode(const long& value);      // 0x004E6748
    iterator Insert(node* trynode, node* wherenode, const long& value, long forceRight);
};

void __cdecl Rebalance_0042971d(CRetailTree_longkey_buy004e6748::node* newnode, CRetailTree_longkey_buy004e6748::node** root);   // 0x0042971D

CRetailTree_longkey_buy004e6748::iterator CRetailTree_longkey_buy004e6748::Insert(node* trynode, node* wherenode, const long& value, long forceRight)
{
    node* newnode;
    if (wherenode == head || (!forceRight && (trynode != 0 || value < wherenode->key))) {
        newnode = Buynode(value);
        wherenode->left = newnode;
        if (wherenode == head) {
            head->parent = newnode;
            head->right = newnode;
        }
        else if (wherenode == head->left)
            head->left = newnode;
    }
    else {
        newnode = Buynode(value);
        wherenode->right = newnode;
        if (wherenode == head->right)
            head->right = newnode;
    }
    newnode->left = 0;
    newnode->right = 0;
    newnode->parent = wherenode;
    Rebalance_0042971d(newnode, &head->parent);
    ++size;
    return iterator(newnode);
}