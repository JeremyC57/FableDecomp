// CTCGatherPointBase::~CTCGatherPointBase() -- the compiler-generated destructor: destroy the Users vector (+0x0C, out of line), then ~CTCBase (tail jump)
// Leaf: ??1CTCGatherPointBase@@UAE@XZ
// Name (agentNF20, 2026-11-11): Ego_r.pdb / FableWin.pdb ??1CTCGatherPointBase@@UAE@XZ; retail callers are the slot-0 ??_G of
//   CTCGatherPointBase 0x004E28D5, CTCGatherPointBuilding 0x004E290E and CTCGatherPointRadialArea 0x004E2947 (rtti-vtables.tsv). Was the
//   descriptive FableDestVal_004e0ae8 stand-in (inner/outer on an address-named struct): the member is Users,
//   std::vector<CIntelligentPointer<const CThing> > +0x0C (shared engine/CTCGatherPointBase.h; Ego_r.pdb), whose destructor the size TU
//   calls out of line at 0x004DF88A (`__destroy_aux` 0x004DA5A1 over [start, finish), then `if (start) free(start)`; FableWin.pdb
//   ??1?$vector@V?$CIntelligentPointer@$$CBVCThing@@@@V?$allocator@...@std@@@std@@QAE@XZ); the tail is ??1CTCBase@@UAE@XZ 0x00686830.
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E0AE8, 19 bytes: `lea ecx,[esi+0Ch] ; call 0x004DF88A ; mov ecx,esi ; jmp 0x00686830`.
// Shape: no vptr store -- the class declares no destructor (the implicit ??1). DestroyOne makes VC7.1 emit the implicit ??1.
#pragma optimize("s", on)
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CTCGatherPointBase.h"

// Emission helper: destroying the object by name is what makes VC7.1 emit the out-of-line implicit ??1.
void DestroyOne(CTCGatherPointBase* object)
{
    object->CTCGatherPointBase::~CTCGatherPointBase();
}
