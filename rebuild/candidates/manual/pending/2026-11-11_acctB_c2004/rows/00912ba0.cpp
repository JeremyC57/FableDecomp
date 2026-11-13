// CAIStateGroup_FlyingThingBase::CalculatePrettyPath(std::deque<CBezierCurve>& path, const C3DVector& destination) -- rebuild a flying creature's path: one random Bezier hop from its position (three random flight positions lifted by a random height), up to two more random hops, then a final curve that leaves the last one along its tangent (halved until it clears the ground, at most 10 times) and ends at the destination
// Leaf: ?CalculatePrettyPath@CAIStateGroup_FlyingThingBase@@IAE_NAAV?$deque@VCBezierCurve@@V?$allocator@VCBezierCurve@@@std@@@std@@ABVC3DVector@@@Z
// Retail: Fable.exe (TLC, VC7.1 RTM) @ 0x00912BA0, 1314 bytes, thiscall `ret 8`. Ego_r.pdb / FableWin.pdb
//   ?CalculatePrettyPath@CAIStateGroup_FlyingThingBase@@IAE_NAAV?$deque@VCBezierCurve@@V?$allocator@VCBezierCurve@@@std@@@std@@ABVC3DVector@@@Z
//   (protected; Ego_r donor order GetRandomFlightPos 0x00936E50, AddRandomBezierCurveToPath 0x00936FB0, CalculatePrettyPath 0x00937310 =
//   retail 0x00912700, 0x00912840, 0x00912BA0).
// Callees: deque<CBezierCurve>::clear 0x00913A20 / _M_push_back_aux_v 0x00913D60 (both landed, same TU), CThingPhysical::PeekPos 0x006A4D00,
//   CAIStateGroup_FlyingThingBase::GetRandomFlightPos 0x00912700 (landed) / AddRandomBezierCurveToPath 0x00912840,
//   CAIStateGroupBase::GetPWorldSeed 0x008FCFD0, GFFloatRandom 0x004978A0, GFROR13 0x00497890 (inside the inline GFRandom),
//   CThingCreatureBase::GetHeightZBelowAt 0x00666D40. Globals: HeightProbeOffset 0x0138C500 (descriptive: a zero-filled const C3DVector
//   with no PDB data name; the established bound spelling), float pool 0.25f 0x0123078C / 0.5f 0x0122F59C.
// Layout: shared headers only; CAIStateGroup_FlyingThingBase / CBezierCurve are new overlay headers, stlport/fable_deque.h gains the
//   STLport back / push_back / clear / _M_push_back_aux_v opt-ins and CThingCreatureBase.h the GetHeightZBelowAt declaration
//   (hdr/README.md).
// Shape: the three random control points are a `point` loop 2..4 (`cmp edi,4 ; jbe`) whose height arms each call GetPWorldSeed
//   (cross-jumped into one GFFloatRandom call) and a switch on the point number (SetPoint2 / 3 / 4); push_back and back() expanded
//   (STLport 4.6); the tangent and control point are copy-initialised and the loop's `control = start + direction` assigns a
//   temporary (retail's temp-then-copy at 0x00912EC0..0x00912EF7); `ground` stays on the x87 stack across the halving loop. The one
//   lever: _M_push_back_aux_v's STLport body visible but out of line (FABLE_RETAIL_STLP_DEQUE_PUSHBACK_AUX_BODY), so VC7.1 knows the
//   second push_back's source is not captured and schedules its 12-dword copy load-ahead (0x0091302B) and back()'s Point4.Y load
//   first (0x00912D9A); declaration-only it alternates load / store (same size, 26 lines).
#define FABLE_RETAIL_CAISTATEGROUP_FLYINGTHINGBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CBEZIERCURVE
#define FABLE_RETAIL_STLP_DEQUE_PUSHBACK
#define FABLE_RETAIL_STLP_DEQUE_PUSHBACK_AUX_BODY
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_HEIGHTBELOW
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#include "engine/CThingAICreature.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/C3DVector.h"
#include "engine/GFRandom.h"
#include "engine/GFFloatRandom.h"
#include "engine/CBezierCurve.h"
#include "stlport/fable_deque.h"
#include "engine/CAIStateGroup_FlyingThingBase.h"

extern const C3DVector HeightProbeOffset;                                // 0x0138C500

bool CAIStateGroup_FlyingThingBase::CalculatePrettyPath(std::deque<CBezierCurve>& path, const C3DVector& destination)
{
    path.clear();
    CBezierCurve curve;
    curve.SetPoint1(GetCreature().PeekPos());
    for (unsigned long point = 2; point <= 4; ++point) {
        C3DVector pos;
        if (!GetRandomFlightPos(curve.GetPoint1(), FlightRange, pos, true))
            return false;
        if (point == 2 || point == 4)
            pos.Z += GFFloatRandom(MaxHeightAboveGround, GetPWorldSeed()) + MinHeightToKeepAboveGroundInFlight;
        else
            pos.Z += GFFloatRandom(MaxHeightAboveGround * 0.25f, GetPWorldSeed()) + MinHeightToKeepAboveGroundInFlight;
        switch (point) {
        case 2: curve.SetPoint2(pos); break;
        case 3: curve.SetPoint3(pos); break;
        case 4: curve.SetPoint4(pos); break;
        }
    }
    path.push_back(curve);
    unsigned long count = GFRandom(3, GetPWorldSeed());
    for (unsigned long i = 0; i < count; ++i)
        if (!AddRandomBezierCurveToPath(path))
            return false;
    CBezierCurve last_curve;
    const C3DVector start = path.back().GetPoint4();
    last_curve.SetPoint1(start);
    C3DVector direction = start - path.back().GetPoint3();
    C3DVector control = start + direction;
    float ground = GetCreature().GetHeightZBelowAt(control + HeightProbeOffset, true, true);
    long tries = 10;
    while (ground + MinHeightToKeepAboveGroundInFlight > control.Z && tries > 0) {
        direction *= 0.5f;
        control = start + direction;
        ground = GetCreature().GetHeightZBelowAt(control + HeightProbeOffset, true, true);
        --tries;
    }
    float min_z = ground + MinHeightToKeepAboveGroundInFlight;
    if (control.Z < min_z)
        control.Z = min_z;
    last_curve.SetPoint2(control);
    last_curve.SetPoint4(destination);
    C3DVector mid;
    if (!GetRandomFlightPos(destination, FlightRange, mid, true))
        return false;
    mid.Z += MinHeightToKeepAboveGroundInFlight;
    last_curve.SetPoint3(mid);
    path.push_back(last_curve);
    return true;
}
