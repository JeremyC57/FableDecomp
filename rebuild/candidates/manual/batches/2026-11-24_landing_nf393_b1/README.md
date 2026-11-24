# Landing nf393 b1: 0x00B217D0 on the shared CArray header with its real nested name (rulings R7 pilot, R10, R11)

0x00B217D0 is `NParticleEngine::CPSCSplineBase::CSplinePosParamData::InterpolateTo`. It was spelled as an unnested `CSplinePosParamData`; the new name comes from the Ego_r.pdb and FableWin.pdb name sets (FableWin 0x02F5E500), and its only retail caller is CPSCSplineBase::InterpolateTo 0x00B22310 (call at 0x00B22483). It now models its CArray<unsigned long> members with `engine/CArray.h`, and `engine/CPSCSplineBase.h` through a new default-off opt-in. Its two calls bind STLport `vector<unsigned long>::operator=` at 0x0045BC09 and the `NParticleEngine::GFInterpolateArray<unsigned long>` template at 0x00B22F30.
- R7 pilot result (NF393): 10 new pick-any COMDATs: the `??4` CArray forwarder and the STLport `<unsigned long>` destructor chain. Rulings R10 and R11 (advisor decision, owner delegated): R10 is ruling R1 tier 1. Each copy is byte-and-reloc identical to the same-named copy in landed 00ad9980, which takes its copies from engine/CArray.h; the coordinator's `comdat_cmp` shows EQ for all of them. R11: `~CArray` (local NParticleEngine::CArray -> global CArray) has equal bytes and relocations, so it pairs with HEAD.
- Coordinator checks:
  - the self section has the same 118 bytes; only its two call relocations change;
  - the source equals the graded source (apart from a trailing newline);
  - landed RELOCATION_MATCH; self = `// Leaf:` = `self_symbols.tsv` (checked by hand, unit u1);
  - the header edit is one default-off `#ifdef` block, and only this row defines the macro;
  - the `.drectve` gains `libcp` (LIBCP allowance) and a VC7.1 template `/alternatename` line of the same shape as landed 00b22310's (NF393 evidence/drectve_b1.txt);
  - baselines: bindings multi 1, funclets 0 broken, S units 3 OK.
- NF393 gates: vland HEAD and new RELOCATION_MATCH; pk_all and pickany_vs_retail with no NEQ and reach flips 0; a VC7.1 census of the 25 NParticleEngine local-CArray rows, the GFInterpolateArray rows and 00b22310 found no reference to the removed spellings or the old self; /FORCE link duplicates 0 -> 0 and nothing HEAD-resolved left unresolved; the new self resolves 00b22310's reference.
- Table:
  - delete `??4?$CArray@K@NParticleEngine@@QAEAAU01@ABU01@@Z -> 0045bc09`;
  - replace `?GFInterpolateArray@NParticleEngine@@YI?AU?$CArray@K@1@...` with the template spelling `??$GFInterpolateArray@K@NParticleEngine@@YI?AV?$CArray@K@@ABV1@0MABK@Z -> 00b22f30` (0x00B22F30 is not landed).
- Follow-up: the catalog label and file name still say `NParticleEngine::CSplinePosParamData` (NAME_FIXES_QUEUE nf393).
