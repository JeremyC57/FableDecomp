// QuadricOptimiserInternals::CQuadric::EvaluateError(const C3DVector& v) const -- the quadric error of a position: v^T Q v over the symmetric 4x4 plane quadric {AA..DD} with v = (x, y, z, 1)
// Grade: DIFFER(96v104) -- x87 term order lane gap: retail keeps every product separate in the order CC*z*z, 2*BC*y*z, BB*y*y, 2*AC*x*z, 2*AB*x*y, 2*CD*z, 2*BD*y, 2*AD*x, AA*x*x, +DD, each with its own `fadd st,st` doubling; VC7.1 (with or without /Op, coefficient-first or -last source) canonicalises the sum and factors the doubled terms into one `fadd st,st`
// Leaf: ?EvaluateError@CQuadric@QuadricOptimiserInternals@@QBE
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AC6D50, 104 bytes (+8 int3), thiscall `ret 4`, result in st(0).
// Identity: Ego_r.pdb `?EvaluateError@CQuadric@QuadricOptimiserInternals@@QBENABVC3DVector@@@Z`; reads the eleven
//   doubles of CQuadric {AA +0, AB +8, AC +0x10, AD +0x18, BB +0x20, BC +0x28, BD +0x30, CC +0x38, CD +0x40,
//   DD +0x48, Weight +0x50} (PDB; landed CQuadric::Initialise 0x00AC7050) against the three floats of the vector.
//   Callers 0x00AC788A, 0x00AC7898, 0x00AC82EB. Catalog label "QuadraticForm_Evaluate" is descriptive.
// Tried (3 grades): this coefficient-first form (96 bytes), the coefficient-last form (identical output), the
//   retail-grouped `x*y*2*AB` form under /Op (124 bytes, double locals spilled; same factoring).
// Next: lane gap: x87 sum canonicalisation (as GOTCHAS chunk 293's CMatrix3x4 inverse).
struct C3DVector { float X, Y, Z; };
namespace QuadricOptimiserInternals {
class CQuadric {
public:
    double AA, AB, AC, AD;
    double BB, BC, BD;
    double CC, CD;
    double DD;
    double Weight;
    double EvaluateError(const C3DVector& v) const;
};
double CQuadric::EvaluateError(const C3DVector& v) const
{
    double x = v.X;
    double y = v.Y;
    double z = v.Z;
    return AA * x * x + 2 * AB * x * y + 2 * AC * x * z + 2 * AD * x
         + BB * y * y + 2 * BC * y * z + 2 * BD * y
         + CC * z * z + 2 * CD * z
         + DD;
}
}
