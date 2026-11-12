// std::_Rb_tree_iterator::operator--() -- STLport 4.6 tree iterator pre-decrement (_Rb_global::_M_decrement inlined), value type pair<const NUISystem::ETableSprites, CCountedPointer<NUISystem::CComponent> >
// Leaf: ??F?$_Rb_tree_iterator@U?$pair@$$CBW4ETableSprites@NUISystem@@V?$CCountedPointer@VCComponent@NUISystem@@@@@std@@U?$_Nonconst_traits@U?$pair@$$CBW4ETableSprites@NUISystem@@V?$CCountedPointer@VCComponent@NUISystem@@@@@std@@@2@@std@@QAEAAU01@XZ
// Identity: the respelled caller 0x00553AE0 (agentFAM12) calls this address as this operator-- of the proven value type
//   (agentFAM12 CALLEE_FIXES.tsv; the caller row's Leaf/types come from its own landed binding + the Ego_r/FableWin name sets);
//   the body is STLport 4.6 _Rb_tree_iterator::operator-- (the head-node test, rightmost of the left subtree, climb while a left child).
//   Was the invented stand-in CRetailTreeIterator::Decrement.
// Model: stlport/fable_map.h + the explicit instantiation of the member (its own COMDAT, as retail).
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CUIDEF
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCHARSTRING
#include "stlport/fable_map.h"
#include "engine/NUISystem_CUIDef.h"
#include "engine/CCountedPointer.h"
namespace NUISystem { class CComponent; }
template std::_Rb_tree_iterator<std::pair<const NUISystem::ETableSprites, CCountedPointer<NUISystem::CComponent> >, std::_Nonconst_traits<std::pair<const NUISystem::ETableSprites, CCountedPointer<NUISystem::CComponent> > > >& std::_Rb_tree_iterator<std::pair<const NUISystem::ETableSprites, CCountedPointer<NUISystem::CComponent> >, std::_Nonconst_traits<std::pair<const NUISystem::ETableSprites, CCountedPointer<NUISystem::CComponent> > > >::operator--();
