// CShadowClipVolume::ClipBoundingBox(const C3DBoundingBox& box) const -- false when the box is culled: outside the bounding sphere (if ClipToSphere) or entirely in front of any clip plane
// Leaf: ClipBoundingBox
// Grade: RELOCATION_MATCH (494 bytes; parked 476v494 'inline-asm SSE lane gap' disproved): the landed retail_asm SSE
//   kernel (0x00BF7DF0 form) for the sphere-centre distance; the fallback's X/Y differences in a frame C3DVector, the Z
//   difference a scalar, the squares summed stepwise in retail order (Z, X, Y); plane normals as named locals.
// Retail-asm: 0x00B9C30C..0x00B9C356 `movss xmm0,[ecx+8] ; movhps xmm0,[ecx] ; ... ; maxps xmm2,xmm0 ; minps xmm2,xmm1 ; subps xmm2,xmm3 ; mulps xmm2,xmm2 ; shufps xmm2,xmm2,0B0h ; addps xmm2,xmm3 ; shufps xmm2,xmm2,3 ; addss xmm2,xmm3 ; movss [esp+0Ch],xmm2` -- C3DBoundingBox::CalcShortestDistanceToPointSquared's hand SSE kernel behind g_SSEAvailable 0x013D2880 (the box argument re-read from its slot, the sphere-centre pointer spilled to [esp+8]); the same kernel as landed 0x00BF7DF0; the x87 branch is the C++ fallback.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B9C2E0, length 494 bytes (`ret 4`), ?ClipBoundingBox@CShadowClipVolume@@QBE_NABVC3DBoundingBox@@@Z.
// Layout: Ego_r.pdb CShadowClipVolume (ClipPlanes[8] +0, ClipPlaneCount +0x80, BoundingSphere +0x84, ClipToSphere +0x94);
// SSE-available flag 0x013D2880 (landed 0x00BF7DF0 g_SSEAvailable); 0.0f 0x0122DEDC.
extern "C" double __cdecl sqrt(double x);
#pragma intrinsic(sqrt)
extern bool g_SSEAvailable;                             // 0x013D2880
class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
};
class C3DBoundingBox {
public:
    C3DVector MinCoord;                                 // +0x00
    C3DVector MaxCoord;                                 // +0x0c
    __forceinline float CalcShortestDistanceToPointSquared(const C3DVector& point) const
    {
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
        float sum = dz * dz;
        sum += d.X * d.X;
        sum += d.Y * d.Y;
        return sum;
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
    bool ClipBoundingBox(const C3DBoundingBox& box) const;
};

bool CShadowClipVolume::ClipBoundingBox(const C3DBoundingBox& box) const
{
    if (ClipToSphere) {
        if (box.CalcShortestDistanceToPointSquared(BoundingSphere.Center) > BoundingSphere.Radius * BoundingSphere.Radius)
            return false;
    }
    for (long i = 0; i < ClipPlaneCount; ++i) {
        const CPlane& plane = ClipPlanes[i];
        float nx = plane.Normal.X, ny = plane.Normal.Y, nz = plane.Normal.Z;
        float x = nx > 0.0f ? box.MinCoord.X : box.MaxCoord.X;
        float y = ny > 0.0f ? box.MinCoord.Y : box.MaxCoord.Y;
        float z = nz > 0.0f ? box.MinCoord.Z : box.MaxCoord.Z;
        if (x * nx + y * ny + z * nz > plane.Distance)
            return false;
    }
    return true;
}