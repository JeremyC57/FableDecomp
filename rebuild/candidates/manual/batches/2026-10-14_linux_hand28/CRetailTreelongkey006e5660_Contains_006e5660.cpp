// CRetailTree_longkey_006e5660::Contains(long) -- true when the key is in the tree (find() != end())
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E5660, length 48 bytes.
// Callees: CRetailTree_longkey::find(const long&) @ 0x006E69E0 (iterator by value through a hidden pointer, key by reference).
// Shape: the one-word frame holds the returned iterator; the comparison against end() reads the head node at +0 of the
// tree, which is the object itself here (`mov esi,ecx`); the key is taken by value (its argument slot's address is what find() receives), the iterator is a named local re-read from its frame slot, the head node is read after the call, and the long result is `xor eax,eax ; cmp ; setne al` (an inline operator!= gives `movzx`). The manifest
// label "CDisplayManager::CopyBackBufferToTexture" is propagated across the three Contains bodies (0x007394C0 at
// +0x20, 0x0079DD80 at +0x48); the class is named by shape and address.
struct CRetailTree_longkey_006e5660 {
    struct node { unsigned char flag; unsigned char _pad[3]; node* parent; node* left; node* right; long key; };
    struct iterator {
        node* current;
        iterator(node* n) : current(n) { }
    };
    node* head;
    unsigned long size;
    iterator find(const long& key);                // 0x006E69E0
    long Contains(long key);
};

long CRetailTree_longkey_006e5660::Contains(long key)
{
    iterator it = find(key);
    return it.current != head;
}
