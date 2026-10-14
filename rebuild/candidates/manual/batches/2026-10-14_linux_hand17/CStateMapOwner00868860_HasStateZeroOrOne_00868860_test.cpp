#include <cstdio>
struct SStateNode { unsigned char Colour; SStateNode* Parent; SStateNode* Left; SStateNode* Right; long Key; };
struct SStateIterator { SStateNode* Node; SStateIterator(SStateNode* node) : Node(node) { } };
struct CRetailStateMap {
    SStateNode* Header; long NodeCount; unsigned char Compare[4];
    SStateNode m_nodes[4];
    SStateIterator Find(const long& key) { for (long i = 0; i < NodeCount; ++i) if (m_nodes[i].Key == key) return SStateIterator(&m_nodes[i]); return SStateIterator(Header); }
    CRetailStateMap() : NodeCount(0) { Header = &m_nodes[3]; }
    void Add(long key) { m_nodes[NodeCount++].Key = key; }
};
struct CStateMapOwner_00868860 {
    unsigned char _pad_0x00[0x30]; CRetailStateMap m_states;
    bool HasStateZeroOrOne()
    {
        SStateNode* end;
        { SStateIterator first = m_states.Find(0); end = m_states.Header; if (first.Node != end) return true; }
        { SStateIterator second = m_states.Find(1); return second.Node != end ? true : false; }
    }
};
int main()
{
    CStateMapOwner_00868860 o;
    if (o.HasStateZeroOrOne()) { std::printf("00868860_TEST FAIL\n"); return 1; }
    o.m_states.Add(2); o.m_states.Add(7);
    if (o.HasStateZeroOrOne()) { std::printf("00868860_TEST FAIL\n"); return 1; }
    o.m_states.Add(1);
    if (!o.HasStateZeroOrOne()) { std::printf("00868860_TEST FAIL\n"); return 1; }
    o.m_states.NodeCount = 0; o.m_states.Add(0);
    if (!o.HasStateZeroOrOne()) { std::printf("00868860_TEST FAIL\n"); return 1; }
    std::printf("00868860_TEST PASS\n"); return 0;
}
