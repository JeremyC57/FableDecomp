// Leaf: @CCreatureActionHeroJump_FinishAction@4
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
#include "rebuild_abi.h"
class CBaseObject;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
struct FableDestVal_0085baf0_t
{ char pad[0x8];  void FABLE_FASTCALL outer(); };
// Naming (NF442 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration (no layout; rulings r2/r3; q1: the PDB member keeps its protected / private access and a friend line gives the caller access):
//   0x00859A20 ?FinishAction@CCombatActionBase@@MAEXXZ (was ?outer@FableDestVal_0085baf0_t@@QAIXXZ)
extern "C" void FABLE_FASTCALL CCreatureActionHeroJump_FinishAction(FableDestVal_0085baf0_t* action);
class CCombatActionBase { protected: virtual void FinishAction(); friend void FABLE_FASTCALL CCreatureActionHeroJump_FinishAction(FableDestVal_0085baf0_t* action); };
extern "C" void FABLE_FASTCALL CCreatureActionHeroJump_FinishAction(FableDestVal_0085baf0_t* action)
{
    FableDestVal_0085baf0_t* inner_this = (FableDestVal_0085baf0_t*)((char*)action + 0x8);
    ((CBaseIntelligentPointer*)inner_this)->GetPItem();
    reinterpret_cast<CCombatActionBase*>(action)->CCombatActionBase::FinishAction();
}
// Stand-in (owner rule 2026-11-05): CCreatureActionHeroJump_FinishAction for CCreatureActionHeroJump::FinishAction; RTTI vtable slot ?FinishAction@CCreatureActionHeroJump@@UAEXXZ + PDB name set (egor/fablewin).
