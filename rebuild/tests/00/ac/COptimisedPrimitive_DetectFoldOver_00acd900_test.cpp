// Behaviour test for 00acd900: the model below is a copy of src/00acd900.cpp (compiled separately), driven by test doubles.
#include <cstdio>
#include <cstring>
static int g_calls = 0, g_foldAt = -1;
namespace QuadricOptimiserInternals {
struct C3DVector {
    float X, Y, Z;
};
struct CSubContraction {
    unsigned long Index1;
    unsigned long Index2;
};
template<class T> struct CArray {
    T* m_pData;
    T* m_pLast;
    T* m_pEnd;
    unsigned long Size() const { return (unsigned long)(m_pLast - m_pData); }
    T* _SweepBegin1() const { return m_pData; }
    T& operator[](unsigned long index) { return *(_SweepBegin1() + index); }
};
struct CVertexPairContraction {
    bool                     Exists;
    bool                     Unsplit;
    CArray<CSubContraction>  SubContractions;
    float                    Weight2;
    float                    Cost;
    C3DVector                ResultingPosition;
    void*                    ListIterator;
};
}
using namespace QuadricOptimiserInternals;
static C3DVector* g_lastPos = 0;
struct COptimisedPrimitive {
    bool DetectFoldOver(CSubContraction* subContraction, C3DVector* position) { ++g_calls; g_lastPos = position; return (int)subContraction->Index1 == g_foldAt; }
    bool DetectFoldOver(CVertexPairContraction* contraction);
};

bool COptimisedPrimitive::DetectFoldOver(CVertexPairContraction* contraction)
{
    for (unsigned long i = 0; i < contraction->SubContractions.Size(); ++i) {
        if (DetectFoldOver(&contraction->SubContractions[i], &contraction->ResultingPosition))
            return true;
    }
    return false;
}

static bool Run(int count, int foldAt, bool expect, int expectCalls)
{
    CSubContraction subs[4]; for (int i = 0; i < 4; ++i) { subs[i].Index1 = i; subs[i].Index2 = 100 + i; }
    CVertexPairContraction c; std::memset(&c, 0, sizeof(c));
    c.SubContractions.m_pData = subs; c.SubContractions.m_pLast = subs + count; c.SubContractions.m_pEnd = subs + 4;
    COptimisedPrimitive p; g_calls = 0; g_foldAt = foldAt; g_lastPos = 0;
    bool r = p.DetectFoldOver(&c);
    if (r != expect || g_calls != expectCalls) return false;
    if (expectCalls && g_lastPos != &c.ResultingPosition) return false;
    return true;
}
int main()
{
    bool ok = Run(4, 2, true, 3) && Run(4, -1, false, 4) && Run(0, 0, false, 0) && Run(3, 0, true, 1);
    if (!ok) { std::printf("00acd900_TEST FAIL\n"); return 1; }
    std::printf("00acd900_TEST PASS\n");
    return 0;
}