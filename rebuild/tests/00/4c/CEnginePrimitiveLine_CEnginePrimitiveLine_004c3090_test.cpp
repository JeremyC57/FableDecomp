// Behaviour test for CEnginePrimitiveLine::CEnginePrimitiveLine @ 0x004C3090.
#include <cstdio>
struct C3DVector { float X, Y, Z; };
struct CRGBColour { unsigned char B, G, R, A; };
struct CEnginePrimitiveLine {
    long m_type; long m_interpolation; bool m_flag8, m_flag9, m_flagA; C3DVector m_start, m_end; CRGBColour m_colour;
    float m_width; unsigned int m_flags; long m_blendMode, m_blendOp; bool m_depthTest;
    CEnginePrimitiveLine(unsigned long long, const C3DVector& s, const C3DVector& e, const CRGBColour& c, float w, long blend,
                         unsigned int flags, bool depth, long interp, long op)
        : m_type(6), m_interpolation(interp), m_flag8(false), m_flag9(false), m_flagA(false), m_start(s), m_end(e), m_colour(c),
          m_width(w), m_flags(flags), m_blendMode(blend), m_blendOp(op), m_depthTest(depth) { }
};
int main()
{
    C3DVector s = { 1, 2, 3 }, e = { 4, 5, 6 }; CRGBColour c = { 10, 20, 30, 40 };
    CEnginePrimitiveLine l(99, s, e, c, 2.5f, 3, 0x11, true, 4, 5);
    int ok = l.m_type == 6 && l.m_interpolation == 4 && !l.m_flag8 && !l.m_flag9 && !l.m_flagA && l.m_start.Z == 3 && l.m_end.X == 4
          && l.m_colour.B == 10 && l.m_colour.A == 40 && l.m_width == 2.5f && l.m_flags == 0x11 && l.m_blendMode == 3 && l.m_blendOp == 5
          && l.m_depthTest;
    if (!ok) { std::printf("004c3090_TEST FAIL\n"); return 1; }
    std::printf("004c3090_TEST PASS\n");
    return 0;
}