// QuadricOptimiserInternals::CQuadric::Clear() -- reset a vertex quadric: all ten symmetric 4x4 coefficients and the weight to 0.0
// Leaf: ?Clear@CQuadric@QuadricOptimiserInternals@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AC6CE0, 99 bytes, __fastcall `ret`, no return value. No callees.
// Identity (label correction): the catalog's "pair<EBodyLocation,CWoundMorphs::CEntry>" is propagated. The body stores 0.0
//   into eleven consecutive doubles (+0x00 .. +0x50) = Ego_r.pdb QuadricOptimiserInternals::CQuadric (88 bytes: AA, AB, AC,
//   AD, BB, BC, BD, CC, CD, DD, Weight; landed Initialise 0x00AC7050); FableWin.pdb ?Clear@CQuadric@QuadricOptimiserInternals@@QAEXXZ.
//   The caller 0x00AC931F (COptimisedPrimitive::ContractVertexPair region) clears a frame quadric before accumulating.
// Shape: one `AA = 0.0;` per coefficient: VC7.1 loads the double literal from the shared `__real@0000000000000000` pool
//   entry 0x0122ED70 (`fld qword`, not `fldz`) and reloads it for every store.
namespace QuadricOptimiserInternals {

class CQuadric {
public:
    double AA, AB, AC, AD;                              // +0x00 .. +0x18
    double BB, BC, BD;                                  // +0x20 .. +0x30
    double CC, CD;                                      // +0x38, +0x40
    double DD;                                          // +0x48
    double Weight;                                      // +0x50
    void Clear();
};

void CQuadric::Clear()
{
    AA = 0.0;
    AB = 0.0;
    AC = 0.0;
    AD = 0.0;
    BB = 0.0;
    BC = 0.0;
    BD = 0.0;
    CC = 0.0;
    CD = 0.0;
    DD = 0.0;
    Weight = 0.0;
}

}