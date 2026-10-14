// CTreeOwner_0079dd80::Contains(long) -- true when the key is in the tree member at +0x48 (find() != end())
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0079DD80, length 48 bytes.
// Callees: CRetailTree_ulongkey::find(const long&) @ 0x0079F210 (iterator by value through a hidden pointer, key by reference).
// Shape: the tree sub-object is addressed once (`lea esi,[ecx+48h]`) and used for the find call and the end() read of its
// head node; the one-word frame holds the returned iterator; the key is taken by value (its argument slot's address is what find() receives), the iterator is a named local re-read from its frame slot, the head node is read after the call, and the long result is `xor eax,eax ; cmp ; setne al` (an inline operator!= gives `movzx`). The
// manifest label "CDisplayManager::CopyBackBufferToTexture" is propagated across the three Contains bodies
// (0x006E5660 at +0, 0x007394C0 at +0x20); the class is named by shape and address.
struct CRetailTree_longkey {
    struct node { unsigned char flag; unsigned char _pad[3]; node* parent; node* left; node* right; long key; };
    struct iterator {
        node* current;
        iterator(node* n) : current(n) { }
    };
    node* head;
    unsigned long size;
    iterator find(const long& key);                // 0x0079F210
};

struct CTreeOwner_0079dd80 {
    unsigned char       _pad[0x48];    // +0x00
    CRetailTree_longkey m_tree;        // +0x48
    long Contains(long key);
};

long CTreeOwner_0079dd80::Contains(long key)
{
    CRetailTree_longkey::iterator it = m_tree.find(key);
    return it.current != m_tree.head;
}