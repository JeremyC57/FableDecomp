# Retail class-model migration, wave 1 (one agent; the chunk 580 authoring agent shares the Wine lane)

S=/tmp/claude-0/-home-user-FableDecomp/a013f3ec-d440-5b47-87a1-ed975cb80dfb/scratchpad. Every Bash call:
`export S=...; source $S/wine/env.sh; export FABLE_ROOT=$S/mig1/repo; cd $S/mig1/repo`.

## Goal (owner rule, 2026-10-29)
A class that has a vtable in retail must be modelled with its real virtuals from a shared header, never a row-local stand-in
(`virtual ~X(); unsigned char _body[N];`, invented `UnknownSlotN` virtuals, a flattened `void* __vftable` member). The header
generator flattens every vptr, so landed rows modelled polymorphic classes locally. Bytes must not change: every touched row
must still grade MATCH/RELOCATION_MATCH + PASS.

## Where you work
- `$S/mig1/repo` is YOUR private git worktree of branch FullDecomp (detached HEAD 7f95f15cc). Edit there only. Never run
  `git commit/push/checkout/reset/stash/worktree` (read-only `git diff/status/log/grep` inside the worktree is fine). Never
  write under /home/user/FableDecomp. Scratch files go in `$S/mig1/work/`.
- Never kill a process. `timeout` on every compile. Never repeat a failing command more than twice with the same arguments.
  No recursive find over `/` or the whole scratchpad.
- No `__asm`, `naked`, `_emit`, `volatile` anywhere, comments included (landed retail_asm rows keep their existing asm).

## Already done (study these first)
Retail opt-ins in the shared headers, written from retail RTTI + vtables + both PDB name lists:
- `FABLE_RETAIL_DEF_CLASS_BASE`: engine/CDefClassBase.h, CDefPointeeBase.h, CResource.h, CIVCountedPointeeBase.h.
- `FABLE_RETAIL_SUSPENDABLE_PROCESS`: engine/CSpawnedFuncBase.h, CASuspendableProcess.h, CSuspendableProcessMicrothread.h,
  CBaseClassNonCopyable.h; plus engine/CBaseClass.h (no opt-in; inline ctors, ~CBaseClass 0x0099A2E0).
- Examples on them: landed 0x00D34D40, 0x00E54AA0 (chunk 577), 0x00E27920, 0x00F05F30 (578), all of chunk 579
  (`rebuild/candidates/manual/batches/2026-10-27_linux_hand57{7,8,9}/`). docs/pipeline/GOTCHAS.md, last ~40 lines.
How to recover a vtable: RTTI COL at vt[-1] (`ghidra_out/rtti_bases.tsv` for bases); objects without RTTI (vt[-1]==0) are
found from the ctor/copy-ctor vtable store. Slot names from catalog / labels / both PDB lists (`ghidra_out/egor_pdb_names.tsv`,
`fablewin_pdb_names.tsv`, `egod_pdb_names.tsv`); class keys `ghidra_out/pdb_class_keys.tsv`; Ego_r layouts
`ghidra_out/struct_layouts_egor.tsv` (Ego_r is not retail: check sizes against retail accesses). Mark anything unproven in
the header comment (as CDefClassBase.h does for GetPSubDefInfo).

## Part A: new retail headers (opt-in, so existing includers stay byte-identical)
For the NScript classes every quest row models locally: CScriptThing (vtable 0x01238C8C; its generated header is flattened),
the game-script interface (vtable 0x01260F0C; real class name from RTTI/PDB, NOT `CGameScriptInterfaceBase` unless that is
proven; `ghidra_out/cgsi_vtable_slots.tsv`), CScriptBase (retail 0x40), CEntityScriptBase, CLevelScriptBase<T>, CTimer, the
predicate classes (CPredicateBase, CIsThingAlive 0x012C32F0, CPredicate_And 0x012E1B70) and CEntityScriptBinding<T>.
Use one opt-in macro `FABLE_RETAIL_NSCRIPT` for the new ones. Prove each class's chain, size, member offsets and every virtual
slot (name + exact PDB signature + retail address); unproven slot names stay explicit `// unproven` with the evidence gap.
Prove each header compiles (VC7.1 and `g++ -m32 -w -std=c++98 -fsyntax-only`) and regrade at least 5 landed rows rewritten
onto it before migrating more. Skip a class you cannot prove and say why.

## Part B: migrate landed rows (stop at about 600 edited rows; order by value)
1. The 7 already-landed rows chunk 579 flagged: 0x00F25F90, 0x00F26420, 0x00F26C30, 0x00F27060, 0x00F261F0, 0x00D35070
   (local CDefClassBase with invented `UnknownDefinitionSlotN` virtuals) and 0x00F08590 (local CBaseClass).
2. Local CSpawnedFuncBase (351 files: `grep -rlE '^(class|struct) CSpawnedFuncBase\b' rebuild/src/compiled`), then local
   CDefClassBase (1,165), then local CBaseClass (1,816), then the Part A classes; take the families where one scripted
   rewrite fits many rows. Write idempotent scripts with verified file lists, as the naming waves do
   (`rebuild/candidates/manual/naming/2026-10-29_wave9/scripts/`).
3. Per row: replace the local model with the opt-in define + include; keep derived classes local but give them their real
   overrides; drop members/virtuals the header now provides. If a row's code depends on a wrong stand-in detail (e.g. a slot
   index it calls virtually), fix the call to the real slot. If the row stops matching, revert it and log the residue.
4. Regrade in batches of ~400: `python3 tools/decomp_pipeline/linux_lane/lane_tools/recheck.py <dir> <addrs...>` (mkdir the dir
   first) then `vland.py <dir>/payload.json <dir>/oracle.tsv --recheck-landed` with `LANDVERIFY_WORK=$S/mig1/work/lv<n>`;
   delete each lv dir afterwards (disk is a fixed allowance; about 9 GB free). Baseline-regrade any failure on an unedited copy
   to tell pre-existing failures from yours. Bindings: if a rewrite changes a row's relocation symbol names, update
   `rebuild/oracles/reloc_bindings.tsv` (one retail address = one name) and run
   `python3 tools/decomp_pipeline/linux_lane/lane_tools/coordinator/bindings_multi_check.py rebuild/oracles/reloc_bindings.tsv` (must print 0).

## Hand-back (files are the channel; `$S/mig1/`)
- The edits stay in the worktree (the coordinator commits them). `$S/mig1/ROWS.tsv` (addr, file, change, grade before/after),
  `REVERTED.tsv` (addr, reason/residue), `HEADERS.md` (each new header: evidence per slot/member, what is unproven),
  `NEXT.md` (exact remaining counts per family and the plan for wave 2), `logs/` (recheck logs), `scripts/`.
- Final message: headers added, rows migrated per family, recheck totals, reverted rows, bindings changes, open questions.
- Leave no background job running.
