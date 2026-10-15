#include <cstdio>
#include <stdlib.h>
// Behaviour test for CEntryLayers_0086bde0::PushEntry (0x0086BDE0).
struct SEntryList {
    void* m_first; void* m_last; void* m_endOfStorage;
    unsigned char m_alloc;
    bool m_flag;
    SEntryList() : m_first(0), m_last(0), m_endOfStorage(0), m_flag(false) {}
    ~SEntryList() { if (m_first) free(m_first); }
};
struct SEntry {
    long m_a; long m_b; SEntryList m_list;
    SEntry(long a, long b) : m_a(a), m_b(b) {}
};
struct SRecord { long a; long b; bool flag; bool listEmpty; };
struct SEntryVector {
    SRecord* m_first; SRecord* m_last; SRecord* m_endOfStorage;
    void push_back(const SEntry& entry)
    {
        m_last->a = entry.m_a; m_last->b = entry.m_b; m_last->flag = entry.m_list.m_flag;
        m_last->listEmpty = entry.m_list.m_first == 0 && entry.m_list.m_last == 0 && entry.m_list.m_endOfStorage == 0;
        ++m_last;
    }
};
struct CEntryLayers_0086bde0 {
    SEntryVector* m_layersFirst; SEntryVector* m_layersLast; SEntryVector* m_layersEnd;
    void PushEntry(long layer, long a, long b);
};
void CEntryLayers_0086bde0::PushEntry(long layer, long a, long b)
{
    m_layersFirst[layer].push_back(SEntry(a, b));
}
int main()
{
    SRecord store0[4], store1[4];
    SEntryVector layers[2];
    layers[0].m_first = layers[0].m_last = store0; layers[0].m_endOfStorage = store0 + 4;
    layers[1].m_first = layers[1].m_last = store1; layers[1].m_endOfStorage = store1 + 4;
    CEntryLayers_0086bde0 o; o.m_layersFirst = layers; o.m_layersLast = layers + 2; o.m_layersEnd = layers + 2;
    o.PushEntry(1, 7, 9);
    o.PushEntry(1, 3, 4);
    o.PushEntry(0, -1, 5);
    if (layers[1].m_last != store1 + 2 || layers[0].m_last != store0 + 1 || store1[0].a != 7 || store1[0].b != 9
        || store1[1].a != 3 || store0[0].a != -1 || store0[0].b != 5 || store1[0].flag || !store1[0].listEmpty)
    { std::printf("0086bde0_TEST FAIL\n"); return 1; }
    std::printf("0086bde0_TEST PASS\n");
    return 0;
}
