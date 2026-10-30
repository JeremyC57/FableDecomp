// CTCHitLocations::GetBoundingSphere(C3DVector&, float&, bool) const -- the sphere enclosing every hit location's primitive sphere, cached as an offset from the thing's position
// Grade: DIFFER(498v498) -- this sweep: a named `const C3DVector& pos` in the cached path fixed the position+offset x87 order (diff 32 -> 30 lines); left: retail keeps begin() in ebx for the first element (mov eax,ebx; add ebx,1Ch before the call) and the squared-length fst order; tried first pointer + ++it, (it++)->first reference
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00815420, 498 bytes (+ int3 padding), `ret 0Ch`.
// Leaf: ?GetBoundingSphere@CTCHitLocations@@QBEXAAVC3DVector@@AAM_N@Z
// Layout: local model -- CTCHitLocations HitLocations vector of 0x1c-byte pair<CHitLocation,long> at +0x0c, mutable cache
// BoundingSphereOffsetFromPos +0x20, Radius +0x2c, Cached +0x30; primitive vtable slot 11 GetBoundingSphere.
#include <math.h>

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
    C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    C3DVector& operator+=(const C3DVector& v) { X = v.X + X; Y = v.Y + Y; Z = v.Z + Z; return *this; }
};

class CThing {
public:
    const C3DVector& GetPos() const;            // 0x004C73D0 ?GetPos@CThing@@QBEABVC3DVector@@XZ (settled)
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

class CTCHitLocations {
public:
    void*              __vftable;                // +0x00
    CThing*            ParentThing;              // +0x04
    unsigned char      _pad_0x08[0x4];
    struct {
        SHitLocationEntry* _M_start;
        SHitLocationEntry* _M_finish;
        SHitLocationEntry* _M_end_of_storage;
        bool empty() const { return _M_start == _M_finish; }
        const SHitLocationEntry* begin() const { return _M_start; }
        const SHitLocationEntry* end() const { return _M_finish; }
        unsigned int size() const { return _M_finish - _M_start; }
        const SHitLocationEntry& operator[](unsigned int n) const { return *(begin() + n); }
        const SHitLocationEntry& front() const { return *begin(); }
    } HitLocations;                              // +0x0c
    void*              PDefaultHitLocation;      // +0x18
    void*              PDef;                     // +0x1c
    mutable C3DVector  BoundingSphereOffsetFromPos;   // +0x20
    mutable float      BoundingSphereRadius;     // +0x2c
    mutable bool       CachedBoundingSphere;     // +0x30
    void GetBoundingSphere(C3DVector& centre, float& radius, bool recalculate) const;

};

void CTCHitLocations::GetBoundingSphere(C3DVector& centre, float& radius, bool recalculate) const
{
    if (HitLocations.empty())
        return;
    if (!recalculate && CachedBoundingSphere) {
        const C3DVector& pos = ParentThing->GetPos();
        centre = pos + BoundingSphereOffsetFromPos;
        radius = BoundingSphereRadius;
        return;
    }
    HitLocations.front().first.PDef->PhysicalPrimitive->GetBoundingSphere(*HitLocations.front().first.POwnerThing, centre, radius);
    for (const SHitLocationEntry* it = HitLocations.begin() + 1; it != HitLocations.end(); ++it) {
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
    BoundingSphereOffsetFromPos = centre - ParentThing->GetPos();
    BoundingSphereRadius = radius;
    CachedBoundingSphere = true;
}
