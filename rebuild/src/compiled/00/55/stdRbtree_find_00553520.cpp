// std::_Rb_tree<NUISystem::ETableSprites, std::pair<NUISystem::ETableSprites const, CCountedPointer<NUISystem::CComponent> >, ...>::find<NUISystem::ETableSprites>(NUISystem::ETableSprites const &) -- STLport 4.6 find(const _KT&): _M_find inlined; iterator returned through the hidden pointer
// Retail: Fable.exe (TLC, VC7.1) @ 0x00553520, 71 bytes.
// Types (proven, agentFAM11, 2026-11-12): map<NUISystem::ETableSprites, CCountedPointer<NUISystem::CComponent> >, the non-const member-template find<ETableSprites>
//   (STLport 4.6.1 _tree.h under _STLP_MEMBER_TEMPLATES). Sources: (1) retail: the landed real-typed NUISystem::CTable::ConstructSpritesToDraw
//   0x00551EA0 calls it out of line (push &key ; push &result ; call) and its relocation binds exactly this spelling (rebuild/oracles/
//   reloc_bindings.tsv; fable_map.h FABLE_RETAIL_STLP_TREE_FIND_MEMBER_TEMPLATE documents the call); (2) Ego_r.pdb NUISystem::CTable's sprite map
//   is map<ETableSprites, CCountedPointer<CComponent> > and FableWin.pdb carries its _Tmap_traits.
//   Was the placeholder std::_Rb_tree<CUnprovenKey_00553520,CUnprovenValue_00553520,CUnprovenKeyOfValue_00553520,std::less<CUnprovenKey_00553520>,std::allocator<CUnprovenValue_00553520> >::find (leaf ?find@?$_Rb_tree@UCUnprovenKey_00553520@@UCUnprovenValue_00553520@@UCUnprovenKeyOfValue_00553520@@U?$less@UCUnprovenKey_00553520@@@std@@V?$allocator@UCUnprovenValue_00553520@@@5@@std@@QAE?AU?$_Rb_tree_iterator@UCUnprovenValue_00553520@@U?$_Nonconst_traits@UCUnprovenValue_00553520@@@std@@@2@ABUCUnprovenKey_00553520@@@Z); name-only change: the
//   retail body, now spelled as the member template the caller binds (the placeholder was the plain find).
// Model: the shared stlport/fable_map.h tree with FABLE_RETAIL_STLP_TREE_FIND_MEMBER_TEMPLATE (STLport 4.6.1 member-template find,
//   _M_find inlined) and the caller's NUISystem::CTable header set; explicit instantiation emits the member as its own COMDAT, as retail
//   kept every used inline member (no /OPT:REF). Replaces the invented stand-in (was CRetailTree_longkey::find).
// Leaf: ??$find@W4ETableSprites@NUISystem@@@?$_Rb_tree@W4ETableSprites@NUISystem@@U?$pair@$$CBW4ETableSprites@NUISystem@@V?$CCountedPointer@VCComponent@NUISystem@@@@@std@@U?$_Select1st@U?$pair@$$CBW4ETableSprites@NUISystem@@V?$CCountedPointer@VCComponent@NUISystem@@@@@std@@@4@U?$less@W4ETableSprites@NUISystem@@@4@V?$allocator@U?$pair@$$CBW4ETableSprites@NUISystem@@V?$CCountedPointer@VCComponent@NUISystem@@@@@std@@@4@@std@@QAE?AU?$_Rb_tree_iterator@U?$pair@$$CBW4ETableSprites@NUISystem@@V?$CCountedPointer@VCComponent@NUISystem@@@@@std@@U?$_Nonconst_traits@U?$pair@$$CBW4ETableSprites@NUISystem@@V?$CCountedPointer@VCComponent@NUISystem@@@@@std@@@2@@1@ABW4ETableSprites@NUISystem@@@Z
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_NORMALISE
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_CUIDEF
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CTABLE_CONSTRUCT
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_FIND_MEMBER_TEMPLATE
#include "engine/NUISystem_CTable.h"

typedef std::pair<const NUISystem::ETableSprites, CCountedPointer<NUISystem::CComponent> > CTableSpriteValue;
typedef std::_Rb_tree<NUISystem::ETableSprites, CTableSpriteValue, std::_Select1st<CTableSpriteValue>, std::less<NUISystem::ETableSprites>,
                      std::allocator<CTableSpriteValue> > CTableSpriteTree;
template CTableSpriteTree::iterator CTableSpriteTree::find<NUISystem::ETableSprites>(const NUISystem::ETableSprites&);
