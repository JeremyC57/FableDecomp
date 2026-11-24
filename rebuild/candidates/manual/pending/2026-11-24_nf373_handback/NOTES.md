# NF373 (Account A naming queue, 2026-11-24): NF370's nine held T2 self fixes, as units

Folder `$S/w22/agentNF373/` (S = the 40effb6c scratchpad). Lock name NF373. No repo write, no git, no header or test change.
Result: 7 units measured ACCEPT on every gate (22 rows in 3 land_scripted run dirs); 2 units skipped as owner-Q2 (strong same-TU copies).
None of the 7 lands with plain `land_scripted.py`: every unit has a member that a T3 line names in column 2 or 3 (table below).

## Method (tools/)
- **Stay-off screen** (`screen373.py` -> `evidence/screen.tsv`): every landed source naming a unit method (150 rows), flagged
  for Account B claims or row files, NF374 addrs, locks, and T3. Off-limits and not compiled:
  - NF374: 0x0099B560 (CWideString), 0x00692F30 (declares SetAnimation, never calls it);
  - Account B: 0x0058578B (shared header, UAE), 0x005A04F7 (comment only).
- **Census** (`census_compile.sh`): HEAD vland compile of the other 146 rows. `refscan373.py` lists every object that DEFs or UNDs
  the old or new spelling (`evidence/refscan_head.tsv`). `comdatsel373.py` reads COMDAT selection (NODUPLICATES = strong under /Gy).
  - Every object that references an old spelling is a member of its unit, except test objects (standalone) and the NF374 / B text
    cases above.
  - The four rows whose own symbol also moves were rescanned: no other object names their old or new spelling.
  - The census-only locks were released right after the scan.
- **Texts** (`edits373.py`: exact asserted replacements on orig/ = HEAD):
  - The self texts are NF370's run1 rows, except 0x005BD4EA, rewritten so the virtual takes the v30 placeholder (RTTI slot 30).
  - The binders get access labels.
  - Units 7 and 8 get `virtual` on the binder's local base model.
  - 0x0099F0A0 gets the nf305 port (below).
- **Prechecks:** clang test, HEAD source and new source for all 24 rows: 72 PASS (`work/pre.log`).
- **Proof** (`unitrun373.py`): vland --recheck-landed, HEAD and new, at ONE build path. Per row:
  - graded symbol: a self equals the new Leaf; a pure binder's is unchanged;
  - objsec (nf314) ALL_EQ under all of the row's renames;
  - DEF / UND sets equal under the renames (COMDAT sets SAME);
  - `??_7` / `??_R` lists unchanged;
  - grade MATCH / RELOCATION_MATCH PASS.
  The results are in the batch `evidence/CHECKS.tsv` and `objsec_*.txt`.
- **Pick-any** (`pkrun373.py` + pkcmp346 + `$S/tools/reachflip.py`): pk_all and pickany_vs_retail, HEAD v new, with the full self
  symbol (or the binder's graded symbol) as the leaf.
  - GATE PASS in all three runs, 0 reach flips.
  - The only DROPPED / ADDED entries are the three renamed pick-any copies (0x00692B30 SetMoveToPos; 0x0099ED10 / 0x0099EE20
    MakeOwnCopyOfData). They are EQ before and after, under both checkers.
- **Link** (`links373.sh`: nf.py link /FORCE, HEAD set v new set, plus HEAD census objects that already bind the PDB spelling):
  `evidence/link_u*.txt`.
- **Table** (`work/tc/*.tsv`): `apply_table.py --dry`, each batch alone and all together: 0 misses, multi-target 1 -> 1 (none new).

## Units
| unit | members (self first) | link HEAD -> new | batch / verdict |
|---|---|---|---|
| u8 0x006A4E90 | 006a4e90, 008374f0, 0083d070 | UBE unresolved x3 -> resolved, dups 0 -> 0; the self's CThing call QBE -> UBE stays unresolved (0x004C74B0 defines a stand-in) | b1 ACCEPT (ruling 0) |
| u9 0x00736590 | 00736590, 00736ad0 | AAE unresolved x2 -> resolved, dups 1 -> 1 (an unrelated GetDef pair) | b2 ACCEPT |
| u5 0x006D5260 | 006d5260, 006d5850 | IAE unresolved -> resolved, dups 0 -> 0 | b2 ACCEPT |
| u2 0x006929E0 | 006929e0, 0069fe60, 00692b30 | IAE resolved; the HEAD dup (00692b30 pick-any v the strong self) only renames, 1 -> 1 | b2 ACCEPT |
| u7 0x005BD4EA | 005bd4ea, 005f05e5 | UAE unresolved x3 -> resolved, dups 0 -> 0 | b2 ACCEPT |
| u3 0x00693030 | 00693030, 00693b30, 008430b0 | IAE unresolved x2 -> resolved, dups 0 -> 0 | b3 ACCEPT, held for ruling |
| u1 0x0099EAF0 | 0099eaf0, 0099f0a0, 0099ed10, 0099ee20, 0099f100, 0099f180, 0099f400 | IAE resolved; with HEAD 0x0099F690 added (link_u1b.txt), ??YCCharString@@QAEABV0@ABV0@@Z goes unresolved -> resolved (the port fixes that binding); HEAD dups (two pick-any copies v the strong self, one UnassignString pair) only rename, 3 -> 3 | b3 ACCEPT, held for ruling |
| u4 0x00B80AD0 | self only | LNK2005 QBE (self v 0x00B80FE0) -> LNK2005 IBE (self v 0x00B80F90) | skipped, owner Q2 |
| u6 0x0055FB80 | self only | dups 1 -> 2: new LNK2005 AAE (self v 0x00560D00) | skipped, owner Q2 |

Notes per unit:
- **u8** (NF299 Q3, NF303 Q1; b1 = the slot-25 variant, run tag b1v):
  - Every model now carries the virtual at retail slot 25. That is the edit NF290 entry 6 prescribed and rejected as a "table model
    change" (pending/2026-11-23_nf290_handback/REJECTED.tsv): replace v025 in each model.
    - **Self:** CThing's `virtual void v025()` placeholder becomes the PDB `virtual DrawGetEditDrawInfo` (RTTI CThing 0x01239ABC slot 25),
      and the non-virtual CThing stand-in declaration goes. CThingPhysical's declaration becomes `virtual` (an override, slot 25).
    - **Binders:** the `void* __vftable` stand-in becomes a v00..v24 placeholder run plus the virtual at slot 25. No offset moves.
    - The self's qualified call therefore moves too: ?DrawGetEditDrawInfo@CThing@@QBE -> UBE (both PDB sets).
      - It is unresolved before and after; 0x004C74B0's own self is a stand-in, ?..@CThing@@QAE_NPAUCEditContext@@...
      - The table line is replaced (b1 TABLE_CHANGES), witness 0x006A4E90.
      - No other census object names either CThing spelling.
  - The binders' own overrides go QBE -> UBE: both PDB name sets, plus RTTI CThingTrackNode 0x01272CE4 / CThingMarker 0x01245524
    slot 25.
  - NF313 Q3 (retyping a virtual-override self needs every landed vftable and binder of the old spelling in the same landing): there
    are none. Every landed source naming DrawGetEditDrawInfo was compiled, and no object names the old TrackNode / Marker spelling.
  - No `??_7` / `??_R` in any object.
  - The first form (NF370's: the virtual appended past slot 29 in the self's model) also passed every gate (run tag b2, evidence
    pkcmp_b2 / link_u8). It is superseded.
- **u7:**
  - Both models take `virtual void OpenInventory()` in the v30 placeholder. RTTI: NInventory::CTCInventoryBase 0x01253DBC slot 30 =
    0x005BD4EA; CTCInventoryAbilities 0x01256AD4 slot 30 = 0x005F05E5.
  - 0x005F05E5's own symbol goes QAE -> UAE. That is the PDB spelling (both sets). There is no table line for the old spelling.
  - NF313 Q3: every landed source naming OpenInventory was compiled except two Account B rows. Their text shows the shared-header UAE
    and a comment. The ~40 other rows that name CTCInventoryAbilities never declare OpenInventory. So no landed vftable or binder holds
    the old spelling.
- **u1, the 0x0099F0A0 port** (T3 nf305: "self port"):
  - The `extern "C" __fastcall CCharString_operator_plusequals(self, edx, rhs)` stub becomes
    `const CCharString& CCharString::operator+=(const CCharString&)`. That is ??YCCharString@@QAEABV0@ABV0@@Z: both PDBs, and the
    table already binds it to 0x0099F0A0 (witness 0x0099F690). No landed source defines it elsewhere.
  - First try: RELOCATION_MATCH PASS; objsec ALL_EQ under {stub -> PDB, MakeOwnCopyOfData QAE -> IAE}.
  - Its test is self-contained (it defines its own copy of the stub) and still PASSes; it was not rewritten.
- **u2 / u1 pick-any copies:** 0x00692B30, 0x0099ED10 and 0x0099EE20 carry in-class SELECT_ANY copies of the self body. They move with
  the unit (access label) and stay EQ.

## T3 lines (land_scripted holds an address named in T3 column 2 or 3, and `--release-t3` frees it only when every such line has that address in column 2)
| batch | line (tag, col 2) | names | closes? |
|---|---|---|---|
| b1 | nf290 0x006A4E90 (l.284) | self col 2; 008374F0 / 0083D070 col 3 | CLOSES (this exact virtual move) |
| b1 | access1 0x006A4E90 (l.2107) | self | CLOSES |
| b2 | nf370 0x006929E0 (l.3663) | self; 0069FE60 col 3 | CLOSES |
| b2 | nf295 0x006929e0 (l.383) | self; 0069FE60 col 3 | STAYS: 0x00856D30 base port (re-add from T3_READD.tsv) |
| b2 | nf288 0x006D5260 (l.118) | self; 006D5850 col 3 | CLOSES (its proposed edit) |
| b2 | nf289 0x006d5850 (l.164) | binder col 2 | STAYS: 0x008A80F0 stand-in (re-add from T3_READD.tsv) |
| b2 | nf289 0x00736590 (l.156) | self col 2 | STAYS: 0x00736AE0 CTCCombat_Methods (re-add from T3_READD.tsv) |
| b2 | nf302 0x005BD4EA (l.614) | self; 005F05E5 col 4 | 005F05E5 part CLOSES; STAYS for 0x005CCF86 / 0x005C2F27 (re-add from T3_READD.tsv) |
| b2 | nf311 0x00414EA0 (l.1547) | 005F05E5 col 4 (not reserving) | untouched (CCharStringVector stand-in remains) |
| b3 | nf295 0x00693030 (l.385) | self; 00693B30 col 3 | STAYS: 0x00856AC0 / 0x00857520 (re-add from T3_READD.tsv) |
| b3 | nf293 0x00693B30 (l.363) | binder col 2 | STAYS: CCreatureActionBase FAMILY UNIT, all-or-nothing -> RULING |
| b3 | nf305 0x0099EAF0 (l.1357) | self; 0099F0A0 col 4 | 0099F0A0 port CLOSES it; STAYS for the 0x0099F390 @CCharString_Detach@4 stub (re-add from T3_READD.tsv) |
| b3 | nf294 0x0099ed10 (l.339) | binder col 2 | STAYS: 0x009EB590 / LowerView binders (re-add from T3_READD.tsv) |
| b3 | nf309 0x009A0460 (l.1697) | 0099F180 col 4 (not reserving) | untouched |

Coordinator hand steps:
0. **Claim line:** close the open NF373 COORDINATION_CLAIMS line first, or land_scripted holds every member it names. Do not append
   NF370's QUEUE_PROPOSALS lines for these nine units (STATUS.tsv here supersedes them).
1. **b1:** close nf290 l.284 and access1 l.2107 by hand. `--release-t3` cannot free 0x008374F0 / 0x0083D070, because they sit in
   nf290's column 3. Ruling 0 first.
2. **b2:**
   - close nf370 l.3663 and nf288 l.118;
   - remove nf295 l.383, nf289 l.156 / l.164 and nf302 l.614 for the landing (or land by hand);
   - right after, re-add them from T3_READD.tsv, which keeps their open clauses and their original column-3 reservations.
   - Note l.164: 0x006D5850's OWN self, ?GetFollowers@CTCFollowed@@QAEXPAU?$CRetailVector..., is itself a stand-in of the PDB
     vector<CThing*>& spelling. Landing u5 changes that row while its own port stays queued (ruling 3).
3. **b3:** rulings 1 and 2 first; then the same re-add for nf295 l.385, nf305 l.1357 and nf294 l.339.
4. **TABLE_CHANGES:** a `delete` for each old-spelling line, because the PDB-spelling line to the same target already exists. The
   witness column holds the binder, so land_scripted keeps the line. Plus one `replace` in b1 (CThing QBE -> UBE).
   `apply_table --dry`: each batch alone and all together, 0 misses, multi-target 1 -> 1.
5. **After landing:** self_symbols / catalog follow RENAMES.tsv. Four binders also change their own symbol: 0x0099F0A0, 0x005F05E5,
   0x008374F0, 0x0083D070.

## Rulings wanted
0. **u8 / b1:** NF290 entry 6 rejected exactly this edit (replace the v025 placeholder in each model) as a "table model change". NF299
   Q3 / NF303 Q1 came later. Under the measured gates (objsec ALL_EQ, COMDAT sets SAME, no ??_7 / ??_R, offsets kept), does it land?
   If the slot-25 replacement is unwanted, the appended form (run b2) is the fallback; it equally passes.
1. **u3:** 0x00693B30 is the nf293 CCreatureActionBase FAMILY UNIT (all-or-nothing). May it take this access-label edit (objsec ALL_EQ,
   its own symbol unchanged) outside that family hand-back? If not, u3 waits for the family.
2. **u1:** 0x0099ED10 is col 2 of nf294, whose subject (binders 0x009EB590 / the LowerView view) is untouched. Land the label edit and
   re-queue nf294 on 0x009EB590? Also confirm the 0x0099F0A0 member port (its self symbol leaves the stub spelling).
3. **u5 / b2:** land 0x006D5850's access edit while its own nf289 line (its GetFollowers self spelling / 0x008A80F0) stays queued?
   Same question for 0x00736590 (nf289 l.156), 0x006929E0 (nf295 l.383) and 0x005BD4EA (nf302 l.614). Their lines' subjects are
   other rows.
4. **u4 / u6** (owner Q2, strong same-TU copies). The self fixes are proven (texts in `skipped/`), but 0x00B80F90 (IBE) and
   0x00560D00 (AAE) define the PDB symbol strongly. A unit would need those copies dropped, or the nf86 / NF310 D1 pick-any form.

## Extra gate evidence (evidence/)
- `comdatsel_and_oldscan.txt`: for every member, COMDAT selection of each external DEF, HEAD v new under the renames: SAME, 22 / 22.
  No old spelling is left in any new object.
- Shared sources: no changed Source serves two addresses; no landed source, test or header #includes a changed source.
- No repo write: `find -newer tools/units373.py` over the repo finds nothing; git status is clean (no git command beyond read-only status).
- Run-tag -> batch map:
  - run_b1 -> batches/b2 (u9, u5, u2);
  - run_b2 -> batches/b2 (u7), plus the superseded u8 form;
  - run_b1v -> batches/b1 (u8);
  - run_b3 -> batches/b3 (u3, u1), plus the u4 / u6 self-only proofs.
  So `pkcmp_b2.txt` inside batches/b1 is the superseded form; `pkcmp_u8_slot25.txt` is b1's own.

## Deviations (disclosed)
- The 146 census rows were locked before any test precheck and compiled (HEAD, read-only) without source prechecks. The unit members'
  prechecks (test, HEAD, new) were run after the locks, before the unit compiles.
- GOTCHAS.md and EXTRA_RULES.md were read by keyword grep plus the later EXTRA_RULES sections, not end to end. COORDINATION.md and
  ASM_RULES.md were read in full.
- 0x0099F0A0's test was not rewritten. It is self-contained and still exercises its own copy of the old stub body.

## Locks
NF373 claimed 146 census rows plus the controls and released them; the 26 unit / control locks were released at the end
(`evidence/locks.log`, `evidence/locks_final.txt`).
