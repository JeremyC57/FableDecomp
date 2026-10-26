#include <cstdio>
#include <cmath>
class C2DVector { public:
    float X, Y;
    float GetLength() const { return (float)std::sqrt(X * X + Y * Y); }
};
struct SRegionLookupEntry00c46570 {
    long Region; C2DVector Position; float Length;
    SRegionLookupEntry00c46570(const C2DVector& position) : Region(0), Position(position), Length(position.GetLength()) { }
};
int main()
{
    C2DVector v = { 3.0f, 4.0f };
    SRegionLookupEntry00c46570 e(v);
    if (e.Region != 0 || e.Position.X != 3.0f || e.Position.Y != 4.0f || e.Length != 5.0f) { std::printf("00c46570_TEST FAIL\n"); return 1; }
    std::printf("00c46570_TEST PASS\n");
    return 0;
}