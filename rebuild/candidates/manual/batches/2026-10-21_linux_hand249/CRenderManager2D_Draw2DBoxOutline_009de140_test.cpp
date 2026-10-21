#include <cstdio>
struct C2DVector {
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) {}
};
struct C2DBoxF { float TLX, TLY, BRX, BRY; };
struct CRGBColour { unsigned char B, G, R, A; };
float GFVXToAX(float x) { return x * 0.5f; }
float GFVYToAY(float y) { return y * 0.25f; }

struct CRenderManager2D {
    float seg[8][4]; int n; int failAt;
    bool Draw2DLine(const C2DVector& from, const C2DVector& to, const CRGBColour&, unsigned long)
    {
        seg[n][0] = from.X; seg[n][1] = from.Y; seg[n][2] = to.X; seg[n][3] = to.Y;
        return ++n != failAt;
    }
    bool Draw2DBoxOutline(const C2DBoxF& box, const CRGBColour& colour, unsigned long flags);
};

bool CRenderManager2D::Draw2DBoxOutline(const C2DBoxF& box, const CRGBColour& colour, unsigned long flags)
{
    long top = (long)box.TLY;
    long left = (long)box.TLX;
    long right = (long)(box.BRX - GFVXToAX(1.0f));
    long bottom = (long)(box.BRY - GFVYToAY(1.0f));
    if (Draw2DLine(C2DVector((float)left, (float)top), C2DVector((float)right, (float)top), colour, flags) == true
        && Draw2DLine(C2DVector((float)right, (float)top), C2DVector((float)right, (float)bottom), colour, flags) == true
        && Draw2DLine(C2DVector((float)right, (float)bottom), C2DVector((float)left, (float)bottom), colour, flags) == true
        && Draw2DLine(C2DVector((float)left, (float)bottom), C2DVector((float)left, (float)top), colour, flags) == true)
        return true;
    return false;
}

int main()
{
    CRenderManager2D rm; rm.n = 0; rm.failAt = 0;
    C2DBoxF box = { 1.7f, 2.2f, 10.9f, 20.6f };
    CRGBColour c = { 0, 0, 0, 0 };
    bool ok = rm.Draw2DBoxOutline(box, c, 0) && rm.n == 4;
    // left 1, top 2, right (long)(10.9 - 0.5) = 10, bottom (long)(20.6 - 0.25) = 20
    const float expect[4][4] = { { 1, 2, 10, 2 }, { 10, 2, 10, 20 }, { 10, 20, 1, 20 }, { 1, 20, 1, 2 } };
    for (int i = 0; ok && i < 4; ++i)
        for (int k = 0; k < 4; ++k)
            ok = ok && rm.seg[i][k] == expect[i][k];
    CRenderManager2D bad; bad.n = 0; bad.failAt = 2;
    ok = ok && !bad.Draw2DBoxOutline(box, c, 0) && bad.n == 2;
    if (!ok) { std::printf("009de140_TEST FAIL\n"); return 1; }
    std::printf("009de140_TEST PASS\n");
    return 0;
}
