// CTCSoundPlayer::GetPotentialCriteriaStrings(const CCharString& criteria, vector<CCharString>& criteria_strings) -- the candidate sound-event names for a criteria: "<entity sound identifier>;CS_<criteria>" when the owner's def has an entity sound sub-def, then "CS_<criteria>"
// Leaf: ?GetPotentialCriteriaStrings@CTCSoundPlayer@@AAEXABVCCharString@@AAV?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0066BC10, 232 bytes (+8 int3 pad), private __fastcall, `ret 8`.
// Name: Ego_r.pdb ?GetPotentialCriteriaStrings@CTCSoundPlayer@@AAEXABVCCharString@@AAV?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@@Z
//   (the landed callers PlayAttachedCriteria 0x0066BD00 / PlayUnattachedCriteria 0x0066BDF0 call this address by that name; the
//   catalog's "Sound_AppendAbilityNameToDescription" is a propagated label).
// Callees: CDefClassBase::GetSubDef<CEntitySoundDef> 0x0065F604 (??$GetSubDef@VCEntitySoundDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCEntitySoundDef@@@@@Z),
//   the __fastcall CCharString operator+ overloads (const CCharString&, const char*) 0x0099F600 / (const CCharString&, const
//   CCharString&) 0x0099F570 / (const char*, const CCharString&) 0x0099F690, CCharString(const CCharString&) 0x0099EC30 /
//   ~CCharString 0x0099EAE0, vector<CCharString>::_M_insert_overflow 0x00433530; literals ";CS_" 0x0125B238, "CS_" 0x0125B234;
//   the sub-def's virtual slot 1 (Delete) on the last release.
// Headers: new engine/CEntitySoundDef.h (overlay), opt-ins FABLE_RETAIL_CTHING_HASBASEDEF (FableWin.pdb inline ?HasBaseDef@CThing@@QBE_NXZ),
//   FABLE_RETAIL_CTCSOUNDPLAYER_CRITERIA (the private declaration) and FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE (the real
//   GetSubDef<T> body kept out of line, as this TU calls it: the def read back from the out pointer then stays in edi).
// Shape: the first name is a named local built in one full-expression (its `SoundIdentifier + ";CS_"` temporary dies before the
//   push_back, 0x0066BC5B), which also keeps that push_back expanded like the second one; the second name is a temporary.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_CONCAT_STRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_HASBASEDEF
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHINGBASEDEF
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CTCSOUNDPLAYER_CRITERIA
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CEntitySoundDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/CTCSoundPlayer.h"

void CTCSoundPlayer::GetPotentialCriteriaStrings(const CCharString& criteria, std::vector<CCharString>& criteria_strings)
{
    // The owner's entity sound sub-def, when its def has one (null-tested def: HasBaseDef, then the sub-def lookup).
    CDefPointer<const CEntitySoundDef> p_entity_sound_def;
    if (ParentThing->HasBaseDef() && ParentThing->PeekBaseDef().GetSubDef(&p_entity_sound_def)) {
        CCharString criteria_string = p_entity_sound_def->SoundIdentifier + ";CS_" + criteria;
        criteria_strings.push_back(criteria_string);
    }
    criteria_strings.push_back("CS_" + criteria);
}