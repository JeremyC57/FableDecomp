# Naming agent FAM13: the 275 engine `vfunc_N` rows, plus FAM12's follow-up lists and queue dt/du

The user, 2026-11-11: naming goes one family per agent, and a new agent starts as soon as the last one lands.
The user, 2026-11-12: naming skips bytes taken directly from a library; library code this project wrote by hand gets only a light,
readable renaming. This brief is engine code only (0x00400000..0x00BFFFFF), which keeps the full rule: exact PDB spelling, two sources.

**Part 1 -- correctness of landed bindings first.**
- BINDER_FIXES.tsv (42 lines from naming agent FAM12: row_address, caller, caller_binds, correct_symbol): landed callers that still
  bind a FAM12-renamed row under another spelling (a map without its allocator argument, an iterator without its traits argument,
  `_M_erase` spelled public, the stand-in `SetGameMaster`, a CPersistTraits<K> stream type). Respell each caller's declaration to the
  correct symbol through the shared header (stlport/fable_map.h, fable_containers.h, engine/CPersistTraits*.h ...) and grade it.
- CALLEE_FIXES.tsv (31 lines): out-of-line callees that FAM12's respelled rows now call by their STLport names while the callee rows
  are still landed under stand-in names: rename those callee rows (self-row renames, RENAMES section 1).
- FAM12_NOTES.md: about 19 callers still bind `~CCountedPointer<NUISystem::CAction>` to 0x00429160, which is really
  `CCountedPointer<NUISystem::CAction>::ReleaseData` (the dtor is the 5-byte jump at 0x00429B72): respell them.
- QUEUE.tsv (tags land1011dt, land1011du), claims to verify: 0x0066AD90 CTCSoundPlayer::ReleaseAttachedLoopingSound (landed as
  StopSample / StopSound); 0x01436E3C NEngineInternal::GPScreenEffectOutlineGlow (landed as g_pWaterRenderer in 0x00B33010 -- that
  file's BeginReflection / EndReflection method names are unproven too -- and as g_pEngineScreenEffectOutlineGlow); 0x007120D0 (a
  folded derived-action dtor landed as `_global::Dtor_CCreatureActionBaseBase`; 0x009658B0 calls it as ~CCreatureAction_PickUpGenericBox:
  pick the one name the evidence supports); 0x004460C0 (0x00847520 binds IsAssignableSpecialMoveButtonInQueue, landed as
  CGamePlayerInterface::IsQuickAccessEventInQueue: folded twins or a wrong name?); 0x013BDA50 (landed `g_StreamingFontBanks_013bda50`:
  find the PDB name, e.g. in NStreamingFontBankGlobals).

**Part 2 -- the family: ROWS.tsv, 275 landed engine rows named `<Class>::vfunc_<N>`.** The class is already known; only the method name
is a slot placeholder (most rows: NUISystem::CSprite 18, NUISystem::CList 12, the CTCInventory* classes, NUISystem::CComponent).
For each row:
1. Name the slot from two sources: the class's vtable in Ego_r.pdb / FableWin.pdb method lists (ghidra_out/struct_layouts_egor.tsv,
   egor_signatures.tsv, _fablewin_api.tsv, naming_stragglers/FableWin.exe.signatures.tsv -- virtuals are listed in declaration order,
   and VC7.1 lays overloads out in reverse declaration order), the retail vtable itself (rebuild/manifest/rtti-vtables.tsv: slot N of
   which class's table points at this address -- a derived class's override shares the base's slot name), sibling classes whose same
   slot is already landed with a real name, and what the body does. The Anniversary 360 Test PDB ($S/anniv_dbg/test4k.pdb,
   llvm-pdbutil; publics in $S/anniv_dbg/publics.txt) is a private third source that confirms only.
2. Rename it (RENAMES section 1: address WITH the 0x prefix, module = the class, name = the METHOD ONLY), give the row an exact
   `// Leaf:`, and where the row models the class locally although engine/<Class>.h exists, port it onto the header (name the slot
   in the header under an `#ifdef` opt-in if it is a placeholder there too). It must still grade MATCH / RELOCATION_MATCH and its
   test must PASS -- run the test with the lander's own check, not only your grader:
   `python3 -c "import sys,os;sys.path.insert(0,'tools/decomp_pipeline');import verify_and_land as V;from pathlib import Path;a=sys.argv[1];w=Path(os.environ['S']+'/w22/agentFAM13/tw/'+a);w.mkdir(parents=True,exist_ok=True);print(a,V.behaviour_of(open(sys.argv[2]).read(),a,a+'_TEST PASS',w,V.env()))" <addr> <test.cpp>`
   (a landing found 16 regex-respelled tests that did not compile under VC7.1 although the agent's own grader said PASS).
3. Respell landed binders that call the slot under the placeholder (section 2).
4. A slot you cannot name from two sources stays as is (LEFT.tsv: what you found).
Work class by class, largest first, and report classes done / rows renamed / rows left.

**FAM12 is landing while you work.** Its 152 changed rows (and author chunk Q27's five) are in BUSY.txt with every RESERVED row. Your
overlay hdr/ STARTS as a copy of FAM12's overlay (see hdr_base_FAM12/): keep those additions, put yours on top, and list in
hdr/README.md which headers you changed beyond FAM12's copy. BINDER_FIXES callers that are FAM12 rows themselves are not in the
list; if a caller you must edit is in BUSY.txt, record it and skip it.

Repo: /home/user/FableDecomp (read-only for you). Scratch: $S = /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad.
Your folder is $S/w22/agentFAM13/. Read CLAUDE.md and tools/decomp_pipeline/linux_lane/lane_tools/chunk_pool/EXTRA_RULES.md first.
The repo is committed and current (HEAD).

## What to do for each entry
1. **Verify the claim.** An entry is a CLAIM. Check it against two sources: the PDB name sets (ghidra_out/egor_pdb_names.tsv,
   fablewin_pdb_names.tsv, _fablewin_api.tsv), the callee body, vtable slots or the binding evidence. A claim that does not hold goes to
   REJECTED.tsv with the reason.
2. **Find every landed binder of the wrong spelling.** Grep the retail target in rebuild/oracles/reloc_bindings.tsv (symbol, target,
   first caller), and grep the wrong spelling's pieces in rebuild/src/compiled. reloc_bindings lists only the FIRST caller of each symbol,
   so the source grep is what finds the rest.
3. **Respell each binder's declaration or call** to the correct symbol, using the shared header declaration where one exists, with an
   `#ifdef` opt-in if the header lacks it. Grade every changed row: it must stay MATCH or RELOCATION_MATCH. Run its test, and keep the
   catalog PassPattern.
4. **When the wrong name is a landed row's own name** (the CALLEE_FIXES rows and the 275 family rows here), respell that row too and list it in
   RENAMES.tsv section 1 (module = class, name = method only; never `Class::Method` in the name column).
5. **Rows reached through stand-in classes** (address-suffixed classes, `_unattributed`, `Row00c...`): respell them only if the correct
   class is proven and a shared header exists. Otherwise record the gap.

## Hard rules
- Never run git in any form, not even read-only commands. Never write under the repo. Never land anything.
- Every changed row must still grade MATCH or RELOCATION_MATCH.
- No `__asm` / `naked` / `_emit` / `register` / bare `__forceinline` in rows or tests. Use `volatile` only where retail used it.
- Use shared headers over local models, and add no NEW local models of classes that have a header.
- Header changes go in $S/w22/agentFAM13/hdr/<path>: a full copy of the CURRENT repo header with `#ifdef` additions only. Grade with
  FABLE_HDR_OVERLAY=$S/w22/agentFAM13/hdr.
- Use exact PDB names: one address, one name, and no made-up names.
- Do not touch the addresses in BUSY.txt (every RESERVED row, FAM12's 152 rows and Q27's 5) or any pending/*/RESERVED.tsv address.
- Run at most one compile at a time (another agent may compile in parallel), inside `timeout 240`. Wine cl can hang on `#error`.
- Make at most 10 grades per row, counting failed compiles.
- Every Bash call must exit 0: end probe, grep and diff pipelines with `; true` (user rule). A grading script prints its result as text.

## Lane
Every Bash call: `export S=...; source $S/wine/env.sh; export S; cd /home/user/FableDecomp`.
Grade: `timeout 240 python3 $S/tools/q.py <addr> <file.cpp> '<full mangled leaf>' [--flags ...]`, taking the flags from the row's
`// Flags:` line. The oracle comes from the catalog. Then `objdump -dr $S/wine/q/<addr>/<addr>.obj` shows which symbol each relocation
binds. Binding pairs: tools/decomp_pipeline/reloc_identity.py `check()`.

## Hand-back (in $S/w22/agentFAM13/)
- rows/<addr>.cpp for every changed row, plus rows/<addr>_test.cpp if its test changed.
- hdr/ and hdr/README.md.
- RENAMES.tsv: section 1 header exactly `address\tmodule\tname\tmangled_leaf\told_catalog_name\told_leaf\tevidence`, for rows whose own
  name changes. Then a `# Section 2` line, followed by caller respellings as address, old_symbol, new_symbol.
- STATUS.tsv: address, change, grade, test, grades_used, note.
- APPLIED.tsv: each queue entry with done / partial / rejected and the count of binders respelled.
- REJECTED.tsv, NOTES.md, LESSONS.md (one line each).

Report briefly at the end: entries applied and rejected, rows changed, rows left and why, header opt-ins, and any rule slip.
