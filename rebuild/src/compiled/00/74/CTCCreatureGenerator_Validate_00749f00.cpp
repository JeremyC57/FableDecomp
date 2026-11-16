// CTCCreatureGenerator::Validate(CCharString& error) const -- a generator needs creature families, and each family's generation types need the matching start positions (hidden for type 2, navigable open ones for types 0 and 1); GENERATE_GENERATOR_ANIMATION (3) is not supported
// Leaf: ?Validate@CTCCreatureGenerator@@UBE_NAAVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00749F00, 510 bytes (vtable slot 17). Name: Ego_r.pdb 0x0075BFF0 / FableWin.pdb 0x025151A0.
// Callees: CTCBase::GetDefinitionManager 0x006869D0, CDefinitionManager::GetDef<CCreatureGenerationFamilyDef>(long,
//   CDefPointer<const CCreatureGenerationFamilyDef>*) const 0x0074D220 (FableWin.pdb template spelling), the def's GetInstantiationName
//   (virtual slot 10) and CDefStringTable::GetString 0x009D49B0 on GDefStringTable 0x013CA828, CCharString assignments 0x0099EFE0 /
//   0x0099EFB0, += 0x0099F100, operator+(const char*, const CCharString&) 0x0099F690, ~CCharString 0x0099EAE0, the out-of-line STLport
//   __find over the family's GenerationTypes (__fastcall, called four times).
// Layout: engine/CTCCreatureGenerator.h (CreatureFamilies +0x0C, OpenPositions +0x60, HiddenPositions +0x6C) and
//   engine/CCreatureGenerationFamilyDef.h (GenerationTypes +0x50).
// NF41 (2026-11-16): was a local CCharString / CDefStringTable / CVector / CDefPointer / CDefinitionManager / CTCBase /
//   CTCCreatureGenerator model with a stand-in GetDef body; now the include chain of the landed sibling 0x0074AF50 with the
//   STLport find opt-ins (FABLE_RETAIL_STLP_FIND_RANDOM_DISPATCH + _NOINLINE_BODY: the __find body visible but out of line).
#define FABLE_RETAIL_STLP_FIND_RANDOM_DISPATCH
#define FABLE_RETAIL_STLP_FIND_NOINLINE_BODY
#define FABLE_RETAIL_CDEFSTRING_FORCEINLINE_GETSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_IS_THING_ALIVE_NOINLINE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_ISCREATUREGENERATIONENABLEDFORGROUP
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCE
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CCREATUREDEF_DIALOGUEVOICES
#define FABLE_RETAIL_CCREATUREDEF_SCREENINVENTORIES
#define FABLE_RETAIL_CCREATUREDEF_RESPAWNWAIT
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CTHINGAICREATUREINIT_DTOR
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_STLP_VECTOR_AT_INLINE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCOMBATACTIONBASE_SOUNDCTOR
#define FABLE_RETAIL_CTCSHAPEMANAGER
#define FABLE_RETAIL_CTCCREATUREGENERATOR
#define FABLE_RETAIL_CTCCREATUREGENERATOR_UNDEAD
#include "fable_string.h"
#include "engine/CDefClassBase.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingAICreatureInit.h"
#include "engine/ThingAlive.h"
#include "engine/CArray.h"
#include "engine/CDefIndex.h"
#include "engine/CWorld.h"
#include "engine/GFRandom.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CCreatureGenerationFamilyDef.h"
#include "engine/CThingCreatureDef.h"
#include "engine/CCreatureDef.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"
#include "engine/CCreatureAction_UndeadIdleUnderground.h"
#include "engine/CTCShapeManager.h"
#include "engine/CTCGeneratedCreature.h"
#include "engine/CTCCreatureGenerator.h"
#include "stlport/fable_algorithm.h"
#include "engine/CDefString.h"

bool CTCCreatureGenerator::Validate(CCharString& error) const
{
    if (CreatureFamilies.empty()) {
        error = "no creature families specified - what am I supposed to generate?";
        return false;
    }
    for (const CDefIndex* it = CreatureFamilies.begin(); it != CreatureFamilies.end(); ++it) {
        CDefPointer<const CCreatureGenerationFamilyDef> def;
        GetDefinitionManager().GetDef(*it, &def);
        {
            CCharString name = def->GetInstantiationName().GetString();
            error = "creature family " + name;
        }
        const ECreatureGeneratorGenerateType* last = def->GenerationTypes.end();
        const ECreatureGeneratorGenerateType* first = def->GenerationTypes.begin();
        if (std::find(first, last, (ECreatureGeneratorGenerateType)2) != last && HiddenPositions.empty()) {
            error += " needs to have hidden start positions";
            return false;
        }
        if (std::find(first, last, (ECreatureGeneratorGenerateType)3) != last) {
            error += " has generate type GENERATE_GENERATOR_ANIMATION and it's not supported";
            return false;
        }
        if (std::find(first, last, (ECreatureGeneratorGenerateType)0) != last && OpenPositions.empty()) {
            error += " needs to create creatures in the open and I can't find any navigable positions";
            return false;
        }
        if (std::find(first, last, (ECreatureGeneratorGenerateType)1) != last && OpenPositions.empty()) {
            error += " needs to create creatures in the open and I can't find any navigable positions";
            return false;
        }
    }
    return true;
}
