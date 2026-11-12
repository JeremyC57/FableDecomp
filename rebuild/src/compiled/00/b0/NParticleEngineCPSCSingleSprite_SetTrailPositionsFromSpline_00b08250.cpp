// NParticleEngine::CPSCSingleSprite::SetTrailPositionsFromSpline(CParticleSystem& system, const CParticleUpdateParameters<unsigned long,unsigned long>& params, float width, float time)
// -- rebuild the sprite trail from the component's spline: clear the trail list, then append one CSpriteTrailEntry (position
// moved into world space by CParticleSystem::TransformPosIfWorld unless the system stays with its emitter, plus the width) per
// spline control point (at most MaxTrailLength) when UseSplinePoints is set, else at pos = 0, step, 2*step ... <= 1.0 with
// step = 1/MaxTrailLength.
// Leaf: ?SetTrailPositionsFromSpline@CPSCSingleSprite@NParticleEngine@@IAEXAAVCParticleSystem@2@ABV?$CParticleUpdateParameters@KK@2@MM@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B08250, 724 bytes, __fastcall `ret 10h` (protected, Ego_r.pdb + FableWin.pdb signature).
// Callees: free 0x00BFEA14 / malloc 0x00BFEA0E (inline STLport list clear / push_back nodes),
//   CPSCSplineBase::GetPointAtIndex 0x00B219A0, CPSCSplineBase::GetPointAtPos 0x00B21940,
//   CParticleSystem::GetSystemTransform(CMatrix3x4&) const 0x00AF2250 (inside inline TransformPosIfWorld). Float 1.0f 0x0122DED8.
// Layout (Ego_r.pdb minus 4: retail 12-byte STLport vector in CCardinalSpline): Spline +0x10, MaxTrailLength +0x88,
//   UseSplinePoints +0x96, TrailUpdatePrimitive +0xAC (its list<CSpriteTrailEntry> Trail at +0x0C). No shared header for
//   CPSCSingleSprite / CPSCSplineBase / CEnginePrimitiveSpriteTrail: padded models as in landed AddTrailPosition 0x00B081A0.
// Shape: count = `MaxTrailLength < n() ? MaxTrailLength : n()` (size computed twice in retail); the point copy is transformed
//   in place by the ZYX-sum CMatrix3x4::TransformPoint (FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_ZYX_SUMS, as 0x00A89740).
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CPARTICLEEMITTER
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CPARTICLESYSTEM
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_ZYX_SUMS
#define FABLE_RETAIL_C3DVECTOR
#include "engine/C3DVector.h"
#include "engine/CMatrix3x4.h"
#include "engine/CCardinalSpline.h"
#include "engine/CParticleSystem.h"
#include "stlport/fable_containers.h"

class CEnginePrimitiveSpriteTrail {
public:
    class CSpriteTrailEntry { public:
        C3DVector Position;                                 // +0x00
        float     Width;                                    // +0x0C
        CSpriteTrailEntry(const C3DVector& pos, float width) : Position(pos), Width(width) { }
    };
    unsigned char                     _pad_0x00[0x0C];
    std::list<CSpriteTrailEntry>     Trail;                // +0x0C
};

namespace NParticleEngine {
template <class K, class V> class CParticleUpdateParameters;

class CPSCSplineBase {
public:
    unsigned char   _pad_0x00[0x10];
    CCardinalSpline Spline;                                 // +0x10
    void GetPointAtPos(const CParticleUpdateParameters<unsigned long, unsigned long>& params, C3DVector& out, float pos, float time);     // 0x00B21940
    void GetPointAtIndex(const CParticleUpdateParameters<unsigned long, unsigned long>& params, C3DVector& out, long index, float time);  // 0x00B219A0
};

class CPSCSingleSprite : public CPSCSplineBase {
public:
    unsigned char               _pad_0x24[0x88 - 0x24];
    long                        MaxTrailLength;             // +0x88
    unsigned char               _pad_0x8c[0x96 - 0x8c];
    bool                        UseSplinePoints;            // +0x96
    unsigned char               _pad_0x97[0xac - 0x97];
    CEnginePrimitiveSpriteTrail TrailUpdatePrimitive;       // +0xAC
protected:
    void SetTrailPositionsFromSpline(CParticleSystem& system, const CParticleUpdateParameters<unsigned long, unsigned long>& params, float width, float time);
};

void CPSCSingleSprite::SetTrailPositionsFromSpline(CParticleSystem& system, const CParticleUpdateParameters<unsigned long, unsigned long>& params, float width, float time)
{
    TrailUpdatePrimitive.Trail.clear();
    if (UseSplinePoints) {
        long count = MaxTrailLength < Spline.GetControlPointCount() ? MaxTrailLength : Spline.GetControlPointCount();
        for (long i = 0; i < count; ++i) {
            C3DVector point;
            GetPointAtIndex(params, point, i, time);
            C3DVector p = point;
            system.TransformPosIfWorld(p);
            TrailUpdatePrimitive.Trail.push_back(CEnginePrimitiveSpriteTrail::CSpriteTrailEntry(p, width));
        }
    } else {
        float step = 1.0f / (float)MaxTrailLength;
        for (float pos = 0.0f; pos <= 1.0f; pos += step) {
            C3DVector point;
            GetPointAtPos(params, point, pos, time);
            C3DVector p = point;
            system.TransformPosIfWorld(p);
            TrailUpdatePrimitive.Trail.push_back(CEnginePrimitiveSpriteTrail::CSpriteTrailEntry(p, width));
        }
    }
}
}