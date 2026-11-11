#ifndef FABLE_ENGINE_CBEZIERCURVE_H
#define FABLE_ENGINE_CBEZIERCURVE_H
// New header (chunk 2004, row 0x00912BA0 CAIStateGroup_FlyingThingBase::CalculatePrettyPath). Retail opt-in only:
// FABLE_RETAIL_CBEZIERCURVE. Class key V (pdb_class_keys.tsv). Members: Ego_r.pdb TPI (struct_layouts_egor.tsv, 48 bytes): Point1
// +0x00, Point2 +0x0C, Point3 +0x18, Point4 +0x24 (C3DVector; the landed deque<CBezierCurve> rows 0x00913D60 / 0x00913E70 copy the
// four of them as 12 dwords). Methods: the Ego_r.pdb / FableWin.pdb inline accessors ?GetPoint1/3/4@CBezierCurve@@QBEABVC3DVector@@XZ
// and ?SetPoint1..4@CBezierCurve@@QAEXABVC3DVector@@@Z (no out-of-line copy in Fable.exe: read / written in place), the default
// constructor ??0CBezierCurve@@QAE@XZ (no code: the points are left unset, 0x00912BA0 stores Point1..4 one by one) and
// ?GetPointOnCurve@CBezierCurve@@QBE?AVC3DVector@@M@Z (not used here, not declared). The copy constructor is implicit.
#ifdef FABLE_RETAIL_CBEZIERCURVE
#include "engine/C3DVector.h"
class CBezierCurve {
public:
    CBezierCurve() { }
    const C3DVector& GetPoint1() const { return Point1; }
    const C3DVector& GetPoint3() const { return Point3; }
    const C3DVector& GetPoint4() const { return Point4; }
    void SetPoint1(const C3DVector& point) { Point1 = point; }
    void SetPoint2(const C3DVector& point) { Point2 = point; }
    void SetPoint3(const C3DVector& point) { Point3 = point; }
    void SetPoint4(const C3DVector& point) { Point4 = point; }
private:
    C3DVector Point1;                                                    // +0x00
    C3DVector Point2;                                                    // +0x0c
    C3DVector Point3;                                                    // +0x18
    C3DVector Point4;                                                    // +0x24
};
FABLE_STATIC_ASSERT(sizeof(CBezierCurve) == 0x30);
#endif
#endif // FABLE_ENGINE_CBEZIERCURVE_H
