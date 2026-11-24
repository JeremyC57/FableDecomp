# NF379 (Account A, 2026-11-24): option S tool patches, and the next S units

Folder `$S/w22/agentNF379/` (S = the 40effb6c scratchpad). Lock name NF379. No repo write and no git. The repo was checked untouched with
`find -newer evidence/repo_marker_start` (no files) after the first test pass, after the land simulation and at the end.

## Result
- **Job 1 (tools): patches (a), (c), (d) and (e) are tested.** Unified diffs against the repo's current files are in `patches/`, with the
  full log in `patches/TEST_LOG.md`.
  - NF376's negative run is held and its sources are restored (hash-checked).
  - Two-column and `note`-column UNITS.tsv runs, and a non-S unit run, behave byte-identically to the unpatched tool.
  - A positive S unit lands.
  - An owner-only edit that breaks an includer is held and restored. The unpatched tool lands it.
  - (b) was not done, as briefed.
- **Job 2:** two S units are built as land_scripted run dirs (`batches/b1`, `batches/b2`). Both land with the patched tool in a scratch root
  (`evidence/landsim.log`). One member is held for a ruling (`held/b1_0044c1bb`). Three candidates are skipped with evidence.

| run dir | unit (owner first) | verdict | what changes |
|---|---|---|---|
| b1 | owner 0x0045C5F6 (unchanged, not in the run); thin 0x00455EF0, 0x0045C5DA | ACCEPT 2 | The two NF363 option A copies of the CThingNoiseDef def TU become thin includes of the owner, so a link has one strong `?Alloc@CThingNoiseDef`. |
| held/b1_0044c1bb | thin 0x0044C1BB | HOLD (NF379-Q1) | Same as b1. Condition (2) is attested by FableWin.pdb only. |
| b2 | owner 0x0094FBB0; thin 0x0094FDF0, 0x00951FB0 | ACCEPT 3 | The ai_state_group_dragon.obj slice IsTargetOnATower / IsStillRunnable / UpdateHover in one TU (flags /d2inlT59). 0x0094FBB0's self goes QAE -> PDB AAE (the NF309 / NF370 held T2 fix). |
| (skip) | 0x00749810 / 0x00749A70 CTCNoise | not S | Condition (3) fails, and the owner text cannot emit the leaf (NF369). |
| (skip) | 0x0099EA60 / 0x0099F9B0 (+ 0x0099F690) | not S now | Condition (3) fails: the in-TU AllocStringData is NEQ. |
| (skip) | 0x008F87A0 / 0x008F8BB0 / 0x008F99A0 LSocket | not S now | Condition (3) fails (the in-TU copies are NEQ), and T3 nf308 reserves the family all-or-nothing. |

## Job 1: tool patches (`patches/`)
- **(a) land_scripted.py, with (d) verify_and_land.py:**
  - UNITS.tsv `kind` is read **by header name**. A third column headed `note` stays free text: the T3n test puts `S` in a note column, and it
    lands exactly as unpatched.
  - An `S` member is never a binder, so its graded symbol must equal RESULTS `new`.
  - S members grade in a separate vland call with `LANDVERIFY_STRICT_LEAF=1`. Non-S rows keep today's call byte for byte, and includers get a
    third call.
  - (d) lives in `parity_of` only. It returns `LEAF_MISMATCH(<graded>)` when the graded symbol is not the leaf, and an anonymous-namespace
    prefix `?A0x` is allowed. Funclet extractors are exempt.
  - `main()` skips the 24-compile pragma / flag sweep on `LEAF_MISMATCH`: T6 took 1 compile instead of 25.
- **(c) naming_status / recheck, wired into landing:**
  - `owner_source()`: a thin row resolves to its include target.
  - `includers_of()`: transitive, and it counts **fold** rows that include another row's file and add code of their own (0x00BEB490).
    NF376's version did not count those.
  - `recheck.py --with-includers`; with no flag its output is byte-identical to today's.
  - land_scripted re-grades every landed file's includers outside the run. A failing includer holds and restores the row it includes, and that
    row's unit (T4). An `Option S` includer must also grade its own Leaf.
- **(e) `coordinator/s_unit_check.py`:**
  - It checks owner, flags / compiler, full leaves, retail address order of the explicit bodies, nested scopes and operators.
  - Compiler-emitted members (??_G, implicit or inline ??0 / ??1, templates) get an `implicit` status. With `--objdir` they must be DEF
    symbols of the unit object; without it they are counted `not verified`. NF376's version failed ??_G and implicit members.
  - It exits 1 on a failure.
  - On the repo (read-only) it reports 1 unit OK. Its negatives are listed in TEST_LOG.

## Job 2: units

### b1: the CThingNoiseDef def TU (owner 0x0045C5F6)
- **Conditions**
  - **(1)** Four landed objects define `?Alloc@CThingNoiseDef@@SIPAVCDefClassBase@@XZ` strongly: 0x0045C5F6 plus the NF363 option A rows
    0x00455EF0, 0x0044C1BB and 0x0045C5DA. That function has its own retail address, 0x0045C5C1, so the HEAD link gives 3 LNK2005
    (`evidence/link_u1_*.txt`).
  - **(2)** Both PDBs put Alloc, ??0CThingNoiseDef, the scalar deleting destructor and ??1CThingNoiseDef in definition_manager.obj:
    - Ego_r #2046 / #624 / #2047 (proc "scalar deleting destructor", public `??_E`; egor_pdb_names lists `??_GCThingNoiseDef`) / #631;
    - FableWin #384 / #385 / #397 / #398.
    - ??0CTCNoiseDef is in FableWin definition_manager.obj #386 only; Ego_r.pdb has no out-of-line copy anywhere. That is NF379-Q1.
  - **(3)** One compile of the owner text, default flags, grades all four leaves RELOCATION_MATCH with the leaf symbol exact
    (`evidence/u1_sgrade_head.txt`). In pk_all the Alloc copy is `EQ 0045c5c1 strong` and every copy with an address is EQ.
- **Form**
  - The owner text and code are unchanged, so the owner is NOT in the run. T3 nf334 (column 2 0x0045C5F6) is an open port line whose open
    items this unit does not close; editing the owner would need `--release-t3 nf334` plus a re-add.
  - A comment-only owner update describing the unit is in `batches/b1/owner_comment_proposal/`, for when nf334 is worked. That
    comment-owner text also graded 3/3 (`evidence/u1_grade_root2_commentowner/`).
  - Each thin row keeps its description, `Retail:` and `Identity` lines, its own first-line `// Leaf:`, an `Option S unit` paragraph and the
    `#include`. Its test is unchanged.
- **Gates**
  - **Grades.** vland `--recheck-landed` strict (in the land simulation): 00455EF0 and 0045C5DA RELOCATION_MATCH PASS. The warnings are
    HEAD's: the STANDIN_LEAF catalog-spelling gap noted in landing nf363, and the RELOC_NAME label on 0045C5F6.
  - **Pick-any.** Both checkers, HEAD v new, with `--leaf` and the catalog length: 5 EQ / 6 NOADDR / 0 NEQ in every object, pvr 0 NEQ,
    reachflip `pairs 3 / 2 NEQ reach flips 0` (`evidence/u1_pk*`).
  - **Link** (/FORCE, one object per TU, census 0x0044C72B Alloc binder, 0x00749810, 0x00749A70):
    - HEAD 4 dups (3 Alloc plus the known ??0CTCNoise pair);
    - b1 (3-member): 2 dups (Alloc via the held 0x0044C1BB, ??0CTCNoise);
    - with 0x0044C1BB as well: 1 dup (??0CTCNoise only);
    - unresolved: 148 in every set, none new.
  - **Census for old spellings:** none needed, because no symbol changes name.
  - **Drop test** (`evidence/drop_tests.txt`): the DEF set of the HEAD text against each new thin object shows 0 symbols gone and 0 added.
  - **Link scope:** `nf.py link` counts only the unresolved names that match its watch regex (`NoiseDef|CTCNoise|Alloc`). The drop test
    covers the rest.
  - **Table:** `apply_table.py --dry` on the empty TABLE_CHANGES gives `{} misses 0`.
  - **s_unit_check:** OK with implicit members, and with `--objdir` it reports `DEF ok`.
- **T3:** b1 closes no T3 line. nf355 0x0045C5C1 (the Alloc same-TU unit) is closed in substance, but its column-2 address has no row:
  close it by hand on landing if the coordinator agrees. nf359 0x0044C1BB / 0x0044F71B (0x00749810's NEQ CTCNoiseDef pick-any copies) are
  pick-any residue, not S, and stay open.

### b2: ai_state_group_dragon.obj slice (owner 0x0094FBB0)
- **Conditions**
  - **(1)** HEAD 0x00951FB0 defines `?IsTargetOnATower@..@@AAE_NXZ` and `?IsStillRunnable@..@@AAE_NXZ` strongly. pk_all shows both
    `EQ strong` at 0x0094FBB0 / 0x0094FDF0 (`evidence/u2_pk/H_00951fb0.pkall.tsv`). The first blocked 0x0094FBB0's T2 self fix QAE -> AAE
    (T3 nf309, NF370); the second is a HEAD LNK2005 with 0x0094FDF0.
  - **(2)** Ego_r ai_state_group_dragon.obj #4 / #6 / #22; FableWin #92 / #1 / #61.
  - **(3)** Under /d2inlT59 (UpdateHover's flags) both copies are EQ at their own addresses, as above.
- **Unit and catalog**
  - The unit grew to include 0x0094FDF0 (IsStillRunnable) once the HEAD object showed its strong EQ copy. 0x0094FDF0 was not on the task's
    list; it was screened clean (no other agent, B, T3 or lock) before its lock was claimed.
  - The owner file (0x0094FBB0, the lowest address) holds 0x00951FB0's text. The only code change is that IsTargetOnATower moves ahead of
    IsStillRunnable, putting all three bodies in retail address order.
  - CATALOG_EDITS.tsv: 0x0094FBB0 and 0x0094FDF0 get CompilerFlags `/O2 /Oy /W3 /d2inlT59` (one object, one flag set). land_scripted does
    not apply catalog edits: `tools/catalog_edit379.py <catalog> batches/b2/CATALOG_EDITS.tsv [--dry]` does, and checks the old value.
    `--dry` passes on the repo catalog.
  - RENAMES.tsv: self 0x0094FBB0 QAE -> AAE, plus self_symbols lines for the three addresses.
- **Gates**
  - **Grades.** One owner compile grades all three leaves RELOCATION_MATCH with exact leaf symbols (`evidence/u2_sgrade_owner.txt`). vland
    strict in root2 and in the land simulation: 3/3 RELOCATION_MATCH PASS.
  - **Pick-any.** HEAD v new: 0 NEQ, pvr 0 NEQ, reach flips 0. The `EQ_AMBIG 2` in the small-leaf views is a leaf-perspective artifact:
    the same object viewed from UpdateHover shows 18 EQ.
  - **Link** (census 0x00951D60, 0x00950CF0, 0x00951070, 0x00951430, 0x00952890; every landed row naming the JackDragon
    IsStillRunnable / IsTargetOnATower / UpdateHover):
    - HEAD 6 dups, new 5. The IsStillRunnable pair 0x0094FDF0 / 0x00951FB0 is gone.
    - No IsTargetOnATower dup appears after the AAE rename. HEAD's QAE self only hid that dup.
    - Unresolved JackDragon names: 10 HEAD = 10 new.
    - The 5 dups left are pre-existing: IsStillRunnable strongly in 0x00950CF0 / 0x00951070 / 0x00951430 / 0x00952890, and a
      GetSubDef<CJackDragonDef> strong copy in 0x00951D60. They are queued as a unit extension (QUEUE_PROPOSALS).
  - **Census for the old spelling** (`?IsTargetOnATower..QAE`):
    - by text, only the self and 0x00951FB0 name IsTargetOnATower;
    - by compile (`evidence/drop_tests.txt`), neither the unit object nor any of the five census objects DEFs or UNDs the QAE spelling (the
      unit object DEFs AAE);
    - no reloc_bindings line binds QAE, and the header declares it private (AAE).
    No table change. `apply_table --dry` gives `{} misses 0`.
  - **Drop test:** the union of the DEF sets of the three HEAD objects against the unit object. Gone: only
    `?IsTargetOnATower@..@@QAE_NXZ` (the intended rename); added: nothing. No census object's UND set needs a gone symbol.
  - **Link scope:** `nf.py link` counts only the unresolved names that match its watch regex (`JackDragon`). The drop test covers the
    rest.
  - **EQ_AMBIG:** the two lines are one pick-any copy,
    `?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@...`. It is EQ at both of its table addresses (0x0040F020, 0x006804C0); the
    small leaves cannot pick one through `a`. It is not a NEQ.
  - **Q3 facts** (`evidence/q3_pk`, pk_all on the HEAD census objects). The IsStillRunnable copies:
    - 0x00951070: strong, EQ at 0x0094FDF0, so it can join the unit;
    - 0x00950CF0: pick-any, EQ;
    - 0x00951430 and 0x00952890: strong and **NEQ** (58 v 55) at 0x0094FDF0, so condition (3) fails for them until their copies are fixed.
    - 0x00951D60's strong GetSubDef<CJackDragonDef> is EQ at 0x008E3300.
- **T3:** closes nf309 0x0094FBB0. `--release-t3 nf309` frees it, as the land simulation showed (`t3 closed: 1`).
- **Landing order (b2):**
  1. Apply `batches/b2/CATALOG_EDITS.tsv` first (`tools/catalog_edit379.py <repo catalog> batches/b2/CATALOG_EDITS.tsv`). Without it,
     0x0094FBB0 and 0x0094FDF0 compile without /d2inlT59. That is unmeasured, and land_scripted does not check flags.
  2. Close the NF379 claim. Its open line holds 0x0094FBB0 / 0x00951FB0, as the first land-simulation try showed.
  3. Run `land_scripted.py <b2> <batch> <tag> --release-t3 nf309`.
  4. Run `s_unit_check.py`.
  5. Add the RENAMES self_symbols lines.
  b1 needs none of these: land it with land_scripted alone.

### Skipped
- **0x00749810 / 0x00749A70 (CTCNoise)**
  - Condition (1) holds: 0x00749A70 defines `??0CTCNoise@@QAE@AAVCThing@@@Z` strongly beside the 0x00749810 self.
  - Condition (2) is half shown: ??0CTCNoise is in tc_noise.obj in both PDBs, ~CTCNoise in FableWin only.
  - **Condition (3) fails.** HEAD 0x00749A70's in-TU ??0CTCNoise is `NEQ 00749810 strong 59 v 79` (`evidence/ctcnoise_pk/`).
  - The owner text (0x00749810's) emits no `??1CTCNoise` on the shared header (NF369: `virtual ~CTCNoise();` is user-declared). So an S
    thin row would have no leaf without NF369-Q1's header device (b).
  - 0x00749810's NEQ ??0CTCNoiseDef / ??_GCTCNoiseDef pick-any copies are pick-any residue: not S.
- **0x0099EA60 / 0x0099F9B0 / 0x0099F690 (CCharString::AllocStringData)**
  - 0x0099EA60 is the `PBDJ` overload: catalog 124 B, Ego_r lib_char_string.obj #20.
  - HEAD 0x0099F9B0's in-TU `?AllocStringData@CCharString@@IAE..PBDJ@Z` is `NEQ 0099ea60 strong 78 v 124`, so **condition (3) fails**. The
    same object also holds a strong EQ `??0CCharString@@QAE@PBDJ@Z` at 0x0099EBF0, a further unit member if this is ever S.
  - 0x0099F690's AllocStringData is an in-class pick-any definition: binder residue, not S.
  - The self row has no Leaf line and a local model; it needs a port before any S form.
- **0x008F87A0 / 0x008F8BB0 / 0x008F99A0 (LSocket)**
  - HEAD 0x008F99A0's in-TU copies are NEQ at their addresses: CreateSocket `111 v 138`, ConvertSockaddr2LAddress `70 v 71`, plus
    ResolveHost 0x008F86A0 and the Receive overload 0x008F9320 (`647 v 674`, leaf-reached). So **condition (3) fails**.
  - The /GS difference (0x008F87A0's catalog flags) probably accounts for 111 v 138.
  - T3 nf308 reserves 0x008F87A0 / 0x008F8BB0 in an all-or-nothing LSocket family unit.
  - Gate only (`evidence/u4_pk/`).
- **0x00645292:** not touched (OFF, Account B's partner row).

## Rulings needed
- **NF379-Q1 (0x0044C1BB into unit b1).** FableWin.pdb puts the out-of-line ??0CTCNoiseDef in definition_manager.obj (#386); Ego_r.pdb has
  no out-of-line copy anywhere, so it neither confirms nor contradicts.
  - Every other gate passes: the leaf is EQ from the owner object, the pick-any checkers are clean, and the 4-member link has 0 Alloc dups.
  - If the ruling accepts one-PDB attestation for a compiler-emitted inline COMDAT, add `held/b1_0044c1bb/rows/0044c1bb.cpp` to b1 (RESULTS
    ACCEPT, the UNITS line in `UNITS_line_if_ruled.tsv`; head payload in `held/b1_0044c1bb/head/`).
- **NF379-Q2 (owner choice).** b1's owner is 0x0045C5F6, not the lowest address (0x00455EF0 in b1, 0x0044C1BB with Q1). It is the file
  whose text already is the TU; with it as owner no graded text changes and no owner edit is needed, which keeps T3 nf334 unaffected.
  NF376's rule (e) said "lowest-address member's file". s_unit_check does not require that.
- **NF379-Q3 (b2 partial slice).** b2 absorbs only the strong duplicates that block the T2 fix. The same retail TU still has four other
  strong IsStillRunnable definers. Land the slice now (dups 6 -> 5) and extend the unit later (queued)? Or require the whole
  ai_state_group_dragon.obj slice at once? That would be a multi-row merge of differently opted-in texts.

## Deviations (disclosed)
- **Locks and land dirs written to the real S.** The first test-runner pass ran with the real S for locks and `land_` dirs. A sourced env
  script saw the runner's `$1`, so the scratch switch was off; it is fixed by `NF379_MODE`.
  - In that pass land_scripted wrote `COORD` into `$S/w22/nflocks/{00b80ad0,00736590,00736ad0}` and its clean step removed those lock dirs.
    None held a live lock, or land_scripted would have held the row. A missing lock reads as free, so the semantics are unchanged, but any
    `released` marker files there are gone.
  - Six `$S/land_T*` dirs were created. I moved them into `work/stray_land_dirs/`.
  - The repo was untouched (checked).
- **0x00952890 and Account B.** 0x00952890 (UpdateGround) is a row file of Account B's package `pending/2026-11-11_acctB_c2009`, and
  my screen of 0x0094FDF0 printed hits from that package. No open B claim or UNMATCHED line names 0x00952890, and its nflocks entry read
  `released`. I locked it, prechecked it and compiled it read-only as a link-census object, and QUEUE_PROPOSALS names it for a later unit
  extension. The coordinator should confirm that this is A's to touch. Nothing of B's was edited. (The NOTES line above that says
  0x0094FDF0 was screened clean refers to its own row: the acctB hits were inside 0x00952890's package text.)
- **Deletes.** I removed scratch outputs in my own folder with `rm -rf` / `rm -f` (test batch dirs, two orig files) instead of
  naming_batch/clean.py.
- **Precheck.** Clang precheck fails only inside VC7.1 headers for some files, as in earlier agents: the tests of 0x0044C72B, 0x008F99A0
  and 0x00951FB0 (the last passes with nowchar). It also fails on 0x008F99A0's own winsock `extern "C" ... FABLE_WSAAPI` lines. All of these
  rows grade under VC7.1 (`gr_*.log`).
- **HEAD objects for b1.** The HEAD texts of the four NoiseDef rows are code-identical (NF363), so the HEAD object of each is one compile of
  that text (`work/sg/u1_head`). That compile was copied for the link set.
- **Compile budget.** About 175 VC7.1 compile invocations, over the 120 asked: the tool tests took about 150, including two sweeps of 25,
  and the units about 25.
- **Reading.** EXTRA_RULES.md and GOTCHAS.md were read by keyword grep (same-TU, pick-any, option S), not end to end. COORDINATION.md and
  the ASM_RULES rules section were read.

## Locks
NF379 claimed:
- unit locks 0045c5f6, 00455ef0, 0044c1bb, 0045c5da, 0094fbb0, 0094fdf0, 00951fb0;
- census / gate locks 0044c72b, 00749810, 00749a70, 00951d60, 00950cf0, 00951070, 00951430, 00952890, 0099f9b0, 008f99a0.

All were released at the end (`evidence/locks.log`, `evidence/locks_final.txt`). Tests were prechecked before each claim, sources after it.
