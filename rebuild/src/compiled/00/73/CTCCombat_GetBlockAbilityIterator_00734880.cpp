// CTCCombat::GetBlockAbilityIterator(const_iterator&, EWeaponClass) const
// Leaf: ?GetBlockAbilityIterator@CTCCombat@@ABE_NAAU?$_Rb_tree_iterator@U?$pair@$$CBW4ECombatAbilityDataType@@U?$pair@V?$CCountedPointer@VCCombatAbilityData@@@@VCCombatAbility@@@std@@@std@@U?$_Const_traits@U?$pair@$$CBW4ECombatAbilityDataType@@U?$pair@V?$CCountedPointer@VCCombatAbilityData@@@@VCCombatAbility@@@std@@@std@@@2@@std@@W4EWeaponClass@@@Z
// Flags:
// This declaration is implemented inline in the shared CTCCombat header.
// The native 007348C0 shipping object provides the exact 00734880 COMDAT.
// The existing SetStruck shipping object calls that same symbol at 0073520D.
#define FABLE_RETAIL_CTCCOMBAT_ISTOBLOCK_PASS30
#define FABLE_RETAIL_CTCCOMBAT
#define FABLE_RETAIL_COMBAT_SETSTRUCK_PASS29
#define FABLE_RETAIL_STLP_MULTIMAP_PASS22_ATMOS_ITERATION
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CTHINGGAMEOBJECT_ONHIT
#define FABLE_RETAIL_CHITPARAMETERS_VECTOR_MEMBERS
#define FABLE_RETAIL_CHITPARAMETERS_AUGMENTATIONS
#define FABLE_RETAIL_CHITPARAMETERS_BLOCKABLE
#define FABLE_RETAIL_CTCWEAPON
#define FABLE_RETAIL_CTCCARRYING
#define FABLE_RETAIL_CTCHEROEXPERIENCE
#define FABLE_RETAIL_CCOMBATABILITY
#define FABLE_RETAIL_CCOMBATABILITY_MEMBERS
#define FABLE_RETAIL_CCOMBATABILITYDATA
#define FABLE_RETAIL_CCOMBATABILITYBLOCKINGDATA
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_EQUAL_MIXED
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHING_ISDEAD_DIRECT
#define FABLE_RETAIL_CTHING_GETTC_DONOR_NAMES_PASS23
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_ACTIONQUERY
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPERCEIVETHING
#define FABLE_RETAIL_CTCPERCEIVETHING_AWARENESS
#define FABLE_RETAIL_STLP_FIND_RANDOM_DISPATCH
#define FABLE_RETAIL_GFDEBUGDYNAMICCAST
#define FABLE_RETAIL_MATH_TRIG_TWO_PI

#include "fable_string.h"
#include "engine/CDefClassBase.h"
#include "engine/C3DVector.h"
#include "engine/ECombatAbilityInterfaceType.h"
#include "engine/EWeaponClass.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/ThingAlive.h"
#include "engine/CWorld.h"
#include "engine/CHitParameters.h"
#include "engine/CTCCombat.h"
#include "engine/CTCWeapon.h"
#include "engine/CTCCarrying.h"
#include "engine/CTCHeroExperience.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CTCPerceiveThing.h"
#include "engine/CCombatAbilityBlockingData.h"
#include "engine/GFDebugPDynamicCast.h"
#include "engine/GFCos.h"
#include "engine/NCreatureMode.h"
#include "stlport/fable_algorithm.h"
#include "fable_assert.h"


// Fidelity: the private instantiation requests the existing inline member for standalone object builds.
namespace {
typedef bool (CTCCombat::*Owner)(std::multimap<ECombatAbilityDataType, std::pair<CCountedPointer<CCombatAbilityData>, CCombatAbility> >::const_iterator&, EWeaponClass) const;
template<Owner Method>
#if defined(__GNUC__) || defined(__clang__)
__attribute__((used))
#endif
static Owner EmitOriginalMember() { return Method; }
#if defined(_MSC_VER) && _MSC_VER == 1310 && !defined(__clang__)
template Owner EmitOriginalMember<static_cast<Owner>(&CTCCombat::GetBlockAbilityIterator)>();
#else
template Owner EmitOriginalMember<&CTCCombat::GetBlockAbilityIterator>();
#endif
}
