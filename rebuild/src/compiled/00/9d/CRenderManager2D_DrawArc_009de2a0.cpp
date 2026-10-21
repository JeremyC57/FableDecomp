// CRenderManager2D::DrawArc(const C2DVector&, float, const CRGBColour&, unsigned long) -- draw a full circle of line segments (about 2/3 segment per screen pixel of half-circumference)
// Leaf: DrawArc
// Retail 0x009DE2A0, 328 bytes, ret 0x10 (frame-pointer function: `and esp,-8` for the double asin argument).
// Callees: 0x009E1E10 / 0x009E1E40 (virtual-to-screen X/Y, __stdcall float; landed GFAXToVX/GFAYToVY) for the centre
// and radius, 0x009E1DA0 / 0x009E1DD0 (the inverse screen-to-virtual X/Y, __stdcall float) for each segment end,
// _CIasin 0x00BFEDC4, __ftol2 0x00BFEA70, CRenderManager2D::Draw2DLine 0x009DD960. Pool: 1.0 (double) 0x01237F00,
// 2/3f 0x012544AC.
// Shape: pi is `(float)(asin(1.0) * 2)` (the `* 2` is `fadd st,st`); the segment count truncates `pi * r * 2/3`, the 2/3
// written as a literal (an `extern const float` operand is reordered to the front); each segment's angles are
// `i * pi * 2 / segments` with the float segment count hoisted; fsin/fcos are VC's intrinsics; the two endpoints are
// C2DVector temporaries built right to left (Y of each before its X).
#include <math.h>
struct C2DVector {
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) {}
};
struct CRGBColour { unsigned char B, G, R, A; };
float __stdcall GFAXToVX(float x);                 // 0x009E1E10
float __stdcall GFAYToVY(float y);                 // 0x009E1E40
float __stdcall GFVXToAX(float x);                 // 0x009E1DA0
float __stdcall GFVYToAY(float y);                 // 0x009E1DD0

struct CRenderManager2D {
    bool Draw2DLine(const C2DVector& from, const C2DVector& to, const CRGBColour& colour, unsigned long flags);   // 0x009DD960
    bool DrawArc(const C2DVector& centre, float radius, const CRGBColour& colour, unsigned long flags);
};

bool CRenderManager2D::DrawArc(const C2DVector& centre, float radius, const CRGBColour& colour, unsigned long flags)
{
    float cx = GFAXToVX(centre.X);
    float cy = GFAYToVY(centre.Y);
    float r = GFAXToVX(radius);
    float pi = (float)(asin(1.0) * 2);
    long segments = (long)(pi * r * 0.6666667f);
    for (long i = 0; i < segments; ++i) {
        float a0 = i * pi * 2 / segments;
        float a1 = (i + 1) * pi * 2 / segments;
        Draw2DLine(C2DVector(GFVXToAX(sin(a0) * r + cx), GFVYToAY(cos(a0) * r + cy)),
                   C2DVector(GFVXToAX(sin(a1) * r + cx), GFVYToAY(cos(a1) * r + cy)), colour, flags);
    }
    return true;
}