#include <cstdio>
#include <math.h>
struct C2DVector {
    float X, Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
};
struct CRGBColour { unsigned char B, G, R, A; };
float GFAXToVX(float x) { return x * 2.0f; }
float GFAYToVY(float y) { return y * 2.0f; }
float GFVXToAX(float x) { return x * 0.5f; }
float GFVYToAY(float y) { return y * 0.5f; }

struct CRenderManager2D {
    int lines; float maxErr; C2DVector first, last; unsigned long gotFlags;
    bool Draw2DLine(const C2DVector& from, const C2DVector& to, const CRGBColour& colour, unsigned long flags)
    {
        if (lines == 0) first = from;
        last = to; ++lines; gotFlags = flags;
        float d1 = (float)sqrt((from.X - 10) * (from.X - 10) + (from.Y - 20) * (from.Y - 20)) - 5;
        float d2 = (float)sqrt((to.X - 10) * (to.X - 10) + (to.Y - 20) * (to.Y - 20)) - 5;
        if (fabs(d1) > maxErr) maxErr = (float)fabs(d1);
        if (fabs(d2) > maxErr) maxErr = (float)fabs(d2);
        return true;
    }
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

int main()
{
    CRenderManager2D rm; rm.lines = 0; rm.maxErr = 0;
    C2DVector c(10, 20); CRGBColour col = { 1, 2, 3, 4 };
    bool ok = rm.DrawArc(c, 5, col, 9);
    long expect = (long)((float)(asin(1.0) * 2) * 10 * 0.6666667f);   // radius 5 -> 10 screen units
    ok = ok && rm.lines == expect && rm.maxErr < 1e-3f && rm.gotFlags == 9
            && fabs(rm.first.X - 10) < 1e-3f && fabs(rm.first.Y - 25) < 1e-3f      // angle 0: (sin 0, cos 0)
            && fabs(rm.last.X - 10) < 1e-3f && fabs(rm.last.Y - 25) < 1e-3f;       // closes at 2*pi
    CRenderManager2D none; none.lines = 0; none.maxErr = 0;
    ok = ok && none.DrawArc(c, 0.1f, col, 0) && none.lines == 0;
    if (!ok) { std::printf("009de2a0_TEST FAIL\n"); return 1; }
    std::printf("009de2a0_TEST PASS\n");
    return 0;
}
