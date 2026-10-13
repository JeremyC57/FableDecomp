// CRetailTreeIterator::Decrement() -- red-black tree iterator operator--: predecessor node
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A2A550, oracle length 79 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov eax,ecx ; push edi ; mov edi,[eax] ; cmp byte ptr [edi],0 ; jne L ; mov ecx,[edi+4] ; cmp [ecx+4],edi ; jne L ; mov edi,[edi+0Ch] ; mov [eax],edi ; pop edi ; ret ; L: push esi ; mov esi,[edi+8] ; test esi,esi ; je P ; (descend right from the left child) ; mov [eax],esi ; pop esi ; pop edi ; ret ; P: (climb while node is its parent's left) ; pop esi ; mov [eax],edx ; pop edi ; ret
// Dinkumware-shaped `_Dec` on the retail node {flag, parent, left, right, value}: the head node
// (flag 0 and parent->parent == node) steps to its right child (the maximum); a node with a left
// child goes to the rightmost node of that subtree; otherwise it climbs while it is its parent's
// left child. Returns `*this`. Every instantiation emits a byte-identical copy; this file is the
// copy at this address. One of 37 bodies of this shape.
struct CRetailTreeIterator {
    struct node { unsigned char flag; unsigned char _pad[3]; node* parent; node* left; node* right; };
    node* current;
    CRetailTreeIterator& Decrement();
};

CRetailTreeIterator& CRetailTreeIterator::Decrement()
{
    node* p = current;
    if (p->flag == 0 && p->parent->parent == p)
        p = p->right;
    else if (p->left) {
        node* q = p->left;
        while (q->right)
            q = q->right;
        p = q;
    }
    else {
        node* q = p->parent;
        while (p == q->left) {
            p = q;
            q = q->parent;
        }
        p = q;
    }
    current = p;
    return *this;
}
