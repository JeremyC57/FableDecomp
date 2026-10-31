# Chunk 632 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- Read the retail early-out targets before blaming register colouring: a "pure ebx/ebp swap" can come from a
  base-class call placed inside a guard when retail runs it on every path. With the call at function scope, `this`
  stays live and takes retail's register (0x00943B40, `je 0x943C08` lands on `mov ecx,ebp ; call base::Cleanup`).
- VC7.1 reassociates `x * x * W` (and `x*W*x`, `x*(x*W)`, `Sqr(x)*W`) to `fld x ; fmul [W] ; fmul x`. Only a named
  `float x2 = x * x;` keeps retail's `fld st0 ; fmul st1 ; fmul [W]` order, and that statement's position then decides
  which term is evaluated first (0x00AC7EB0).
- A member read inside a call argument that also contains rand() is hoisted ahead of the call when the read comes
  first in the source: `pGlobalScriptDef->FishPrizeWeights[1] * 0.25f + (rand() % 50) * 0.1f` loads the vector start
  into edi before rand(), with no invented pointer local (0x00E7F330).
- Vector zero stores do sink past argument pushes. With the list iterators in named locals before the vector, the
  three `mov [esp+x],0` move below all the pushes. With only the end() iterator in a local, they land between the
  functor push and the begin load, as in retail (0x0090BC60).
- `std::not1(Pred())` over an empty predicate with only implicit copies emits an uninitialised byte copy
  (`mov al,[esp+3] ; mov [esp+14h],al`). A user copy constructor on the predicate removes it (0x008E7F80, matching
  FableWin's count_if<.., unary_negate<CIsImposterHit> >).
- engine/CIVCountedPointeeBase.h `Release()` (returns `--IVRefCount`) cannot give retail's `dec [ecx+4] ; jne`. Test the
  member directly (`if (--p->IVRefCount == 0) p->Delete();`), and nest the null test in a destructor:
  `if (p && --p->IVRefCount == 0)` gives `mov eax,[esi+4] ; dec eax ; mov [esi+4],eax` (0x005E1210).
- Swapping the inline reference accessor `PBrain->GetCreature()` (returns `*Creature`) for a direct `*PBrain->Creature`
  through engine/CAIBrain.h breaks a match: `mov ecx,[eax+18h] ; push ecx` instead of `mov eax ; push eax`, and the
  body after the call changes. Keep the accessor and clear the shared-header warning with the `#define CAIBrain
  CAIBrainLayout` include pattern (0x00943B40; vland is then clean).

Review at landing:
- Landed 0x009B9570 CStringParser_CalcInQuotes_009b9570.cpp emits a `__fastcall` stand-in symbol
  (?CalcInQuotes@CStringParser@@QAI_NPAX_NPAD2@Z with a dummy edx parameter). The PDB symbol is
  ?CalcInQuotes@CStringParser@@IBE_N_NPBD1@Z (protected const thiscall). See NAME_FIXES.tsv.
- Landed std_countif_008e8a90 / _008e8b80 name the predicate CThingFilter_IsNotInMode. The PDBs say CIsImposterHit in
  an anonymous namespace (the ?A0x hash is per-TU, so the binding needs a policy decision).
- Landed GenerateCompetitionEntry 0x00E7F730 calls 0x00E7F7D0 "GetPrizeThreshold". The landed callee itself is
  GetNextPrizeWeight (one address, one name).
- The reloc_bindings table holds 8+ spellings each for 0x007EE540 and 0x008FCFD0, and two each for 0x0125A8A4,
  0x0128B0B0 and 0x00659EFC. NAME_FIXES.tsv lists the PDB-proven ones.
- pGlobalScriptDef 0x0143E90C: the PDB has a dynamic initializer and an atexit destructor, so it is probably
  CDefPointer<CScriptDef>, not CScriptDef*.
- The 0x0090BC60 differ source uses an unnatural begin() (from the end() local's node) to get within one register. The
  natural form's residue is the store/load schedule described in its Grade line.
- 0x005E1210's differ file is the old 221-byte non-template stand-in. The respelled template version (probe
  005e1210_a.cpp) is 225 bytes because of the header's Release; fold the two together.
