# NF466 (2026-11-26): speed-up C1 in bulk (access letter / const onto the one PDB symbol) + 4 WatchFor* binders

Scratch: `$S/w22/agentNF466`. Repo read only (no git; oracle md5s identical before/after every sandbox run, `evidence/sandbox_all.txt`).
Tools (`tools/`): `pool.py` (a9.py adapted: writes the pool), `excl.py` (T3 col 2/3, every 8-hex address in NF465's T1_pool.tsv, live locks),
`census466.py` (retail refs: E8/E9/0F8x rel32 + absolute, all targets one exe pass -> landed rows), `hscan.sh` (vland HEAD compile of a row list),
`xr466.py` (per caller object: the symbol bound at relocations resolving to the target), `mkplan.py`, `af466.py` (access_fix.py edit logic driven by a
plan; several edits per row; access/const only, never virtuality/static; nested scopes allowed), `fixpt.py`, `runpair.sh` (NF464, env + -I),
`gate466.py` (objsec ALL_EQ under the row's renames + full retail-caller census; a caller whose object holds ANY other spelling with the
target's `?name@scope@@` prefix is BAD), `fin466.py` (grade/test/WARN/objsec/census/per-unit link -> RESULTS, CHECKS, SELF_SYMBOLS),
`mkb466.py` (batch layout), `shadow466.sh` (shadow tree with the new sources + tables: apply_table dry/apply, SELF_SYMBOLS, descriptive_check,
naming_status --out/--dump-rec), `wf466.py` (NF464 wfedit.py, several callbacks per binder), `recdiff453.py`.

## Pool
a9 logic on `$S/ns_464/rec.json`: 267 access_letter/const addresses (195 + 72; 766 virtuality not taken). Dropped 49: T3 queue 27, lock CODEX29 17,
NF465 pool 5 -> 218 (`work/pool_c1.tsv`). Census: 385 rows compiled at HEAD; 36 targets had retail callers the table does not list.

## Batches (header-free; NF464 layout; HEADERS.tsv empty)
| batch | rows | targets | verdict | tracker (shadow dry run) |
|---|---:|---:|---|---|
| c1a_access | 34 | 26 | ACCEPT x36 lines | 16 agree_unverified -> agree_pdb, 10 split_unknown -> split_fixable |
| c1b_access | 40 | 31 | ACCEPT x42 | 22 -> agree_pdb, 9 split_unknown -> split_fixable |
| c1c_access | 25 | 19 | ACCEPT x27 | 9 -> agree_pdb, 10 split_unknown -> split_fixable |
| bWF_watchfor2 | 4 (+1 held) | 4 (+2 held) | ACCEPT x4, HOLD 00f09640 (r466a) | 1 agree_unverified -> agree_pdb (00e1f520), 3 split_fixable -> agree_pdb |
Cumulative shadow (all four, re-run on the repo as of 03:40 after a coordinator table change; baseline rebuilt): 48 agree_unverified -> agree_pdb,
29 split_unknown -> split_fixable, 3 split_fixable -> agree_pdb; no address worse; the retail census of the accepted targets re-run on the current
build_candidates.ps1 is unchanged and no changed source is a census caller or a source-scan row
(`evidence/TRACKER_EFFECT_all.tsv`). split_unknown -> split_fixable: the self is a stand-in / other name (not respelt here); its callers now bind the PDB name.

Gates per batch (evidence/): vland --recheck-landed HEAD v new at one path, grade + test unchanged for every row; WARN set identical or smaller
(after mapping the renames); objsec (nf314, every non-debug section incl. .drectve) ALL_EQ under renames.tsv for every row, and every self's new object
defines the PDB symbol and not the old; fullcensus.tsv 0 BAD (every retail caller row, new object if in the batch else HEAD object); per-unit link
HEAD v new (link_units.txt): dup never up, unresolved never up (callee-only units where no object defines the target swap old -> PDB in the set, r466c);
shadow: apply_table --dry 0 misses, multi-target 1 -> 1, descriptive_check 2,030 lines 0 FAIL, clash 0 (53 self symbols + every table new spelling).
After the gates, the `// Leaf:` line af466 had put above the line-1 description in 19 self rows was moved to line 2 (comment order only;
recompiled 006b7510 / 00557860 / 0053d540: objsec ALL_EQ against the gated objects, grades unchanged; af466 fixed). One test edit: 0x00892F70's VC7.1 test stub of CQuestManager::IsQuestFailed takes `const` (and its observed-this pointer `const CQuestManager*`),
else LINK_FAIL / TCC_FAIL (c1b_access/tests/00892f70_test.cpp). Names: every target spelling is in both Ego_r and FableWin.

## HOLD / not taken (C1_DISPOSITION.tsv, all 267)
ACCEPT 76; dropped 49 (above); HOLD 142:
- 13 virtual targets (U -> M/E access) with no line in labels_rtti_port.tsv: held for the RTTI-placement check access_fix --wide requires
  (0053d540 0053d6c0 0053d970 0054dc30 0054dcc0 0054de70 00557860 00557880 00557af0 00723930 00736ad0 007eeb20 00ef5cb0; all self-only,
  objsec ALL_EQ and grade/test unchanged: they land once the placement is proven);
- 38 a retail caller binds a third spelling (stand-in, other params): not an access edit (T1/T3 work);
- 23 a unit member is reserved (T3 queue / NF465 pool / lock);
- 22 the declaration is virtual or static (U<->M/E access, S<->K): needs the RTTI-placement check; next increment of af466;
- 12 SRC_FAIL after the edit (10 rows; not diagnosed per row: the two read are const selves whose body calls non-const local-model members);
- 8 the declaration does not start its line (af466 parser); 4 no local class (header edit -> one `_hdr` batch); 1 no row emits the old spelling;
- 6 identity: the PDB spelling is already bound/defined at another address; 5 identity: the OLD spelling is shared by 2-3 addresses
  (BoughtSomething x2, GetMemoryFootprint@CMap, 0x009EA620/640: which address owns the PDB name needs proof);
- 3 source scan (below): 0x008527F0 / 0x00A67420 (another landed object also DEFINES the old spelling: 0x008529D0, 0x00A67770) and 0x009D5410
  (0x009D5BF0 already DEFINES the PDB spelling: the respell would add a duplicate);
- 5 one-PDB-set (r466b); 2 census found a caller the table misses that cannot follow: 0x00BF6530 (0x00BF8810 via a shared candidates header) and
  0x00BF65B0 (0x00BF8810 binds `K` where the PDB has `J`). 0x006B7510's missed caller 0x006B7760 joined its unit.
Source scan (advisor, after the census): every landed source naming both the member and the class of a self respell (188 rows, 291 objects,
HEAD) checked for the old or the PDB spelling (evidence/srcscan_selves.tsv): 0064d7a6 / 008728c0 / 00ef1c50 / 00ef1cf0 / 00ef1d10 already
reference the PDB spelling (they resolve once the self is respelt); the three definers above held their targets. Includers of all final rows:
none (recheck.py --with-includers). The one NOREF census line (006b7510 <- 006b7760) is OK by the caller's symbol table (it binds the PDB
spelling outside its leaf section).
Gotcha: the census by retail references must also scan each caller object's whole symbol table for the `?name@scope@@` prefix: 0x006B7760 calls
Clear outside its leaf section and 0x00BF8810 is a DIFFER row, so the relocation-offset method alone reported NOREF for both.

## WatchFor (second job)
bWF_watchfor2: 0x00D167C0 Hostage1Killed, 0x00D16BB0 Hostage2Killed, 0x00E1F3E0 BanditDead, 0x00E9DAC0 Attack respelt onto CScriptThing (NF464 bW
pattern; objproof_keyed ALL_EQ incl. the consequential functor ctor rename; census 0 BAD; functor-set link dup 3 -> 3, unresolved 195 -> 192).
Held: 0x00F09640 (r466a). Three targets are T3 column-2 addresses (nf301/nf326 0x00D16920, 0x00D16D00; nf374 0x00E9DC80): the binders are not
reserved and the brief assigned the job, so the batch keeps them with T3_RESOLVED.tsv (r466d, as NF464 bQ / r464d). Not done (need a callback CLASS change, not only the parameter): WatchForDeputy2Killed (0x00D169E0 binds it on a local
CQ_AmbushTradersScript; PDB is NScript::CQ_BountyHuntScript) and the CQ_AcolyteParentScript pair (0x00E6AAC0 / 0x00EDC720; PDB CV_ChapelOfEvilScript /
CV_TempleOfLightScript; 0x00E6AF20's self also lacks NScript).

## Resume
0. The 13 virtual targets: prove the RTTI placement (vtable slot -> address) and land them as gated (rows in work/gen5/rows).
1. Rulings r466a / r466b; then the 22 virtual/static-declaration targets (af466: allow `virtual` with RTTI placement = PDB and unchanged ??_7).
2. The 12 const SRC_FAIL targets (make the called local-model members const where the PDB says so) and the 8 parser skips.
3. The header items (4 + 0x00BF6530) as one `_hdr` batch; Deputy2Killed / Acolyte class respells.
4. Rerun from `work/` (census.tsv, xr.tsv, plan6.tsv): pool.py -> excl -> census466 -> hscan -> xr466 -> mkplan -> af466 -> fixpt -> runpair -> fin466 -> shadow466.

## Locks (NF466)
103 rows: every row in the four batches' rows.txt (c1a 34, c1b 40, c1c 25, bWF 4). Taken 006b7760 late (unit member). Released the 28 generated
rows that did not make a batch (incl. 008527f0 009d5410 00a67420 and the 13 virtual-target rows) and 0x00F09640 (held; source in bWF held/).

End state: repo HEAD 3e57a104 (FullDecomp). All compiles ran before the coordinator's 03:40 table change; the census, source scan, includer check
and every shadow / tracker run were repeated after it (no batch source, test, header or catalog line changed). The source scan covers self
respells only; callee-only units rest on the retail census + per-caller symbol tables.
