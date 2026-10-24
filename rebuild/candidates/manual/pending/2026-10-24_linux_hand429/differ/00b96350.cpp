// CShadowClipVolume::ClipSphere(const C3DBoundingSphere& sphere) const -- whether a bounding sphere can touch the shadow clip volume: rejected when it lies outside the volume's own bounding sphere (if clipping to it) or wholly in front of any clip plane
// Grade: DIFFER(208v208) -- only the plane dot product's x87 order: retail multiplies Z, then X, adds, then Y (induction pointer at the plane's Y); `X*v.X + Y*v.Y + Z*v.Z` gives Y, X, then Z (pointer at Z), whether written centre.Dot(normal) or normal.Dot(centre). Tried: forwarding to an inline ClipSphere(const C3DVector&, float) (left a call, 17v208), centre-before-radius locals (esi pushed at entry). Next: a Dot spelled with another term order (e.g. `X*v.X + Z*v.Z + Y*v.Y`), or the plane loop over a `const CPlane&` local.
// Leaf: ?ClipSphere@CShadowClipVolume@@QBE_NABVC3DBoundingSphere
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B96350, 208 bytes, thiscall, `ret 4`, bool in al.
// Identity: Ego_d.pdb / FableWin.pdb ?ClipSphere@CShadowClipVolume@@QBE_NABVC3DBoundingSphere@@@Z (a sibling
//   ?ClipSphere@...@@QBE_NABVC3DVector@@M@Z overload exists; forwarding to it inline leaves it a call); Ego_r.pdb CShadowClipVolume layout
//   (152 bytes): ClipPlanes[8] (16-byte CPlane {Normal, D}) +0, ClipPlaneCount +0x80, BoundingSphere +0x84 (radius +0x90),
//   ClipToSphere +0x94. No direct callers in retail. Catalog "CFrustum_CheckSphereVisibility" is descriptive.
// Shape: the sphere is copied into locals, radius first (into the dead argument slot), then the centre; the
//   sphere-in-sphere test is `d.GetLengthSquared() > r * r` (squares Z, Y, X as retail's single-expression sum) with its
//   own early `return false` before esi is pushed; each plane test is `centre.Dot(normal) - radius > D`.
class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    float Dot(const C3DVector& v) const { return X * v.X + Y * v.Y + Z * v.Z; }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
};

class CPlane {
public:
    C3DVector Normal;                              // +0x00
    float     D;                                   // +0x0c
};

class C3DBoundingSphere {
public:
    C3DVector Center;                              // +0x00
    float     Radius;                              // +0x0c
};

class CShadowClipVolume {                          // Ego_r.pdb, 152 bytes
public:
    CPlane            ClipPlanes[8];               // +0x00
    long              ClipPlaneCount;              // +0x80
    C3DBoundingSphere BoundingSphere;              // +0x84
    bool              ClipToSphere;                // +0x94

    bool ClipSphere(const C3DBoundingSphere& sphere) const;
};

bool CShadowClipVolume::ClipSphere(const C3DBoundingSphere& sphere) const
{
    float radius = sphere.Radius;
    C3DVector centre = sphere.Center;
    if (ClipToSphere) {
        C3DVector d = centre - BoundingSphere.Center;
        float r = radius + BoundingSphere.Radius;
        if (d.GetLengthSquared() > r * r)
            return false;
    }
    for (long i = 0; i < ClipPlaneCount; ++i)
        if (centre.Dot(ClipPlanes[i].Normal) - radius > ClipPlanes[i].D)
            return false;
    return true;
}
