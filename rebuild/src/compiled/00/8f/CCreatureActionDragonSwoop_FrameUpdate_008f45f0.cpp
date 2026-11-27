// 008f45f0 CCreatureAction_DragonSwoop::FrameUpdate
// Leaf: ?FrameUpdate@CCreatureAction_DragonSwoop@@UAEXXZ
// Inline-budget: Retail expands all three AppendControlPoint calls. S79 leaves calls; S80 expands them and all four mapped copies match.
// Flags: /d2inlS80
#define FABLE_RETAIL_DRAGON_SWOOP_PASS29
#define FABLE_RETAIL_CCREATUREACTION_DRAGONSWOOP_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_KRAKEN_ROAR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_CROSSWITHUP
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_CTOR_INLINE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CCARDINALSPLINE_APPENDCONTROLPOINT
#define FABLE_RETAIL_CCARDINALSPLINE_GETSPLINEPOSITION
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_DECL_ONLY
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_GROUPED_CROSS_PASS22
#define FABLE_RETAIL_C3DVECTOR_CROSS_OPERATOR_PASS29
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CCreatureAction_DragonSwoop.h"
#include "engine/CCardinalSpline.h"

void CCreatureAction_DragonSwoop::FrameUpdate()
{
    CCardinalSpline spline;
    spline.AppendControlPoint(SwoopPoints[0]);
    spline.AppendControlPoint(SwoopPoints[1]);
    spline.AppendControlPoint(SwoopPoints[2]);
    spline.SetTension(0.5f);
    C3DVector pos, delta;
    spline.GetSplinePosition(pos, GetPercentageComplete());
    delta = pos - PCreature->GetPos();
    delta.AccurateNormalise();
    PCreature->GetPhysicsTC()->ResetPosition(pos);
    PCreature->GetPhysicsTC()->SetRHSet(CRightHandedSet(delta.CrossWithUp().Cross(delta), delta));
    CCombatActionBase::FrameUpdate();
}