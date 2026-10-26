// Behaviour test for std::vector<CMatrix3x4>::_M_allocate_and_copy<const CMatrix3x4*> @ 0x00AAFCA0
#include <cstdio>
#include <cstdlib>
#include <new>

class CMatrix3x4 { public: float m_row[3][4]; };

static CMatrix3x4* allocate_and_copy(unsigned int n, const CMatrix3x4* first, const CMatrix3x4* last)
{
    CMatrix3x4* result = n != 0 ? (CMatrix3x4*)std::malloc(n * sizeof(CMatrix3x4)) : 0;
    CMatrix3x4* cur = result;
    for ( ; first != last; ++first, ++cur)
        new (cur) CMatrix3x4(*first);
    return result;
}

int main()
{
    CMatrix3x4 src[2];
    for (int m = 0; m < 2; ++m)
        for (int r = 0; r < 3; ++r)
            for (int c = 0; c < 4; ++c)
                src[m].m_row[r][c] = (float)(m * 100 + r * 10 + c);
    CMatrix3x4* copy = allocate_and_copy(4, src, src + 2);
    int ok = copy != 0;
    for (int m = 0; m < 2 && ok; ++m)
        for (int r = 0; r < 3; ++r)
            for (int c = 0; c < 4; ++c)
                if (copy[m].m_row[r][c] != src[m].m_row[r][c]) ok = 0;
    if (allocate_and_copy(0, src, src) != 0) ok = 0;
    std::free(copy);
    if (!ok) { std::printf("00aafca0_TEST FAIL\n"); return 1; }
    std::printf("00aafca0_TEST PASS\n");
    return 0;
}