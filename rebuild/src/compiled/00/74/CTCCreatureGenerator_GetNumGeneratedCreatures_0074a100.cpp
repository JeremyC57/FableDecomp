// CTCCreatureGenerator::GetNumGeneratedCreatures() const -- total number of creature entries over all of the generator's creature-family defs
// Leaf: ?GetNumGeneratedCreatures@CTCCreatureGenerator@@QBEJXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0074A100, length 137 bytes (+ the out-of-range tail). Name: Ego_r.pdb 0x0075C1D0 /
// FableWin.pdb 0x02519000 ?GetNumGeneratedCreatures@CTCCreatureGenerator@@QBEJXZ; its caller is
// CGameScriptInterface::CreatureGeneratorGetNumGeneratedCreatures @ 0x0089EB80 (the call at 0x0089EBDF on the thing's interface-0x3D
// component, result returned).
// Callees: CTCBase::GetDefinitionManager @ 0x006869D0, CDefinitionManager::GetDef<CCreatureGenerationFamilyDef>(long,
// CDefPointer<const CCreatureGenerationFamilyDef>*) const @ 0x0074D220 (FableWin.pdb template spelling), the def's slot-1 release,
// puts @ 0x00BFEAF2 + abort @ 0x00BFEAEC (STLport vector::at range check, literal "vector" @ 0x01268AE4).
// Layout: CTCCreatureGenerator CreatureFamilies +0x0C (CArray<CDefIndex>); CCreatureGenerationFamilyDef Creatures +0x3C (vector<long>).
// Shape: `for (unsigned i = 0; i < size(); ++i)` with `at(i)`; a zeroed CDefPointer local per pass whose inline dtor releases it.
// NF41 (2026-11-16): was a local CRetailVector / CDefClassBase / CDefPointer / CDefinitionManager / CTCBase / CTCCreatureGenerator model
//   with a stand-in GetDef body over a made-up `CCreatureFamilyDef`; now the include chain of the landed sibling 0x0074AF50
//   (engine/CTCCreatureGenerator.h UNDEAD + the NF41 opt-in FABLE_RETAIL_CTCCREATUREGENERATOR_GETNUMGENERATEDCREATURES,
//   engine/CCreatureGenerationFamilyDef.h, engine/CDefinitionManager.h GETDEF_GLOBAL_BODY).
#define FABLE_RETAIL_CTCCREATUREGENERATOR_GETNUMGENERATEDCREATURES
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
#define FABLE_RETAIL_CTCCREATUREGENERATOR_HASHITCREATIONLIMIT_DEFINITION
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

long CTCCreatureGenerator::GetNumGeneratedCreatures() const
{
    long total = 0;
    for (unsigned long i = 0; i < CreatureFamilies.size(); ++i) {
        CDefPointer<const CCreatureGenerationFamilyDef> def;
        long family = CreatureFamilies.at(i);
        GetDefinitionManager().GetDef(family, &def);
        total += def->Creatures.size();
    }
    return total;
}
