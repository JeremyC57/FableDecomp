#define FABLE_RETAIL_CVECTORMAP_COMPONENT_LOWERBOUND_OWNER_PASS26
#define FABLE_RETAIL_STLP_BITSET_TEST_OWNER_PASS27
// 00832260 CThingAICreature::InitialActivate
// Flags:
// Leaf: ?InitialActivate@CThingAICreature@@UAEXXZ
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_LEVELSCRIPT
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGPHYSICAL_LEVELSCRIPT
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGGAMEOBJECT_LEVELSCRIPT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_LEVELSCRIPT
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGAICREATURE_HOMEBUILDING
#define FABLE_RETAIL_CTHINGAICREATURE_SAVE
#define FABLE_RETAIL_CTHINGAICREATURE_LEVELSCRIPT
#define FABLE_RETAIL_CTHINGBASEDEF
#define FABLE_RETAIL_CTHINGBUILDING
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CSTRINGPARSER
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_CSTRINGPARSER_READASUHUGE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_AI_RELATIONSHIPS_PASS29
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTHING_MANAGERACCESS
#define FABLE_RETAIL_CTHING_GETTHINGMANAGER
#define FABLE_RETAIL_CTHING_GETTC_ORIGINAL_CHAIN
#define FABLE_RETAIL_CTHINGAICREATURE_DEBUGRENDER
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CTHINGMANAGER

#define FABLE_RETAIL_IS_THING_ALIVE
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingBuilding.h"
#include "engine/CThingManager.h"
#include "engine/CAIBrain.h"
#include "engine/CTCFamilyMember.h"
#include "engine/ThingAlive.h"

#ifdef _MSC_VER
// Inline-budget: the 18-byte AI default-constructor owner selects the size-compiled 28-byte virtual cleanup.
#pragma optimize("s",on)
#endif
template<> inline CIntelligentPointer<CThingAICreature>::CIntelligentPointer() {}
template CIntelligentPointer<CThingAICreature>::CIntelligentPointer();

#ifdef _MSC_VER
#pragma optimize("t",on)
#endif
void CThingAICreature::InitialActivate()
{
    CThingGameObject::InitialActivate();
    if (!RelationshipsInit.IsNull()) {
        CTCFamilyMember* ptc_family_member=0;
        GetTC(&ptc_family_member);
        if (RelationshipsInit->HomeBuildingUID) {
            unsigned __int64 proper_uid=GetThingManager().RemapUIDForSerialisation(false,IsLevelLess(),GetCurrentMapNumber(),RelationshipsInit->HomeBuildingUID);
            CThing* pthing=GetThingManager().GetPThingWithUID(proper_uid);
            if (IS_THING_ALIVE(pthing)) {
                HomeBuilding=GFDebugPDynamicCast(pthing,(CThingBuilding*)0);
            }
        }
        if (RelationshipsInit->WorkBuildingUID) {
            unsigned __int64 proper_uid=GetThingManager().RemapUIDForSerialisation(false,IsLevelLess(),GetCurrentMapNumber(),RelationshipsInit->WorkBuildingUID);
            CThing* pthing=GetThingManager().GetPThingWithUID(proper_uid);
            if (IS_THING_ALIVE(pthing)) {
                WorkBuilding=GFDebugPDynamicCast(pthing,(CThingBuilding*)0);
            }
        }
        if (RelationshipsInit->FatherCreatureUID) {
            unsigned __int64 proper_uid=GetThingManager().RemapUIDForSerialisation(false,IsLevelLess(),GetCurrentMapNumber(),RelationshipsInit->FatherCreatureUID);
            CThing* pthing=GetThingManager().GetPThingWithUID(proper_uid);
            if (IS_THING_ALIVE(pthing)) {
                FatherCreature=GFDebugPDynamicCast(pthing,(CThingAICreature*)0);
                if (ptc_family_member) ptc_family_member->AddRelative(*FatherCreature);
                CTCFamilyMember* ptc_father_family_member;
                if (FatherCreature->GetTC(&ptc_father_family_member)) ptc_father_family_member->AddRelative(*this);
            }
        }
        if (RelationshipsInit->MotherCreatureUID) {
            unsigned __int64 proper_uid=GetThingManager().RemapUIDForSerialisation(false,IsLevelLess(),GetCurrentMapNumber(),RelationshipsInit->MotherCreatureUID);
            CThing* pthing=GetThingManager().GetPThingWithUID(proper_uid);
            if (IS_THING_ALIVE(pthing)) {
                MotherCreature=GFDebugPDynamicCast(pthing,(CThingAICreature*)0);
                if (ptc_family_member) ptc_family_member->AddRelative(*MotherCreature);
                CTCFamilyMember* ptc_mother_family_member;
                if (MotherCreature->GetTC(&ptc_mother_family_member)) ptc_mother_family_member->AddRelative(*this);
            }
        }
        if (RelationshipsInit->SpouseCreatureUID) {
            unsigned __int64 proper_uid=GetThingManager().RemapUIDForSerialisation(false,IsLevelLess(),GetCurrentMapNumber(),RelationshipsInit->SpouseCreatureUID);
            CThing* pthing=GetThingManager().GetPThingWithUID(proper_uid);
            if (IS_THING_ALIVE(pthing)) {
                SpouseCreature=GFDebugPDynamicCast(pthing,(CThingAICreature*)0);
                if (ptc_family_member) ptc_family_member->AddRelative(*SpouseCreature);
            }
        }
    }
    RelationshipsInit.Reset(0);
    if (EnableCreatureAutoPlacing && PBrain.Get()) PBrain->EnableCreatureAutoPlacing();
}