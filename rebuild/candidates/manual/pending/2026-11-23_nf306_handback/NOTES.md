# NF306 notes (2026-11-23): tail of the T1 "other" made-up-name batch after NF303 (hand-back 1)

## Summary
- **28 rows changed**: 26 landing-ready (`rows.txt`) and 2 that wait on question Q2 (`rows_conditional.txt`, entry 24).
  - All 28 graded RELOCATION_MATCH PASS in vland `--recheck-landed` (`recheck_final.log`, payload work/rc1: 28 RELOCATION_MATCH PASS,
    0 DIFFER, 0 RELOC_CONFLICT, 0 LINK_FAIL / TCC_FAIL).
  - The warnings are pre-existing local models and labels, except three expected RELOC_NAME lines: GetResult -> 0x00B8D0B0,
    AddPrimitive -> 0x00BACDD0 and GetMiniMap -> 0x00437CE0. Each of those self rows defines a stand-in under another name (NAME_FIXES).
- No header, flag, catalog or test change; hdr/ is empty. Every test of a changed row is a standalone model copy, so tests are unchanged
  (each was prechecked before its lock).
- **Entries: all 88 have a STATUS line.**
  - done 16
  - done-partial 8
  - done-conditional 1 (entry 24)
  - skipped-held 14 (a binder or the target is an NF304 / NF305 entry, witness or lock, or a T3 row)
  - skipped-port 29:
    - 11 by the `.drectve` ruling (NF303 Q2);
    - 18 because every binder DEFINES its stand-in body in its own TU, so a respell would make a strong copy under the owner's PDB name
      (NF281 D2).
  - open 20, not reached (lines 1-3 and the high fan-out signature entries); handed back here on context, at about 60 addresses.
- Order of work: unit A (13 single-binder entries) -> template probes -> unit C (12 entries) -> entry 11 identity read -> classification of the rest.

## Units
- **unit A** (entries 10, 14, 17, 18, 19 partial, 20, 22, 26, 27, 28, 37, 46, 48, 50): 15 binders respelled. Each was a name change plus
  pointer -> reference / const / integer-type parameters, as in the PDB spelling. The call sites pass `*p` where the parameter became a reference.
  - **0x00945B90**: the declaration moved from the local CAIStateGroup_WorkBase model into its CAIStateGroup_SitForMeal model as private
    IsMealTime() const. It is a non-virtual member, so there is no layout change. The caller is a SitForMeal member.
  - **0x0066B080 owner self fix** (entry 48, NF302 Q5):
    - InternalPlayAttachedSample was public (QAE) in the owner. Its other two exe callers 0x0066B280 / 0x0066BA80 already bound the PDB
      AAE spelling, so they were unresolved at HEAD.
    - The owner's declaration is now private and its Leaf is written in full (RENAMES section 1). No landed row binds QAE.
    - The new link resolves AAE for all three callers.
- **unit C** (entries 12, 25, 34, 35, 36, 45, 47, 49, 87): 9 binders, plus a second respell in 0x00602F30 (GetPMiniMapDisplay -> GetMiniMap).
  - **0x0054E4B0** (entry 87): its free-function declaration moved out of `namespace NUISystem` to global scope under the PDB name.
    The same TU closes and reopens the namespace around it.
  - **0x0049F180** (entry 36): keeps its pre-existing cast over the header's untyped InitialActiveQuests branch (the row's CWorld_PDB
    alias include) and dereferences it.
- **entry 11**: identity read.
  - 0x005E4430 declared GetQuestName() for 0x00702470. GetQuestName is the PDB member at 0x00702310, and the shared-header callers
    0x0058578B / 0x0061E6D0 / 0x0061B610 bind it there.
  - The donor Activate calls GetQuestObjective, so the binder was respelled `GetQuestObjective() const`.
- **entry 24** (CONDITIONAL, see Q2): the two binders were respelled GetResult(unsigned long, bool).
- **Template probes** (entries 60 and 85; rows reverted): a free function template (GFDataIsInsideBlock<C4DVector*>) and a CAFile member
  template (ReadVar<unsigned char>). Both are RELOCATION_MATCH, and objsec is equal except `.drectve`, which gains
  `/alternatename:<instance>=<non-template name>` (evidence/probe_templates.txt). So the NF303 Q2 shape holds for free templates and for
  other classes' member templates too. The remaining declared-only template entries (59, 61, 62, 68, 72, 73, 74, 76, 81) are
  classified by shape (the GetSubDef ones via NF303 proof_u6).

## Proof
- **objsec** (tools/proof2.py with exact old=new symbol pairs, from tools/symdiff.py): ALL_EQ for all 28 rows, `.drectve` included. Every new
  symbol is the entry's PDB spelling, 1:1. Table count is unchanged (0x00923510's vtable section is included and equal).
  - Files: evidence/proof_uA.txt, proof_uA2.txt (owner self), proof_uC.txt (includes 0x00602F30 with both respells; the uA block for it is
    superseded), proof_u11.txt, proof_u24.txt.
- **Links** (evidence/link_uA.txt, link_uC.txt):
  - Every moved reference resolves against its owner's HEAD object, or stays unresolved as at HEAD because its owner was not compiled.
    The uncompiled owners are:
    - 0x006E93C0 / 0x006E9680 / 0x006E9D10: their tests fail clang only inside VC7.1 `<functional>`. Their source Leaf is the PDB spelling.
    - 0x006E9700 (NF305 witness) and 0x007BA000 (NF304 witness): stay-off.
    - 0x0054E350: its source fails clang on MSVC-only syntax; the lock was released uncompiled.
    - 0x00B8D0B0 / 0x00BACDD0 / 0x00437CE0: stand-in self rows (NF302 Q2; NAME_FIXES).
  - Nothing HEAD resolved becomes unresolved. Duplicates: one pre-existing equal-copy LNK4006 in link_uA (same in HEAD and new), 0 in link_uC.
- **Every table delete was checked against all exe callers**, using work/xs_all.tsv (every E8/E9 site), tools/bk.py (what each landed caller's
  code names) and tools/absrefs.py (no target touched has an absolute reference).
  - Where a caller names the same identifier, I read its declaration: 0x004B5500 / 0x004B4A10 for ActivateMultipleQuests;
    0x0058578B / 0x0061E6D0 / 0x0061B610 for GetQuestName; 0x0066B280 / 0x0066BA80 for InternalPlayAttachedSample.
  - tools/tblafter3.py applies TABLE_CHANGES in memory: every touched target keeps its PDB line. Lines of other stand-in binders are left.
- **Name census** (evidence/name_census.txt): each new name is defined only by its owner. The exceptions are pre-existing and in rows I did
  not change:
  - 0x01229370 (global_E2) hosts its static in a stand-in SetWeightedThemes body;
  - 0x004B4A10 / 0x004B5500 carry same-TU noinline ActivateMultipleQuests copies;
  - 0x00A032F0's own Allocate is a different overload.
- **Staleness**: every orig/ copy equals HEAD at hand-back. The locks I hold are exactly the 28 changed rows.
- **Mid-run landing**: reloc_bindings.tsv and about 300 repo files (NF304 / NF305 agent_tools plus their landed rows) changed at 04:37,
  during my run. I did not write them. Re-validated after it:
  - every TABLE_CHANGES line against the current table (0 problems; tblafter3: all 25 touched targets PDB_OK);
  - every orig/ copy still equals HEAD;
  - mkstayoff rerun: none of my 28 rows is in stay-off or the T3 queue;
  - bk.py rerun (work/bk_final.txt): no newly landed row names a stand-in identifier I delete.
- **Template classifier**: tools/tmplcls.py was fixed to print each defining line (evidence/tmplcls.txt). Every "same-TU body" verdict
  has its line. Entry 84 is skipped-held instead: one binder defines a body, the other declares a stand-in; both are stay-off.

## Questions
- **Q1 (entries 1-3, not reached)**: no new question. NF303's Q1 ruling stands.
  - Entry 1's witness 0x005469E0 has an already-polymorphic local base, so `virtual` adds no layout change.
  - The next batch should take these with the 13 FinishAction binders of NF303 entry 14.
- **Q2 (entry 24, CONDITIONAL)**: the target is a 5-byte `xor eax,eax ; ret 8` stub. GOTCHAS NF30 says such bodies can be folded under
  several names ("reject rather than pick"). Against a fold:
  - its byte-twins are separate functions at 0x00B8FD90 / 0x00B23460 / 0x00CA8530, so the linker did not fold this shape;
  - GetResult is the only two-argument (K, _N) CEngineOcclusionManager member in both name sets;
  - every exe caller is an occlusion site.

  The donor evidence is one step indirect: the donor binders call GetPrevFrameResult(K), which calls GetResult, and retail pushes two
  arguments, so GetPrevFrameResult was inlined. Do the two binders land on that, or do they wait for a direct second source?
  (My reading, not a ruling: NF30 is about naming the self row, while the nf104 line (GOTCHAS) governs what callers bind.)
- **Q3**: entries 19 / 49 (and the stay-off owners) respell binders whose target self row is an NF305 / NF304 witness. I did not edit or
  compile those self rows; their Leaf already is the PDB spelling. Is a binder-only fix there acceptable, or should it wait for their landing?

## Rule notes
- **Prechecks and locks**: every compiled source passed its precheck (clang, work/pre_*.log) in an earlier call, after its lock. Each landed
  test was prechecked before its lock was claimed. The planted control is in work/pre_control.log (FAIL on the planted error, PASS on the clean file).
  - Entry 24's two rows were reverted after their first grade (before the fold check) and restored. Their tests were prechecked again
    before the locks were re-claimed, and the rows were rechecked and regraded.
- **Background runs**: every grade, link and vland ran in the background with `timeout 900` on its own launch line (inside gr.sh / lk.sh /
  rf.sh). Completion was read with kill -0 / logs in later calls.
- **Never done**: sleep, rm, git, writes under the repo, `__asm` / `naked` / `_emit`, header edits.
- **Stay-off**: tools/mkstayoff.py rebuilds the set before each unit. It covers:
  - NF304 / NF305 entries;
  - their witnesses, from reloc_bindings.tsv in full (the ENTRIES spellings column is truncated for big entries);
  - their rows/ and rows.txt;
  - other agents' locks, the standing held rows, the Codex p28 reserved rows, and the T3 queue rows.

  No lock conflict was met.
