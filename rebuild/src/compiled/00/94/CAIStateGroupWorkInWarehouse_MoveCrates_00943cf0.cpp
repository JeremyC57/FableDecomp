// 00943cf0 CAIStateGroup_WorkInWarehouse::MoveCrates
#define FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD
#define FABLE_RETAIL_CBASICSTRING_COMPARE_BODY
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CDEFPOINTER_ORIGINAL_DIRECT_DEREF_PASS26
#define FABLE_RETAIL_CTHING_STATEACCESS
// Leaf: ?MoveCrates@CAIStateGroup_WorkInWarehouse@@AAE_NXZ
// Flags: /O2 /Oy /Gr
#define FABLE_RETAIL_WAREHOUSE_MOVE_CRATES_PASS29
#define FABLE_RETAIL_CAISTATEGROUP_MOVEGOODSBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_FILTER
#define FABLE_RETAIL_CVILLAGETASKCARRYCRATE
#define FABLE_RETAIL_CTCPRECALCULATEDNAVIGATIONROUTE_LOAD_PASS26
#define FABLE_RETAIL_CTCCRATESTACK
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CTCVILLAGE_PEEKMARKERS_PASS27
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CTHINGBASEDEF
#define FABLE_RETAIL_CCRATESTACKDEF_VIRTUAL_MODEL
#define FABLE_RETAIL_CCHARSTRING_APPEND_CHAR
#define FABLE_RETAIL_CTHING_GETSCRIPTNAME
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CTHINGMARKER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_MUTABLE
#define FABLE_RETAIL_CTHING_PEEKTC_ORIGINAL_CHAIN
#define FABLE_RETAIL_CTCCARRYING
#define FABLE_RETAIL_CDEFPOINTER
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_DONOR_NAMES_PASS23
#define FABLE_RETAIL_CVECTORMAP_ORIGINAL_FIND
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
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CDefPointer.h"
#include "engine/CThingBaseDef.h"
#include "engine/CCrateStackDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/CThingMarker.h"
#include "engine/CTCVillageMember.h"
#include "engine/CTCVillage.h"
#include "engine/CTCCarrying.h"
#include "engine/CTCPreCalculatedNavigationRoute.h"
#include "engine/CTCCrateStack.h"
#include "engine/CAIStateGroup_WorkInWarehouse.h"

#include "engine/CAIFilter_ShouldStillBeWorkingInWarehouse.h"

#ifdef _MSC_VER
// Native size-built default constructors select the two 28-byte deleting destructors.
#pragma optimize("s",on)
#endif
template<> inline CIntelligentPointer<CVillageTaskCarryCrate>::CIntelligentPointer() {}
template CIntelligentPointer<CVillageTaskCarryCrate>::CIntelligentPointer();
template<> inline CIntelligentPointer<CThingAICreature>::CIntelligentPointer() {}
template CIntelligentPointer<CThingAICreature>::CIntelligentPointer();

#ifdef _MSC_VER
#pragma optimize("t",on)
#endif
namespace {
    extern CCharString PICK_UP_ANIM_NAME_LEVEL1, PICK_UP_ANIM_NAME_LEVEL2;
    extern CCharString PUT_CRATE_IN_PILE_ANIM_NAME_LEVEL1, PUT_CRATE_IN_PILE_ANIM_NAME_LEVEL2;
}
bool CAIStateGroup_WorkInWarehouse::MoveCrates()
{
    if (PTask!=0 && PTask->IsValid()) {
        const CTCVillageMember* ptc_village_member;
        GetCreature().PeekTC(&ptc_village_member);
        CTCVillage* ptc_village=ptc_village_member->GetPVillage();
        const CThing* precalc_thing=0;
        CDefPointer<const CCrateStackDef> crate_stack_def;
        PTask->GetCrateStack()->PeekBaseDef().GetSubDef(&crate_stack_def);
        char stack_type_char='0'+crate_stack_def->StackType;
        CCharString script_name_to_look_for("Crate");
        script_name_to_look_for+=stack_type_char;
        typedef std::list<CIntelligentPointer<CThingMarker> >::const_iterator iterator;
        for (iterator it=ptc_village->PeekVillageMarkers().begin(),end_it=ptc_village->PeekVillageMarkers().end(); it!=end_it; ++it) {
            CThingMarker* pmarker=it->Get();
            if (pmarker && pmarker->HasTC(TCI_PRE_CALCULATED_NAVIGATION_ROUTE) && pmarker->GetScriptName()==script_name_to_look_for) {
                CTCPreCalculatedNavigationRoute* ptc_pre_calc_route=0;
                pmarker->GetTC(&ptc_pre_calc_route);
                float distance_to_target=(ptc_pre_calc_route->GetRouteDestination()-GetCreature().GetPos()).GetMagnitude();
                float beginnings_distance_to_target=(ptc_pre_calc_route->GetRouteDestination()-pmarker->GetPos()).GetMagnitude();
                const float MULTIPLIER_FOR_DISTANCE=1.2f;
                if (beginnings_distance_to_target<distance_to_target*MULTIPLIER_FOR_DISTANCE) {precalc_thing=pmarker;break;}
            }
        }
        CThing* crate_stack=PTask->GetCrateStack();
        CTCCrateStack* ptc_crate_stack=0;
        crate_stack->GetTC(&ptc_crate_stack);
        if (PTask->IsPickup()) {
            std::vector<const CCharString*> anims;
            anims.push_back(&PICK_UP_ANIM_NAME_LEVEL1);
            anims.push_back(&PICK_UP_ANIM_NAME_LEVEL2);
            if (FetchGoods(crate_stack,4.0f,anims,CAIFilter_ShouldStillBeWorkingInWarehouse(PTask,&GetCreature(),ptc_village),precalc_thing)) return true;
        } else {
            const CTCCarrying* ptc_carrying;
            GetCreature().PeekTC(&ptc_carrying);
            CIntelligentPointer<CThing> pcrate(ptc_carrying->GetThingInPrimarySlot());
            std::vector<const CCharString*> anims;
            anims.push_back(&PUT_CRATE_IN_PILE_ANIM_NAME_LEVEL1);
            anims.push_back(&PUT_CRATE_IN_PILE_ANIM_NAME_LEVEL2);
            if (PlaceGoods(crate_stack,4.0f,anims,CAIFilter_ShouldStillBeWorkingInWarehouse(PTask,&GetCreature(),0),precalc_thing)) {
                if (ptc_crate_stack) ptc_crate_stack->AddCrate(pcrate.Get());
                return true;
            }
        }
    }
    return false;
}