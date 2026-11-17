// NUISystem::CList::AddScrollingChildDynamically(CCountedPointer<CComponent>& child) -- add a child at run time: the base CChangingStateComponent bookkeeping, then record it in ScrollingChildren (+0x164), mark it visible in ScrollingChildrenVisibility (+0x1c8), copy its state map (GetStates(), slot 82) into ScrollingChildrenStates (+0x17c) and store its row index (last of Children +0xb0) in ShapeChildren (+0xe4)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0053BEAC, length 116 bytes.
// Callees: CChangingStateComponent::AddChildDynamically @ 0x0052CAB0 (direct base call), vector<CComponent*>::push_back
//          @ 0x00486024, the visibility flags' push_back(const bool&) @ 0x0053CB4F, vector<map<unsigned long, CUIState> >
//          ::push_back @ 0x0053CFC2 (landed under a local std::CUIStateMap stand-in; this row binds the PDB
//          map spelling), vector<unsigned long>::push_back @ 0x005178A0, the child's virtual slot 82 (+0x148) GetStates(), whose returned
//          reference is pushed straight on (Ego_r.pdb ?GetStates@CComponent@NUISystem@@UAEAAV?$map@KVCUIState@...).
// Layout: Ego_r.pdb CList ScrollingChildren +0x194, ScrollingChildrenStates +0x1b4, ScrollingChildrenVisibility +0x204;
//          retail is 0x30 below at ScrollingChildren and a further 4 below per STLport 12-byte vector (+0x164 / +0x17c /
//          +0x1c8). The earlier model called +0x17c a CScrollLayout with AddChild(long) (FINDINGS chunk 376).
// Shape: the frame-pointer size TU (`optimize("y", off)` + `("s", on)`); every push_back argument is an rvalue, so
// VC copies it into the dead `child` argument slot (`mov [ebp+8],eax ; lea eax,[ebp+8]`), the `true` into that
// slot's high byte (`mov byte ptr [ebp+0Bh],1`); the row count is the signed `(last - first) >> 3` minus one.
// Name fix (agentNF54): ported onto engine/NUISystem_CList.h (FABLE_RETAIL_CLIST: the 12-byte byte-per-element vector<bool>) and
//   engine/NUISystem_CComponent.h. The self row now binds the PDB spelling (Ego_r.pdb 0x005B540E + FableWin.pdb 0x0219C0E5
//   ?AddScrollingChildDynamically@CList@NUISystem@@UAEXAAV?$CCountedPointer@VCComponent@NUISystem@@@@@Z: virtual, non-const
//   reference; the local model spelled QAEXABV...), and the row index goes through CComponent::ShapeChildren (Ego_r.pdb
//   vector<unsigned long> at +0xE4 in retail) to vector<unsigned long>::push_back 0x005178A0 (the local model bound
//   ?push_back@?$CRetailVector@J@@QAEXABJ@Z). The four push_backs stay calls (FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CLIST
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE
#include "engine/NUISystem_CList.h"

void NUISystem::CList::AddScrollingChildDynamically(CCountedPointer<CComponent>& child)
{
    CChangingStateComponent::AddChildDynamically(child);
    ScrollingChildren.push_back(child.operator->());
    ScrollingChildrenVisibility.push_back(true);
    ScrollingChildrenStates.push_back(child->GetStates());
    ShapeChildren.push_back(Children.size() - 1);
}
