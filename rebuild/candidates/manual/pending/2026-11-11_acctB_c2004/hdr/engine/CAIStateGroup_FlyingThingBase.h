#ifndef FABLE_ENGINE_CAISTATEGROUP_FLYINGTHINGBASE_H
#define FABLE_ENGINE_CAISTATEGROUP_FLYINGTHINGBASE_H
// New header (chunk 2004, row 0x00912BA0 CAIStateGroup_FlyingThingBase::CalculatePrettyPath). Retail opt-in only:
// FABLE_RETAIL_CAISTATEGROUP_FLYINGTHINGBASE (needs FABLE_RETAIL_CAISTATEGROUPBASE, FABLE_RETAIL_CBEZIERCURVE and
// FABLE_RETAIL_STLP_DEQUE_PUSHBACK with stlport/fable_deque.h).
// Class: Fable.exe RTTI .?AVCAIStateGroup_FlyingThingBase@@ : CAIStateGroupBase (class key V). Members: Ego_r.pdb TPI (36 bytes; no STL
// member, the PDB offsets are retail's): MinHeightToKeepAboveGround +0x14, MinHeightToKeepAboveGroundInFlight +0x18 (0x00912C31),
// MaxHeightAboveGround +0x1C (0x00912C10 / 0x00912C26), FlightRange +0x20 (0x00912BE0). Methods (Ego_r.pdb / FableWin.pdb, protected):
// ?GetRandomFlightPos@CAIStateGroup_FlyingThingBase@@IBE_NABVC3DVector@@MAAV2@_N@Z (0x00912700, landed),
// ?AddRandomBezierCurveToPath@..@@IAE_NAAV?$deque@VCBezierCurve@@V?$allocator@VCBezierCurve@@@std@@@std@@@Z (0x00912840),
// ?CalculatePrettyPath@..@@IAE_NAAV?$deque@..@std@@ABVC3DVector@@@Z (0x00912BA0) and ?AIAction_RandomFlyToPos@..@@IAE?AW4EAIActionReturn@@ABVC3DVector@@@Z.
#ifdef FABLE_RETAIL_CAISTATEGROUP_FLYINGTHINGBASE
#include "rebuild_abi.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CBezierCurve.h"
#include "stlport/fable_deque.h"
class C3DVector;
class CAIStateGroup_FlyingThingBase : public CAIStateGroupBase {
protected:
    bool GetRandomFlightPos(const C3DVector& from, float range, C3DVector& pos, bool) const;   // 0x00912700
    bool AddRandomBezierCurveToPath(std::deque<CBezierCurve>& path);                         // 0x00912840
    bool CalculatePrettyPath(std::deque<CBezierCurve>& path, const C3DVector& destination);  // 0x00912BA0
    float MinHeightToKeepAboveGround;                                                         // +0x14
    float MinHeightToKeepAboveGroundInFlight;                                                 // +0x18
    float MaxHeightAboveGround;                                                               // +0x1c
    float FlightRange;                                                                        // +0x20
};
FABLE_STATIC_ASSERT(sizeof(CAIStateGroup_FlyingThingBase) == 0x24);
#endif
#endif // FABLE_ENGINE_CAISTATEGROUP_FLYINGTHINGBASE_H
