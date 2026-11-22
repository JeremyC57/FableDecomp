// std::_List_base<const CThingAICreature*>::~_List_base() -- STLport list base destructor: free every node, relink the sentinel to itself, then free the sentinel
// Leaf: ??1?$_List_base@PBVCThingAICreature@@V?$allocator@PBVCThingAICreature@@@std@@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00685BC0, 62 bytes, thiscall, plain `ret`. Callees: free 0x00BFEA14.
// Name fix (NF246, 2026-11-22, the DTOR_OUTOFLINE census, nf243 NOTES section 2): was the stand-in `void __fastcall
//   SortTreeRecursively_00436900(TreeList*)` (catalog `_global::StdList_Clear_00685bc0`; "CGuiControlTreePane::SortTreeRecursively"
//   was a label propagated across byte-identical list destructors). Two sources: (a) retail 0x006860E0 is the size-optimised
//   list<const CThingAICreature*>::~list() COMDAT, a 5-byte `jmp 0x00685BC0` (`ret 4` before, CC padding after), directly after the
//   list<const CThingAICreature*>(const allocator&) constructor 0x006860C0 (0xC-byte node), STLport 4.6 _list.h `~list() {}` with the
//   clear + free in ~_List_base, the layout of 0x004AC060 / 0x004AC080 and 0x004046F0 / 0x00404710; (b) its callers destroy a
//   std::list<const CThingAICreature*> here: CTCCreatureOpinionOfHero::InformOfDeed 0x00682810 (stlport/fable_containers.h) and
//   CAIStateGroup_OpinionReactionRoot::MarriageCommentIsRunnable 0x00986510, the wives list CTCHeroStats::GetWivesInMarriageState
//   0x0057AE53 fills (Ego_r / FableWin: list<const CThingAICreature*>&). The PDBs carry a Dinkumware list (no _List_base), so the base
//   spelling is STLport 4.6's: descriptive-by-library, as nf239 / nf241 / nf243 / nf245 landed the other list destructors.
// Shape: STLport 4.6 `~_List_base() { clear(); _M_node.deallocate(_M_node._M_data, 1); }` with clear() inlined: the loop variable
//   advances before the node is freed (esi), the sentinel is re-read from `this` (edi) after every call; the element is a pointer
//   (nothing destroyed); the sentinel free is null-tested by the malloc allocator's deallocate.
#include "stlport/fable_containers.h"
class CThingAICreature;
template std::_List_base<const CThingAICreature*, std::allocator<const CThingAICreature*> >::~_List_base();
