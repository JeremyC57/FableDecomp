# Name-fix agent NF26: queue dh + di (8 entries), then FAM09's 97 alias respellings of the newly named globals

The user, 2026-11-11: naming goes one family per agent, and a new agent starts as soon as the last one lands.

**Part 1 -- QUEUE.tsv (8 entries, tags land1011dh and land1011di). They are claims: verify each one (two sources).**
- dh: 0x008780D0 (catalog label of an UNLANDED row -- only record it, nothing to respell), 0x005DA4C0 SetAimingTimeOverride,
  0x005DA490 IsAimedForLongEnough, 0x00658A35 IsSchoolTime and 0x00658A80 IsStoryTime (both landed rows are named IsCookTime --
  self-row renames onto engine/CTCVillage.h / CVillageDef.h like the landed IsWorkTime 0x00658B20), 0x00662410 IsInMeleeAsAttacker
  (62 call sites, many under made-up names such as IsRegisteredCombatant, CanAttackNow, IsCombatant).
- di: 0x0079C740 is CTCBulletTime::GetTimeMultiplier const (landed as `_unattributed::GetFloatThrough_0x20_0x12c_0079c740`; self row
  and its binders), 0x004FB150 is CWorldMap::GetLoadedRegion const (24 landed local models spell it GetCurrentRegionNumber).

**Part 2 -- FAM09_FIXES.tsv (97 lines from naming agent FAM09).** FAM09 gave the 399 `_static_init::g_*` objects their PDB names
(object_va, wrong_spelling, correct_name, defining_row, files, note). Landed binders outside that family still read those objects
under descriptive aliases: `g_EmptyWideString` and variants = `CWideString::NullString` (~50 files), `g_EmptyString` /
`g_EmptyCharString` = `CScriptThing::NullString` (~47), `g_DayOnlySuffix` / `g_NightOnlySuffix` (13 each), `ComboStrikes::*` =
the `STRIKE_EVENT_*` globals, `CAFile::CurrentPath` = `NFile::CurrentPath`, and more. Respell each binder to the PDB name, through the
shared header where one exists (e.g. engine/CWideString.h's NullString under its existing opt-in). FAM09 flagged some bindings at
those addresses as possibly unrelated (`kHomeMapName`, `g_7332d0`): check the address each binder really reads before respelling.
FAM09's NOTES (FAM09_NOTES.md here) also lists suspected mislabels: 0x0121A670 (`CThing::TCsToBeRemoved` vs
`NThingManager::UIDsToReplaceWithUID`) and the `_global::$E_InventoryOrientationRHSet*` rows (anonymous-namespace
`*_FORTY_FIVE_DEGREES_ORIENTATION` initializers) -- treat those as claims to verify, and settle the `CScriptThing::NullString` access
question (header default public, ORIGINAL_ACCESS private, Anniversary PDB protected) with the TLC PDBs.

**FAM09 is landing while you work.** Its 348 changed rows are in BUSY.txt (do not touch them). Your header overlay hdr/ STARTS as a copy
of FAM09's overlay (7 headers, documented in hdr_base_FAM09/README.md): keep those additions, put yours on top, and list in
hdr/README.md which headers you changed beyond FAM09's copy. Q20 (an author chunk) owns 20 rows, also in BUSY.txt.

Private third source (never committed): the Anniversary 360 Test PDB at $S/anniv_dbg/test4k.pdb (llvm-pdbutil works on it;
publics pre-dumped in $S/anniv_dbg/publics.txt). It only confirms a name that FableWin.pdb or Ego_r.pdb also has.

Repo: /home/user/FableDecomp (read-only for you). Scratch: $S = /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad.
Your folder is $S/w22/agentNF26/. Read CLAUDE.md and tools/decomp_pipeline/linux_lane/lane_tools/chunk_pool/EXTRA_RULES.md first.
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
4. **When the wrong name is a landed row's own name** (0x00658A35, 0x00658A80, 0x0079C740 here), respell that row too and list it in
   RENAMES.tsv section 1 (module = class, name = method only; never `Class::Method` in the name column).
5. **Rows reached through stand-in classes** (address-suffixed classes, `_unattributed`, `Row00c...`): respell them only if the correct
   class is proven and a shared header exists. Otherwise record the gap.

## Hard rules
- Never run git in any form, not even read-only commands. Never write under the repo. Never land anything.
- Every changed row must still grade MATCH or RELOCATION_MATCH.
- No `__asm` / `naked` / `_emit` / `register` / bare `__forceinline` in rows or tests. Use `volatile` only where retail used it.
- Use shared headers over local models, and add no NEW local models of classes that have a header.
- Header changes go in $S/w22/agentNF26/hdr/<path>: a full copy of the CURRENT repo header with `#ifdef` additions only. Grade with
  FABLE_HDR_OVERLAY=$S/w22/agentNF26/hdr.
- Use exact PDB names: one address, one name, and no made-up names.
- Do not touch the addresses in BUSY.txt (every RESERVED row, FAM09's 348 rows and Q20's 20) or any pending/*/RESERVED.tsv address.
- Run at most one compile at a time (another agent may compile in parallel), inside `timeout 240`. Wine cl can hang on `#error`.
- Make at most 10 grades per row, counting failed compiles.
- Every Bash call must exit 0: end probe, grep and diff pipelines with `; true` (user rule). A grading script prints its result as text.

## Lane
Every Bash call: `export S=...; source $S/wine/env.sh; export S; cd /home/user/FableDecomp`.
Grade: `timeout 240 python3 $S/tools/q.py <addr> <file.cpp> '<full mangled leaf>' [--flags ...]`, taking the flags from the row's
`// Flags:` line. The oracle comes from the catalog. Then `objdump -dr $S/wine/q/<addr>/<addr>.obj` shows which symbol each relocation
binds. Binding pairs: tools/decomp_pipeline/reloc_identity.py `check()`.

## Hand-back (in $S/w22/agentNF26/)
- rows/<addr>.cpp for every changed row, plus rows/<addr>_test.cpp if its test changed.
- hdr/ and hdr/README.md.
- RENAMES.tsv: section 1 header exactly `address\tmodule\tname\tmangled_leaf\told_catalog_name\told_leaf\tevidence`, for rows whose own
  name changes. Then a `# Section 2` line, followed by caller respellings as address, old_symbol, new_symbol.
- STATUS.tsv: address, change, grade, test, grades_used, note.
- APPLIED.tsv: each queue entry with done / partial / rejected and the count of binders respelled.
- REJECTED.tsv, NOTES.md, LESSONS.md (one line each).

Report briefly at the end: entries applied and rejected, rows changed, rows left and why, header opt-ins, and any rule slip.
