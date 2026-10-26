// CTargetPointFilter_FurtherFromPosThan::operator()(const CTargetPoint& point) -- true when the target point's TargetingPosition lies further than Range from *Pos (squared distance against Range squared)
// Leaf: ??RCTargetPointFilter_FurtherFromPosThan
// Retail: Fable.exe @ 0x00805920, 73 bytes. No callees.
// Layout: PDB CTargetPointFilter_FurtherFromPosThan (Pos +0, Range +4) and CTargetPoint (PThing +0, Index +8,
// TargetingPosition +0xc, FrameRejected +0x18); no engine headers, modelled locally with the PDB names.
// Shape: the delta (point - pos) is built field by field on the x87 stack (x, y, z); retail sums z*z + y*y first and
// adds x*x last. VC7.1 (no /Op) re-associates the float sum: `X*X + Y*Y + Z*Z` comes out (z*z + x*x) + y*y and
// `Z*Z + Y*Y + X*X` differs too; only the grouping `X*X + (Y*Y + Z*Z)` reproduces retail. `Range * Range` is compared
// against it (`fcompp ; test ah,5 ; jp`) and the bool is materialised per branch (`mov al,1` / `xor al,al`).
class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float MagnitudeSquared() const { return X * X + (Y * Y + Z * Z); }
};

struct CIntelligentPointer_CThing {                 // CIntelligentPointer<CThing>, 8 bytes
    void* __vftable;
    void* PData;
};

struct CTargetPoint {
    CIntelligentPointer_CThing PThing;             // +0x00
    long      Index;                               // +0x08
    C3DVector TargetingPosition;                   // +0x0c
    long      FrameRejected;                       // +0x18
};

struct CTargetPointFilter_FurtherFromPosThan {
    const C3DVector* Pos;                          // +0x00
    float            Range;                        // +0x04
    bool operator()(const CTargetPoint& point) const;
};

bool CTargetPointFilter_FurtherFromPosThan::operator()(const CTargetPoint& point) const
{
    if ((point.TargetingPosition - *Pos).MagnitudeSquared() > Range * Range)
        return true;
    return false;
}