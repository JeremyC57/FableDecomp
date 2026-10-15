// Behaviour test for 0x00B49950 CShaderDirectionalLight constructor.
#include <cstdio>
struct C3DVector { float X, Y, Z; C3DVector() {} C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {} C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {} };
struct CRGBFloatColour { float R, G, B, A; };
struct CShaderDirectionalLight {
    C3DVector Direction; C3DVector Position; CRGBFloatColour Colour; CRGBFloatColour BackColour;
    CShaderDirectionalLight(const C3DVector& d, const CRGBFloatColour& c, const CRGBFloatColour& b) : Direction(d), Colour(c), BackColour(b) {}
};
int main()
{
    CRGBFloatColour c = { 0.1f, 0.2f, 0.3f, 1.0f }, b = { 0.5f, 0.6f, 0.7f, 0.8f };
    CShaderDirectionalLight l(C3DVector(0, -1, 0), c, b);
    if (l.Direction.Y != -1 || l.Colour.G != 0.2f || l.BackColour.A != 0.8f || l.BackColour.R != 0.5f) { std::printf("00b49950_TEST FAIL\n"); return 1; }
    std::printf("00b49950_TEST PASS\n");
    return 0;
}
