#include "rebuild_abi.h"
struct C { char p[0x2c]; void* f2c; void Detach(); void Helper(); };
// Naming (NF443 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration or the row's own model (no layout; rulings r2/r3, NF438 q1 friend):
//   0x007846B0 ?OnHeroLeaveBoastingArea@CTCCreatureGroupBoastingCrowd@@QAEXXZ (was ?Helper@C@@QAEXXZ)
class CTCCreatureGroupBoastingCrowd { public: void OnHeroLeaveBoastingArea(); };
void C::Detach(){ if(this->f2c) reinterpret_cast<CTCCreatureGroupBoastingCrowd*>(this)->OnHeroLeaveBoastingArea(); }