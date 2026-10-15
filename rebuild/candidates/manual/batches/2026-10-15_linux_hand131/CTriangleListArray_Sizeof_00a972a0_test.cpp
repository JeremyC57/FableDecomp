// Behaviour test for 0x00A972A0 CTriangleListArray::Sizeof: 12 + 4 per slot, plus each list's 0x3c + 6 per allocated
// triangle + 2 per allocated index (4 for an empty slot).
#include <cstdio>
struct CList { int capTriangles, capIndices; unsigned int Sizeof() const { return 0x3c + capTriangles * 6 + capIndices * 2; } };
struct CPtr { CList* m_p; unsigned int Sizeof() const { return m_p ? m_p->Sizeof() : 4; } };
struct CArr {
    CPtr* m_pFirst; CPtr* m_pLast;
    unsigned int Sizeof() const
    {
        unsigned int size = 12 + (unsigned int)(m_pLast - m_pFirst) * 4;
        for (const CPtr* it = m_pFirst; it != m_pLast; ++it) size += it->Sizeof();
        return size;
    }
};
int main()
{
    CList a = { 10, 30 }, b = { 0, 0 };
    CPtr slots[3]; slots[0].m_p = &a; slots[1].m_p = 0; slots[2].m_p = &b;
    CArr arr; arr.m_pFirst = slots; arr.m_pLast = slots + 3;
    unsigned int expect = 12 + 12 + (0x3c + 60 + 60) + 4 + 0x3c;
    if (arr.Sizeof() != expect) { std::printf("00a972a0_TEST FAIL\n"); return 1; }
    arr.m_pLast = slots;
    if (arr.Sizeof() != 12) { std::printf("00a972a0_TEST FAIL\n"); return 1; }
    std::printf("00a972a0_TEST PASS\n");
    return 0;
}
