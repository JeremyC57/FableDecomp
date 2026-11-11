#ifndef FABLE_ENGINE_CAISTATEGROUP_FLYINGTHINGBASE_H
#define FABLE_ENGINE_CAISTATEGROUP_FLYINGTHINGBASE_H
// NEW header (chunk Q6, row 0x00912840), hand-written to retail: CAIStateGroup_FlyingThingBase : CAIStateGroupBase (Ego_r.pdb
// layout 0x24: the 0x14-byte base, then MinHeightToKeepAboveGround +0x14, MinHeightToKeepAboveGroundInFlight +0x18,
// MaxHeightAboveGround +0x1c, FlightRange +0x20). Requires engine/CAIStateGroupBase.h's retail form (FABLE_RETAIL_CAISTATEGROUPBASE
// + _MEMBERS) and stlport/fable_deque.h. Protected members: the Ego_r.pdb spellings (IBE / IAE).
#include "rebuild_abi.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CBezierCurve.h"
#include "stlport/fable_deque.h"
class C3DVector;
class CAIStateGroup_FlyingThingBase : public CAIStateGroupBase {
protected:
    bool GetRandomFlightPos(const C3DVector& centre, float range, C3DVector& out, bool allowGround) const;   // 0x00912700
    bool AddRandomBezierCurveToPath(std::deque<CBezierCurve>& path);                                        // 0x00912840
    bool CalculatePrettyPath(std::deque<CBezierCurve>& path, const C3DVector& target);
    EAIActionReturn AIAction_RandomFlyToPos(const C3DVector& pos);
    float MinHeightToKeepAboveGround;                                    // +0x14
    float MinHeightToKeepAboveGroundInFlight;                            // +0x18
    float MaxHeightAboveGround;                                          // +0x1c
    float FlightRange;                                                   // +0x20
};
FABLE_STATIC_ASSERT(sizeof(CAIStateGroup_FlyingThingBase) == 0x24);
#endif // FABLE_ENGINE_CAISTATEGROUP_FLYINGTHINGBASE_H
