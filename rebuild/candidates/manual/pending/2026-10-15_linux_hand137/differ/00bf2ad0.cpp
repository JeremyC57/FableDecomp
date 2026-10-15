// Grade: DIFFER(115v115) -- esi/edi swap only: retail keeps the list tail in esi and the climbing parent in edi; the lane assigns them the other way round under every declaration order tried
// CEngineTreeFilerBase::ConvertTreeToList(CNodeHeader*) -- flatten a key tree into a singly linked list of its leaves (in depth-first order), freeing every interior node on the way; a leaf root is returned as a one-element list
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BF2AD0, length 115 bytes (+ int3 padding).
// Callees: operator delete @ 0x00BFE9BC (interior nodes are PODs: no destructor call).
// Node layout as the landed sibling CEngineTreeFilerBase::DeleteTree @ 0x00BF29E0: leaf flag byte +0x00, +0x08 link
// word, next sibling / next list entry +0x0c, first child +0x10, parent +0x18.
// Shape: the walk cursor is the `root` parameter itself (kept in eax); a `__thiscall` member that never reads `this` (`ret 4`); the head/tail locals are zeroed (ebp/esi) before the
// leaf test; the walk is iterative: pop the first child, append a leaf (the tail store, then the +0x08 word, which retail
// sets to the leaf itself when the list was non-empty and to null for the first leaf), descend into an interior child,
// or climb to the parent and free the exhausted node; the shared `last = child ; next = 0 ; parent = 0` tail is
// duplicated into both append arms by VC7.1.
void __cdecl operator delete(void* block);         // 0x00BFE9BC

struct CNodeHeader {
    unsigned char m_isLeaf;                        // +0x00
    unsigned char _pad01[7];
    CNodeHeader*  m_pLink;                         // +0x08
    CNodeHeader*  m_pNext;                         // +0x0c
    CNodeHeader*  m_pFirstChild;                   // +0x10
    unsigned char _pad14[4];
    CNodeHeader*  m_pParent;                       // +0x18
};

struct CEngineTreeFilerBase {
    CNodeHeader* ConvertTreeToList(CNodeHeader* root);
};

CNodeHeader* CEngineTreeFilerBase::ConvertTreeToList(CNodeHeader* root)
{
    CNodeHeader* parent;
    CNodeHeader* first = 0;
    CNodeHeader* last = 0;
    if (root->m_isLeaf) {
        root->m_pNext = 0;
        return root;
    }
    root->m_pParent = 0;
    do {
        CNodeHeader* child = root->m_pFirstChild;
        if (child) {
            root->m_pFirstChild = child->m_pNext;
            if (child->m_isLeaf) {
                if (last) {
                    last->m_pNext = child;
                    child->m_pLink = last->m_pNext;
                } else {
                    first = child;
                    child->m_pLink = 0;
                }
                last = child;
                child->m_pNext = 0;
                child->m_pParent = 0;
            } else {
                root = child;
            }
        } else {
            parent = root->m_pParent;
            delete root;
            root = parent;
        }
    } while (root);
    return first;
}
