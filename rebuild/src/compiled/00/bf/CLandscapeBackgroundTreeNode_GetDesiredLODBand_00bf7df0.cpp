// CLandscapeBackgroundTreeNode::GetDesiredLODBand() -- the LOD band for the camera's shortest distance to the node's bounding box (the renderer's LOD Z bands), clamped to the node's [FirstBand, LastBand]
// Leaf: ?GetDesiredLODBand@CLandscapeBackgroundTreeNode@@QAEJXZ
// Retail-asm: 0x00BF7E1E..0x00BF7E58 `movss xmm0,[ecx+8] ; movhps xmm0,[ecx] ; ... ; maxps xmm2,xmm0 ; minps xmm2,xmm1 ; subps xmm2,xmm3 ; mulps xmm2,xmm2 ; shufps xmm2,xmm2,0B0h ; addps xmm2,xmm3 ; shufps xmm2,xmm2,3 ; addss xmm2,xmm3 ; movss [esp+0Ch],xmm2` -- C3DBoundingBox::CalcShortestDistanceToPointSquared's hand SSE kernel behind the g_SSEAvailable byte 0x013D2880 (the box and point pointers are spilled to [esp+8]/[esp+4] and reloaded because the asm names them; VC7.1 C++ emits no packed SSE: /arch:SSE only produces scalar ss code and the xmmintrin intrinsics would not reload the spilled pointers); the x87 branch is the C++ fallback.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BF7DF0, 379 bytes, thiscall, plain `ret`, long in eax.
// Identity (proven): Ego_r.pdb ?GetDesiredLODBand@CLandscapeBackgroundTreeNode@@QAEJXZ (long, no arguments); `this` +0xa8 is
//   BoundingBox and +0xe0/+0xe2 FirstBand/LastBand (Ego_r +0xb0/+0xe8/+0xea, retail 8 smaller), and the callers 0x00BF7FBE,
//   0x00BF805D (ProcessBackgroundLoading 0x00BF8030) and 0x00BF8890 (UpdateStaticLOD 0x00BF8810) are CLandscapeBackgroundTreeNode
//   members. The catalog label "CEngineLandscapePatch::DebugCalcInForgroundRange" (QAE_NXZ, bool) is wrong.
// Inlines: C3DBoundingBox::CalcShortestDistanceToPoint(Squared) (Ego_r QBEMABVC3DVector@@@Z) and
//   CEngineLandscapeRenderer::GetLODZBandAtZ(float) (Ego_r QAEJM@Z: first band whose LODZBandsZ[band] exceeds z, minus one; 7
//   past the last), both __forceinline as retail expands them.
// Globals: g_pEngineCamera 0x01436EA0 (CameraDesc.WorldPosition at +0x54 = Ego_r CEngineCamera +0x14 + CEngineCameraDesc +0x40),
//   the landscape renderer pointer 0x01436EA8 (LODZBandsZ[8] at +0x498; Ego_r CEngineLandscapeRenderer LODZBandsZ float[8]),
//   g_SSEAvailable 0x013D2880.
// Shape: the distance feeds GetLODZBandAtZ in one expression (a named local float changes the x87 stack order of the
//   fallback's sum of squares).
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
        return (point - closest).GetLengthSquared();
    }
    __forceinline float CalcShortestDistanceToPoint(const C3DVector& point) const
    {
        return (float)sqrt(CalcShortestDistanceToPointSquared(point));
    }
};

class CEngineCamera {
public:
    unsigned char _pad00[0x54];
    C3DVector     WorldPosition;                        // +0x54 (CameraDesc +0x14, WorldPosition +0x40)
};
extern CEngineCamera* g_pEngineCamera;                  // 0x01436EA0

class CEngineLandscapeRenderer {
public:
    unsigned char _pad[0x498];
    float         LODZBandsZ[8];                        // +0x498
    long GetLODZBandAtZ(float z)
    {
        for (long band = 2; band < 8; ++band)
            if (z < LODZBandsZ[band])
                return band - 1;
        return 7;
    }
};
extern CEngineLandscapeRenderer* g_pLandscapeRenderer;            // 0x01436EA8

class CLandscapeBackgroundTreeNode {
public:
    unsigned char  _pad00[0xa8];
    C3DBoundingBox BoundingBox;                         // +0xa8
    unsigned char  _pad_c0[0xe0 - 0xc0];
    unsigned char  FirstBand;                           // +0xe0
    unsigned char  FirstNonSplitBand;                   // +0xe1
    unsigned char  LastBand;                            // +0xe2
    long GetDesiredLODBand();
};

long CLandscapeBackgroundTreeNode::GetDesiredLODBand()
{
    long band = g_pLandscapeRenderer->GetLODZBandAtZ(BoundingBox.CalcShortestDistanceToPoint(g_pEngineCamera->WorldPosition));
    if (band < FirstBand)
        return FirstBand;
    if (band > LastBand)
        return LastBand;
    return band;
}