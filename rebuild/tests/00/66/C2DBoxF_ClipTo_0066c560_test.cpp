#include <cstdio>
struct C2DBoxF {
    float X0, Y0, X1, Y1;
    void ClipTo(const C2DBoxF& other);
};
void C2DBoxF::ClipTo(const C2DBoxF& other)
{
    if (X0 < other.X0) X0 = other.X0;
    else if (X0 > other.X1) X0 = other.X1;
    if (Y0 < other.Y0) Y0 = other.Y0;
    else if (Y0 > other.Y1) Y0 = other.Y1;
    if (X1 < other.X0) X1 = other.X0;
    else if (X1 > other.X1) X1 = other.X1;
    if (Y1 < other.Y0) Y1 = other.Y0;
    else if (Y1 > other.Y1) Y1 = other.Y1;
}
int main()
{
    C2DBoxF clip = { 0.0f, 0.0f, 10.0f, 20.0f };
    C2DBoxF b = { -5.0f, 25.0f, 15.0f, 5.0f };
    b.ClipTo(clip);
    C2DBoxF c = { 3.0f, 4.0f, 5.0f, 6.0f };
    c.ClipTo(clip);
    if (b.X0 != 0.0f || b.Y0 != 20.0f || b.X1 != 10.0f || b.Y1 != 5.0f ||
        c.X0 != 3.0f || c.Y0 != 4.0f || c.X1 != 5.0f || c.Y1 != 6.0f) {
        std::printf("0066c560_TEST FAIL\n"); return 1;
    }
    std::printf("0066c560_TEST PASS\n");
    return 0;
}