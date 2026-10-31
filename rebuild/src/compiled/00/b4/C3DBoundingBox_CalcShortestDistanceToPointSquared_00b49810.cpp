// C3DBoundingBox::CalcShortestDistanceToPointSquared(const C3DVector& point) const -- squared distance from a point to the box (0 inside), SSE kernel or x87 clamp path
// Leaf: ?CalcShortestDistanceToPointSquared@C3DBoundingBox@@QBEMABVC3DVector@@@Z
// Grade: RELOCATION_MATCH over the true extent (282 bytes; catalog 319 = int3 padding + the next body): the landed
//   0x00BF7DF0 C3DBoundingBox model out of line (retail_asm SSE kernel + C++ x87 fallback). Parked 'inline-asm SSE lane
//   gap' disproved.
// Retail-asm: 0x00B4981F..0x00B49863 `movss xmm0,[ecx+8] ; movhps xmm0,[ecx] ; ... ; maxps xmm2,xmm0 ; minps xmm2,xmm1 ; subps xmm2,xmm3 ; mulps xmm2,xmm2 ; shufps xmm2,xmm2,0B0h ; addps xmm2,xmm3 ; shufps xmm2,xmm2,3 ; addss xmm2,xmm3 ; movss [esp+4],xmm2` -- the hand SSE kernel behind g_SSEAvailable 0x013D2880 (`this` spilled to [esp] and reloaded, the point reloaded from its argument slot, because the asm names them; VC7.1 C++ emits no packed SSE); the x87 branch is the C++ fallback. The same kernel as landed 0x00BF7DF0.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B49810, 282 bytes (`ret 4`, float in st0); the catalog's 319 runs over int3 padding into
//   the next body (0x00B49930, a 16-byte copy): BOUNDS_FIX.tsv. Identity: Ego_r.pdb / FableWin.pdb
//   ?CalcShortestDistanceToPointSquared@C3DBoundingBox@@QBEMABVC3DVector@@@Z (inline, emitted out of line here); no fsqrt
//   (its twin with fsqrt is CalcShortestDistanceToPoint 0x00B9C500).
// Layout: C3DBoundingBox {MinCoord +0x00, MaxCoord +0x0c} (Ego_r.pdb); the body is the landed 0x00BF7DF0 model.
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
    float CalcShortestDistanceToPointSquared(const C3DVector& point) const;
    float CalcShortestDistanceToPoint(const C3DVector& point) const;
};
float C3DBoundingBox::CalcShortestDistanceToPointSquared(const C3DVector& point) const
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