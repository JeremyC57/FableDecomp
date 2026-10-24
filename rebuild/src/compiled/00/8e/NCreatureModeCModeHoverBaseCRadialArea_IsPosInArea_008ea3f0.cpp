// NCreatureMode::CModeHoverBase::CRadialArea::IsPosInArea(const C2DVector& pos, bool checkRadius) -- is the offset inside the angular sector [ThetaLow, ThetaHigh] (fractions of a turn) and, when asked, inside RadHigh
// Leaf: ?IsPosInArea@
// Retail 0x008EA3F0, 0xE6 bytes (+int3 padding), thiscall `ret 8`, bool in al. Manifest label
//   "C3DVector_PointInConvexQuad_Plane" is descriptive; FableWin PDB name set:
//   `?IsPosInArea@CRadialArea@CModeHoverBase@NCreatureMode@@QBE_NABVC2DVector@@_N@Z` (the only (const C2DVector&, bool)
//   const predicate in the set). Callers: 16 sites in the hover-mode searches 0x008EBB50 / 0x008EBC90 and 0x008EB810.
// What it does: a zero-length offset (< 0.0001) is always inside; otherwise the two sector edge directions are
//   (sin, cos) of ThetaLow*2pi and ThetaHigh*2pi, the offset is normalised, and it is inside when (radius check off or
//   length < RadHigh) and its dot with each edge is at least the edges' own dot (the cosine of the sector's span).
// Callees: none (sqrt, sin and cos are x87 intrinsics). Constants: 0.0001f 0x0129BA3C, double 2pi 0x0124F2B8
//   (= (double)6.2831855f, the landed 0x00878A80 `angle * (double)TWO_PI` spelling), 1.0f 0x0122DED8.
// Layout (Ego_r.pdb): CRadialArea { float ThetaLow +0, float ThetaHigh +4, float RadHigh +8 }.
// Shape: grade 1 (`pos / length` through an inline operator/ and an early `if (check && !(length < RadHigh)) return false`)
//   gave its own false exit and X-first dots. Retail needs (a) the radius test as the condition of a block that falls
//   through to the shared `return false`, and (b) the normalised vector built by two statements so its Y load is created
//   after its X load: the dots then come out Y-term first (GOTCHAS load-creation rule), as retail's do.
extern "C" double __cdecl sin(double);
extern "C" double __cdecl cos(double);
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sin, cos, sqrt)

static const float TWO_PI = 6.2831855f;

class C2DVector {
public:
    float X, Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
    float GetLength() const { return (float)sqrt(X * X + Y * Y); }
    float Dot(const C2DVector& other) const { return X * other.X + Y * other.Y; }
};

namespace NCreatureMode {
class CModeHoverBase {
public:
    class CRadialArea {
    public:
        float ThetaLow;                                 // +0x00
        float ThetaHigh;                                // +0x04
        float RadHigh;                                  // +0x08
        bool IsPosInArea(const C2DVector& pos, bool checkRadius) const;
    };
};
}

bool NCreatureMode::CModeHoverBase::CRadialArea::IsPosInArea(const C2DVector& pos, bool checkRadius) const
{
    float length = pos.GetLength();
    if (length < 0.0001f)
        return true;
    double thetaLow = ThetaLow * (double)TWO_PI;
    C2DVector low;
    low.X = (float)sin(thetaLow);
    low.Y = (float)cos(thetaLow);
    double thetaHigh = ThetaHigh * (double)TWO_PI;
    C2DVector high;
    high.X = (float)sin(thetaHigh);
    high.Y = (float)cos(thetaHigh);
    float scale = 1.0f / length;
    C2DVector direction;
    direction.X = pos.X * scale;
    direction.Y = pos.Y * scale;
    if (!checkRadius || length < RadHigh) {
        float arcCos = high.Dot(low);
        if (direction.Dot(low) >= arcCos && direction.Dot(high) >= arcCos)
            return true;
    }
    return false;
}