// std::_Rb_tree<EObjectAugmentationType, std::pair<const EObjectAugmentationType,CWeaponTrailGraphicSet>, ...>::find<EObjectAugmentationType>(const EObjectAugmentationType&) const -- STLport 4.6 const member-template find: the lower-bound walk, then end() unless the found key is not greater
// Leaf: ??$find@W4EObjectAugmentationType@@@?$_Rb_tree@W4EObjectAugmentationType@@U?$pair@$$CBW4EObjectAugmentationType@@UCWeaponTrailGraphicSet@@@std@@U?$_Select1st@U?$pair@$$CBW4EObjectAugmentationType@@UCWeaponTrailGraphicSet@@@std@@@3@U?$less@W4EObjectAugmentationType@@@3@V?$allocator@U?$pair@$$CBW4EObjectAugmentationType@@UCWeaponTrailGraphicSet@@@std@@@3@@std@@QBE?AU?$_Rb_tree_iterator@U?$pair@$$CBW4EObjectAugmentationType@@UCWeaponTrailGraphicSet@@@std@@U?$_Const_traits@U?$pair@$$CBW4EObjectAugmentationType@@UCWeaponTrailGraphicSet@@@std@@@2@@1@ABW4EObjectAugmentationType@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0083B170, 71 bytes. Called at 0x0083935D by CThingGameObject::GetWeaponTrailGraphicIndex 0x00839100
//   as WeaponTrails.find(0) on the const CWeaponDef (map<EObjectAugmentationType,CWeaponTrailGraphicSet>, struct_layouts_egor.tsv
//   CWeaponDef::WeaponTrails; key compared signed jl/jge as an enum); that landed caller already binds this spelling, and its own text
//   compiled with the fable_map.h const member-template find (owner decision D1) emits this body.
// Name: the compiler's spelling of the STLport member template (descriptive-by-library; both PDBs are Dinkumware builds; the key and
//   value types are the Ego_r.pdb member type of CWeaponDef::WeaponTrails, the template argument the call proves).
// NF41 (2026-11-16): was one row of a placeholder instantiation (CUnprovenKey/Value/KeyOfValue_0083b170, non-const find); now the
//   shared stlport/fable_map.h tree under the include chain of the landed caller 0x00839100, emitted by the non-graded helper
//   Use_0083b170 (one const find on the map, as the caller does).
#include "fable_compiler.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGGAMEOBJECT_WEAPONTRAIL
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_ANIMATION
#define FABLE_RETAIL_CTCTRAPBASE
#define FABLE_RETAIL_CTCCOMBAT
#define FABLE_RETAIL_CTCCOMBAT_WEAPONTRAIL
#define FABLE_RETAIL_CCOMBATABILITY
#define FABLE_RETAIL_CCOMBATABILITYDATA
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBERSERK_HIT_EFFECT
#define FABLE_RETAIL_CTCBERSERK_WEAPONTRAIL
#define FABLE_RETAIL_AUGMENTATION_PARTICLES
#define FABLE_RETAIL_STLP_TREE_CONSTFIND_TEMPLATE_BODY
#define FABLE_RETAIL_STLP_TREE_DEC_OUTOFLINE
#define FABLE_RETAIL_STLP_LIST_REVERSE
#define FABLE_RETAIL_STLP_MAP_REVERSE
#define FABLE_RETAIL_CWEAPONDEF_WEAPONTRAILS
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CThingBaseDef.h"
#include "engine/CDefPointer.h"
#include "engine/CWeaponDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/CTCCarryable.h"
#include "engine/CTCTrapBase.h"
#include "engine/CTCBerserk.h"
#include "engine/CTCCombat.h"
#include "engine/CTCObjectAugmentations.h"

typedef std::map<EObjectAugmentationType, CWeaponTrailGraphicSet> TWeaponTrails;
TWeaponTrails::const_iterator Use_0083b170(const TWeaponTrails& trails, const EObjectAugmentationType& key) { return trails.find(key); }
