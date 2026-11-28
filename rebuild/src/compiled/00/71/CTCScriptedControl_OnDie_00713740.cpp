// Flags: 
// 00713740 CTCScriptedControl::OnDie
// Leaf: ?OnDie@CTCScriptedControl@@UAEXXZ
#define FABLE_RETAIL_CANIMSET_TYPE
#define FABLE_RETAIL_CTCCREATURENAVIGATION
#define FABLE_RETAIL_CTCCREATURENAVIGATION_NAVIGABLE
#define FABLE_RETAIL_CTC_SCRIPTED_CONTROL_DIE_PASS30
#define FABLE_RETAIL_CANIMATETHING_UPDATEANIMATION
#define FABLE_RETAIL_CTCANIMATIONCOMPLEX_FULL
#define FABLE_RETAIL_CTCANIMATIONCOMPLEX_GETANIMATION_PASS26
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CAnimateThing.h"
#include "engine/CTCAnimationComplex.h"
#include "engine/CTCScriptedControl.h"
#include "engine/CTCCreatureNavigation.h"

void CTCScriptedControl::OnDie()
{
    CTCCreatureNavigation* pnav;
    if (GetParentThing().GetTC(&pnav))
        pnav->InterruptNavigation();
    CTCAnimationComplex* ptc_animation;
    if (GetParentThing().GetTC(&ptc_animation))
        ptc_animation->GetAnimation().SetInterruptable(true);
    CTCBase::OnDie();
}
