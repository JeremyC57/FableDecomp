// CShadowClipVolume::ClipBoundingBox(const C3DBoundingBox& box) const -- false when the box is culled: outside the bounding sphere (if ClipToSphere) or entirely in front of any clip plane
// Grade: DIFFER(476v494) -- retail's SSE branch is an inline-asm block in the original (known lane gap): it re-reads
// the box argument and a spilled `centre` pointer local by name ([esp+8]), uses fixed xmm0..xmm3 with no 16-byte aligned
// frame, and hands the result back through a float local (`movss [esp+0Ch]` + `fld`); the intrinsics model below forces
// an `and esp,-16` frame and its own register allocation. The x87 fallback also differs (retail keeps the clamped Z on
// the FPU stack and spills d.X/d.Y). Not landable without asm; parked.
// Leaf: ClipBoundingBox
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B9C2E0, length 494 bytes (`ret 4`), ?ClipBoundingBox@CShadowClipVolume@@QBE_NABVC3DBoundingBox@@@Z.
// Layout: Ego_r.pdb CShadowClipVolume (ClipPlanes[8] +0, ClipPlaneCount +0x80, BoundingSphere +0x84, ClipToSphere +0x94);
// SSE-available flag 0x013D2880 (as in the landed UpdateCombinedProjectionTransform 0x00988A50); 0.0f 0x0122DEDC.
#include <xmmintrin.h>
extern bool g_SSEAvailable;                         // 0x013D2880
struct C3DVector {
    float X, Y, Z;
};
struct C3DBoundingBox {
    C3DVector MinCoord;                             // +0x00
    C3DVector MaxCoord;                             // +0x0c
};
struct C3DBoundingSphere {
    C3DVector Center;                               // +0x00
    float     Radius;                               // +0x0c
};
struct CPlane {
    C3DVector Normal;                               // +0x00
    float     Distance;                             // +0x0c
};
__forceinline float DistanceSquaredSSE(const C3DBoundingBox& box, const C3DVector& point)
{
    __m128 mn = _mm_loadh_pi(_mm_load_ss(&box.MinCoord.Z), (const __m64*)&box.MinCoord);
    __m128 mx = _mm_loadh_pi(_mm_load_ss(&box.MaxCoord.Z), (const __m64*)&box.MaxCoord);
    __m128 p = _mm_loadh_pi(_mm_load_ss(&point.Z), (const __m64*)&point);
    __m128 orig = p;
    p = _mm_max_ps(p, mn);
    p = _mm_min_ps(p, mx);
    p = _mm_sub_ps(p, orig);
    p = _mm_mul_ps(p, p);
    __m128 t = p;
    p = _mm_shuffle_ps(p, p, 0xb0);
    p = _mm_add_ps(p, t);
    p = _mm_shuffle_ps(p, p, 3);
    p = _mm_add_ss(p, t);
    float result;
    _mm_store_ss(&result, p);
    return result;
}
struct CShadowClipVolume {
    CPlane            ClipPlanes[8];                // +0x00
    long              ClipPlaneCount;               // +0x80
    C3DBoundingSphere BoundingSphere;               // +0x84
    bool              ClipToSphere;                 // +0x94
    bool ClipBoundingBox(const C3DBoundingBox& box) const;
};

bool CShadowClipVolume::ClipBoundingBox(const C3DBoundingBox& box) const
{
    if (ClipToSphere) {
        const C3DVector& centre = BoundingSphere.Center;
        float distance;
        if (g_SSEAvailable)
            distance = DistanceSquaredSSE(box, centre);
        else {
            C3DVector closest = centre;
            if (closest.X < box.MinCoord.X) closest.X = box.MinCoord.X;
            else if (closest.X > box.MaxCoord.X) closest.X = box.MaxCoord.X;
            if (closest.Y < box.MinCoord.Y) closest.Y = box.MinCoord.Y;
            else if (closest.Y > box.MaxCoord.Y) closest.Y = box.MaxCoord.Y;
            if (closest.Z < box.MinCoord.Z) closest.Z = box.MinCoord.Z;
            else if (closest.Z > box.MaxCoord.Z) closest.Z = box.MaxCoord.Z;
            C3DVector d;
            d.X = centre.X - closest.X;
            d.Y = centre.Y - closest.Y;
            d.Z = centre.Z - closest.Z;
            distance = d.Z * d.Z + d.X * d.X + d.Y * d.Y;
        }
        if (distance > BoundingSphere.Radius * BoundingSphere.Radius)
            return false;
    }
    for (long i = 0; i < ClipPlaneCount; ++i) {
        const CPlane& plane = ClipPlanes[i];
        float x = plane.Normal.X > 0.0f ? box.MinCoord.X : box.MaxCoord.X;
        float y = plane.Normal.Y > 0.0f ? box.MinCoord.Y : box.MaxCoord.Y;
        float z = plane.Normal.Z > 0.0f ? box.MinCoord.Z : box.MaxCoord.Z;
        if (plane.Normal.X * x + plane.Normal.Y * y + plane.Normal.Z * z > plane.Distance)
            return false;
    }
    return true;
}
