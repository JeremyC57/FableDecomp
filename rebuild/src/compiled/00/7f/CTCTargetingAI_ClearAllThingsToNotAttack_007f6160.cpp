// CTCTargetingAI::ClearAllThingsToNotAttack() -- forget every thing the targeting AI was told not to attack
// Leaf: ?ClearAllThingsToNotAttack@CTCTargetingAI@@QAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x007F6160, 8 bytes: `add ecx,48h ; jmp 0x004B8050` (ThingsToNotAttack.clear(), tail call).
// Identity (two sources): Ego_r.pdb / FableWin.pdb ?ClearAllThingsToNotAttack@CTCTargetingAI@@QAEXXZ, the class's only clear of its
//   list<CIntelligentPointer<CThing> > ThingsToNotAttack (Ego_r struct_layouts +0x48 on the 0x38-byte CTCTargetingBase; the list is
//   the last member, 4 bytes in retail); the callee 0x004B8050 is the out-of-line STLport _List_base<CIntelligentPointer<CThing> >::clear
//   (as bound by the landed ~CGameCameraManager 0x00699BF0); the sole caller is CAIStateGroup_WanderAroundInitialPos::Update 0x00911BBF.
//   The catalog's "_Cons_val<..CParticleMorphs..>" was a propagated label.
// Callees: ?clear@?$_List_base@V?$CIntelligentPointer@VCThing@@@@V?$allocator@V?$CIntelligentPointer@VCThing@@@@@std@@@std@@QAEXXZ 0x004B8050
//   (STLport 4.6 defines _List_base::clear out of class in _list.c; retail calls it out of line, so the TU sees the declaration of
//   that instantiation only, as the landed ~CGameCameraManager does).
// Header: engine/CTCTargetingAI.h (new) over engine/CTCTargetingBase.h.
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#include "engine/CTCTargetingAI.h"
template <> void std::_List_base<CIntelligentPointer<CThing>, std::allocator<CIntelligentPointer<CThing> > >::clear();   // 0x004B8050
void CTCTargetingAI::ClearAllThingsToNotAttack()
{
    ThingsToNotAttack.clear();
}