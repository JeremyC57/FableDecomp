// Grade: DIFFER(787v831) -- lane gap: retail's box-to-sphere squared distance is a hand-written inline-assembly SSE block (movss/movhps loads through frame slots, maxps/minps clamp, shufps reduction); the intrinsic spelling compiles differently; the x87 clamp, far-corner test and plane loop are ordinary C++ (x87 operand order not tuned)
// CShadowClipVolume::ClipBoundingBox(const CAABox& box, bool* fullyInside) const -- false when the box lies outside the volume's bounding sphere or entirely in front of one of its planes; *fullyInside stays true only when the box's farthest corner is inside the sphere and every plane's far vertex is behind its plane
// Leaf: ClipBoundingBox
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B4CED0, 829 bytes (+ int3 padding), `ret 8`.
// Callees: none (fabs inline).
// Globals: SSE-available flag 0x013D2880, 0.0f at 0x0122DEDC.
// Layout (descriptive): CShadowClipVolume = 8 planes (normal + distance, 16 bytes each) +0x00, PlaneCount (long) +0x80,
//   SphereCentre (C3DVector) +0x84, SphereRadius +0x90, HasSphere (bool) +0x94; CAABox = Min +0x0, Max +0xc.
// Shape: retail's box-to-centre squared distance has a hand-written inline-assembly SSE branch (movss/movhps loads,
//   maxps/minps clamp, shufps reduction); this C++ form spells it with SSE intrinsics, the x87 branch as the clamp.
#include <math.h>
#include <xmmintrin.h>
struct C3DVector {
    float X, Y, Z;
};
struct CAABox {
    C3DVector Min;                               // +0x0
    C3DVector Max;                               // +0xc
};
struct CClipPlane {
    C3DVector Normal;                            // +0x0
    float     Distance;                          // +0xc
};
extern bool g_SSEAvailable;                      // 0x013D2880
__forceinline float SquaredDistance(const CAABox& box, const C3DVector& p)
{
    if (g_SSEAvailable) {
        __m128 mn = _mm_loadh_pi(_mm_load_ss(&box.Min.Z), (const __m64*)&box.Min.X);
        __m128 mx = _mm_loadh_pi(_mm_load_ss(&box.Max.Z), (const __m64*)&box.Max.X);
        __m128 v = _mm_loadh_pi(_mm_load_ss(&p.Z), (const __m64*)&p.X);
        __m128 d = _mm_sub_ps(_mm_min_ps(_mm_max_ps(v, mn), mx), v);
        d = _mm_mul_ps(d, d);
        __m128 s = _mm_add_ps(_mm_shuffle_ps(d, d, 0xb0), d);
        s = _mm_add_ss(_mm_shuffle_ps(s, s, 3), d);
        float result;
        _mm_store_ss(&result, s);
        return result;
    }
    C3DVector c = p;
    if (c.X < box.Min.X)
        c.X = box.Min.X;
    else if (c.X > box.Max.X)
        c.X = box.Max.X;
    if (c.Y < box.Min.Y)
        c.Y = box.Min.Y;
    else if (c.Y > box.Max.Y)
        c.Y = box.Max.Y;
    if (c.Z < box.Min.Z)
        c.Z = box.Min.Z;
    else if (c.Z > box.Max.Z)
        c.Z = box.Max.Z;
    float dx = p.X - c.X, dy = p.Y - c.Y, dz = p.Z - c.Z;
    return dz * dz + dy * dy + dx * dx;
}
struct CShadowClipVolume {
    CClipPlane    Planes[8];                     // +0x00
    long          PlaneCount;                    // +0x80
    C3DVector     SphereCentre;                  // +0x84
    float         SphereRadius;                  // +0x90
    bool          HasSphere;                     // +0x94
    bool ClipBoundingBox(const CAABox& box, bool* fullyInside) const;
};

bool CShadowClipVolume::ClipBoundingBox(const CAABox& box, bool* fullyInside) const
{
    *fullyInside = true;
    if (HasSphere) {
        if (SquaredDistance(box, SphereCentre) > SphereRadius * SphereRadius) {
            *fullyInside = false;
            return false;
        }
        const C3DVector& c = SphereCentre;
        float fx = fabsf(c.X - box.Min.X) > fabsf(c.X - box.Max.X) ? box.Min.X : box.Max.X;
        float fy = fabsf(c.Y - box.Min.Y) > fabsf(c.Y - box.Max.Y) ? box.Min.Y : box.Max.Y;
        float fz = fabsf(c.Z - box.Min.Z) > fabsf(c.Z - box.Max.Z) ? box.Min.Z : box.Max.Z;
        float dx = c.X - fx, dy = c.Y - fy, dz = c.Z - fz;
        if (dz * dz + dy * dy + dx * dx > SphereRadius * SphereRadius)
            *fullyInside = false;
    }
    for (long i = 0; i < PlaneCount; ++i) {
        const CClipPlane& plane = Planes[i];
        const C3DVector& n = plane.Normal;
        float vx = n.X > 0.0f ? box.Min.X : box.Max.X;
        float vy = n.Y > 0.0f ? box.Min.Y : box.Max.Y;
        float vz = n.Z > 0.0f ? box.Min.Z : box.Max.Z;
        if (vz * n.Z + vy * n.Y + vx * n.X > plane.Distance) {
            *fullyInside = false;
            return false;
        }
        if (*fullyInside) {
            float wx = n.X > 0.0f ? box.Max.X : box.Min.X;
            float wy = n.Y > 0.0f ? box.Max.Y : box.Min.Y;
            float wz = n.Z > 0.0f ? box.Max.Z : box.Min.Z;
            if (wz * n.Z + wy * n.Y + wx * n.X > plane.Distance)
                *fullyInside = false;
        }
    }
    return true;
}
