// CThingAICreature::SerialiseGameState(CPersistContext& context) -- after the base creature serialise, persist the relationship block (home/work buildings, father, mother, spouse as saved UIDs), the initial position, the overriding brain name and the ten AI flag bits
// Grade: RELOCATION_MATCH
// Leaf: ?SerialiseGameState@CThingAICreature@@UAE_NAAVCPersistContext@@@Z
// Retail: 0x00830FE0, 1504 bytes, virtual thiscall.
// Flags: none
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_LEVELSCRIPT
#define FABLE_RETAIL_CTHINGCREATUREBASE_CONSTRUCT
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGAICREATURE_HOMEBUILDING
#define FABLE_RETAIL_CTHINGAICREATURE_SAVE
#define FABLE_RETAIL_CTHINGAICREATURE_LEVELSCRIPT
#define FABLE_RETAIL_CTHINGAICREATURE_SERIALISEGAMESTATE
#define FABLE_RETAIL_CTHINGBUILDING
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CPERSISTCONTEXT_TRANSFER_BINARY_BODY
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAINPUTSTREAM_ORIGINAL_READ_SCOPE
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CPERSISTTRAITS_FLOAT_CLASS
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CCOUNTEDPOINTER_RESET_CAMERA_DECL
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingBuilding.h"
#include "engine/CPersistContext.h"
#include "engine/CDataOutputStream.h"
#include "engine/CPersistTraits.h"
#include "engine/CMemoryDataInputStream.h"

// Inline-budget: The original same-TU float trait reader has a retail
// out-of-line COMDAT at 0x0040EFB0. Make its lack of object access visible.
#ifdef _MSC_VER
__declspec(noinline)
#endif
void CPersistTraits<float>::TransferBinaryIn(CMemoryDataInputStream& stream, float& value)
{
    float tval;
    stream.Read(&tval, sizeof(tval));
    value = tval;
}

bool CThingAICreature::SerialiseGameState(CPersistContext& context)
{
    if (CThingCreatureBase::SerialiseGameState(context)) {
        bool has_relationships = true;
        context.Transfer("HasRelationships", has_relationships, false);
        RelationshipsInit.Reset(new CRelationshipsInit);
        if (context.IsSaving()) {
            if (HomeBuilding.Get() && !HomeBuilding.Get()->IsDead())
                RelationshipsInit->HomeBuildingUID = HomeBuilding.Get()->GetUniqueID();
            if (WorkBuilding.Get() && !WorkBuilding.Get()->IsDead())
                RelationshipsInit->WorkBuildingUID = WorkBuilding.Get()->GetUniqueID();
            if (FatherCreature.Get() && !FatherCreature.Get()->IsDead())
                RelationshipsInit->FatherCreatureUID = FatherCreature.Get()->GetUniqueID();
            if (MotherCreature.Get() && !MotherCreature.Get()->IsDead())
                RelationshipsInit->MotherCreatureUID = MotherCreature.Get()->GetUniqueID();
            if (SpouseCreature.Get() && !SpouseCreature.Get()->IsDead())
                RelationshipsInit->SpouseCreatureUID = SpouseCreature.Get()->GetUniqueID();
        }
        context.Transfer(InitialPos.X);
        context.Transfer(InitialPos.Y);
        context.Transfer(InitialPos.Z);
        if (has_relationships) {
            context.Transfer("HomeBuildingUID", RelationshipsInit->HomeBuildingUID, (unsigned __int64)0);
            context.Transfer("WorkBuildingUID", RelationshipsInit->WorkBuildingUID, (unsigned __int64)0);
            context.Transfer("FatherCreatureUID", RelationshipsInit->FatherCreatureUID, (unsigned __int64)0);
            context.Transfer("MotherCreatureUID", RelationshipsInit->MotherCreatureUID, (unsigned __int64)0);
            context.Transfer("SpouseCreatureUID", RelationshipsInit->SpouseCreatureUID, (unsigned __int64)0);
        }
        if (context.IsSaving())
            RelationshipsInit.ReleaseData();
        context.Transfer("OverridingBrainName", OverridingBrainName);
        {
            bool temp = EnableCreatureAutoPlacing;
            context.Transfer("EnableCreatureAutoPlacing", temp, false);
            EnableCreatureAutoPlacing = temp;
        }
        {
            bool temp = HasInformation;
            context.Transfer("HasInformation", temp, false);
            HasInformation = temp;
        }
        {
            bool temp = WanderWithInformation;
            context.Transfer("WanderWithInformation", temp, false);
            WanderWithInformation = temp;
        }
        {
            bool temp = WaveWithInformation;
            context.Transfer("WaveWithInformation", temp, false);
            WaveWithInformation = temp;
        }
        {
            bool temp = ContinueAIWithInformation;
            context.Transfer("ContinueAIWithInformation", temp, false);
            ContinueAIWithInformation = temp;
        }
        {
            bool temp = AllowedToFollowHero;
            context.Transfer("AllowedToFollowHero", temp, false);
            AllowedToFollowHero = temp;
        }
        {
            bool temp = RegionFollowingOverriddenFromScript;
            context.Transfer("RegionFollowingOverriddenFromScript", temp, false);
            RegionFollowingOverriddenFromScript = temp;
        }
        {
            bool temp = RespondingToFollowAndWait;
            context.Transfer("RespondingToFollowAndWait", temp, true);
            RespondingToFollowAndWait = temp;
        }
        {
            bool temp = CanBeCourted;
            context.Transfer("CanBeCourted", temp, false);
            CanBeCourted = temp;
        }
        {
            bool temp = CanBeMarried;
            context.Transfer("CanBeMarried", temp, false);
            CanBeMarried = temp;
        }
        return true;
    }
    return false;
}
