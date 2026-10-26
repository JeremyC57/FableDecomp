// CSkeletalMorphResourceManager::UnloadBoneConfigs() -- clears the bone-config tree (STLport _Rb_tree::clear inlined: erase from the root, header links reset, count zeroed) and resets the three-word record at +0x38 to a default-constructed one
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B93DF0, length 73 bytes.
// Leaf: UnloadBoneConfigs
// Callees: _Rb_tree<...>::_M_erase(_Rb_tree_node*) @ 0x00B95EE0 (`__fastcall` on the tree, the root pushed).
// Layout: the PDB (Ego_r) CSkeletalMorphResourceManager is 20 bytes (a CachedMorphs map at +4) and does not match the
// retail body; members here are at the retail offsets: an STLport map {header node, node count, comparator} at +0x2c
// (node base: colour +0, parent +4, left +8, right +0xc) and a three-word record at +0x38.
// Shape: `if (_M_node_count != 0)` with the tree's address in esi, the header re-read for every link store
// (leftmost = header, root = 0, rightmost = header), then the record assigned from a default-constructed temporary:
// its three members are separate zero values (`xor eax,eax` / `xor ecx,ecx` / `xor edx,edx`) stored through the
// member's address (`add edi,38h`).
struct SRbTreeNodeBase {                        // STLport _Rb_tree_node_base
    long             m_color;                   // +0x00
    SRbTreeNodeBase* m_parent;                  // +0x04
    SRbTreeNodeBase* m_left;                    // +0x08
    SRbTreeNodeBase* m_right;                   // +0x0c
};

struct CBoneConfigTree {                        // STLport _Rb_tree (map<CCharString, ...>), 12 bytes
    SRbTreeNodeBase* m_header;                  // +0x00 (_M_header._M_data)
    unsigned long    m_nodeCount;               // +0x04
    unsigned char    m_compare;                 // +0x08 (empty comparator)
    void Erase(SRbTreeNodeBase* node);          // 0x00B95EE0 (_M_erase)
    SRbTreeNodeBase*& Root() { return m_header->m_parent; }
    SRbTreeNodeBase*& Leftmost() { return m_header->m_left; }
    SRbTreeNodeBase*& Rightmost() { return m_header->m_right; }
    void clear()
    {
        if (m_nodeCount != 0) {
            Erase(Root());
            Leftmost() = m_header;
            Root() = 0;
            Rightmost() = m_header;
            m_nodeCount = 0;
        }
    }
};

struct SBoneConfigRecord {                      // three words, all cleared by the default constructor
    long m_first;
    long m_second;
    long m_third;
    SBoneConfigRecord() : m_first(0), m_second(0), m_third(0) { }
};

class CSkeletalMorphResourceManager { public:
    unsigned char     _pad00[0x2c];
    CBoneConfigTree   m_boneConfigs;            // +0x2c
    SBoneConfigRecord m_boneConfigRecord;       // +0x38
    void UnloadBoneConfigs();
};

void CSkeletalMorphResourceManager::UnloadBoneConfigs()
{
    m_boneConfigs.clear();
    m_boneConfigRecord = SBoneConfigRecord();
}