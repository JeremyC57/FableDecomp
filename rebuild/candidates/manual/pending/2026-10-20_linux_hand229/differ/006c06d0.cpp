// CTCBuyableHouse::GetRentMoneyPos(C3DVector& pos, long index, long count) -- where the index-th of count rent gold bags goes: one unit in front of the main sign post, then on a 0.7-radius circle around that point at index/count of a turn
// Grade: DIFFER(312v312) -- one x87 operand order: retail forms the normalised Y as `fld [Forward.Y] ; fmul st(2)`, ours
//   as `fld st(1) ; fmul [Forward.Y]` (X and Z match); the single-term commutation lane gap of retry 10
// Leaf: GetRentMoneyPos
// Retail: Fable.exe (TLC, VC7.1) @ 0x006C06D0, 312 bytes (+8 int3 padding), `ret 0Ch`.
// Callees: CTCBuyableHouse::GetSignPostPos(bool, C3DVector*, CRightHandedSet*) 0x006BFCD0, the C2DVector rotate-by-
//   turn-fraction member 0x00A141D0 (manifest "Vector2_RotateAndScale"; descriptive name). Globals: the 128-entry
//   reciprocal-square-root mantissa table 0x013CE558, 3.0f 0x0122DED4, 0.5f 0x0122F59C, 0.7f 0x01255318.
// Shape: the sign post's forward axis is normalised into a copy with C3DVector's inline fast normalise (exponent
//   (380 - e) / 2, table mantissa, one Newton step; the squared length summed Z, X, Y as statements) and added to the
//   post position; the circle offset is a C2DVector built from the raw forward axis times 0.7. The success path is nested
//   (an early `return false` shrink-wraps the frame and grows it to 0x40).
// Tried: early return (0x40 frame), this nested form, `r * Y` for the Y term (same code), a named result (0x40 frame).
extern unsigned long g_InvSqrtMantissaTable[128];   // 0x013CE558
extern const float g_radius_01255318;               // 0x01255318 (0.7f)

struct C2DVector {
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) {}
    void RotateByTurnFraction(float fraction);      // 0x00A141D0
};
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    __forceinline C3DVector GetNormalised() const
    {
        float lengthSquared = Z * Z;
        lengthSquared += X * X;
        lengthSquared += Y * Y;
        unsigned long bits = *(unsigned long*)&lengthSquared;
        unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                                 (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
        float r = *(float*)&estimate;
        r = (3.0f - r * r * lengthSquared) * r * 0.5f;
        return C3DVector(X * r, Y * r, Z * r);
    }
    void operator+=(const C3DVector& v)
    {
        X += v.X;
        Y += v.Y;
        Z += v.Z;
    }
};
struct CRightHandedSet {
    C3DVector Up;                                   // +0x00
    C3DVector Forward;                              // +0x0c
};

class CTCBuyableHouse {
public:
    bool GetSignPostPos(bool main, C3DVector* pos, CRightHandedSet* axes);   // 0x006BFCD0
    bool GetRentMoneyPos(C3DVector& pos, long index, long count);
};

bool CTCBuyableHouse::GetRentMoneyPos(C3DVector& pos, long index, long count)
{
    C3DVector signPos;
    CRightHandedSet axes;
    if (GetSignPostPos(true, &signPos, &axes)) {
        signPos += axes.Forward.GetNormalised();
        C2DVector offset(axes.Forward.X * 0.7f, axes.Forward.Y * 0.7f);
        offset.RotateByTurnFraction((float)index / count);
        signPos.X += offset.X;
        signPos.Y += offset.Y;
        pos = signPos;
        return true;
    }
    return false;
}
