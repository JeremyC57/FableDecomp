// CRetailTreeIterator::Increment() -- red-black tree iterator operator++: successor node
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009EE470, oracle length 62 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov eax,ecx ; mov edx,[eax] ; mov ecx,[edx+0Ch] ; test ecx,ecx ; je P ; (descend left from the right child) ; mov [eax],ecx ; ret ; P: push esi ; mov esi,[edx+4] ; (climb while node is its parent's right) ; cmp [edx+0Ch],esi ; je K ; mov [eax],esi ; pop esi ; ret ; K: mov [eax],edx ; pop esi ; ret
// Dinkumware-shaped `_Inc` on the retail node {flag, parent, left, right, value}: a node with a
// right child goes to the leftmost node of that subtree; otherwise it climbs while it is its
// parent's right child and steps to that parent unless it already is the node's right child (the
// end() case). Returns `*this`. Byte-identical per instantiation; this is the copy at this
// address. One of 48 bodies of this shape.
struct CRetailTreeIterator {
    struct node { unsigned char flag; unsigned char _pad[3]; node* parent; node* left; node* right; };
    node* current;
    CRetailTreeIterator& Increment();
};

CRetailTreeIterator& CRetailTreeIterator::Increment()
{
    node* p = current;
    if (p->right) {                       // descend: leftmost node of the right subtree (one variable, no helper)
        p = p->right;
        while (p->left)
            p = p->left;
    }
    else {
        node* q;
        while (p == (q = p->parent)->right)
            p = q;
        if (p->right != q)
            p = q;
    }
    current = p;
    return *this;
}