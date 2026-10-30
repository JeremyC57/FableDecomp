# Retail class-model migration, wave 2 (one agent; the chunk 581 authoring agent shares the Wine lane)

S=/tmp/claude-0/-home-user-FableDecomp/a013f3ec-d440-5b47-87a1-ed975cb80dfb/scratchpad. Every Bash call:
`export S=...; source $S/wine/env.sh; export FABLE_ROOT=$S/mig2/repo; cd $S/mig2/repo`.

Wave 1 landed as commit 08436554e: 613 rows, 12 new headers, CDefClassBase corrected to 0x25 packed. Its records, including
HEADERS.md (evidence per slot) and NEXT.md (the plan you are executing), are in
`rebuild/candidates/manual/headers/2026-10-29_retail_models_wave1/`. Read `$S/mig1/BRIEF.md` for the full rules (same rules
apply; your worktree is `$S/mig2/repo`, detached HEAD 08436554e; scratch in `$S/mig2/work/`; hand-back files in `$S/mig2/`).

## Goal
Same owner rule: a class with a retail vtable is modelled with its real virtuals from a shared header, never a row-local
stand-in. Bytes must not change; every touched row regrades MATCH/RELOCATION_MATCH + PASS. Stop at about 700 edited rows.

## Order of work (by retail-correctness value; NEXT.md has the counts and dry-run numbers)
1. **New retail branches needed before families can move:**
   - `CParentDefClassBase.h`: retail branch (vtable 0x01230CBC; overrides slots 10-16 and 21-23; members InstantiationName
     and a CVectorMap; ghidra_out has the addresses). Blocks 332 rows.
   - `CIVCountedPointeeBase.h`: add the FableWin inline `AddRef() const`, `Release() const`, `GetRefCount() const`
     (IVRefCount mutable or const_cast; prove which from retail bytes).
   - A real `CDefPointer<T>` header (PDB members: Object +0, Release, InternalGetObject, SetDynamicCast<U>, operator=<U>, Get,
     GetRefCount, Reset). Prove the inline bodies on 5 rows first (temporary release `mov edi,[eax+4]; dec edi; mov [eax+4],edi`
     vs member release `dec [ecx+4]`). Blocks the 151-row GetSubDef<T> family (dd084063): its invented `vNN` slots go, slot
     14 becomes the proven const `GetPSubDefInfo` (`p->DefIndex`).
2. **CBaseClass (BC) remainder:** the 238 exact-shape rows (cd08cff0) by script; then the dtor-only / inline-body forms with
   mcheck. Retail calls the CBaseClass ctor out of line at 0x0099A2D0 (1,225 callers) yet some rows matched with an inline
   vptr store: decide per row from the bytes which it is, and record the rule you find in LESSONS (do not keep a local inline
   ctor without that evidence).
3. **DCB remainder:** the 83101c63 (18), 46f7e374 (15), 662692ff (14), 012a455e (24) / fe4a0baa (7) CDefBase, and the
   Destroy / RefCount stand-in clusters; the 35 local-CResource rows; the 16 SFB hand edits.
4. **NScript families** (the bulk: CScriptBase 1,702, CScriptThing 2,501, CEntityScriptBase 1,120, CGameScriptInterface
   1,381): interface-only rows (`Game->X(...)`) first, by script, per NEXT.md's (a)-(d); then CScriptBase + CLevelScriptBase;
   then CEntityScriptBase + CScriptThing. Named interface slots must be checked against the header spelling before the local
   block is dropped. Open item 1 (the CScriptThing copy inline at one site, out of line at another) is worth settling early:
   try `inline` copy ctor + non-forceinline CSpawnedFunc1 ctor across several spawn rows.
5. Respell row 0067c980's leaf `?Copy@CBlurredAttitudesDef@@QAE..` -> `UAE..` (both PDBs) and un-revert it; update its
   binding and build_candidates.ps1 Leaf if present.

## Rules (unchanged from wave 1)
- Private worktree only; never git state changes; never write under /home/user/FableDecomp; never kill processes; `timeout`
  on every compile; never repeat a failing command more than twice; no recursive find over `/` or the scratchpad.
- No `__asm`, `naked`, `_emit`, `volatile` (comments included); landed retail_asm rows keep their asm.
- Regrade in batches of ~400 (`recheck.py` + `vland.py --recheck-landed`, `LANDVERIFY_WORK=$S/mig2/work/lv<n>`, delete each lv
  after; ~9 GB free). Baseline-regrade failures on an unedited copy. Revert rows that stop matching; log residue.
- Bindings: one retail address = one name; update `rebuild/oracles/reloc_bindings.tsv` when a rewrite changes a row's
  relocation symbols; `bindings_multi_check.py` must print 0. Do NOT touch the 0x0143E8F8 multi-name issue (coordinator).
- Header comments cite the retail evidence per slot/member; unproven items say `// unproven` with the gap.

## Hand-back (`$S/mig2/`)
ROWS.tsv, REVERTED.tsv, HEADERS.md (new/changed headers with evidence), NEXT.md (wave 3 counts and plan), NAME_FIXES.tsv,
LESSONS.md, logs/, scripts/. Final message: headers added/changed, rows per family, recheck totals, reverted rows, bindings
changes, open questions. Leave no background job running.
