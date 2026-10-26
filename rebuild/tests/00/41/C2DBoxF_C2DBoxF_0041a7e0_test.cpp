#include <cstdio>
// Behaviour test for C2DBoxF::C2DBoxF(const C2DVector&, const C2DExtentsF&) (retail 0x0041A7E0): the top-left corner
// is copied and the bottom-right corner is top-left plus the extents.
class C2DVector { public: float X, Y; };
class C2DExtentsF { public: float X, Y; };
class C2DBoxF { public:
    float TLX, TLY, BRX, BRY;
    C2DBoxF(const C2DVector& topLeft, const C2DExtentsF& extents)
        : TLX(topLeft.X), TLY(topLeft.Y), BRX(topLeft.X + extents.X), BRY(extents.Y + topLeft.Y) {}
};
int main()
{
    C2DVector tl; tl.X = 10.5f; tl.Y = -4.0f;
    C2DExtentsF ext; ext.X = 100.0f; ext.Y = 20.25f;
    C2DBoxF box(tl, ext);
    int ok = box.TLX == 10.5f && box.TLY == -4.0f && box.BRX == 110.5f && box.BRY == 16.25f;
    if (!ok) { std::printf("0041a7e0_TEST FAIL\n"); return 1; }
    std::printf("0041a7e0_TEST PASS\n");
    return 0;
}