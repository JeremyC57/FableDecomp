// CRetailTree_ulongkey::find(const unsigned long&) -- lower-bound walk, then end() unless the key matches
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00883860, oracle length 71 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov ecx,[ecx] ; mov eax,[ecx+4] ; test eax,eax ; push esi ; push edi ; mov edi,[esp+10h] ; mov edx,ecx ; je E ; mov esi,[edi] ; L: cmp [eax+10h],esi ; jb R ; mov edx,eax ; mov eax,[eax+8] ; jmp T ; R: mov eax,[eax+0Ch] ; T: test eax,eax ; jne L ; E: cmp edx,ecx ; je H ; mov eax,[edi] ; cmp eax,[edx+10h] ; jae F ; H: (result = head) ; ret 8 ; F: (result = node) ; ret 8
// std::map/set::find on the retail tree with an unsigned 32-bit key at +0x10: lower_bound descends
// from head->parent (right when node->key < key, else remember the node and go left), then the
// result is end() (the head) when nothing was found or key < found->key. The iterator is returned
// through the hidden pointer (`ret 8`: result pointer + key reference). One of 14 bodies.
struct CRetailTree_ulongkey {
    struct node { unsigned char flag; unsigned char _pad[3]; node* parent; node* left; node* right; unsigned long key; };
    struct iterator { node* current; iterator(node* n) : current(n) { } };
    node* head;
    unsigned long size;
    iterator find(const unsigned long& key);
};

CRetailTree_ulongkey::iterator CRetailTree_ulongkey::find(const unsigned long& key)
{
    node* result = head;
    node* n = head->parent;
    while (n) {
        if (!(n->key < key)) {            // Dinkumware _Lbound: `if (!comp(key(node), k))`
            result = n;
            n = n->left;
        }
        else
            n = n->right;
    }
    node* found = result;
    if (found == head || key < found->key)
        found = head;                      // end(): a single return through one pointer keeps retail's block order
    return iterator(found);
}
