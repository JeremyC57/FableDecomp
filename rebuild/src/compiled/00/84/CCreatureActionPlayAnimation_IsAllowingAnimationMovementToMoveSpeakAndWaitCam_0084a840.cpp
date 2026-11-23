// Layout: retail STLport 4.6 container sizes put the PDB member on these bytes; renamed `f_3` -> `AnimMayNeedCameraPositionUpdatedInSpeakAndWait`.
// Leaf: ?IsAllowingAnimationMovementToMoveSpeakAndWaitCameras@CCreatureAction_PlayAnimation@@UBE_NXZ
// Switch route (NF318, 2026-11-23): defined on the real class of engine/CCreatureAction_PlayAnimation.h (class-model switch FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION; was ?IsAllowingAnimationMovementToMoveSpeakAndWaitCameras@CCreatureAction_PlayAnimation_Methods@@QAEEXZ);
//   the retail RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) names this PDB member at this address. Bytes unchanged.
// Preamble: the define / include block of the landed precedent NTCScriptedControlCActionPlayAnimation_Update_00712900.cpp.
#define FABLE_RETAIL_CSOUNDPAIR_ORIGINAL_CTORS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCSCRIPTEDCONTROL_ACTIONBASE
#define FABLE_RETAIL_CTCSCRIPTEDCONTROL_CREATURE
#define FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_ACTIONQUERY
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_STLP_LIST
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThingCreatureBase.h"
#include "engine/NTCScriptedControl_CActionPlayAnimation.h"
#include "engine/CSoundPair.h"
#include "engine/CCreatureAction_PlayAnimationWithLookTurning.h"
#include "engine/CCreatureAction_PlayAnimation.h"  // retyped onto the PDB layout; byte parity re-verified
bool CCreatureAction_PlayAnimation::IsAllowingAnimationMovementToMoveSpeakAndWaitCameras() const{ return this->AnimMayNeedCameraPositionUpdatedInSpeakAndWait; }
