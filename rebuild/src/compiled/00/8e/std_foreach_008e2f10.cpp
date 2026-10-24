// std::for_each(C3DVector* first, C3DVector* last, CGetBestIciclePointForTarget f) -- keeps the icicle point with the highest 1/(1+d^2) XY-closeness score to the target
// Leaf: for_each
// Retail: Fable.exe (TLC, VC7.1) @ 0x008E2F10, 158 bytes, `ret 20h`. Caller: CModeIceTrollGroundSmashIcicleAssault::FrameUpdate 0x008DE9C2.
// Constants: 1.0f @ 0x0122DED8.
// SGI/STLport `for_each` (`for (; first != last; ++first) f(*first); return f;`), compiled /Gr in retail so it is `__fastcall` here: the functor is returned through the hidden pointer (ecx), a pointer `first` rides in edx, the rest on the stack.
// Shape: C3DVector has a user copy ctor, so the functor's implicit copy is memberwise (seven scheduled dword moves) and the best
// point is stored through memory each hit; the squares come out `dy*dy` first (`fld st(0) ; fmul st(1)`).
namespace std {
template <class InputIter, class Function>
Function __fastcall for_each(InputIter first, InputIter last, Function f)
{
    for ( ; first != last; ++first)
        f(*first);
    return f;
}
}
class C3DVector {
public:
    float x, y, z;
    C3DVector() {}
    C3DVector(const C3DVector& v) : x(v.x), y(v.y), z(v.z) {}
};
class CGetBestIciclePointForTarget {
public:
    C3DVector m_target;                           // +0x00
    float m_bestScore;                            // +0x0C
    C3DVector m_bestPoint;                        // +0x10
    CGetBestIciclePointForTarget() {}
    void operator()(const C3DVector& p)
    {
        float dx = p.x - m_target.x;
        float dy = p.y - m_target.y;
        float score = 1.0f / (dy * dy + dx * dx + 1.0f);
        if (score > m_bestScore) {
            m_bestScore = score;
            m_bestPoint = p;
        }
    }
};
template CGetBestIciclePointForTarget __fastcall std::for_each(C3DVector*, C3DVector*, CGetBestIciclePointForTarget);