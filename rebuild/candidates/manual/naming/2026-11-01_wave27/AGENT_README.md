# Naming pass wave 27 (2026-11-01): 40 landed rows respelled, plus 3 header dependents, all byte-identical (43/43 MATCH or RELOCATION_MATCH + PASS)

## Inputs and index
- Inputs:
  - the brief naming/briefs/BRIEF_W27.md;
  - QUEUE_W27_input.tsv: 39 rows, including the 22 c695 rows the coordinator appended mid-wave;
  - DEFERRED_W26_INPUT.tsv.
- Index:
  - idx/full.jsonl is HEAD. It is the wave 26 after.jsonl plus chunk 695 (idx/delta_c695.jsonl, 12 rows), which I found by modification time, with no git.
  - Before hand-back, nothing in the repo outside .git is newer than idx/.c695_marker.
  - idx/after.jsonl is HEAD plus ov_final.

## Commands
Run from $S/naming/w27 with S exported and $S/wine/env.sh sourced:
1. `python3 scripts/compose27.py ov_final`
   - Runs scripts/hand27/h27_<fam>.py on fresh repo copies.
   - Writes runs/<fam>.rows.txt and ov_<fam>/.
2. `bash scripts/chain27.sh` does all of the following:
   - the recheck of runs/final_plus.rows.txt (the family rows plus runs/hdrdep.rows.txt);
   - mkindex and after.jsonl;
   - qscan27, mkq27 (with mkq27_notes.py), mkdef27, mkbind27 (and runs/reloc_bindings.after.tsv);
   - bindings_multi_check, which reports 0;
   - mkren27 then fixren27, and mklabels27 then fixlab27.

New helpers:
- scripts/ovinc.py rewrites `#include "X.h"` to the overlay copy when ov/rebuild/include/X exists. Overlay header edits (the fdosw opt-in) therefore reach both the row source and its test. It is used by mkpayload_fx.py, bld.py and mkindex.py.
- scripts/hand27/hdr.py edits overlay headers.

## Families
Listed in compose order.

- **main (7): 00d9f270 00ee76f0 00e0e2e0 00d3a1a0 00e8b020 00ef29e0 00ef3000** (DECISIONS_W26 D1)
  - Each entity is nested in its NScript script class, with `private: virtual void Main()` (EAE).
  - The local CEntityScriptBase gets the slot list of vtable 0x012C3224, and its `VTable` / `__vftable` members are removed.
  - The callees take their PDB spellings: PlayCutscene, LookAfterHealth, GetCutsceneName, and CScriptBase::AddSpawnedFunction.
- **fdosw (5): 00994480 009a7720 00a60aa0 009a7d40 00a60c30** (D2)
  - engine/CFileDataOutputStream.h gains the opt-in FABLE_RETAIL_FILE_DATA_OUTPUT_STREAM. 0x00994480 is the member Write (UAE), and its binders call it.
  - CreateRetailBank and Save are private. The callers 0x009A7D40 and 0x00A60C30 are respelled to match.
- **pxs (3): 00a5ecb0 0098a9a0 00a5d620** (D4)
  - The CPixelShader ctor and CreatePixelShader are private (AAE), with friends CShaderRenderManager and CShaderResource.
- **acc (5): 0071ce80 007fecb0 007fede0 005fc620 006116e0** (D5 access only)
  - GetTattooNames is QAE, UpdateBlockedActionPoints is IAE and FindTeleports is AAE. The callers follow.
- **dm (4): 009aef60 009af480 009ade90 009cc530** (D5 plus c695)
  - The protected CDefinitionManager members get their PDB parameter types.
  - CSymbolMap::UpdateDirty() const replaces the invented ?Finalise@CSymbolMap / ?Finalise@CSymbolTable.
- **virt (2): 00a86e20 004ff080** (D5 virtual)
  - GetPossibleCollisionSet is MBE and LoadTopologyFromFile is UAE (the `_vptr` member is removed).
- **vl2 (5): 008865c0 00f3f5a0 00f372b0 00774100 00aebaf0** (D6)
  - STLport vector<T, allocator<T> > and CArray<T> : std::vector, with each second target on its bound spelling.
  - 00774100 and 00aebaf0 take their Ego_r / FableWin Leafs.
- **pb (2): 004128a0 005955ab** (D7)
  - 004128a0 is the STLport vector<CWideString, allocator<CWideString> >::push_back, by explicit instantiation (renamed from Vector_PushBack).
  - 005955ab's GetProfileNames is IAE.
- **sym (3): 009b0470 00ae85f0 009ae5f0**
  - The rows use UpdateDirty() const. 009ae5f0 calls CSymbolMap::DeallocateUnusedMemory, and its own Leaf is IAE.
- **c695 (4): 008544c0 00584a73 00a0d410 007196b0**
  - 008544c0 calls ~CScriptModuleFOVZoom qualified.
  - 00584a73 calls the virtual NInventory::CTCInventoryBase::AddCloseInventoryEvent.
  - 00a0d410 calls the protected RemoveInputProcess, with CAInputProcess a friend.
  - 007196b0 is ~_List_base<CCountedPointer<CTCScriptedControl::CActionBase> >. The compiler emits exactly the queue's hand-mangled spelling.

Devices used:
- Explicit instantiation (004128a0, 007196b0) and `__declspec(noinline)` on out-of-class template bodies (00774100). Both have landed precedent: 919 landed files carry `template class` explicit-instantiation lines, and `template void` instantiations land too (e.g. 00bbd7f0) and 2,655 use noinline.
- Two new friend lines (DECISIONS_W27 D1).

## Outputs
- QUEUE_W27.tsv: 39 rows. Outcomes: 6 respelled, 3 respelled-partial, 7 done, 22 deferred (scope 3 not reached), 1 hold.
- DEFERRED.tsv:
  - 25 queue deferrals;
  - 2 wave-27 findings (runs/deferred27_manual.tsv: the 00666b50 family and the 005955ab callers);
  - 452 carried rows;
  - 12 wave-26 entries resolved (runs/w26_deferred_resolved.tsv).
- BINDINGS.tsv: 32 RETIRE, 16 ADD, 10 KEEP, 0 CONFLICT.
- RENAMES.tsv has 8 entries:
  - 004128a0 and 007196b0;
  - the six D1 Main rows (00d9f270 was already named).
- LABELS.tsv has 18 entries.
- DECISIONS_W27.tsv has 7 items.
