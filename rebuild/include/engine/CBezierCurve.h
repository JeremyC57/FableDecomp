#ifndef FABLE_ENGINE_CBEZIERCURVE_H
#define FABLE_ENGINE_CBEZIERCURVE_H
// NEW header (chunk Q6, row 0x00912840). Ego_r.pdb CBezierCurve (ghidra_out/struct_layouts_egor.tsv: 48 bytes, Point1..Point4 C3DVector;
// class key V from ?AddRandomBezierCurveToPath@CAIStateGroup_FlyingThingBase@@IAE_NAAV?$deque@VCBezierCurve@@...). No virtuals, no
// out-of-line members in either PDB name set; retail deque<CBezierCurve> holds two 0x30-byte curves per 0x60 node.
#include "rebuild_abi.h"
#include "engine/C3DVector.h"
class CBezierCurve {
public:
    C3DVector Point1;                                                    // +0x00
    C3DVector Point2;                                                    // +0x0c
    C3DVector Point3;                                                    // +0x18
    C3DVector Point4;                                                    // +0x24
#ifdef FABLE_RETAIL_CBEZIERCURVE
    // Retail opt-in (acctB chunk 2004, row 0x00912BA0 CAIStateGroup_FlyingThingBase::CalculatePrettyPath): the Ego_r.pdb / FableWin.pdb
    // inline accessors ?GetPoint1/3/4@CBezierCurve@@QBEABVC3DVector@@XZ and ?SetPoint1..4@CBezierCurve@@QAEXABVC3DVector@@@Z (no
    // out-of-line copy in Fable.exe: read / written in place) and the default constructor ??0CBezierCurve@@QAE@XZ (no code: the
    // points are left unset, 0x00912BA0 stores Point1..4 one by one). The copy constructor is implicit.
    CBezierCurve() { }
    const C3DVector& GetPoint1() const { return Point1; }
    const C3DVector& GetPoint3() const { return Point3; }
    const C3DVector& GetPoint4() const { return Point4; }
    void SetPoint1(const C3DVector& point) { Point1 = point; }
    void SetPoint2(const C3DVector& point) { Point2 = point; }
    void SetPoint3(const C3DVector& point) { Point3 = point; }
    void SetPoint4(const C3DVector& point) { Point4 = point; }
#endif
#ifdef FABLE_RETAIL_CBEZIERCURVE_GETPOINTONCURVE
    // FableWin/Ego_r: ?GetPointOnCurve@CBezierCurve@@QBE?AVC3DVector@@M@Z (retail 0x009122A0).
    C3DVector GetPointOnCurve(float t) const;
#endif
};
FABLE_STATIC_ASSERT(sizeof(CBezierCurve) == 0x30);
#endif // FABLE_ENGINE_CBEZIERCURVE_H
