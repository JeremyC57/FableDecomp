// CRenderManager2D::DrawArc(const C2DVector&, float, const CRGBColour&, unsigned long) -- draw a full circle of line segments (about 2/3 segment per screen pixel of half-circumference)
// Leaf: ?DrawArc@CRenderManager2D@@QAE_NABVC2DVector@@MABVCRGBColour@@K@Z
// Retail 0x009DE2A0, 328 bytes, ret 0x10 (frame-pointer function: `and esp,-8` for the double asin argument).
// Callees: GFVXToAX / GFVYToAY 0x009E1E10 / 0x009E1E40 (virtual-to-screen X/Y: x * width / 65536) for the centre
// and radius, GFAXToVX / GFAYToVY 0x009E1DA0 / 0x009E1DD0 (the inverse screen-to-virtual X/Y) for each segment end,
// _CIasin 0x00BFEDC4, __ftol2 0x00BFEA70, CRenderManager2D::Draw2DLine 0x009DD960. Pool: 1.0 (double) 0x01237F00,
// 2/3f 0x012544AC.
// Shape: pi is `(float)(asin(1.0) * 2)` (the `* 2` is `fadd st,st`); the segment count truncates `pi * r * 2/3`, the 2/3
// written as a literal (an `extern const float` operand is reordered to the front); each segment's angles are
// `i * pi * 2 / segments` with the float segment count hoisted; fsin/fcos are VC's intrinsics; the two endpoints are
// C2DVector temporaries built right to left (Y of each before its X).
#include <math.h>
class C2DVector {
public:
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) {}
};
class CRGBColour { public: unsigned char B, G, R, A; };
// Names (rename1, FINDINGS chunk 382): the four converters are Ego_r.pdb ?GFAXToVX@@YIMM@Z, ?GFAYToVY@@YIMM@Z,
// ?GFVXToAX@@YIMM@Z, ?GFVYToAY@@YIMM@Z (__fastcall; the float goes on the stack, so the call sites are unchanged).
// V is the 65536-unit virtual space (GFRoundVYToNearestPixel 0x009E1D50 rounds a V value to a pixel), A the render
// target's pixels: 0x009E1DA0 is 65536 * x / width = GFAXToVX (its catalog name; Ego_r.pdb orders GFRoundVYToNearestPixel,
// GFAXToVX, GFAYToVY, GFVXToAX, GFVYToAY exactly as retail 0x009E1D50 / 0x009E1DA0 / 0x009E1DD0 / 0x009E1E10 /
// 0x009E1E40), and 0x009E1E10 is width * x / 65536 = GFVXToAX. The earlier declarations here had the pairs swapped.
float __fastcall GFVXToAX(float x);                // 0x009E1E10
float __fastcall GFVYToAY(float y);                // 0x009E1E40
float __fastcall GFAXToVX(float x);                // 0x009E1DA0
float __fastcall GFAYToVY(float y);                // 0x009E1DD0

class CRenderManager2D {
public:
    bool Draw2DLine(const C2DVector& from, const C2DVector& to, const CRGBColour& colour, unsigned long flags);   // 0x009DD960
    bool DrawArc(const C2DVector& centre, float radius, const CRGBColour& colour, unsigned long flags);
};

bool CRenderManager2D::DrawArc(const C2DVector& centre, float radius, const CRGBColour& colour, unsigned long flags)
{
    float cx = GFVXToAX(centre.X);
    float cy = GFVYToAY(centre.Y);
    float r = GFVXToAX(radius);
    float pi = (float)(asin(1.0) * 2);
    long segments = (long)(pi * r * 0.6666667f);
    for (long i = 0; i < segments; ++i) {
        float a0 = i * pi * 2 / segments;
        float a1 = (i + 1) * pi * 2 / segments;
        Draw2DLine(C2DVector(GFAXToVX(sin(a0) * r + cx), GFAYToVY(cos(a0) * r + cy)),
                   C2DVector(GFAXToVX(sin(a1) * r + cx), GFAYToVY(cos(a1) * r + cy)), colour, flags);
    }
    return true;
}