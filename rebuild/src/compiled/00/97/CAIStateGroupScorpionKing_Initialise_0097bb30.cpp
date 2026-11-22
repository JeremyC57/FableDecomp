// CAIStateGroup_ScorpionKing::Initialise(...) -- collect live arena traps and deactivate them, then reset charging.
// Leaf: ?Initialise@CAIStateGroup_ScorpionKing@@UAEXAAVCAIBrain@@AAVCThingAICreature@@JW4EScriptAIPriority@@ABVCCharString@@@Z
// Flags: (none)
// Evidence: both release PDB methods; Ego_d vector<CThing*> things, indices, n and scoped ptc_trap.
#define FABLE_RETAIL_STLP_PASS22_FLARE_ABI
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#define FABLE_STLP_OUTOFLINE_UNINIT_COPY
#define FABLE_STLP_OUTOFLINE_UNINIT_FILL_N
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_GETTCDRIVER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ORIGINAL_SETPTR_PASS22
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEK
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEK_PROTECTED
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUP_SCORPIONKING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_CLIPAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_EGOD
#define FABLE_RETAIL_CTHINGMAP_PEEKCELL_INLINE
#define FABLE_RETAIL_C2DBOXI_COPY
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DBOXI_RADIUS_INITIALIZERS_FIRST
#define FABLE_RETAIL_C2DBOXI_GETWIDTH
#define FABLE_RETAIL_C2DBOXI_GETHEIGHT
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_STATTYPE
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_CTCTRAPBASE
#define FABLE_RETAIL_IS_THING_ALIVE
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CThingAICreature.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CAIStateGroup_ScorpionKing.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CTCMapwho.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingFilter_HasDefGlobalIndex.h"
#include "engine/C2DBoxI.h"
#include "engine/CTCTrapBase.h"
#include "engine/ThingAlive.h"
void CAIStateGroup_ScorpionKing::Initialise(CAIBrain& brain,CThingAICreature& creature,long priority,EScriptAIPriority script_ai_priority,const CCharString& name)
{
    CAIStateGroupBase::Initialise(brain,creature,priority,script_ai_priority,name);
    std::vector<CThing*> things;
    long speartrap_def_index=CGameDefinitionManager::Get().GetDefGlobalIndexFromName("OBJECT_HERO_ARENA_SPEAR_TRAP_01");
    long bladetrap_def_index=CGameDefinitionManager::Get().GetDefGlobalIndexFromName("OBJECT_HERO_ARENA_BLADES_TRAP_01");
    PeekThingSearchTools().GetAllThingsInRoughArea<CThing>(C2DBoxI(GetCreature().GetPos().ToC2DCoordI(),40),CThingFilter_HasDefGlobalIndex(speartrap_def_index),things);
    PeekThingSearchTools().GetAllThingsInRoughArea<CThing>(C2DBoxI(GetCreature().GetPos().ToC2DCoordI(),40),CThingFilter_HasDefGlobalIndex(bladetrap_def_index),things);
    long n;
    for(n=0;n<things.size();++n) {
        if(IS_THING_ALIVE(things[n])) {
            TrapThings.push_back(things[n]);
            CTCTrapBase* ptc_trap;
            if(things[n]->GetTC(&ptc_trap))ptc_trap->ManualDeactivate();
        }
    }
    ChargeCount=0;
}