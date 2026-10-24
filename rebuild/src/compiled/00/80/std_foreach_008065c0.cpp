// std::for_each(NPlayerTargeting::CTargetPoint* first, NPlayerTargeting::CTargetPoint* last, CGetBestTargetInDirection f) -- feeds every 28-byte target point to the best-target functor (out-of-line operator() 0x00805A90) and returns it
// Leaf: for_each
// Retail: Fable.exe (TLC, VC7.1) @ 0x008065C0, 89 bytes, `ret 20h`. Caller: the player-targeting update 0x008050DF.
// Callee: CGetBestTargetInDirection::operator()(CTargetPoint*) @ 0x00805A90 (manifest): it reads an origin pointer (+0x00), a direction
// (+0x04..+0x0C), a side sign (+0x10) and keeps the best point (+0x14) and score (+0x18).
// SGI/STLport `for_each` (`for (; first != last; ++first) f(*first); return f;`), compiled /Gr in retail so it is `__fastcall` here: the functor is returned through the hidden pointer (ecx), a pointer `first` rides in edx, the rest on the stack.
// Shape: the 28-byte functor is copied out with three scheduled registers = a memberwise user copy ctor over seven dwords
// (the implicit copy is a `rep movsd`).
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
struct CTargetPoint { long m_words[7]; };          // 28 bytes (position at +0x0C/+0x10)
}
class CGetBestTargetInDirection {
public:
    // the float members are modelled as dwords: the copy ctor moves them with integer moves (GOTCHAS wave 19)
    const void* m_pOrigin;                        // +0x00 (a C3DVector*)
    long m_dirX, m_dirY, m_dirZ;                  // +0x04 direction
    long m_sideSign;                              // +0x10
    const NPlayerTargeting::CTargetPoint* m_pBest;    // +0x14
    long m_bestScore;                             // +0x18
    CGetBestTargetInDirection() {}
    CGetBestTargetInDirection(const CGetBestTargetInDirection& o)
        : m_pOrigin(o.m_pOrigin), m_dirX(o.m_dirX), m_dirY(o.m_dirY), m_dirZ(o.m_dirZ), m_sideSign(o.m_sideSign),
          m_pBest(o.m_pBest), m_bestScore(o.m_bestScore) {}
    void operator()(const NPlayerTargeting::CTargetPoint& p);   // 0x00805A90
};
template CGetBestTargetInDirection __fastcall std::for_each(NPlayerTargeting::CTargetPoint*, NPlayerTargeting::CTargetPoint*, CGetBestTargetInDirection);