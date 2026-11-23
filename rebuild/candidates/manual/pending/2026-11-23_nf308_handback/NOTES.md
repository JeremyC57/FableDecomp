# NF308 notes (2026-11-23): T1 "other" class / namespace / form, continuation of NF305, hand-back 1

Base: the repo working tree as of today. All 10 orig/ copies are cmp-equal to HEAD at hand-back. Nothing was written under the repo.
"Entry n" means the n-th data line of ENTRIES.tsv (file line n+1).

## Summary (STATUS.tsv, 118 verdicts)
- **Entries reached:**
  - entries 1-104 in order;
  - pulled ahead under E2 (QUAL / SCOPE shapes from `tools/mdiff.py`, worked out in evidence/mdiff_all.txt): 105, 158-161, 163, 164, 179, 183, 191, 222, 223, 234, 237.
- **Verdict counts:** 5 done (13, 72, 90, 91, 92), 1 done-partial (191), 2 done-already (19 and 99, stale table lines), 1 done-equivalent (105),
  86 skipped-port, 23 skipped-held. No rejects. No header, test, flag or catalog-source-path change. No vtable change.
- **10 rows changed** (rows.txt), all in place:
  - `namespace X { decl } using X::name;` on the declaration's own line: 0x008CC8E0, 0x007FD580, 0x0064F6DC, 0x0064499C, 0x0049E8C0;
  - a 3-line namespace wrap of a one-member local struct: 0x007032F0;
  - access labels on the member's own line: 0x006EB390, 0x006EB600;
  - two owner self fixes: 0x006EA2C0 (access + full Leaf) and 0x009E9F20 (namespace wrap + description line + full Leaf).
- **Proof:**
  - HEAD and new objects are both RELOCATION_MATCH;
  - objsec ALL_EQ with only the listed rename (.drectve included) and COMDAT sets SAME (evidence/proof_b1/b2/b3/b3o.txt, cmpfn_b1/b2/b3.txt). The one emitted table (0x006EB390) is unchanged.
  - **Links, HEAD v new** (evidence/link_b1/b2/b3.txt). In each, every reference HEAD resolved stays resolved, every moved reference now resolves, and there are 0 duplicates.
    - link_b1: owner 0x0077C490 + 0x007032F0;
    - link_b2: 0x006EA2C0 / 0x006EB390 / 0x006EB600 + the PDB binder 0x006EA430;
    - link_b3: owners 0x009054F0 / 0x008F58C0 / 0x00643CA1 / 0x009E9F20, the five binders, and the PDB binders 0x00648820 / 0x0064D404.
  - vland `--recheck-landed`: 10/10 RELOCATION_MATCH + test PASS (recheck_final.log).
- **Table** (TABLE_CHANGES.tsv, 8 lines): 5 delete, 3 replace. `apply_table.py --dry`: 0 misses, no new multi-target symbol (evidence/apply_table_dry.txt).

## LAND TOGETHER
- **0x006EA2C0 with 0x006EB390 and 0x006EB600.** The owner's HEAD object defined the public `QAE` stand-in, and both binders resolved against it.
- **0x009E9F20 with 0x0049E8C0.** The owner `global_StopProgress` defined the global stand-in.
- **Catalog for 0x009E9F20:** Module `_global` should become `NProgressDisplay` (RENAMES section 1). The file name `global_StopProgress_009e9f20.cpp` can stay or be renamed.

## How the skipped verdicts were reached (evidence level)
- **Opened by hand:** 27 skipped entries (STATUS grade column says so): 12, 15, 41, 48, 49, 55, 61-71, 74-76, 78, 81, 88, 100, 158, 159, 179.
- **Census only (NF299 Q4 form):** the other 82.
  - Each was classified from its tracker stand-in spelling by `tools/mdiff.py` (shape: CLASS / FREE / SIG / anonymous hash / member template).
  - Then `tools/stalescreen.py` (evidence/stalescreen.txt) checked every (stand-in, witness) pair: it demangles the stand-in and confirms that the witness's HEAD source still carries every identifier of it, with no PDB-scope qualification for free functions.
  - Results: 140 OK; 13 misses, all opened.
- **What the 13 misses turned up:**
  - one more stale table line, entry 99 (0x00AE2CB0 already declares the NVectorMath PDB form; its HEAD object proves it);
  - two tracker spellings truncated onto PDB-form binders (55: 0x008D0610; 78: 0x008BC1A0);
  - the rest real stand-ins (global declarations beside a namespace that holds other members).
- **1-in-10 sample of the census-only entries, opened** (declaration line read, STATUS says so): 1, 11, 25, 35, 46, 59, 87, 103, 237. All are still the stand-in.
- **Not done:** the "binder includes a shared header that declares the PDB member" check from NF299 Q4. Most census binders declare the stand-in locally, which the stale screen confirms.

## Census (the table under-reports binders)
For every respelled target I listed every exe call site (tools/xsites.py, evidence/xsites_b1 / c1 / c2 / c3.tsv) and every absolute reference (absrefs.py: 0 for all seven targets). Then I read each caller row's declaration (callcensus.py) and compiled the doubtful ones.
- **No table line:** 0x0064499C (GFAnchorToTopRight) and 0x006EB600 (PerformSequenceReset) were found only this way.
- **Not compiled, read from source:** 0x00434F60, 0x004A5A40, 0x0088E970, 0x004A3740 (NF307 witness) and 0x0048E78E / 0x0064BFBA / 0x00654438 bind the PDB forms (qualified call or inside the namespace), as do the two PDB-line witnesses 0x004A2F10 / 0x0064CA15.
- **New-name census** (evidence/namecensus.txt; the filter now looks only at the text before the name, so one-line `{return ...}` bodies count): every definition of the six names is either the owner row or another class / another signature. That covers the 0x00C15E30 / 0x00C15EA0 SetRotationAngle stand-in classes, `float SetLungedThisFrame()` at 0x00C62350 and CProgressDisplay::StopProgress.
  - The pre-existing same-TU copies of NPlayerGui::GFAnchorToTopRight in 0x0064BFBA / 0x0064CA15 were already there at HEAD and already bound by the PDB-form callers. This batch adds no definition.
- **Includers by path:** none of the 10 changed sources is included by another landed source or test.

## Families and decisions worth reading
1. **LSocket family (entries 61-71), all skipped-port: hand to T3 as one unit.**
   - Every stand-in row uses a global local model of LSocket / LSocketServer / LAddress.
   - Most binders are LSocket members that define their own global stand-in self: TryToSendDataFromQueue, Bind, Attach, Listen, Open, Close, ~LSocket, RequestListener, CheckForAcceptedConnectionsOnSocket. Re-scoping them renames their own definitions (NF300 Q2).
   - 0x0090B080 (`LSocketServer::RequestListener(LAddress&)`) ties the ctor, the dtor and SetNonBlocking together. Moving LSocket alone puts its `Listen(LAddress&)` call on another wrong form (NF305 Q1). Moving LAddress or LSocketServer as well renames its own self. So the dtor owner self fix cannot take every old-spelling binder with it.
   - 0x00840070 (LHHttp2::Close) and 0x008410D0 / 0x0083FF80 / 0x0090AA40 already show the target state: engine/LSocket.h or a namespaced local model.
2. **Entry 15 (vtable decision, not compiled).**
   - 0x00606D20 is CTCInventoryMap's ctor, and its local CTCInventoryBase is the first of two polymorphic bases.
   - The vftable it emits is therefore named after that base (`??_7CTCInventoryMap@@6BCTCInventoryBase@@@`), and moving the base into NInventory renames the table. Stopped, as the brief says.
3. **Entry 19.** The prefix expected a source change for 0x00ED4830. The HEAD source already has CV_StatueMasterScript in NScript (naming wave 35), and both caller objects bind the PDB form. The table line is simply stale (deleted).
   - Precheck: clang fails on both unchanged sources only with MSVC-accepted `deleted function '~CStatueMasterChest' / '~CStatueMasterStatue' cannot override` (a private virtual base dtor). That is the NF300 D1 extension, not NF305 Q2: `cl /Zs` ZS_OK came in an earlier call (evidence/zs_h1.log, zs_h2.log).
4. **Entry 179 (held for NF307).**
   - The owner 0x0097E330 defines the public stand-in, so binder-only `protected:` fixes would unresolve what HEAD resolves.
   - The owner fix needs 0x009689C0 (an NF307 witness, public declaration) to move as well.
   - The three other respells are prepared in evidence/held_179/ (sources only) (graded RELOCATION_MATCH, objsec not run). They are not in rows/ and not in rows.txt.
5. **Entry 12 (held).** The only stand-in binder, 0x007CC120, also calls 0x004BFFC0 (T3_PORT_QUEUE identity research, ClearHighlighted v ClearMeshEffect) on the same local struct, so the re-scope would rebind that call onto a different form (NF305 Q1).
6. **Entry 105 (done-equivalent).** The tracker's "PDB spelling" is the VC10 Dinkumware `struct char_traits` (U). Retail STLport has `class char_traits` (V, stlport/fable_string.h), which 0x0083ECF0 binds.
   - 0x0083EBD0 binds the U form, so it is the odd one out (NAME_FIXES question).
   - 0x00777F80's non-template stand-in is a port.
7. **Other ports, in brief:**
   - anonymous-namespace entries 158-164: the hash follows the rebuild TU path (GOTCHAS 357 / 471 / 501), so they need a TU fold, not a respelling;
   - GetSubDef / FindMostRecentMessage member-template instances 55-60: NF303 Q2, .drectve;
   - entries 41 / 48 / 49 / 100: the re-scope renames the binder's own definition or emits its vftable.

## Open stand-ins left in changed rows (not this batch's entries)
- **0x007032F0:** SetScale on the global CTCGraphicAppearance (0x004BFA50; both PDBs NGraphicAppearance). Queued in NAME_FIXES.
- **0x0064499C:** GFACoordToVCoord (pointer parameters, global) is still a stand-in.
- **0x0049E8C0:** DisplayProgress keeps its HEAD declaration.

## Rule notes
- **Locks:**
  - Every compiled row was locked before its compile.
  - Every landed test was prechecked before its lock (pre_tests1..5). 0x0064F6DC's test was covered by `cl /Zs` (D1: clang fails only on `_assert`), zs_tests4.log.
  - Every source was prechecked after its lock and before its grade (pre_srcH1..7, pre_rows1..5).
  - **Slip:** rows/0064f6dc.cpp was first prechecked (pre_rows3) before I claimed its lock. I then claimed the lock, prechecked HEAD and new again in a later call (pre_srcH6, pre_rows4), and only then graded.
  - **Held now:** exactly the 10 changed rows. **Released:** 0x00ED4830, 0x00ED41D0, 0x0077C490, 0x009054F0, 0x008F58C0, 0x00643CA1, 0x00648820, 0x0064D404, 0x006EA430, 0x00967930, 0x00967B40, 0x00968BC0, 0x0097E330, 0x00AE2CB0.
- **Background:** every grade, `cl /Zs`, link and vland run went to the background with `timeout 900` on its launch line (tools/gr.sh, zsrun.sh, lk.sh, rf.sh) and was checked in a later call. No sleep.
- **Prohibitions kept:** no rm (nothing was deleted; the entry-179 drafts were moved within my folder), no header edit, no `__asm` / `naked` / `_emit`.
- **Git slip:** one tool call ran `git -C /dev/null status` by mistake (it failed: not a repository, and touched nothing). No other git command was run.
- **No retail or donor bytes in hand-back files:** a hex-run grep over every hand-back file and folder found nothing. The vland oracle stays in work/pay1.
- **Off-limits set:** tools/offl.py now builds it from NF306 / NF307 entries and witnesses, live locks (NF306's 18), unexpired codex-p28 reservations, held rows and T3_PORT_QUEUE. It was re-run before each lock batch. None of my 10 rows or the compiled owners is in it.
- **Reading:**
  - Read in full: the prefix, the standing brief, GOTCHAS.md, CLAUDE.md, and NF305's hand-back (NOTES, LESSONS, STATUS, APPLIED, RENAMES, TABLE_CHANGES, NAME_FIXES, recheck_final) with its landing README; EXTRA_RULES.md through the owner 2026-10-28 rule.
  - Not read: ASM_RULES.md (no asm here), COORDINATION.md, the nf281..nf285 landing READMEs.
- **tools/:**
  - NF305's set: offl.py retargeted to NF306 / NF307; srcof.sh / flags.sh already bounded to the row's own catalog block.
  - New: v308.tsv + mkstatus308.py (STATUS from verdict lines), hb308.py / hb308b.py (the exact edits), mkpay308.py, namecensus308.py.

## Note on GFAnchorToTopRight (entry 91)
0x0064CA15 (`#pragma auto_inline(off)`, inside NPlayerGui) and 0x0064BFBA each define a same-TU strong copy of NPlayerGui::GFAnchorToTopRight, beside the owner 0x00643CA1. These are pre-existing HEAD duplicates. This batch adds no definition, and the two respelled binders now bind the name those copies already carry.

## Questions
1. **LSocket family:** queue entries 61-71 as one T3 family port (owners 0x008F82E0 / 0x008F8870 / 0x008F8C70 / 0x008F9780 plus the member binders listed above)?
2. **Entry 15:** a re-scope that renames an emitted multiple-inheritance vftable (`6B<base>@@`) is a port, not a decision for each row?
3. **Entry 105 / 0x0083EBD0:** should the U (`struct char_traits`) table lines be respelled to the STLport V form? That is the opposite direction to the tracker.
4. **0x009E9F20 (entry 92) is the one change that renames a row's own definition by scope** and changes its catalog Module (`_global` to `NProgressDisplay`). NF300 Q2 excludes own-definition renames, so I relied on NF304 Q6 (owner PDB-signature self fix, bytes equal, every landed old-spelling binder, here only 0x0049E8C0, moving in the same landing). If that reading is wrong, drop 0x009E9F20 + 0x0049E8C0 + the `?StopProgress@@YIX_N0@Z` delete line as one unit; nothing else depends on them.
5. **Entry 179:** land the prepared three respells and the owner self fix once NF307 releases 0x009689C0, or hand them to NF307?
