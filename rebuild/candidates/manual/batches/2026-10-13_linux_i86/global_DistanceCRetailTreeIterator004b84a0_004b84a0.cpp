// distance<CRetailTreeIterator>(first, last) -- counts increments from first to last with the red-black tree successor walk inlined
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004B84A0, oracle length 78 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov ecx,[first] ; mov edx,[last] ; mov ecx,[ecx] ; push esi ; mov esi,[edx] ; xor eax,eax ; cmp ecx,esi ; je E ; L: (successor of ecx: descend left from the right child, else climb while the node is its parent's right) ; inc eax ; cmp ecx,esi ; jne L ; E: pop esi ; ret
// Iterator distance over the retail red-black tree: `n = 0; for (i = first; i != last; ++i) ++n;`
// with the Dinkumware-shaped `_Inc` inlined (a node with a right child goes to the leftmost
// node of that subtree; otherwise it climbs while it is its parent's right child and steps to
// that parent unless it already is the node's right child). Both iterators arrive by const
// reference (`__cdecl`, two stack arguments). The manifest label (`BinaryTree_Height`) is
// propagated. One of 10 bodies of this shape.
struct CRetailTreeIterator {
    struct node { unsigned char flag; unsigned char _pad[3]; node* parent; node* left; node* right; };
    node* current;
    CRetailTreeIterator(node* n) : current(n) { }
    CRetailTreeIterator& operator++()
    {
        node* p = current;
        if (p->right) {
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
    bool operator!=(const CRetailTreeIterator& other) const { return current != other.current; }
};

long __cdecl Distance_CRetailTreeIterator_004b84a0(const CRetailTreeIterator& first, const CRetailTreeIterator& last)
{
    long n = 0;
    CRetailTreeIterator i = first;
    while (i != last) {                  // SGI __distance: advance, then count
        ++i;
        ++n;
    }
    return n;
}
