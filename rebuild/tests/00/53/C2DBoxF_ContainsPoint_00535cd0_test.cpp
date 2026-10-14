#include <cstdio>
struct C2DVector { float X; float Y; };
struct C2DBoxF {
    float TLX; float TLY; float BRX; float BRY;
    long ContainsPoint(const C2DVector& p) const;
};
long C2DBoxF::ContainsPoint(const C2DVector& p) const
{
    if (p.X >= TLX && p.X < BRX && p.Y >= TLY && p.Y < BRY)
        return 1;
    return 0;
}
int main()
{
    C2DBoxF b; b.TLX = 1.0f; b.TLY = 2.0f; b.BRX = 5.0f; b.BRY = 6.0f;
    C2DVector in = { 1.0f, 2.0f };      // on the top-left edge: inside (closed)
    C2DVector out1 = { 5.0f, 3.0f };    // on the bottom-right edge: outside (open)
    C2DVector out2 = { 0.5f, 3.0f };
    C2DVector out3 = { 3.0f, 6.5f };
    C2DVector mid = { 3.0f, 4.0f };
    if (b.ContainsPoint(in) != 1 || b.ContainsPoint(mid) != 1 || b.ContainsPoint(out1) != 0 ||
        b.ContainsPoint(out2) != 0 || b.ContainsPoint(out3) != 0) {
        std::printf("00535cd0_TEST FAIL\n"); return 1;
    }
    std::printf("00535cd0_TEST PASS\n");
    return 0;
}