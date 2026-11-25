#pragma optimize("s",on)
struct MT { char pad38[0x38]; char f38; char pad58[0x58-0x39]; char f58; void tgt(); };
// Naming (NF443 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration or the row's own model (no layout; rulings r2/r3, NF438 q1 friend):
//   0x00788229 ?Trigger@CTCTrapBase@@IAEXXZ (was ?tgt@MT@@QAEXXZ)
class CTCTrapBase { public: protected: void Trigger(); public: friend void __fastcall fn_00788388(MT* self); };
void __fastcall fn_00788388(MT* self){ if(self->f58==0) return; if(self->f38!=0) return; reinterpret_cast<CTCTrapBase*>(self)->Trigger(); }