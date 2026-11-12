// CAIStateGroup_MinionWalkToMarker::Update() -- walk the minion between markers carrying the AI scratchpad's TargetName: first to
// the nearest such marker, then, while the scratchpad's ScriptingStateGroup stays ESSG_WALK_TO_RANDOM (8) or
// ESSG_WALK_TO_NEAREST_DIFFERENT (9), pick the next one (8: a GFRandom pick from all of them; 9: the nearest one that is neither
// of the last two) and walk to it (MoveToThingProximity repeated while it reports AI_ACTION_IN_PROGRESS); stop with Terminate
// when the group changes or the state group is stopping.
// Leaf: ?Update@CAIStateGroup_MinionWalkToMarker@@MAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00922010, 651 bytes, thiscall, plain `ret`, frame 0x4c, no EH frame
//   (vtable 0x0128A84C slot 12; Ego_r.pdb / FableWin.pdb ?Update@CAIStateGroup_MinionWalkToMarker@@MAEXXZ).
// Callees: CThing::GetTC<CTCAIScratchpad> expanded (CVectorMap::LowerBound 0x0040F020, TCI_AI_SCRATCHPAD 0xAA),
//   CThingPhysical::PeekPos 0x006A4D00, CCharString copy ctor 0x0099EC30 / ~CCharString 0x0099EAE0,
//   CAIStateGroupBase::PeekThingSearchTools 0x008FD000 / GetPWorldSeed 0x008FCFD0 / AIAction_MoveToThingProximity 0x0097E380 /
//   IsStopping 0x008FD010 / Terminate 0x008FCF40, CThingSearchTools::GetPBestThing<CThingFilter_HasScriptName,
//   CThingCompare_Nearest,CThing> 0x00922320, GetAllThings<CThing,vector<CThing*>,CThingFilter_HasScriptName> 0x005ED860,
//   GetPBestThing<CFilter_Bind3<CThingFilter_IsNot,CThingFilter_IsNot,CThingFilter_HasScriptName>,CThingCompare_Nearest,CThing>
//   0x009223A0, GFRandom (expanded; GFROR13 0x00497890), free 0x00BFEA14 (vector dtor), CIntelligentPointer<const CThing>(T*)
//   inline (CBaseIntelligentPointer ctor 0x00A01B10, SetPItem 0x00A01B90).
// Shape: the search templates' bodies are visible to the TU (same-TU noinline bodies; 0x00922320 / 0x009223A0 follow this
//   function), and so is CThingCompare_Nearest::operator() (inline header body): the caller then knows the Nearest temporaries are
//   not captured and schedules the Pos copy freely (loads first, stores late). The group is read into a local for the 8/9 dispatch
//   (retail re-tests `cmp eax,8` after the loop condition), mode 8 falls through, mode 9 is placed out of line.
// CAIStateGroup_MinionWalkToMarker has no shared header: modelled on its CAIStateGroupBase base with only the overridden slot.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ALLTHINGSBODY
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTTHING_BODY
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEK
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#include "rebuild_abi.h"
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/fable_ai_action_enums.h"
#include "engine/GFRandom.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingPhysical.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CTCAIScratchpad.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingFilter_HasScriptName.h"
#include "engine/CThingFilter_IsNot.h"
#include "engine/CThingCompare_Nearest.h"
#include "engine/CFilter_Bind3.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"

class CAIStateGroup_MinionWalkToMarker : public CAIStateGroupBase {
protected:
    virtual void Update();
};
void CAIStateGroup_MinionWalkToMarker::Update()
{
    CTCAIScratchpad* scratchpad;
    GetCreature().GetTC(&scratchpad);
    CThing* target;
    {
        CThingCompare_Nearest nearest(GetCreature().PeekPos());
        CCharString name(scratchpad->TargetName);
        CThingFilter_HasScriptName filter(name);
        PeekThingSearchTools().GetPBestThing(filter, nearest, &target);
    }
    CThing* previous = 0;
    CThing* beforePrevious = 0;
    while (scratchpad->ScriptingStateGroup == ESSG_WALK_TO_RANDOM || scratchpad->ScriptingStateGroup == ESSG_WALK_TO_NEAREST_DIFFERENT) {
        EScriptingStateGroups group = scratchpad->ScriptingStateGroup;
        if (group == ESSG_WALK_TO_RANDOM) {
            std::vector<CThing*> things;
            {
                CCharString name(scratchpad->TargetName);
                CThingFilter_HasScriptName filter(name);
                PeekThingSearchTools().GetAllThings<CThing>(filter, things);
            }
            target = things[GFRandom(things.size(), GetPWorldSeed())];
        } else {
            CThingCompare_Nearest nearest(GetCreature().PeekPos());
            CCharString name(scratchpad->TargetName);
            CThingFilter_HasScriptName filter(name);
            CThingFilter_IsNot notPrevious(*previous);
            CThingFilter_IsNot notBeforePrevious(*beforePrevious);
            PeekThingSearchTools().GetPBestThing(CFilter_Bind3<CThingFilter_IsNot, CThingFilter_IsNot, CThingFilter_HasScriptName>(notPrevious, notBeforePrevious, filter), nearest, &target);
        }
        bool reached;
        while ((scratchpad->ScriptingStateGroup == ESSG_WALK_TO_RANDOM || scratchpad->ScriptingStateGroup == ESSG_WALK_TO_NEAREST_DIFFERENT) &&
               AIAction_MoveToThingProximity(ACTION_REPEAT_CALLBACK, target, 0.0f, reached, AI_MOVEMENT_WALKING, true, false, true, true, 0.0f) == AI_ACTION_IN_PROGRESS)
            ;
        if (IsStopping())
            break;
        beforePrevious = previous;
        previous = target;
    }
    Terminate();
}