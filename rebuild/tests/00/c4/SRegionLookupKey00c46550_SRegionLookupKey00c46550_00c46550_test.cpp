// Behaviour test for the SRegionLookupKey00c46550 constructor @ 0x00C46550: the vector copied, its length appended.
#include <cstdio>
#include <cmath>

struct C2DVector {
    float X, Y;
    float GetLength() const { return (float)std::sqrt(X * X + Y * Y); }
};
struct SRegionLookupKey00c46550 {
    C2DVector Position; float Length;
    SRegionLookupKey00c46550(const C2DVector& position) : Position(position), Length(position.GetLength()) {}
};

int main()
{
    C2DVector v; v.X = 3.0f; v.Y = -4.0f;
    SRegionLookupKey00c46550 k(v);
    if (k.Position.X != 3.0f || k.Position.Y != -4.0f || k.Length != 5.0f) { std::printf("00c46550_TEST FAIL\n"); return 1; }
    std::printf("00c46550_TEST PASS\n");
    return 0;
}