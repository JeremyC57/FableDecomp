// CCombatantMap::erase(iterator first, iterator last) -- STLport range erase of the UID -> CCombatantInfo tree: [begin, end) clears through the recursive _M_erase; otherwise each node is unlinked, its CCombatantInfo destroyed (defender info out of line, the attacker's sequence vector inline) and freed
// Leaf: DestroyNodeAndCleanup
// Retail: Fable.exe (TLC, VC7.1) @ 0x006F4E40, 235 bytes (240 with padding), `ret 8`. The catalog name
//   "CCombatantInfo::DestroyNodeAndCleanup" is kept as the leaf; the body is the map's erase(first, last).
// Callees: the tree's recursive _M_erase(root) 0x006F4C20, _Rb_tree_rebalance_for_erase 0x0042A34E (__cdecl; landed caller
//   0x00835440), CDefendingCombatantInfo::~CDefendingCombatantInfo 0x006F48A0 (value +0x34), the counted-pointer
//   _Destroy_range 0x006F2300 (landed; first in ecx, last in edx, the allocator on the stack), free 0x00BFEA14.
// Layout: tree {header, count}; node 0x88 bytes = links + key (+0x10) + CCombatantInfo (+0x18): AttackerInfo.CombatSequences
//   vector at node +0x40, DefenderInfo at node +0x4c (landed 0x006E9E40 layout).
// Shape: the landed 0x00835440 erase-range shape plus the value's inline destructor; the _Destroy_range allocator is a
//   temporary bound to a reference (it lands in the dead `first` slot).
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14

struct SCountBlock;
struct CSequencePointer {                                   // CCountedPointer<CCombatSequenceBase>
    void* m_pObject;
    SCountBlock* m_pCount;
};

struct CRetailAllocator {
    CRetailAllocator() { }
};

void __fastcall DestroyRange_CountedPointer_006f2300(CSequencePointer* first, CSequencePointer* last, const CRetailAllocator& al);   // 0x006F2300

struct CSequenceVector {                                    // retail vector<CCountedPointer<CCombatSequenceBase>>, 12 bytes
    CSequencePointer* _M_start;
    CSequencePointer* _M_finish;
    CSequencePointer* _M_end_of_storage;
    ~CSequenceVector()
    {
        DestroyRange_CountedPointer_006f2300(_M_start, _M_finish, CRetailAllocator());
        if (_M_start)
            free(_M_start);
    }
};

class CAttackingCombatantInfo { public:                            // retail 0x30 bytes
    unsigned char _data[0x24];
    CSequenceVector CombatSequences;                        // +0x24
};

class CDefendingCombatantInfo { public:                            // retail 0x3c bytes
    unsigned char _data[0x3c];
    ~CDefendingCombatantInfo();                             // 0x006F48A0
};

struct CCombatantInfo {                                     // retail 0x70 bytes
    void* Parent;                                           // +0x00
    CAttackingCombatantInfo AttackerInfo;                   // +0x04
    CDefendingCombatantInfo DefenderInfo;                   // +0x34
};

struct SCombatantValue {                                    // pair<const unsigned __int64, CCombatantInfo>
    unsigned __int64 first;                                 // +0x00
    CCombatantInfo second;                                  // +0x08
};

struct SNode {
    long m_color;                                           // +0x00
    SNode* m_parent;                                        // +0x04
    SNode* m_left;                                          // +0x08
    SNode* m_right;                                         // +0x0c
};

struct SCombatantNode : SNode {
    SCombatantValue m_value;                                // +0x10
};

extern "C" SNode* __cdecl Unlink_0042a34e(SNode* node, SNode** root, SNode** leftmost, SNode** rightmost);   // 0x0042A34E

struct CCombatantMap {
    SNode* m_header;                                        // +0
    unsigned long m_size;                                   // +4
    void Erase(SNode* root);                                // 0x006F4C20
    static SNode* Successor(SNode* node)
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
    void DestroyNodeAndCleanup(SNode* first, SNode* last);
};

void CCombatantMap::DestroyNodeAndCleanup(SNode* first, SNode* last)
{
    if (first == m_header->m_left && last == m_header) {
        if (m_size != 0) {
            Erase(m_header->m_parent);
            m_header->m_left = m_header;
            m_header->m_parent = 0;
            m_header->m_right = m_header;
            m_size = 0;
        }
    }
    else {
        while (first != last) {
            SNode* victim = first;
            first = Successor(first);
            SCombatantNode* node = (SCombatantNode*)Unlink_0042a34e(victim, &m_header->m_parent, &m_header->m_left, &m_header->m_right);
            node->m_value.~SCombatantValue();
            if (node)
                free(node);
            --m_size;
        }
    }
}