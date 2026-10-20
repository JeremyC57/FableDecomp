// NHitLocations::CPhysicalPrimitiveCylinder::GetBoundingSphere(const CThing& thing, C3DVector& centre, float& radius) const -- the sphere around the hit cylinder: centred between its base and top, radius = sqrt(cylinderRadius^2 + (height / 2)^2), with the radius and height defaulting to the thing's values when they are not set
// Leaf: GetBoundingSphere
// Retail: Fable.exe (TLC, VC7.1) @ 0x00815AA0, 256 bytes, `ret 0Ch`. PDB:
//   ?GetBoundingSphere@CPhysicalPrimitiveCylinder@NHitLocations@@UBEXABVCThing@@AAVC3DVector@@AAM@Z.
// Callees: CPhysicalPrimitiveCylinder::GetCylinderBaseAndTop(const CThing&, C3DVector&, C3DVector&) const 0x008158D0 (PDB;
//   catalogued "GetDummyObjectOriginTransformed"), CThingCreatureBase::GetCombatRadius 0x00661F90, the thing def's virtual
//   slot 1 (+4, destroy at refcount 0). Constants: 0x0129BA3C (the 1e-4f epsilon variable), 0x0122F59C = 0.5f.
// Layout: Ego_r.pdb CPhysicalPrimitiveCylinder {vptr, BaseName +4, Radius +8, Height +0x0C}; the thing's def pointer at
//   +0x70 (refcount at def+4, default cylinder height at def+0x5C), its graphic at +0x60 (radius +0x34), the creature
//   flag bit 3 of the byte at +0x6C (descriptive names).
// Shape: the PDB's GetCylinderHeight / GetCylinderRadius are inline; the height copies the def pointer (add-ref, short
//   `dec [ecx+4] ; jne` release) and lives in the dead `thing` slot; the centre is the inline `(base + top) * 0.5f` with `top` in an inner
//   block (its slot then holds the product temporary, retail's 0x24 frame; both outer gives 0x30, both inner 0x18).
#include <math.h>
#pragma intrinsic(sqrt)

extern const float g_epsilon_0129ba3c;                  // 0x0129BA3C (1e-4f)

struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
};

class CThingDef {
public:
    virtual void v00();
    virtual void Destroy();                             // slot 1 (+0x04)
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
    float GetCombatRadius() const;                      // 0x00661F90
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
    void GetCylinderBaseAndTop(const CThing& thing, C3DVector& base, C3DVector& top) const;   // 0x008158D0
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
