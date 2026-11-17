// std::_Rb_tree<ECombatAbilityDataType, pair<ECombatAbilityDataType const, std::pair<CCountedPointer<CCombatAbilityData>, CCombatAbility > >, _Select1st<pair<ECombatAbilityDataType const, std::pair<CCountedPointer<CCombatAbilityData>, CCombatAbility > > >, less<ECombatAbilityDataType >, allocator<pair<ECombatAbilityDataType const, std::pair<CCountedPointer<CCombatAbilityData>, CCombatAbility > > > >::insert_equal(const value_type& v) -- STLport multimap insertion: descend from the root on key(v) < key(x), then _M_insert(x, y, v) (0x00739670)
// Leaf: ?insert_equal@?$_Rb_tree@W4ECombatAbilityDataType@@U?$pair@$$CBW4ECombatAbilityDataType@@U?$pair@V?$CCountedPointer@VCCombatAbilityData@@@@VCCombatAbility@@@std@@@std@@U?$_Select1st@U?$pair@$$CBW4ECombatAbilityDataType@@U?$pair@V?$CCountedPointer@VCCombatAbilityData@@@@VCCombatAbility@@@std@@@std@@@3@U?$less@W4ECombatAbilityDataType@@@3@V?$allocator@U?$pair@$$CBW4ECombatAbilityDataType@@U?$pair@V?$CCountedPointer@VCCombatAbilityData@@@@VCCombatAbility@@@std@@@std@@@3@@std@@QAE?AU?$_Rb_tree_iterator@U?$pair@$$CBW4ECombatAbilityDataType@@U?$pair@V?$CCountedPointer@VCCombatAbilityData@@@@VCCombatAbility@@@std@@@std@@U?$_Nonconst_traits@U?$pair@$$CBW4ECombatAbilityDataType@@U?$pair@V?$CCountedPointer@VCCombatAbilityData@@@@VCCombatAbility@@@std@@@std@@@2@@2@ABU?$pair@$$CBW4ECombatAbilityDataType@@U?$pair@V?$CCountedPointer@VCCombatAbilityData@@@@VCCombatAbility@@@std@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00739B00, 58 bytes (64 with padding), thiscall `ret 8`; /O2 TU.
// Label correction: catalog "RBTree_Find" -- it is the CTCCombat::CombatAbilities tree's insert_equal (multimap::insert):
//   nine callers in the CTCCombat ability set-up (0x0073574E .. 0x00735F56, CTCCombat::SetCombatType), callee its _M_insert 0x00739670.
// Identity (NF58, round 7): Ego_r.pdb CTCCombat::CombatAbilities = multimap<ECombatAbilityDataType, pair<CCountedPointer<
//   CCombatAbilityData>, CCombatAbility> >; the 0x24-byte node (0x10 links + the 20-byte value) in _M_insert 0x00739670 agrees. Was the
//   stand-in CCombatAbilityTree::insert_equal (local namespace model); now the explicit instantiation of stlport/fable_map.h's
//   insert_equal (FABLE_RETAIL_STLP_MAP + FABLE_RETAIL_STLP_TREE_INSERT_EQUAL_BODY, the 0x00ACA830 opt-in), binding the STLport
//   _M_insert the re-authored 0x00739670 defines. STLport 4.6.2 source, descriptive-by-library.
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_INSERT_EQUAL_BODY
#include "stlport/fable_map.h"
#include "engine/ECombatAbilityDataType.h"
#include "engine/CCountedPointer.h"
#include "engine/CCombatAbility.h"
class CCombatAbilityData;
namespace std {
template _Rb_tree<ECombatAbilityDataType, pair<ECombatAbilityDataType const, std::pair<CCountedPointer<CCombatAbilityData>, CCombatAbility > >, _Select1st<pair<ECombatAbilityDataType const, std::pair<CCountedPointer<CCombatAbilityData>, CCombatAbility > > >, less<ECombatAbilityDataType >, allocator<pair<ECombatAbilityDataType const, std::pair<CCountedPointer<CCombatAbilityData>, CCombatAbility > > > >::iterator _Rb_tree<ECombatAbilityDataType, pair<ECombatAbilityDataType const, std::pair<CCountedPointer<CCombatAbilityData>, CCombatAbility > >, _Select1st<pair<ECombatAbilityDataType const, std::pair<CCountedPointer<CCombatAbilityData>, CCombatAbility > > >, less<ECombatAbilityDataType >, allocator<pair<ECombatAbilityDataType const, std::pair<CCountedPointer<CCombatAbilityData>, CCombatAbility > > > >::insert_equal(pair<ECombatAbilityDataType const, std::pair<CCountedPointer<CCombatAbilityData>, CCombatAbility > > const&);
}
