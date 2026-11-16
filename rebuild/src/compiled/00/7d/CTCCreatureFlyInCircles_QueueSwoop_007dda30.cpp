// CTCCreatureFlyInCircles::QueueSwoop(const CThingCreatureBase& target, const C3DVector& start, const C3DVector& middle, const C3DVector& end) -- remember the swoop target and offset, set the swoop speed from the path length, rebuild the swoop spline (start, middle, end offset by the target's position), clear the 0x3a component's first flag and move the creature to the start
// Identity: Ego_r.pdb / FableWin.pdb ?QueueSwoop@CTCCreatureFlyInCircles@@QAEXABVCThingCreatureBase@@ABVC3DVector@@11@Z; members by Ego_r layout.
// Leaf: ?QueueSwoop@CTCCreatureFlyInCircles@@QAEXABVCThingCreatureBase@@ABVC3DVector@@11@Z
//
// Retail 0x007DDA30, 642 bytes (`ret 0x10`).
// Callees: CBaseIntelligentPointer ctor 0x00A01B10 / SetPItem 0x00A01B90 / GetPItem 0x00A01B50 / dtor 0x00A01C10 (the
//   CIntelligentPointer<const CThingCreatureBase> temporary of `PSwoopTarget = &target`, vtable 0x0125AAEC), CThing::GetPos
//   0x004C73D0, STLport __copy<C3DVector*> 0x0074CA20 (__fastcall random-access copy: the vector clear of DeleteAllControlPoints),
//   vector<C3DVector>::_M_insert_overflow 0x0066E370 (the three AppendControlPoint push_backs), CVectorMap::LowerBound
//   0x0040F020 (GetTC(TCI_MAPWHO)), CTCPhysicsBase::ResetPosition (slot 31, +0x7c). Constant: 2.0f 0x0126FA6C (.rdata float pool).
// Headers: all shared. New engine/CTCCreatureFlyInCircles.h (Ego_r members at retail offsets); engine/CCardinalSpline.h
//   DeleteAllControlPoints / AppendControlPoint (FableWin.pdb names; new opt-in, AppendControlPoint expanded per TU); the shared
//   STLport vector<C3DVector> (non-POD: __false_type _M_insert_overflow 0x0066E370 visible out of line, erase through the
//   out-of-line random-access __copy, FABLE_RETAIL_STLP_COPY_NOINLINE); CTCMapwho::SetInMap (FableWin.pdb, new opt-in); CThing::GetTC;
//   CIntelligentPointer operator= (the item read into a local first).
// Shape: C3DVector operator- groups its X difference (FABLE_RETAIL_C3DVECTOR_SUBTRACT_X_GROUP, a no-code PAREN tuple) and
//   GetMagnitude is the FableWin named-squared text (FABLE_RETAIL_C3DVECTOR_MAGNITUDE_NAMED_SQUARED): together they put the
//   clear()'s `push 0` between the second magnitude's `fld st(4)` and `fmul st(5)` (0x007DDB16).
#include "fable_compiler.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_SUBTRACT_X_GROUP
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE_NAMED_SQUARED
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_STATTYPE
#define FABLE_RETAIL_CTCMAPWHO_SETINMAP
#define FABLE_RETAIL_CCARDINALSPLINE_APPENDCONTROLPOINT
#define FABLE_RETAIL_CCARDINALSPLINE_APPENDCONTROLPOINT_FORCEINLINE
#define FABLE_RETAIL_STLP_COPY_NOINLINE
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#define FABLE_STLP_OUTOFLINE_UNINIT_COPY
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CTCMapwho.h"
#include "engine/CCardinalSpline.h"
#include "engine/CTCCreatureFlyInCircles.h"

void CTCCreatureFlyInCircles::QueueSwoop(const CThingCreatureBase& target, const C3DVector& start, const C3DVector& middle,
                                         const C3DVector& end)
{
    PSwoopTarget = &target;
    Mode = (EFlyMode)2;
    Transitioning = false;
    SwoopTargOffset = middle;
    C3DVector targetPos = PSwoopTarget->GetPos();
    SwoopProgress = 0.0f;
    SwoopDelta = 2.0f / ((end - middle).GetMagnitude() + (middle - start).GetMagnitude());
    SwoopSpline.DeleteAllControlPoints();
    SwoopSpline.AppendControlPoint(targetPos + start);
    SwoopSpline.AppendControlPoint(targetPos + middle);
    SwoopSpline.AppendControlPoint(targetPos + end);
    CTCMapwho* mapwho;
    if (Creature->GetTC(TCI_MAPWHO, &mapwho))
        mapwho->SetInMap(false);
    Creature->GetPhysicsTC()->ResetPosition(start);
}