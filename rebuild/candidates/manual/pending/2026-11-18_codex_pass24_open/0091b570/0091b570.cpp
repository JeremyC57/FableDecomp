// CAIStateGroup_SeekPerceivedThing::Update() -- alert nearby creatures and run toward the perceived thing.
// Grade: DIFFER(515v498) -- Shared list, filter, perception and animation types preserve the call boundaries. The compiler still allocates the actor, list and rectangle temporaries differently.
// Leaf: ?Update@CAIStateGroup_SeekPerceivedThing@@UAEXXZ
// Retail: 0x0091B570, 498 bytes, public virtual thiscall.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_ACTIONQUERY
#define FABLE_RETAIL_CTHINGCREATUREBASE_ANIMATION
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTCPERCEIVETHING
#define FABLE_RETAIL_CTCPERCEIVETHING_AWARENESS
#define FABLE_RETAIL_CTCPERCEIVETHING_ALERTNEARBY_PASS24
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION
#define FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION_IMPLICIT_DTOR
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETTHINGMANAGER_PASS24
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAISTATEGROUPBASE_TARGETFACE
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_CAISTATEGROUP_SEEKPERCEIVEDTHING
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTHINGMANAGER_ACCESSORS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_TCSINROUGHAREA
#define FABLE_RETAIL_CTHINGFILTER_ISPERCEIVINGTHING
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_DTOR_OUTOFLINE
#define FABLE_RETAIL_STLP_MEM_FUN_PTR
#include "fable_string.h"
#include "engine/fable_ai_action_enums.h"
#include "engine/CThingAICreature.h"
#include "engine/CTCPerceiveThing.h"
#include "engine/CSoundPair.h"
#include "engine/CCreatureAction_PlayAnimation.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CAIStateGroup_SeekPerceivedThing.h"
#include "engine/CThingManager.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingFilter_IsNot.h"
#include "engine/CThingFilter_IsPerceivingThing.h"
#include "engine/CFilter_Bind2.h"
#include "engine/C2DBoxI.h"
#include "stlport/fable_algorithm.h"

void CAIStateGroup_SeekPerceivedThing::Update()
{
    const CTCPerceiveThing* tc_perception;
    if (!GetCreature().GetTC(TCI_PERCEIVE_THING, &tc_perception) || !tc_perception->IsAwareOfThing())
        return;
    if (tc_perception->IsAwarenessOfThingDirect()) {
        std::list<CTCPerceiveThing*> tclist;
        const C3DVector& pos = GetCreature().GetPos();
        C2DBoxI area;
        area.Initialise((long)(pos.X - 8.0f), (long)(pos.Y - 8.0f),
                        (long)(pos.X + 8.0f), (long)(pos.Y + 8.0f));
        GetThingManager().GetThingSearchTools().GetAllTCsInRoughArea<CTCPerceiveThing>(area,
            CFilter_Bind2<CThingFilter_IsPerceivingThing, CThingFilter_IsNot>(
                CThingFilter_IsPerceivingThing(*tc_perception->GetPerceivedThing()), CThingFilter_IsNot(GetCreature())),
            tclist);
        std::for_each(tclist.begin(), tclist.end(), std::mem_fun(&CTCPerceiveThing::SetAsIndirectlyAwareOfPerceivedThing));
    }
    const CAnimationEntry& anim_entry = GetCreature().GetAnimFromName("ALARM", 0);
    CCreatureAction_PlayAnimation action(GetCreature(), anim_entry, CSoundPair(), false, true, false, 0, 0);
    AIAction_PerformActionTargetFaceAndWait(action, CIntelligentPointer<const CThing>(tc_perception->GetPerceivedThing()), false);
    RunToPerceivedThingPosition();
}
