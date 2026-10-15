// Grade: DIFFER(122v122) -- register allocation of the size (eax vs ecx) and of the copy loop: retail strength-reduces both arrays (dest pointer in eax, the source as `key - dest` in edx, index in ecx, `[edx+eax]`), the lane keeps `[edx+eax*4]` with the index in eax; pointer-walk and header+1 forms tried
// CEngineTreeFilerBase::AddNewKey(const unsigned long*) -- allocate a leaf node with room for the filer's key, copy the key words into it, insert it into the tree (re-rooting when the insert returns a new root) and return the node
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BF3030, length 122 bytes.
// Callees: operator new[] @ 0x00BFEB22 (0x1c-byte header + 4 bytes per key word), AddNodeAutoExpandTree(root, node) @ 0x00BF2E50.
// Node layout as the landed sibling CEngineTreeFilerBase::DeleteTree @ 0x00BF29E0 (CNodeHeader, key words from +0x1c);
// the filer holds the root at +0x00 and the key length in words at +0x04 (no engine header for this class).
// Shape: the node header is cleared field by field in retail's store order (leaf flag = 1 first, then the three flag
// bytes high to low, then the dwords); the key length is re-read from `this` on every copy iteration (the stores
// through the node may alias it), the copy is a signed `for` over word indices.
void* __cdecl operator new[](unsigned int size);   // 0x00BFEB22

struct CNodeHeader {
    unsigned char m_isLeaf;                        // +0x00
    unsigned char m_flag1;                         // +0x01
    unsigned char m_flag2;                         // +0x02
    unsigned char m_flag3;                         // +0x03
    long          m_count;                         // +0x04
    CNodeHeader*  m_pLink;                         // +0x08
    CNodeHeader*  m_pNext;                         // +0x0c
    CNodeHeader*  m_pFirstChild;                   // +0x10
    long          m_childCount;                    // +0x14
    CNodeHeader*  m_pParent;                       // +0x18
    unsigned long m_key[1];                        // +0x1c (m_keyWords words)
};

struct CEngineTreeFilerBase {
    CNodeHeader* m_pRoot;                          // +0x00
    long         m_keyWords;                       // +0x04
    CNodeHeader* AddNodeAutoExpandTree(CNodeHeader* root, CNodeHeader* node);   // 0x00BF2E50
    CNodeHeader* AddNewKey(const unsigned long* key);
};

CNodeHeader* CEngineTreeFilerBase::AddNewKey(const unsigned long* key)
{
    CNodeHeader* node = (CNodeHeader*)operator new[](m_keyWords * 4 + 0x1c);
    node->m_isLeaf = 1;
    node->m_flag3 = 0;
    node->m_flag2 = 0;
    node->m_flag1 = 0;
    node->m_pFirstChild = 0;
    node->m_childCount = 0;
    node->m_pLink = 0;
    node->m_pNext = 0;
    node->m_pParent = 0;
    node->m_count = 0;
    for (long i = 0; i < m_keyWords; ++i)
        node->m_key[i] = key[i];
    CNodeHeader* root = AddNodeAutoExpandTree(m_pRoot, node);
    if (root)
        m_pRoot = root;
    return node;
}
