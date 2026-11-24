# nf360 b2: 0x00428B10 respelled in place onto ?DeleteData@?$CCountedPointer@VCComponent@NUISystem@@@@SIXPAX@Z

Agent NF360 (Account A naming queue; hand-back `pending/2026-11-24_nf360_handback/`). The landed self was the behavioural extern "C"
`@DeleteIfSet@4`. The row is now an explicit specialization of engine/CCountedPointer.h's `DeleteData` with the landed body, under the landed
FABLE_RETAIL_CCOUNTEDPOINTER_DELETEDATA_DECL_ONLY opt-in (with the in-class body VC7.1 emitted no symbol). Names: FableWin.pdb; retail
stores this address as the count block's DeleteFunc in Reset 0x00429C6F and the (T*) constructor 0x00429C15; the reloc_bindings line
(witness 0x005331A0) already binds the PDB name here.

Ruling R3 (advisor decision, owner delegated, 2026-11-24): land the strong form. The link adds duplicates against 0x00626750 (same body) and
0x005820E7 (8 B plain operator delete, NEQ, pre-existing); those different-body copies already exist under the PDB name at HEAD, so the owner
brings no new body. The pick-any variant (v2) is rejected: a local model of a class that has a shared header. The 8 B copies are queued
(T3 nf360 0x00428B10).

Coordinator verification from the repo: vland `--recheck-landed` MATCH PASS; pk_all `--leaf`: no NEQ copy at HEAD or after; reachflip
0 flips; pickany_vs_retail NEQ lines identical to the agent's HEAD run. self_symbols line refreshed; NAME_FIXES lines 541 / nf356 for
0x00428B10 closed (`NAME_FIXES_CLOSED.tsv`). Not applied: the proposed file rename `global_DeleteData_00428b10` ->
`CCountedPointerNUISystemCComponent_DeleteData_00428b10` (Module `_global` -> `CCountedPointer<NUISystem::CComponent>`), left for a rename
pass as with NF356. 0x0042AF3C (b1) is not landed: it lands after the R2 edit (cast marked scaffolding) and a regrade.
