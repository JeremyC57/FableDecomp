// CTCHitLocations::GetBoundingSphere(C3DVector&, float&, bool) const -- the sphere enclosing every hit location's primitive sphere, cached as an offset from the thing's position
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00815420, 498 bytes (+ int3 padding), `ret 0Ch`.
// Leaf: GetBoundingSphere
// Grade: DIFFER(498v498) -- register assignment: retail saves only edi and keeps `this` in edi / the iterator in ebx, ours
// pushes ebx+edi with `this` in ebx; the squared-length order matches. Variant probe 5420d (494) gets the shrink-wrap right but
// swaps this/it registers and the x-term operand order.
// Layout: local model -- CTCHitLocations HitLocations vector of 0x1c-byte pair<CHitLocation,long> at +0x0c, mutable cache
// BoundingSphereOffsetFromPos +0x20, Radius +0x2c, Cached +0x30; primitive vtable slot 11 GetBoundingSphere.
#include <math.h>

struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
    C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    C3DVector& operator+=(const C3DVector& v) { X = v.X + X; Y = v.Y + Y; Z = v.Z + Z; return *this; }
};

struct CThing {
    const C3DVector& GetPosition() const;       // 0x004C73D0
};

namespace NHitLocations {

struct CPhysicalPrimitiveBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10();
    virtual void GetBoundingSphere(const CThing& owner, C3DVector& centre, float& radius) const;   // slot 11
};

struct CHitLocationDef {
    unsigned char           _pad_0x00[0x3c];
    CPhysicalPrimitiveBase* PhysicalPrimitive;   // +0x3c
};

struct CHitLocation {
    CThing*                POwnerThing;         // +0x00
    const CHitLocationDef* PDef;                // +0x04
    void*                  PArmourDef;          // +0x08
    long                   HitResponseAction;   // +0x0c
    long                   BlockResponseAction; // +0x10
    bool                   Enabled;             // +0x14
};

}

struct SHitLocationEntry {                        // std::pair<NHitLocations::CHitLocation, long>
    NHitLocations::CHitLocation first;
    long                        second;          // +0x18 priority
};

struct CTCHitLocations {
    void*              __vftable;                // +0x00
    CThing*            ParentThing;              // +0x04
    unsigned char      _pad_0x08[0x4];
    SHitLocationEntry* HitLocations_First;       // +0x0c
    SHitLocationEntry* HitLocations_Last;        // +0x10
    SHitLocationEntry* HitLocations_End;         // +0x14
    void*              PDefaultHitLocation;      // +0x18
    void*              PDef;                     // +0x1c
    mutable C3DVector  BoundingSphereOffsetFromPos;   // +0x20
    mutable float      BoundingSphereRadius;     // +0x2c
    mutable bool       CachedBoundingSphere;     // +0x30
    void GetBoundingSphere(C3DVector& centre, float& radius, bool recalculate) const;
};

void CTCHitLocations::GetBoundingSphere(C3DVector& centre, float& radius, bool recalculate) const
{
    if (HitLocations_First == HitLocations_Last)
        return;
    if (!recalculate && CachedBoundingSphere) {
        centre = ParentThing->GetPosition() + BoundingSphereOffsetFromPos;
        radius = BoundingSphereRadius;
        return;
    }
    SHitLocationEntry* it = HitLocations_First;
    it->first.PDef->PhysicalPrimitive->GetBoundingSphere(*it->first.POwnerThing, centre, radius);
    for (++it; it != HitLocations_Last; ++it) {
        C3DVector c;
        float r;
        it->first.PDef->PhysicalPrimitive->GetBoundingSphere(*it->first.POwnerThing, c, r);
        C3DVector d = c - centre;
        float len = (float)sqrt(d.Z * d.Z + d.Y * d.Y + d.X * d.X);
        if (len > 0.0001f) {
            float inv = 1.0f / len;
            d.X *= inv;
            d.Y *= inv;
            d.Z *= inv;
        }
        if (!(r + len < radius)) {
            float newRadius = (len + radius + r) * 0.5f;
            float shift = newRadius - radius;
            if (shift < 0.0f)
                shift = 0.0f;
            centre += d * shift;
            radius = newRadius;
        }
    }
    BoundingSphereOffsetFromPos = centre - ParentThing->GetPosition();
    BoundingSphereRadius = radius;
    CachedBoundingSphere = true;
}
