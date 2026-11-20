#ifndef FABLE_ENGINE_NSHAPE_H
#define FABLE_ENGINE_NSHAPE_H
// NShape free functions and the find_if predicate (new header, p25J 2026-11-20, row 0x00763060 NShape::FractureShape).
// Ego_d.pdb TPI 0x3F5BC `NShape::ECirculationOrder` (CIRCULATION_CLOCKWISE = 0, CIRCULATION_ANTICLOCKWISE = 1) and 0x403B6
// `NShape::CIsPointFinalLinkInTriangleWithMatchingCirculation` (sizeof 20: PointA const C3DVector& +0, PointB const C3DVector& +4,
// UpDirection C3DVector +8; ctor (const C3DVector&, const C3DVector&, ECirculationOrder); bool operator()(const C3DVector&) const);
// struct_layouts_egor.tsv agrees on the offsets. Function spellings: Ego_r.pdb ?...@NShape@@YI... (all __fastcall).
// The ctor body is retail's inline expansion at 0x007630C0..0x007630FD: UpDirection = (0, 0, circ == CLOCKWISE ? 1.0f : -1.0f).
#include "rebuild_abi.h"
#include "engine/C3DVector.h"
#include "engine/C2DLineF.h"
#include "stlport/fable_containers.h"
namespace NShape {
enum ECirculationOrder { CIRCULATION_CLOCKWISE = 0, CIRCULATION_ANTICLOCKWISE = 1 };
class CIsPointFinalLinkInTriangleWithMatchingCirculation {
public:
    CIsPointFinalLinkInTriangleWithMatchingCirculation(const C3DVector& a, const C3DVector& b, ECirculationOrder circ)
        : PointA(a), PointB(b), UpDirection(0.0f, 0.0f, circ == CIRCULATION_CLOCKWISE ? 1.0f : -1.0f) { }
    bool operator()(const C3DVector& point) const;
private:
    const C3DVector& PointA;                                             // +0x00
    const C3DVector& PointB;                                             // +0x04
    C3DVector UpDirection;                                               // +0x08
};
ECirculationOrder __fastcall GetCirculationOrder(const std::vector<C3DVector>& shape);
bool __fastcall IsVectorBetween2OthersInThePlane(const C3DVector& a, const C3DVector& b, const C3DVector& c);   // 0x0075FF00
bool __fastcall IsLineIntersectingWithOthersInList(const C2DLineF& line, const std::vector<C3DVector>& points_list);  // 0x00760020
bool __fastcall IsTriangleWithPointsInside(const std::vector<C3DVector>& triangle, const std::vector<C3DVector>& p_list,
                                           ECirculationOrder circ);                                            // 0x00760190
bool __fastcall IsShapeConvex(const std::vector<C3DVector>& pts);                                              // 0x007602F0
void __fastcall FractureShape(const std::vector<C3DVector>& test_shape, ECirculationOrder& circ,
                              std::vector<std::vector<C3DVector> >& convex_shapes,
                              std::vector<std::vector<C3DVector> >& concave_shapes);                           // 0x00763060
}
FABLE_STATIC_ASSERT(sizeof(NShape::CIsPointFinalLinkInTriangleWithMatchingCirculation) == 0x14);
#endif
