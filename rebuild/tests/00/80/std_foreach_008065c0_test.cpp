#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#endif
namespace std {
template <class InputIter, class Function>
Function __fastcall for_each(InputIter first, InputIter last, Function f)
{
    for ( ; first != last; ++first)
        f(*first);
    return f;
}
}
namespace NPlayerTargeting {
struct CTargetPoint { long m_words[7]; };
}
class CGetBestTargetInDirection {
public:
    const void* m_pOrigin;
    long m_dirX, m_dirY, m_dirZ;
    long m_sideSign;
    const NPlayerTargeting::CTargetPoint* m_pBest;
    long m_bestScore;
    CGetBestTargetInDirection() : m_pOrigin(0), m_dirX(1), m_dirY(2), m_dirZ(3), m_sideSign(4), m_pBest(0), m_bestScore(-1) {}
    CGetBestTargetInDirection(const CGetBestTargetInDirection& o)
        : m_pOrigin(o.m_pOrigin), m_dirX(o.m_dirX), m_dirY(o.m_dirY), m_dirZ(o.m_dirZ), m_sideSign(o.m_sideSign),
          m_pBest(o.m_pBest), m_bestScore(o.m_bestScore) {}
    void operator()(const NPlayerTargeting::CTargetPoint& p)       // test stand-in: keep the record with the largest word 3
    {
        if (p.m_words[3] > m_bestScore) { m_bestScore = p.m_words[3]; m_pBest = &p; }
    }
};
int main()
{
    NPlayerTargeting::CTargetPoint p[3];
    for (int i = 0; i < 3; ++i) for (int j = 0; j < 7; ++j) p[i].m_words[j] = i * 10 + j;
    p[1].m_words[3] = 99;
    CGetBestTargetInDirection f = std::for_each(p, p + 3, CGetBestTargetInDirection());
    if (f.m_pBest != &p[1] || f.m_bestScore != 99 || f.m_dirX != 1 || f.m_dirZ != 3 || f.m_sideSign != 4) { std::printf("008065c0_TEST FAIL\n"); return 1; }
    std::printf("008065c0_TEST PASS\n");
    return 0;
}