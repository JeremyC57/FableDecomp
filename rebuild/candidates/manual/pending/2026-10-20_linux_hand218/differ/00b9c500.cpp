// C3DBoundingBox::CalcShortestDistanceToPoint(const C3DVector& point) const -- distance from a point to the box (0 inside), SSE or x87 path (square root of the squared form)
// Grade: DIFFER(ours v retail) -- same inline-asm SSE block as 0x00B49810 plus fsqrt; the row is CalcShortestDistanceToPoint (label propagated)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B9C500, 284 bytes (manifest label CalcShortestDistanceToPointSquared is propagated: this twin ends in fsqrt) (`ret 4`, float in st0).
// Ego_r.pdb: ?CalcShortestDistanceToPointSquared@C3DBoundingBox@@QBEMABVC3DVector@@@Z. No callees; the SSE-available
// flag is the byte global 0x013D2880.
// Shape: with SSE the box corners and the point are loaded as (Z, -, X, Y) quads (`_mm_load_ss` + `_mm_loadh_pi`), the
// point clamped into the box (max then min), the squared delta summed horizontally with two shuffles (0xB0, 3) and
// stored through `_mm_store_ss`; otherwise the point is copied (X, Y as dwords, Z on the x87 stack), each component
// clamped with `if (< min) = min; else if (> max) = max;` and the squared length of `point - clamped` returned.
// Layout: PDB C3DBoundingBox MinCoord +0, MaxCoord +0xc.
// Leaf: CalcShortestDistanceToPoint@C3DBoundingBox
#include <xmmintrin.h>
#include <math.h>

struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
};

extern bool g_bSSEAvailable;                       // 0x013D2880

struct C3DBoundingBox {
    C3DVector MinCoord;                            // +0
    C3DVector MaxCoord;                            // +0xc
    float CalcShortestDistanceToPoint(const C3DVector& point) const;
};

float C3DBoundingBox::CalcShortestDistanceToPoint(const C3DVector& point) const
{
    if (g_bSSEAvailable) {
        __m128 boxMin = _mm_loadh_pi(_mm_load_ss(&MinCoord.Z), (const __m64*)&MinCoord.X);
        __m128 boxMax = _mm_loadh_pi(_mm_load_ss(&MaxCoord.Z), (const __m64*)&MaxCoord.X);
        __m128 p = _mm_loadh_pi(_mm_load_ss(&point.Z), (const __m64*)&point.X);
        __m128 delta = _mm_sub_ps(_mm_min_ps(_mm_max_ps(p, boxMin), boxMax), p);
        delta = _mm_mul_ps(delta, delta);
        __m128 sum = _mm_add_ps(_mm_shuffle_ps(delta, delta, 0xb0), delta);
        sum = _mm_add_ss(_mm_shuffle_ps(sum, sum, 3), delta);
        float result;
        _mm_store_ss(&result, sum);
        return (float)sqrt(result);
    }
    C3DVector clamped = point;
    if (clamped.X < MinCoord.X)
        clamped.X = MinCoord.X;
    else if (clamped.X > MaxCoord.X)
        clamped.X = MaxCoord.X;
    if (clamped.Y < MinCoord.Y)
        clamped.Y = MinCoord.Y;
    else if (clamped.Y > MaxCoord.Y)
        clamped.Y = MaxCoord.Y;
    if (clamped.Z < MinCoord.Z)
        clamped.Z = MinCoord.Z;
    else if (clamped.Z > MaxCoord.Z)
        clamped.Z = MaxCoord.Z;
    return (float)sqrt((point - clamped).GetLengthSquared());
}
