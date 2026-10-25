// Behaviour test for 00c3ab30: SIndexNode_00c3ab50 constructor -- value and index stored
#include <cstdio>
struct SIndexNode_00c3ab50 {
    void* m_pValue; unsigned long m_index;
    SIndexNode_00c3ab50(void* value, unsigned long index) : m_pValue(value), m_index(index) {}
};
int main()
{
    int v; SIndexNode_00c3ab50* n = new SIndexNode_00c3ab50(&v, 7u);
    bool ok = n->m_pValue == &v && n->m_index == 7u && sizeof(SIndexNode_00c3ab50) == 8;
    delete n;
    if (!ok) { std::printf("00c3ab30_TEST FAIL\n"); return 1; }
    std::printf("00c3ab30_TEST PASS\n");
    return 0;
}
