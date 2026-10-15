// Behaviour test for 0x00B493C0 CManagerShadowedSpotLight value constructor: every field takes its argument.
#include <cstdio>
struct C3DVector { float X, Y, Z; C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { } C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { } };
struct CRGBFloatColour { float R, G, B, A; };
struct CManagerShadowedSpotLight {
    virtual ~CManagerShadowedSpotLight() { }
    C3DVector Position, Direction;
    float Radius, SpotRadius, ConeRadius;
    CRGBFloatColour Colour;
    CManagerShadowedSpotLight(const C3DVector& p, const C3DVector& d, float r, float s, float c, const CRGBFloatColour& col)
        : Position(p), Direction(d), Radius(r), SpotRadius(s), ConeRadius(c), Colour(col) { }
};
int main()
{
    CRGBFloatColour col = { 0.25f, 0.5f, 0.75f, 1.0f };
    CManagerShadowedSpotLight l(C3DVector(1, 2, 3), C3DVector(0, 0, -1), 10.0f, 4.0f, 0.5f, col);
    if (l.Position.X != 1 || l.Position.Z != 3 || l.Direction.Z != -1 || l.Radius != 10.0f || l.SpotRadius != 4.0f ||
        l.ConeRadius != 0.5f || l.Colour.R != 0.25f || l.Colour.A != 1.0f) {
        std::printf("00b493c0_TEST FAIL\n");
        return 1;
    }
    std::printf("00b493c0_TEST PASS\n");
    return 0;
}
