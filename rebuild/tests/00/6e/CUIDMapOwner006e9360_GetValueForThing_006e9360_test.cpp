#include <cstdio>
struct SEntryValue { long m_data; };
struct SMapNode { long m_colour; SMapNode* m_pParent; SMapNode* m_pLeft; SMapNode* m_pRight; unsigned long m_keyLo, m_keyHi; SEntryValue m_value; };
static unsigned long g_lastLo = 0, g_lastHi = 0;
struct CUIDMap {
    SMapNode* m_pHeader; SMapNode* m_nodes; int m_count;
    SMapNode* FindNode(const unsigned long* key) const
    {
        g_lastLo = key[0]; g_lastHi = key[1];
        for (int i = 0; i < m_count; ++i)
            if (m_nodes[i].m_keyLo == key[0] && m_nodes[i].m_keyHi == key[1]) return &m_nodes[i];
        return m_pHeader;
    }
};
struct CKeyedThing { unsigned long m_uidLo, m_uidHi; };
struct COwner {
    CUIDMap m_values;
    SEntryValue* GetValueForThing(const CKeyedThing* thing)
    {
        unsigned long key[2]; key[0] = thing->m_uidLo; key[1] = thing->m_uidHi;
        return &m_values.FindNode(key)->m_value;
    }
};
int main()
{
    SMapNode header; header.m_value.m_data = -1;
    SMapNode nodes[3];
    for (int i = 0; i < 3; ++i) { nodes[i].m_keyLo = 10 + i; nodes[i].m_keyHi = 7; nodes[i].m_value.m_data = 100 + i; }
    COwner o; o.m_values.m_pHeader = &header; o.m_values.m_nodes = nodes; o.m_values.m_count = 3;
    CKeyedThing t; t.m_uidLo = 11; t.m_uidHi = 7;
    SEntryValue* v = o.GetValueForThing(&t);
    if (v != &nodes[1].m_value || v->m_data != 101 || g_lastLo != 11 || g_lastHi != 7) { std::printf("006e9360_TEST FAIL\n"); return 1; }
    t.m_uidHi = 8;
    if (o.GetValueForThing(&t) != &header.m_value) { std::printf("006e9360_TEST FAIL\n"); return 1; }
    std::printf("006e9360_TEST PASS\n");
    return 0;
}