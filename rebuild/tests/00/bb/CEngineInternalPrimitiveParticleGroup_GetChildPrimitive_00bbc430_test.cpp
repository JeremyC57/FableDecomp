#include <cstdio>
struct CPrim { int x; };
struct CHandle { long m_id; CPrim* m_pPrimitive; };
struct SNode { SNode* m_pNext; SNode* m_pPrev; CHandle m_value; };
struct CIt { unsigned char m_used; SNode* m_pNode; };
struct CArr { CIt* m_pFirst; CIt* m_pLast; CIt* m_pEnd; unsigned long GetCount() const { return m_pLast - m_pFirst; } CIt& operator[](unsigned long i) { return m_pFirst[i]; } };
struct CGroup { unsigned char _pad[0x48]; void* m_pPrimitives; CArr PrimitiveIDToIterator;
    CPrim* GetChildPrimitive(unsigned long id) { if (id < PrimitiveIDToIterator.GetCount()) { CIt& e = PrimitiveIDToIterator[id]; if (e.m_used) return e.m_pNode->m_value.m_pPrimitive; } return 0; } };
int main()
{
    CPrim p0, p1; SNode n0 = { 0, 0, { 0, &p0 } }, n1 = { 0, 0, { 1, &p1 } };
    CIt its[3] = { { 1, &n0 }, { 0, &n1 }, { 1, &n1 } };
    CGroup g; g.PrimitiveIDToIterator.m_pFirst = its; g.PrimitiveIDToIterator.m_pLast = its + 3; g.PrimitiveIDToIterator.m_pEnd = its + 3;
    if (g.GetChildPrimitive(0) != &p0 || g.GetChildPrimitive(1) != 0 || g.GetChildPrimitive(2) != &p1 || g.GetChildPrimitive(3) != 0 || g.GetChildPrimitive((unsigned long)-1) != 0) { std::printf("00bbc430_TEST FAIL\n"); return 1; }
    std::printf("00bbc430_TEST PASS\n"); return 0;
}