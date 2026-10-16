// CRetailTree_00461533::CRetailTree_00461533(const CRetailTree_00461533& other) -- tree copy constructor: allocates the head node (0x18 bytes, red, count 0); an empty source leaves the head linked to itself, otherwise the source's root is cloned under the head and the leftmost/rightmost links re-derived by walking the copy; the count is copied last
// Leaf: ??0CRetailTree_00461533
// Grade: DIFFER(122v126) -- zero-register choice: retail (`pop ecx` after malloc, so a size TU) holds 0 in ebx (`xor ebx,ebx`, `mov [m],ebx`, `cmp [ecx+4],ebx`, `cmp edx,ebx`), our size TU emits `and dword ptr [m],0` / `test` instead (tried: speed TU (ebx zero but immediates and a split tail), size TU, a `node* nil = 0` local); next: lane gap: size-TU zero register (possibly the QFE-4035 toolset)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004F97B3, length 126 bytes, `ret 4`; retail keeps the zero in ebx for every null store and walk compare
// and drops the malloc argument with `pop ecx`.
// Catalog label "CUIBinaryTree::CopyConstructor" is descriptive only: this is the retail map/_Tree copy ctor of the
// landed CRetailTree_00461533 family (operator= 0x004614C0 uses the same Copy @ 0x00461533); its caller 0x004F9784
// (catalog "CUIMiniMapMarker", whose PDB layout does not fit) copies a word, then this tree at +4, then two words.
// Callees: malloc @ 0x00BFEA0E (the head node), CRetailTree_00461533::Copy(node*, node*) @ 0x00461533.
#pragma optimize("s", on)
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
struct CRetailTree_00461533 {
    struct node {
        unsigned char color;                       // +0
        unsigned char _pad[3];
        node* parent;                              // +4
        node* left;                                // +8
        node* right;                               // +0xC
        unsigned long value[2];                    // +0x10 (8-byte value)
    };
    node* head;                                    // +0
    unsigned long size;                            // +4
    node* Copy(node* src, node* parent);           // 0x00461533
    CRetailTree_00461533(const CRetailTree_00461533& other);
};
CRetailTree_00461533::CRetailTree_00461533(const CRetailTree_00461533& other)
    : head(0)
{
    head = (node*)malloc(sizeof(node));
    size = 0;
    head->color = 0;
    if (other.head->parent == 0)
    {
        head->parent = 0;
        head->left = head;
        head->right = head;
    }
    else
    {
        node* h = head;
        h->parent = Copy(other.head->parent, h);
        node* n = head->parent;
        while (n->left)
            n = n->left;
        head->left = n;
        n = head->parent;
        while (n->right)
            n = n->right;
        head->right = n;
    }
    size = other.size;
}
