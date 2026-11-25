#include "rebuild_abi.h"
// CCombatActionBase::GetAnimationTransitionInTime @ 0x008571a0
struct Res { char pad[8]; int m_v; };
struct Sub74 { void Get(Res **out); };
class CCombatActionBase { public: char pad[0x74]; Sub74 *m_sub; int GetAnimationTransitionInTime(); };
// Naming (NF442 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration (no layout; rulings r2/r3):
//   0x005E0280 ??$GetComponent@VCAnimComponentTransitionInTime@@@CAnimationEntry@@QBEXPAPBVCAnimComponentTransitionInTime@@@Z (was ?Get@Sub74@@QAEXPAPAURes@@@Z)
class CAnimComponentTransitionInTime;
class CAnimationEntry { public: template <class T1> void GetComponent(T1 const**) const; };
int CCombatActionBase::GetAnimationTransitionInTime(){
    Res *r;
    reinterpret_cast<const CAnimationEntry*>(m_sub)->GetComponent<CAnimComponentTransitionInTime>((CAnimComponentTransitionInTime const**)(&r));
    return r->m_v;
}