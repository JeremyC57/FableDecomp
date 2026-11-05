// CCardinalSpline::CCardinalSpline(const C3DVector& start, const C3DVector& end, float tension, bool endPointsConnected) -- two-point cardinal spline: empty control-point array, the tension and the end-points-connected flag, then both points appended
// Grade: RELOCATION_MATCH -- C3DVector (retail pack(1)) with a memberwise user copy constructor: the first placement copy then reads x through ecx and the overflow call pushes the tag from edx, as retail (the implicit copy gave the edx/ecx swap in the first push_back only)
// Leaf: ??0CCardinalSpline@@
// Plan phase 4 (2026-11-05), C3DVector wave (rows with a second value class, from their landed source): the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A56AA0, 167 bytes, `ret 10h` (PDB ??0CCardinalSpline@@QAE@ABVC3DVector@@0M_N@Z).
// Callees: vector<C3DVector>::_M_insert_overflow(pos, x, const __false_type&, n, atend) @ 0x0066E370 (twice).
// Label: the manifest's `CPSCSplineBase_Initialize` is descriptive (the particle spline component holds a CCardinalSpline).
// Layout: Ego_r.pdb CCardinalSpline: ControlPoints (CArray<C3DVector>, a 12-byte STLport vector in retail) +0x00,
//   Tension +0x0c, EndPointsConnected +0x10 (PDB +0x10 / +0x14 with the 16-byte Dinkumware container). No engine header.
// Shape: the CArray constructor is STLport's `_Vector_base(const allocator&)` (`_M_start(0), _M_finish(0)` and the
//   end-of-storage pointer in an `_STLP_alloc_proxy` over an `allocator_type()` temporary); Tension / EndPointsConnected
//   come from the mem-initialiser list; the two inline push_backs (placement-new null test, three dword copies,
//   `add [esi+4],0Ch`, else _M_insert_overflow with the tag homed in the dead bool argument slot).

#pragma pack(push, 1)
#include "engine/C3DVector.h"
#pragma pack(pop)

#include "engine/CArray.h"


class CCardinalSpline {
public:
    CArray<C3DVector> ControlPoints;                    // +0x00
    float             Tension;                          // +0x0c
    bool              EndPointsConnected;               // +0x10
    CCardinalSpline(const C3DVector& start, const C3DVector& end, float tension, bool endPointsConnected);
};

CCardinalSpline::CCardinalSpline(const C3DVector& start, const C3DVector& end, float tension, bool endPointsConnected)
    : Tension(tension), EndPointsConnected(endPointsConnected)
{
    ControlPoints.push_back(start);
    ControlPoints.push_back(end);
}