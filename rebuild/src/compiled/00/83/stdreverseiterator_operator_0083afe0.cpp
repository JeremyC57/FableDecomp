// std::reverse_iterator<std::_Rb_tree_iterator<std::pair<const EObjectAugmentationType,CWeaponTrailGraphicSet>, std::_Const_traits<..> > >::operator++() -- STLport 4.6 _iterator.h: step the reverse iterator by decrementing the underlying tree iterator, return *this
// Leaf: ??E?$reverse_iterator@U?$_Rb_tree_iterator@U?$pair@$$CBW4EObjectAugmentationType@@UCWeaponTrailGraphicSet@@@std@@U?$_Const_traits@U?$pair@$$CBW4EObjectAugmentationType@@UCWeaponTrailGraphicSet@@@std@@@2@@std@@@std@@QAEAAV01@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0083AFE0, 12 bytes: `push esi ; mov esi,ecx ; call 0x0083AEA0 ; mov eax,esi ; pop esi ; ret`.
// Identity (NF41, 2026-11-16): the body is exactly STLport's `reverse_iterator& operator++() { --current; return *this; }` (current at +0,
//   `this` returned), and its callee 0x0083AEA0 is _Rb_tree_iterator<pair<const EObjectAugmentationType,CWeaponTrailGraphicSet>,
//   _Const_traits<..> >::operator-- (the spelling the landed CThingGameObject::GetWeaponTrailGraphicIndex 0x00839100 binds four times,
//   where its const_reverse_iterator walk over CWeaponDef::WeaponTrails inlines this same ++). The COMDAT sits right after that TU's
//   0x0083AEA0 and has no caller of its own (an unreferenced out-of-line copy, emitted here by taking its address). Name: the
//   compiler's spelling of the STLport member (descriptive-by-library; both PDBs are Dinkumware builds).
// Was: a `CStringOwner_0083afe0` default-constructor placeholder over a made-up `CMember_0083aea0Member` member constructor.
// Include chain: the landed caller 0x00839100's (CWeaponDef::WeaponTrails, FABLE_RETAIL_STLP_MAP_REVERSE + the out-of-line
//   tree decrement FABLE_RETAIL_STLP_TREE_DEC_OUTOFLINE).
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

typedef std::map<EObjectAugmentationType, CWeaponTrailGraphicSet>::const_reverse_iterator TWeaponTrailReverseIterator;
TWeaponTrailReverseIterator& (TWeaponTrailReverseIterator::*Use_0083afe0)() = &TWeaponTrailReverseIterator::operator++;
