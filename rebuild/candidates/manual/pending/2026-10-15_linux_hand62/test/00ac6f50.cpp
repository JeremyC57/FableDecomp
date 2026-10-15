#include <cstdio>

// COptimisedFace::RemapIndex: every corner equal to `from` becomes `to`, the others are kept.
struct COptimisedFace {
    long m_vertex[3];
    void RemapIndex(long from, long to);
};
void COptimisedFace::RemapIndex(long from, long to)
{
    if (m_vertex[0] == from) m_vertex[0] = to;
    if (m_vertex[1] == from) m_vertex[1] = to;
    if (m_vertex[2] == from) m_vertex[2] = to;
}

int main()
{
    COptimisedFace f;
    f.m_vertex[0] = 4; f.m_vertex[1] = 9; f.m_vertex[2] = 4;
    f.RemapIndex(4, 11);
    bool ok = f.m_vertex[0] == 11 && f.m_vertex[1] == 9 && f.m_vertex[2] == 11;
    f.RemapIndex(5, 1);
    ok = ok && f.m_vertex[0] == 11 && f.m_vertex[1] == 9 && f.m_vertex[2] == 11;
    f.RemapIndex(9, 0);
    ok = ok && f.m_vertex[1] == 0;
    if (!ok) {
        std::printf("00ac6f50_TEST FAIL\n");
        return 1;
    }
    std::printf("00ac6f50_TEST PASS\n");
    return 0;
}
