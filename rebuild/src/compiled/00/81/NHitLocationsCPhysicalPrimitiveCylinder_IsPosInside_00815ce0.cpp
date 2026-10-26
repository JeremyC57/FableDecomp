// NHitLocations::CPhysicalPrimitiveCylinder::IsPosInside(const CThing& thing, const C3DVector& pos) -- true when the position lies within the thing's cylinder: its projection on the base-to-top axis is inside the length and its distance from the axis is below the radius (each widened by the epsilon)
// Leaf: ?IsPosInside@CPhysicalPrimitiveCylinder@
#pragma intrinsic(sqrt)
extern "C" double __cdecl sqrt(double);
extern float g_fEpsilon;                           // 0x0129BA3C
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    C3DVector& operator*=(float s) { X *= s; Y *= s; Z *= s; return *this; }
    C3DVector& operator-=(const C3DVector& o) { X -= o.X; Y -= o.Y; Z -= o.Z; return *this; }
    float operator*(const C3DVector& o) const { return Z * o.Z + Y * o.Y + X * o.X; }
    float MagSquared() const { return Z * Z + Y * Y + X * X; }
    float Length() const { return (float)sqrt(Z * Z + Y * Y + X * X); }
};
struct CThingDef {
    unsigned char _pad00[0x34];
    float         Radius;                          // +0x34
};
class CThing {
public:
    unsigned char _pad00[0x60];
    CThingDef*    Def;                             // +0x60
    unsigned char _pad64[8];
    unsigned char Flags;                           // +0x6c (bit 3: creature)
};
class CThingCreatureBase : public CThing {
public:
    float GetCombatRadius() const;                 // 0x00661F90
};
namespace NHitLocations {
struct CPhysicalPrimitiveCylinder {
    virtual void v00();
    void*         BaseName;                        // +0x04 CDefString
    float         Radius;                          // +0x08
    float         Height;                          // +0x0c
    void GetCylinderBaseAndTop(const CThing& thing, C3DVector& base, C3DVector& top) const;   // 0x008158D0
    float GetCylinderRadius(const CThing& thing) const
    {
        float radius = Radius;
        if (radius < g_fEpsilon) {
            radius = thing.Def->Radius;
            if (thing.Flags & 8)
                radius = static_cast<const CThingCreatureBase&>(thing).GetCombatRadius();
        }
        return radius;
    }
    bool IsPosInside(const CThing& thing, const C3DVector& pos) const;
};

bool CPhysicalPrimitiveCylinder::IsPosInside(const CThing& thing, const C3DVector& pos) const
{
    C3DVector base, top;
    GetCylinderBaseAndTop(thing, base, top);
    float radius = GetCylinderRadius(thing);
    C3DVector axis = top - base;
    float length = axis.Length();
    if (length > g_fEpsilon)
        axis *= 1.0f / length;
    C3DVector offset = pos - base;
    float along = offset * axis;
    if (-g_fEpsilon < along && along < g_fEpsilon + length) {
        offset -= axis * along;
        float limit = g_fEpsilon + radius;
        if (offset.MagSquared() < limit * limit)
            return true;
    }
    return false;
}
}