#include <cstdio>
#include <cstring>
#include <math.h>
static int g_destroyed;
const float g_epsilon_0129ba3c = 1e-4f;

struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
};

class CThingDef {
public:
    virtual ~CThingDef() {}
    virtual void Destroy() { ++g_destroyed; }
    long          RefCount;                             // +0x04
    unsigned char _pad08[0x5c - 0x08];
    float         CylinderHeight;                       // +0x5c
};
struct CDefPointer {
    CThingDef* Object;
    CDefPointer(const CDefPointer& o) : Object(o.Object) { if (Object) ++Object->RefCount; }
    ~CDefPointer() { if (Object && --Object->RefCount == 0) Object->Destroy(); }
    CThingDef* operator->() const { return Object; }
};
struct CThingGraphic {
    unsigned char _pad00[0x34];
    float         Radius;                               // +0x34
};
class CThing {
public:
    unsigned char  _pad00[0x60];
    CThingGraphic* PGraphic;                            // +0x60
    unsigned char  _pad64[0x6c - 0x64];
    unsigned char  _flag0 : 1, _flag1 : 1, _flag2 : 1, IsCreature : 1;   // +0x6c
    unsigned char  _pad6d[0x70 - 0x6d];
    CDefPointer    PDef;                                // +0x70
};
class CThingCreatureBase : public CThing {
public:
    float GetCombatRadius() const { return 7.0f; }
};

namespace NHitLocations {

class CPhysicalPrimitiveBase {
public:
    virtual void GetBoundingSphere(const CThing& thing, C3DVector& centre, float& radius) const = 0;
};

class CPhysicalPrimitiveCylinder : public CPhysicalPrimitiveBase {
public:
    void* BaseName;                                     // +0x04 CDefString
    float Radius;                                       // +0x08
    float Height;                                       // +0x0c
    void GetCylinderBaseAndTop(const CThing& thing, C3DVector& base, C3DVector& top) const { (void)thing; base = C3DVector(1, 2, 3); top = C3DVector(1, 2, 3 + 8); }
    float GetCylinderHeight(const CThing& thing) const
    {
        CDefPointer def = thing.PDef;
        float height = Height;
        if (height < g_epsilon_0129ba3c)
            height = def->CylinderHeight;
        return height;
    }
    float GetCylinderRadius(const CThing& thing) const
    {
        float radius = Radius;
        if (radius < g_epsilon_0129ba3c) {
            radius = thing.PGraphic->Radius;
            if (thing.IsCreature)
                radius = static_cast<const CThingCreatureBase&>(thing).GetCombatRadius();
        }
        return radius;
    }
    virtual void GetBoundingSphere(const CThing& thing, C3DVector& centre, float& radius) const;
};

void CPhysicalPrimitiveCylinder::GetBoundingSphere(const CThing& thing, C3DVector& centre, float& radius) const
{
    C3DVector base;
    {
        C3DVector top;
        GetCylinderBaseAndTop(thing, base, top);
        centre = (base + top) * 0.5f;
    }
    float halfHeight = GetCylinderHeight(thing) * 0.5f;
    float cylinderRadius = GetCylinderRadius(thing);
    radius = (float)sqrt(cylinderRadius * cylinderRadius + halfHeight * halfHeight);
}

} // namespace NHitLocations

using namespace NHitLocations;
int main()
{
    CThingDef def; def.RefCount = 1; def.CylinderHeight = 6.0f;
    CThingGraphic graphic; graphic.Radius = 4.0f;
    static unsigned char raw[sizeof(CThingCreatureBase)];
    std::memset(raw, 0, sizeof(raw));
    CThingCreatureBase& thing = *(CThingCreatureBase*)raw;
    thing.PGraphic = &graphic; thing.PDef.Object = &def; thing.IsCreature = 0;
    CPhysicalPrimitiveCylinder cyl; cyl.Radius = 3.0f; cyl.Height = 8.0f;
    C3DVector centre; float radius = 0;
    cyl.GetBoundingSphere(thing, centre, radius);
    if (centre.X != 1 || centre.Y != 2 || centre.Z != 7 || radius != 5.0f || def.RefCount != 1 || g_destroyed != 0) { std::printf("00815aa0_TEST FAIL\n"); return 1; }
    cyl.Radius = 0.0f; cyl.Height = 0.0f;   /* defaults: graphic radius 4, def height 6 -> sqrt(16 + 9) */
    cyl.GetBoundingSphere(thing, centre, radius);
    if (radius != 5.0f) { std::printf("00815aa0_TEST FAIL\n"); return 1; }
    thing.IsCreature = 1;   /* creature: combat radius 7, half height 3 */
    cyl.GetBoundingSphere(thing, centre, radius);
    if (radius < 7.6157f || radius > 7.6158f) { std::printf("00815aa0_TEST FAIL\n"); return 1; }
    std::printf("00815aa0_TEST PASS\n");
    return 0;
}