// CCombatSequenceBase::StatRegisterClass(const std::map<CCharString, CCombatSequenceBase* (__fastcall*)(CThingCreatureBase&, CThingCreatureBase&)>&) -- static registration of the combat-sequence modules: the eight IsValid predicates (ids 1..8, id 0 a null predicate), the OnStart module (id 1, id 0 null) and the null OnStop module into the three protected static maps
// Leaf: ?StatRegisterClass@CCombatSequenceBase@@SIXABV?$map@VCCharString@@P6IPAVCCombatSequenceBase@@AAVCThingCreatureBase@@0@ZU?$less@VCCharString@@@std@@V?$allocator@U?$pair@$$CBVCCharString@@P6IPAVCCombatSequenceBase@@AAVCThingCreatureBase@@0@Z@std@@@5@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008B08B0, length 425 bytes (`ret`, aligned ebp frame `and esp,-8`); public static __fastcall.
// Identity (agentNF69, q10820): Ego_d publics + Ego_r.pdb 0x008EBDB0 carry the Leaf symbol (docs/journal/2026-11/EGOD_FINDINGS.md
//   section 1). Its only reference is the tail `jmp` at 0x008B13BC that ends CCombatManager::RegisterCombatSequences 0x008B0D10, with
//   ecx = this + 4 (Ego_r CCombatManager's map<CCharString, CCombatSequenceBase* (__fastcall*)(...)> at +4: this function's parameter,
//   unused in the TLC body); Ego_d puts both in combat_sequence.obj. Was the propagated label CScreenEffectPriorityStack::Process.
// Body: twelve map::insert(value_type(id, module)) calls, each STLport's inline map::insert -> the out-of-line _Rb_tree::insert_unique of
//   its map (0x008B2DD0 IsValidPredicates 0x013BAFA8, 0x008B2E90 OnStartModules 0x013BAFB4, 0x008B2F50 OnStopModules 0x013BAFC0;
//   q10822..q10824, q10828 / q10830 / q10832), pair<iterator, bool> through a hidden pointer, ignored. The ids are the enums'
//   values (Ego_d: ECombatSequenceIsValidType NULL + 8, ECombatSequenceOnStartModuleType NULL + CONTINUE_AIMING,
//   ECombatSequenceOnStopModuleType NULL only); the enumerators are in no shared header, so they are written as casts.
//   The modules are Ego_r.pdb's NCombatSequenceIsValidModules / NCombatSequenceOnStartModules functions (q10837..q10845); were the
//   HandlerR1..R8 / HandlerD1 stand-ins.
// Shape: VC7.1 RTM shares one 8-aligned result slot and one value slot for all twelve calls (hence `and esp,-8`); the id-0 entries
//   store the null module through esi.
// Layout: CCombatSequenceBase has no shared header (engine/CCombatSequenceBase.h is quarantined; the landed rows 0x008B0B60 / 0x006F1770
//   model it locally); declared here with its static maps (protected, Ego_d LF_STMEMBER) and their Ego_d typedefs only.
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"

class CThingCreatureBase;
enum ECombatSequenceIsValidType;
enum ECombatSequenceOnStartModuleType;
enum ECombatSequenceOnStopModuleType;

class CCombatSequenceBase {
public:
    typedef std::map<ECombatSequenceIsValidType, bool (__fastcall*)(CThingCreatureBase const&, CThingCreatureBase const&)> TIsValidPredicateContainer;
    typedef std::map<ECombatSequenceOnStartModuleType, void (__fastcall*)(CThingCreatureBase&, CThingCreatureBase&)> TOnStartModuleContainer;
    typedef std::map<ECombatSequenceOnStopModuleType, void (__fastcall*)(CThingCreatureBase&, CThingCreatureBase&)> TOnStopModuleContainer;
    static void __fastcall StatRegisterClass(const std::map<CCharString, CCombatSequenceBase* (__fastcall*)(CThingCreatureBase&, CThingCreatureBase&)>& special_sequences);
protected:
    static TIsValidPredicateContainer IsValidPredicates;    // 0x013BAFA8
    static TOnStartModuleContainer OnStartModules;          // 0x013BAFB4
    static TOnStopModuleContainer OnStopModules;            // 0x013BAFC0
};

namespace NCombatSequenceIsValidModules {
bool __fastcall IsTargetBlocking(const CThingCreatureBase& creature, const CThingCreatureBase& target);               // 0x00905500
bool __fastcall IsToLoadRangedWeapon(const CThingCreatureBase& creature, const CThingCreatureBase& target);           // 0x00905690
bool __fastcall IsToFireRangedWeaponAtTarget(const CThingCreatureBase& creature, const CThingCreatureBase& target);   // 0x009056F0
bool __fastcall IsTargetHealthOver75Percent(const CThingCreatureBase& creature, const CThingCreatureBase& target);    // 0x00905510
bool __fastcall IsMyHealthBelow30Percent(const CThingCreatureBase& creature, const CThingCreatureBase& target);       // 0x00905530
bool __fastcall IsBalverineAbleToLunge(const CThingCreatureBase& creature, const CThingCreatureBase& target);         // 0x009055E0
bool __fastcall IsAbleToSummon(const CThingCreatureBase& creature, const CThingCreatureBase& target);                 // 0x009059D0
bool __fastcall IsTargetInLineOfSight(const CThingCreatureBase& creature, const CThingCreatureBase& target);          // 0x00905A20
}
namespace NCombatSequenceOnStartModules {
void __fastcall OnStartContinueAiming(CThingCreatureBase& creature, CThingCreatureBase& target);                      // 0x00905C30
}

void CCombatSequenceBase::StatRegisterClass(const std::map<CCharString, CCombatSequenceBase* (__fastcall*)(CThingCreatureBase&, CThingCreatureBase&)>& special_sequences)
{
    IsValidPredicates.insert(TIsValidPredicateContainer::value_type((ECombatSequenceIsValidType)0, 0));
    IsValidPredicates.insert(TIsValidPredicateContainer::value_type((ECombatSequenceIsValidType)1, &NCombatSequenceIsValidModules::IsTargetBlocking));
    IsValidPredicates.insert(TIsValidPredicateContainer::value_type((ECombatSequenceIsValidType)2, &NCombatSequenceIsValidModules::IsToLoadRangedWeapon));
    IsValidPredicates.insert(TIsValidPredicateContainer::value_type((ECombatSequenceIsValidType)3, &NCombatSequenceIsValidModules::IsToFireRangedWeaponAtTarget));
    IsValidPredicates.insert(TIsValidPredicateContainer::value_type((ECombatSequenceIsValidType)4, &NCombatSequenceIsValidModules::IsTargetHealthOver75Percent));
    IsValidPredicates.insert(TIsValidPredicateContainer::value_type((ECombatSequenceIsValidType)5, &NCombatSequenceIsValidModules::IsMyHealthBelow30Percent));
    IsValidPredicates.insert(TIsValidPredicateContainer::value_type((ECombatSequenceIsValidType)6, &NCombatSequenceIsValidModules::IsBalverineAbleToLunge));
    IsValidPredicates.insert(TIsValidPredicateContainer::value_type((ECombatSequenceIsValidType)7, &NCombatSequenceIsValidModules::IsAbleToSummon));
    IsValidPredicates.insert(TIsValidPredicateContainer::value_type((ECombatSequenceIsValidType)8, &NCombatSequenceIsValidModules::IsTargetInLineOfSight));
    OnStartModules.insert(TOnStartModuleContainer::value_type((ECombatSequenceOnStartModuleType)0, 0));
    OnStartModules.insert(TOnStartModuleContainer::value_type((ECombatSequenceOnStartModuleType)1, &NCombatSequenceOnStartModules::OnStartContinueAiming));
    OnStopModules.insert(TOnStopModuleContainer::value_type((ECombatSequenceOnStopModuleType)0, 0));
}
