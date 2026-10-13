// CRetailTree_longkey_ins0046fbc9::insert(const value_type&) -- unique-key insert: descend on `key < node.key`, step back unless at begin(), then _Insert @ 0x0046FBC9
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0046FB43, oracle length 134 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push ebp ; mov ebp,esp ; push ebx ; mov ebx,ecx ; mov ecx,[ebx] ; push esi ; mov esi,[ecx+4] ; test esi,esi ; push edi ; mov edi,ecx ; mov dl,1 ; je D ; L: mov eax,[ebp+0Ch] ; mov eax,[eax] ; cmp eax,[esi+10h] ; mov edi,esi ; setl dl ; test dl,dl ; je R ; mov esi,[esi+8] ; jmp T ; R: mov esi,[esi+0Ch] ; T: test esi,esi ; jne L ; D: test dl,dl ; mov eax,edi ; je C ; cmp edi,[ecx+8] ; jne P ; push 0 ; push [ebp+0Ch] ; push edi ; push edi ; jmp I ; P: push edi ; call _Max ; pop ecx ; C: mov edx,[eax+10h] ; mov ecx,[ebp+0Ch] ; cmp edx,[ecx] ; jge F ; push 0 ; push ecx ; push edi ; push esi ; I: lea eax,[ebp+0Ch] ; push eax ; mov ecx,ebx ; call _Insert ; mov ecx,[eax] ; mov eax,[ebp+8] ; mov [eax],ecx ; mov byte ptr [eax+4],1 ; jmp E ; F: mov ecx,[ebp+8] ; mov [ecx],eax ; mov byte ptr [ecx+4],0 ; mov eax,ecx ; E: pop edi ; pop esi ; pop ebx ; pop ebp ; ret 8
// The retail (VC6-era Dinkumware) `_Tree::insert` for a signed 32-bit key at +0x10: walk down
// from head->parent remembering the parent and whether the key compares less; at the leaf, when
// the last step went left and the parent is begin() insert immediately (`_Insert` @ 0x0046FBC9
// called with the parent twice: a non-null first argument means "link as the left child"),
// otherwise step back to the predecessor (the out-of-line `--iterator` helper @ 0x004294E2) and insert only if that node's key is less than the new key;
// the pair<iterator,bool> result goes through the hidden pointer. The compiler re-uses the dead
// value-reference argument slot for `_Insert`'s hidden iterator result. Frame pointer + merged
// call site: this TU is `optimize("y",off)` + `("s",on)`. One of 40 bodies.
#pragma optimize("y", off)
#pragma optimize("s", on)

struct CRetailTree_longkey_ins0046fbc9 {
    struct node { unsigned char flag; unsigned char _pad[3]; node* parent; node* left; node* right; long key; };
    struct iterator { node* current; iterator(node* n) : current(n) { } };
    struct pairib { iterator first; bool second; pairib(iterator f, bool s) : first(f), second(s) { } };
    node* head;
    unsigned long size;
    static node* Prev(node* n);            // 0x004294E2: predecessor of a node (the `--iterator` step, out of line)
    iterator Insert(node* trynode, node* wherenode, const long& value, node* newnode);   // 0x0046FBC9
    pairib insert(const long& value);
};

CRetailTree_longkey_ins0046fbc9::pairib CRetailTree_longkey_ins0046fbc9::insert(const long& value)
{
    node* trynode = head->parent;
    node* wherenode = head;
    bool addleft = true;
    while (trynode) {
        wherenode = trynode;
        addleft = value < trynode->key;
        trynode = addleft ? trynode->left : trynode->right;
    }
    iterator where(wherenode);
    if (!addleft)
        ;
    else if (where.current == head->left)
        return pairib(Insert(wherenode, wherenode, value, 0), true);
    else
        where.current = Prev(where.current);
    if (where.current->key < value)
        return pairib(Insert(trynode, wherenode, value, 0), true);
    return pairib(where, false);
}