#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#define __stdcall
typedef long long Int64;
typedef unsigned long long UInt64;
#else
typedef __int64 Int64;
typedef unsigned __int64 UInt64;
#endif
// Behaviour test for 0042d5b1: operator[] returns the mapped value of an existing key and inserts a default CUIState for a missing one
// (lower_bound and insert are small test doubles over a sorted node array; the operator[] body is the reconstructed logic).
namespace NUISystem { struct CComponent { long m_id; }; }
static int g_inserts = 0;
struct CUIState {
    long m_marker;
    CUIState() : m_marker(-7) {}
    CUIState(const CUIState& other) : m_marker(other.m_marker) {}
};
struct SComponentStatePair {
    NUISystem::CComponent* first;
    CUIState second;
    SComponentStatePair() : first(0) {}
    SComponentStatePair(NUISystem::CComponent* const& key, const CUIState& value) : first(key), second(value) {}
};
struct SComponentStateNode {
    long m_color;
    SComponentStateNode* m_pParent;
    SComponentStateNode* m_pLeft;
    SComponentStateNode* m_pRight;
    SComponentStatePair m_value;
};
struct SComponentStateIterator {
    SComponentStateNode* m_pNode;
    explicit SComponentStateIterator(SComponentStateNode* node) : m_pNode(node) {}
    SComponentStateIterator(const SComponentStateIterator& other) : m_pNode(other.m_pNode) {}
    SComponentStatePair& operator*() const { return m_pNode->m_value; }
    bool operator==(const SComponentStateIterator& other) const { return m_pNode == other.m_pNode; }
};
struct CComponentStateMap {
    SComponentStateNode* m_pHeader;
    unsigned long m_count;
    SComponentStateNode m_nodes[8];          // test storage: kept sorted by key
    SComponentStateNode* LowerBoundNode(NUISystem::CComponent* const& key)
    {
        for (unsigned long i = 0; i < m_count; ++i)
            if (!(m_nodes[i].m_value.first < key))
                return &m_nodes[i];
        return m_pHeader;
    }
    SComponentStateIterator InsertUnique(SComponentStateIterator, const SComponentStatePair& value)
    {
        unsigned long i = m_count;
        while (i > 0 && value.first < m_nodes[i - 1].m_value.first) { m_nodes[i].m_value = m_nodes[i - 1].m_value; --i; }
        m_nodes[i].m_value = value;
        ++m_count; ++g_inserts;
        return SComponentStateIterator(&m_nodes[i]);
    }
    SComponentStateIterator lower_bound(NUISystem::CComponent* const& key) { return SComponentStateIterator(LowerBoundNode(key)); }
    SComponentStateIterator end() { return SComponentStateIterator(m_pHeader); }
    CUIState& operator[](NUISystem::CComponent* const& key);
};
CUIState& CComponentStateMap::operator[](NUISystem::CComponent* const& key)
{
    SComponentStateIterator it = lower_bound(key);
    if (it == end() || key < (*it).first) {
        CUIState defaultValue;
        it = InsertUnique(it, SComponentStatePair(key, defaultValue));
    }
    return (*it).second;
}
int main()
{
    NUISystem::CComponent comps[3];
    SComponentStateNode header;
    CComponentStateMap map; map.m_pHeader = &header; map.m_count = 0;
    NUISystem::CComponent* k1 = &comps[1];
    NUISystem::CComponent* k0 = &comps[0];
    NUISystem::CComponent* k2 = &comps[2];
    CUIState& s1 = map[k1];                   // missing -> inserted with the default
    if (g_inserts != 1 || s1.m_marker != -7) { std::printf("0042d5b1_TEST FAIL\n"); return 1; }
    s1.m_marker = 100;
    if (map[k1].m_marker != 100 || g_inserts != 1) { std::printf("0042d5b1_TEST FAIL\n"); return 1; }   // existing key: no insert
    map[k0].m_marker = 50;                    // key below the lower bound node -> inserted
    map[k2].m_marker = 200;                   // key past every node (lower_bound == end) -> inserted
    if (g_inserts != 3 || map.m_count != 3 || map[k0].m_marker != 50 || map[k1].m_marker != 100 || map[k2].m_marker != 200 || g_inserts != 3) { std::printf("0042d5b1_TEST FAIL\n"); return 1; }
    std::printf("0042d5b1_TEST PASS\n");
    return 0;
}