// CAIStateGroup_FlyingThingBase::AddRandomBezierCurveToPath(std::deque<CBezierCurve>&) -- continue the path smoothly from its last curve: mirror the previous control point (halving it until it is above ground, on the map and in the same map region), then pick two random flight positions for the remaining control points and append the curve
// Leaf: ?AddRandomBezierCurveToPath@CAIStateGroup_FlyingThingBase@@IAE_NAAV?$deque@VCBezierCurve@@V?$allocator@VCBezierCurve@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00912840, 860 bytes, `ret 4`, thiscall; protected (Ego_r.pdb
//   ?AddRandomBezierCurveToPath@CAIStateGroup_FlyingThingBase@@IAE_NAAV?$deque@VCBezierCurve@@V?$allocator@VCBezierCurve@@@std@@@std@@@Z).
// Callees: CAIStateGroupBase::PeekWorldMap 0x008FCFF0, the world map's GetMapNumberAt (virtual slot 16), CThingCreatureBase::GetHeightZBelowAt
//   0x00666D40, CAIStateGroup_FlyingThingBase::GetRandomFlightPos 0x00912700, CAIStateGroupBase::GetPWorldSeed 0x008FCFD0,
//   GFFloatRandom 0x004978A0 (__fastcall), deque<CBezierCurve>::_M_push_back_aux_v 0x00913D60; HeightProbeOffset 0x0138C500.
// Lever: retail's TU holds the STLport _M_push_back_aux_v body, so the curve's address is not captured and the inline
//   push_back copy is the software-pipelined 12-dword copy (0x00912B19..0x00912B46); with a declaration only, each C3DVector
//   copy pairs its load and store. The body is the STLport 4.6 _deque.c one, kept out of line (auto_inline off).
// Headers: engine/CAIStateGroupBase.h retail form (PeekWorldMap / GetPWorldSeed / GetCreature), engine/CThingAICreature.h,
//   engine/CWorldMap.h retail, engine/CBezierCurve.h + engine/CAIStateGroup_FlyingThingBase.h (new), stlport/fable_deque.h
//   (FABLE_STLPORT_DEQUE_PUSH_BACK + _AUX_NOINLINE); grade with FABLE_HDR_OVERLAY=agentQ6/hdr.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_HEIGHTZBELOW
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_SPEECH
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_STLPORT_DEQUE_PUSH_BACK
#define FABLE_STLPORT_DEQUE_PUSH_BACK_AUX_NOINLINE
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CWorldMap.h"
#include "engine/GFFloatRandom.h"
#include "engine/CAIStateGroup_FlyingThingBase.h"
extern const C3DVector HeightProbeOffset;          // 0x0138C500

bool CAIStateGroup_FlyingThingBase::AddRandomBezierCurveToPath(std::deque<CBezierCurve>& path)
{
    CBezierCurve curve;
    C3DVector end = path.back().Point4;
    curve.Point1 = end;
    C3DVector dir = end - path.back().Point3;
    C3DVector control = dir + end;
    long mapNumber = PeekWorldMap().GetMapNumberAt(end);
    float height = GetCreature().GetHeightZBelowAt(HeightProbeOffset + control, true, true);
    long tries = 10;
    while ((height + MinHeightToKeepAboveGroundInFlight > control.Z || control.X < 0.0f || control.Y < 0.0f
            || PeekWorldMap().GetMapNumberAt(control) != mapNumber) && tries > 0) {
        dir *= 0.5f;
        control = dir + end;
        height = GetCreature().GetHeightZBelowAt(HeightProbeOffset + control, true, true);
        --tries;
    }
    curve.Point2 = control;
    for (unsigned long i = 0; i < 2; ++i) {
        C3DVector pos;
        if (!GetRandomFlightPos(end, FlightRange, pos, false))
            return false;
        switch (i) {
        case 0:
            pos.Z = pos.Z + MinHeightToKeepAboveGroundInFlight;
            curve.Point3 = pos;
            break;
        case 1: {
            unsigned long* seed = GetPWorldSeed();
            pos.Z = GFFloatRandom(MaxHeightAboveGround, seed) + pos.Z + MinHeightToKeepAboveGroundInFlight;
            curve.Point4 = pos;
            break;
        }
        }
    }
    path.push_back(curve);
    return true;
}