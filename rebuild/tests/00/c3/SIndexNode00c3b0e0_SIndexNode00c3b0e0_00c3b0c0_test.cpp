// Behaviour test for 00c3b0c0: SIndexNode_00c3b0e0 constructor -- value and index stored
#include <cstdio>
struct SIndexNode_00c3b0e0 {
    void* m_pValue; unsigned long m_index;
    SIndexNode_00c3b0e0(void* value, unsigned long index) : m_pValue(value), m_index(index) {}
};
int main()
{
    int v; SIndexNode_00c3b0e0* n = new SIndexNode_00c3b0e0(&v, 7u);
    bool ok = n->m_pValue == &v && n->m_index == 7u && sizeof(SIndexNode_00c3b0e0) == 8;
    delete n;
    if (!ok) { std::printf("00c3b0c0_TEST FAIL\n"); return 1; }
    std::printf("00c3b0c0_TEST PASS\n");
    return 0;
}
