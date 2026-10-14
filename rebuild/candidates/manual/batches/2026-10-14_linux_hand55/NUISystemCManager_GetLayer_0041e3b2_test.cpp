#include <cstdio>
#include <cstdlib>
#include <cstring>
struct SLayerNode { long m_key; long m_value; };
struct CLayerTree {                                // the head is the sentinel: end()
    SLayerNode m_nodes[8]; long m_count; SLayerNode* m_pHead;
    CLayerTree() : m_count(0), m_pHead(&m_nodes[7]) { }
    void Add(long key, long value) { m_nodes[m_count].m_key = key; m_nodes[m_count].m_value = value; ++m_count; }
    SLayerNode* Find(const long& key) { for (long i = 0; i < m_count; ++i) if (m_nodes[i].m_key == key) return &m_nodes[i]; return m_pHead; }
    long& operator[](const long& key) { SLayerNode* n = Find(key); if (n == m_pHead) { Add(key, 0); return m_nodes[m_count - 1].m_value; } return n->m_value; }
};
struct CManager {
    CLayerTree m_layers;
    long GetLayer(long key)
    {
        if (key < -3) key = -3; else if (key > 13) key = 13;
        SLayerNode* node = m_layers.Find(key);
        if (node != m_layers.m_pHead) return node->m_value;
        key = 0;
        return m_layers[key];
    }
};

int main()
{
    CManager m; m.m_layers.m_count = 0; m.m_layers.m_nodes[0].m_value = 999;
    m.m_layers.Add(-3, 10); m.m_layers.Add(0, 20); m.m_layers.Add(13, 30); m.m_layers.Add(5, 50);
    if (m.GetLayer(5) != 50 || m.GetLayer(-100) != 10 || m.GetLayer(100) != 30 || m.GetLayer(-3) != 10) { std::printf("0041e3b2_TEST FAIL\n"); return 1; }
    if (m.GetLayer(7) != 20 || m.GetLayer(13) != 30) { std::printf("0041e3b2_TEST FAIL\n"); return 1; }
    std::printf("0041e3b2_TEST PASS\n");
    return 0;
}
