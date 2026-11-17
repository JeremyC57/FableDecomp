// NParticleEngine::CPSCSplineBase::CSplinePosParamData::CSplinePosParamData(CCardinalSpline*) -- stores the spline, constructs the empty parameter array and resizes it to one zeroed entry per spline control point
// Leaf: ??0CSplinePosParamData
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B21A00, 73 bytes (+7 int3 padding). Callee: retail
// CArray<unsigned long>::resize(n, const unsigned long&) @ 0x00464931 (landed family shape; manifest label "push_back").
// Callers: 0x00B224F9 / 0x00B22606 (CPSCSplineBase).
// Layout (PDB CSplinePosParamData, retail-sized): PParentParticleSystem +0x0 and MaintainSplineShape +0x8 are left
// uninitialised; Spline +0x4; SplinePosParam +0xC is retail's three-pointer CArray (PDB 0x10). CCardinalSpline begins
// with its CArray<C3DVector> ControlPoints (12-byte elements: the 0x2AAAAAAB multiply + `sar edx,1` is /12).
// Shape: the init-list store of Spline is re-read through the member for the count, which is a NAMED local (passing
// the expression straight to resize schedules the zero temporary before the member load and colours the zero in eax
// instead of retail's hoisted `xor edx,edx`); CArray's inline constructor zeroes its three words from one zero register; the `unsigned long()` temporary bound to resize's const reference is homed in
// the constructor's dead argument slot (`mov [esp+8],edx ; lea edx,[esp+0Ch]`).
// NF48 (round 7): CArray<unsigned long> is the shared engine/CArray.h (a public STLport 4.6.2 std::vector<unsigned long>,
// a local begin/end/capacity model before); resize(n, const unsigned long&) is vector<unsigned long>::resize 0x00464931
// (?resize@?$vector@KV?$allocator@K@std@@@std@@QAEXIABK@Z), kept out of line by FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE.
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE
#include "engine/CArray.h"
#include "engine/C3DVector.h"


class CCardinalSpline { public:
    CArray<C3DVector> ControlPoints;                     // +0x0
    float             Tension;                           // +0xC
    bool              EndPointsConnected;                // +0x10
};

struct CParticleSystem;

struct CSplinePosParamData {
    CParticleSystem*      PParentParticleSystem;         // +0x00
    CCardinalSpline*      Spline;                        // +0x04
    bool                  MaintainSplineShape;           // +0x08
    CArray<unsigned long> SplinePosParam;                // +0x0C
    CSplinePosParamData(CCardinalSpline* spline);
};

CSplinePosParamData::CSplinePosParamData(CCardinalSpline* spline)
    : Spline(spline)
{
    unsigned long count = Spline->ControlPoints.size();
    SplinePosParam.resize(count, 0);
}