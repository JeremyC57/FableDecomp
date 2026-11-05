// CShadowClipVolume::ClipBoundingBox(const C3DBoundingBox& box, bool& fullyInside) const -- false when the box lies outside the volume's bounding sphere or entirely in front of one of its clip planes; fullyInside stays true only when the box's farthest corner is inside the sphere and every plane's far vertex is behind its plane
// Leaf: ?ClipBoundingBox@CShadowClipVolume@@QBE_NABVC3DBoundingBox@@AA_N@Z
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Also: local GetLengthSquared respelled GetSquaredMagnitude (the header / FableWin.pdb method, same computation).
// Grade: RELOCATION_MATCH (829 bytes; parked 787v831 'inline-asm SSE lane gap' disproved): retail_asm SSE kernel (landed
//   0x00BF7DF0 C3DBoundingBox model, fallback as 0x00B99BB0); PDB overload with bool& (not bool*); the centre passed
//   directly to the distance call, the far corner a frame C3DVector with scalar differences; plane normals as named
//   locals.
// Retail-asm: 0x00B4CF08..0x00B4CF52 `movss xmm0,[ecx+8] ; movhps xmm0,[ecx] ; ... ; maxps xmm2,xmm0 ; minps xmm2,xmm1 ; subps xmm2,xmm3 ; mulps xmm2,xmm2 ; shufps xmm2,xmm2,0B0h ; addps xmm2,xmm3 ; shufps xmm2,xmm2,3 ; addss xmm2,xmm3 ; movss [esp+10h],xmm2` -- C3DBoundingBox::CalcShortestDistanceToPointSquared's hand SSE kernel behind g_SSEAvailable 0x013D2880 (the box re-read from its argument slot, the sphere-centre pointer spilled into the dead fullyInside slot); the same kernel as landed 0x00BF7DF0; the x87 branch is the C++ fallback.
// Portable (2026-11-03 asm twins): the asm kernel is this function's own block (retail has no separate helper for it), so it stays here in two-path form: under FABLE_PORTABLE the #ifndef drops the SSE branch and the row runs retail's own non-SSE C++ path (Strategy B; equal to the SSE kernel up to float rounding order -- retail ran this path on CPUs without SSE).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B4CED0, 829 bytes (+ int3 padding), `ret 8`. Identity: FableWin.pdb / Ego_r.pdb
//   ?ClipBoundingBox@CShadowClipVolume@@QBE_NABVC3DBoundingBox@@AA_N@Z (the bool& overload of 0x00B9C2E0).
// Layout: Ego_r.pdb CShadowClipVolume (ClipPlanes[8] +0, ClipPlaneCount +0x80, BoundingSphere +0x84, ClipToSphere +0x94);
//   0.0f at 0x0122DEDC; fabs inline.
extern "C" double __cdecl sqrt(double x);
extern "C" double __cdecl fabs(double x);
#pragma intrinsic(sqrt)
#pragma intrinsic(fabs)
extern bool g_SSEAvailable;                             // 0x013D2880
#include "engine/C3DVector.h"
class C3DBoundingBox {
public:
    C3DVector MinCoord;                                 // +0x00
    C3DVector MaxCoord;                                 // +0x0c
    __forceinline float CalcShortestDistanceToPointSquared(const C3DVector& point) const
    {
#ifndef FABLE_PORTABLE   // portable build: the retail non-SSE C++ path below (the twin of the SSE asm kernel)
        if (g_SSEAvailable) {
            float result;
            __asm {
                mov     ecx, this
                mov     edx, point
                movss   xmm0, [ecx+8]
                movhps  xmm0, [ecx]
                movss   xmm1, [ecx+14h]
                movhps  xmm1, [ecx+0Ch]
                movss   xmm2, [edx+8]
                movhps  xmm2, [edx]
                movaps  xmm3, xmm2
                maxps   xmm2, xmm0
                minps   xmm2, xmm1
                subps   xmm2, xmm3
                mulps   xmm2, xmm2
                movaps  xmm3, xmm2
                shufps  xmm2, xmm2, 0B0h
                addps   xmm2, xmm3
                shufps  xmm2, xmm2, 3
                addss   xmm2, xmm3
                movss   result, xmm2
            }
            return result;
        }
#endif
        C3DVector closest(point);
        if (closest.X < MinCoord.X)
            closest.X = MinCoord.X;
        else if (closest.X > MaxCoord.X)
            closest.X = MaxCoord.X;
        if (closest.Y < MinCoord.Y)
            closest.Y = MinCoord.Y;
        else if (closest.Y > MaxCoord.Y)
            closest.Y = MaxCoord.Y;
        if (closest.Z < MinCoord.Z)
            closest.Z = MinCoord.Z;
        else if (closest.Z > MaxCoord.Z)
            closest.Z = MaxCoord.Z;
        C3DVector d;
        d.X = point.X - closest.X;
        d.Y = point.Y - closest.Y;
        float dz = point.Z - closest.Z;
        return dz * dz + d.X * d.X + d.Y * d.Y;
    }
    __forceinline float CalcShortestDistanceToPoint(const C3DVector& point) const
    {
        return (float)sqrt(CalcShortestDistanceToPointSquared(point));
    }
};
class C3DBoundingSphere { public:
    C3DVector Center;                               // +0x00
    float     Radius;                               // +0x0c
};
class CPlane { public:
    C3DVector Normal;                               // +0x00
    float     Distance;                             // +0x0c
};
class CShadowClipVolume { public:
    CPlane            ClipPlanes[8];                // +0x00
    long              ClipPlaneCount;               // +0x80
    C3DBoundingSphere BoundingSphere;               // +0x84
    bool              ClipToSphere;                 // +0x94
    bool ClipBoundingBox(const C3DBoundingBox& box, bool& fullyInside) const;
};

bool CShadowClipVolume::ClipBoundingBox(const C3DBoundingBox& box, bool& fullyInside) const
{
    fullyInside = true;
    if (ClipToSphere) {
        if (box.CalcShortestDistanceToPointSquared(BoundingSphere.Center) > BoundingSphere.Radius * BoundingSphere.Radius) {
            fullyInside = false;
            return false;
        }
        const C3DVector& centre = BoundingSphere.Center;
        C3DVector farCorner;
        farCorner.X = (float)fabs(centre.X - box.MinCoord.X) > (float)fabs(centre.X - box.MaxCoord.X) ? box.MinCoord.X : box.MaxCoord.X;
        farCorner.Y = (float)fabs(centre.Y - box.MinCoord.Y) > (float)fabs(centre.Y - box.MaxCoord.Y) ? box.MinCoord.Y : box.MaxCoord.Y;
        farCorner.Z = (float)fabs(centre.Z - box.MinCoord.Z) > (float)fabs(centre.Z - box.MaxCoord.Z) ? box.MinCoord.Z : box.MaxCoord.Z;
        float dx = centre.X - farCorner.X, dy = centre.Y - farCorner.Y, dz = centre.Z - farCorner.Z;
        if (dz * dz + dy * dy + dx * dx > BoundingSphere.Radius * BoundingSphere.Radius)
            fullyInside = false;
    }
    for (long i = 0; i < ClipPlaneCount; ++i) {
        const CPlane& plane = ClipPlanes[i];
        float nx = plane.Normal.X, ny = plane.Normal.Y, nz = plane.Normal.Z;
        float x = nx > 0.0f ? box.MinCoord.X : box.MaxCoord.X;
        float y = ny > 0.0f ? box.MinCoord.Y : box.MaxCoord.Y;
        float z = nz > 0.0f ? box.MinCoord.Z : box.MaxCoord.Z;
        if (x * nx + y * ny + z * nz > plane.Distance) {
            fullyInside = false;
            return false;
        }
        if (fullyInside) {
            float fx = nx > 0.0f ? box.MaxCoord.X : box.MinCoord.X;
            float fy = ny > 0.0f ? box.MaxCoord.Y : box.MinCoord.Y;
            float fz = nz > 0.0f ? box.MaxCoord.Z : box.MinCoord.Z;
            if (fx * nx + fy * ny + fz * nz > plane.Distance)
                fullyInside = false;
        }
    }
    return true;
}