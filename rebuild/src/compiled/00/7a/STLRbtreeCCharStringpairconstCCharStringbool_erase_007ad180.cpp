// _STL::_Rb_tree<CCharString, pair<const CCharString,bool>, ...>::erase(iterator first, iterator last) -- STLport range erase of a map<CCharString,bool>: [begin, end) clears the whole tree (recursive _M_erase from the root, header re-linked to itself, count zeroed); otherwise erase(first++) node by node
// Retail: Fable.exe (TLC, VC7.1) @ 0x007AD180, 196 bytes, thiscall, `ret 8`.
// Callees: _Rb_tree::_M_erase(_Link_type) 0x007ACC50 (recursive subtree release), __cdecl
//   _Rb_tree_rebalance_for_erase(node, root&, leftmost&, rightmost&) 0x0042A34E, ~CCharString 0x0099EAE0, free 0x00BFEA14.
// Shape: the landed EraseRange 0x00835440 (STLport 4.5: header pointer at +0, node count at +4; node colour +0, parent
//   +4, left +8, right +0xC, value +0x10) with a non-trivial value: erase(iterator) destroys the key string of the
//   unlinked node before the null-tested free. The in-order successor (`first++`) is inline. The value is a
//   pair<const CCharString, bool> (the bool byte at +0x14 is what the sibling writer 0x007AD270 serialises).
// Owner: the CTCAIScratchpad map<CCharString,bool> (landed TransferMap<CCharString,bool> 0x007AE3D0 names its siblings);
//   the only caller is map::erase(first, last) 0x007ADE60. The catalog label "LTextBinTree_EraseRange" is descriptive.
// Leaf: ?erase@CStringBoolTree
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14

class CCharString {
public:
    void* PStringData;
    ~CCharString();                                         // 0x0099EAE0
};

struct SNode {
    long   m_color;                                         // +0
    SNode* m_parent;                                        // +4
    SNode* m_left;                                          // +8
    SNode* m_right;                                         // +0xC
    CCharString m_key;                                      // +0x10 value.first
    bool        m_value;                                    // +0x14 value.second
};

extern "C" SNode* __cdecl _Rb_tree_rebalance_for_erase(SNode* node, SNode** root, SNode** leftmost, SNode** rightmost);   // 0x0042A34E

struct CStringBoolTree {
    SNode*        m_header;                                 // +0
    unsigned long m_node_count;                             // +4
    void _M_erase(SNode* root);                             // 0x007ACC50
    static SNode* _M_increment(SNode* node)
    {
        if (node->m_right != 0) {
            node = node->m_right;
            while (node->m_left != 0)
                node = node->m_left;
        }
        else {
            SNode* y = node->m_parent;
            while (node == y->m_right) {
                node = y;
                y = y->m_parent;
            }
            if (node->m_right != y)
                node = y;
        }
        return node;
    }
    void erase(SNode* first, SNode* last);
};

void CStringBoolTree::erase(SNode* first, SNode* last)
{
    if (first == m_header->m_left && last == m_header) {
        if (m_node_count != 0) {
            _M_erase(m_header->m_parent);
            m_header->m_left = m_header;
            m_header->m_parent = 0;
            m_header->m_right = m_header;
            m_node_count = 0;
        }
    }
    else {
        while (first != last) {
            SNode* victim = first;
            first = _M_increment(first);
            SNode* node = _Rb_tree_rebalance_for_erase(victim, &m_header->m_parent, &m_header->m_left, &m_header->m_right);
            node->m_key.~CCharString();
            if (node)
                free(node);
            --m_node_count;
        }
    }
}