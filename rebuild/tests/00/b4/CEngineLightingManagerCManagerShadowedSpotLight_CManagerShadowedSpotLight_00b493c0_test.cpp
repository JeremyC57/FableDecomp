// behaviour test for 0x00B493C0 CEngineLightingManager::CManagerShadowedSpotLight value constructor, linked against the
// built constructor: every field takes its argument and the vtable is installed
#include <cstdio>
#include <cstring>
#include <new>
class C3DVector { public: float X, Y, Z; C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { } C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { } };
class CRGBFloatColour { public: float R, G, B, A; };
class CEngineLightingManager {
public:
    struct CManagerShadowedSpotLight {
        virtual void Render();
        C3DVector Position, Direction;
        float Radius, SpotRadius, ConeRadius;
        CRGBFloatColour Colour;
        CManagerShadowedSpotLight(const C3DVector& p, const C3DVector& d, float r, float s, float c, const CRGBFloatColour& col);
    };
};
void CEngineLightingManager::CManagerShadowedSpotLight::Render() { }
int main()
{
    typedef CEngineLightingManager::CManagerShadowedSpotLight L;
    if (sizeof(L) != 0x38) { std::printf("00b493c0_TEST FAIL size\n"); return 1; }
    CRGBFloatColour col = { 0.25f, 0.5f, 0.75f, 1.0f };
    unsigned char buf[sizeof(L)];
    std::memset(buf, 0xCD, sizeof(buf));
    L* l = new (buf) L(C3DVector(1, 2, 3), C3DVector(0, 0, -1), 10.0f, 4.0f, 0.5f, col);
    if (l->Position.X != 1 || l->Position.Y != 2 || l->Position.Z != 3 || l->Direction.Z != -1 || l->Radius != 10.0f || l->SpotRadius != 4.0f ||
        l->ConeRadius != 0.5f || l->Colour.R != 0.25f || l->Colour.G != 0.5f || l->Colour.B != 0.75f || l->Colour.A != 1.0f
        || *(unsigned long*)buf == 0xCDCDCDCDu) {
        std::printf("00b493c0_TEST FAIL\n");
        return 1;
    }
    std::printf("00b493c0_TEST PASS\n");
    return 0;
}
