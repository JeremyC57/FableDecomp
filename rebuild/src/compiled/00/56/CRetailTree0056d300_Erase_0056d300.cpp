// CRetailTree_0056d300::Erase(SNode*) -- unlinks the node from the STLport red-black tree, destroys its value (two wide strings and a string), frees the node and decrements the node count
// Leaf: ?Erase@CRetailTree_0056d300@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0056D300, 78 bytes (`ret 4`). 0x0056D720 is its byte-identical twin.
// Callees: the `__cdecl` rebalance-for-erase @ 0x0042A34E (node, &header->parent, &header->left, &header->right;
// labelled "CMemoryAllocatorVariableSize::Erase"), CWideString::~CWideString @ 0x0099B510 (+0x28, +0x24),
// CCharString::~CCharString @ 0x0099EAE0 (+0x20), free @ 0x00BFEA14.
// Label: "CParticleMorphs::Erase" is propagated (no CParticleMorphs map holds strings); the tree is named by
// address, as the landed CRetailTree_00756030 family (0x00756030, same unlink helper and node layout).
// Shape: STLport `erase(iterator)`: the header's three link addresses formed in place (`add eax,4` last), the
// unlinked node's value destroyed member by member in reverse, the null-tested deallocate, the count
// decremented in memory (`dec dword ptr [edi+4]`, the result unused).
extern "C" void __cdecl free(void* memory);         // 0x00BFEA14

struct CCharString {
    void* m_pStringData;
    ~CCharString();                                  // 0x0099EAE0
};

struct CWideString {
    void* m_pStringData;
    ~CWideString();                                  // 0x0099B510
};

struct SValue_0056d300 {                                  // pair<const key, record> at node +0x10
    unsigned char m_key[0x10];                       // +0x10 (trivially destructible)
    CCharString   m_name;                            // +0x20
    CWideString   m_pathA;                           // +0x24
    CWideString   m_pathB;                           // +0x28
};

struct CRetailTree_0056d300 {
    struct SNode {
        long          _M_color;                      // +0x00
        SNode*        _M_parent;                     // +0x04
        SNode*        _M_left;                       // +0x08
        SNode*        _M_right;                      // +0x0c
        SValue_0056d300 _M_value;                      // +0x10
    };
    SNode*        _M_header;                         // +0x00
    unsigned long _M_node_count;                     // +0x04
    void Erase(SNode* where);
};

extern "C" CRetailTree_0056d300::SNode* __cdecl RebalanceForErase_0042a34e(CRetailTree_0056d300::SNode* where,
    CRetailTree_0056d300::SNode** root, CRetailTree_0056d300::SNode** leftmost, CRetailTree_0056d300::SNode** rightmost);   // 0x0042A34E

void CRetailTree_0056d300::Erase(SNode* where)
{
    SNode* victim = RebalanceForErase_0042a34e(where, &_M_header->_M_parent, &_M_header->_M_left, &_M_header->_M_right);
    victim->_M_value.~SValue_0056d300();
    if (victim)
        free(victim);
    --_M_node_count;
}