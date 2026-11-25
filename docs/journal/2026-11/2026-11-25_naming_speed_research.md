# Research: quicker ways to finish naming (2026-11-25)

Research only: nothing written to the repo, nothing compiled, no git. Every number comes from committed files and the scripts in
this folder (`a1.py` .. `a9.py`, `tok_nc.py`, `stale_nc.py`); `rec.json` is `naming_status.py --dump-rec` on the current tree.

## 0. Corrections to the brief's numbers
- Non-done is 64,041 - 34,586 = **29,455**, not 35,455.
- The brief's list leaves out **rtti_identified 2,480** and **rtti_conflict 98**; with them the statuses sum to 64,041.
- "T1 1,482 / 719" and "T3 20,824 / 8,155" are **addresses / class batches**, not lines / targets.

## 1. Short answer
The compile gates are not what limits speed. NF443's batch a1 (48 rows) was gated in 3 min 8 s, about 4 s a row (file times in
`$S/w22/agentNF443/work`, table in section 7), and an end-to-end scripted pipeline already exists (`pipeline438.sh`).
What limits speed:
1. most of what is left is not a respell: T3 holds 20,824 of the 22,330 tiered addresses;
2. about 4,230 open addresses are vendor code (libpk, 0xC08AE0-0xCA0000) and about 2,910 are data symbols, which the two PDB name sets cover poorly
   (libpk has no PDB at all; for data, Ego_d helps: A2);
3. holds that are process, not evidence: rows skipped because a T3 queue line names them, cross-batch table lines, ruling round trips;
4. the 35-minute equiv_check paid once per header batch.

The quick wins are (A) three no-compile fixes worth about +540 to +650 done, (B) one registry pass worth up to 3,672 best guesses
(counted apart), and (D) process changes that let the existing generators reach rows they now skip.

## 2. Ranked options

### A. No compile; moves the proven count
**A1. Read the library symbol for `LIBRARY_MATCH` rows (tracker only). +239 done.**
- Measured (`a7.py`): of 340 unmeasured rows, 246 are `LIBRARY_MATCH` in `self_symbols.tsv` with an empty symbol column. Every one has a
  `library_signatures.tsv` line, and 239 of those symbols pass the tracker's own `is_std` (proven library symbol or `__imp_`).
  The rest: 78 `LIBRARY_MISMATCH`, 14 `DIFFER`, 2 `NOT_GRADED`; those need a regrade, not a tracker change.
- Effort: about 10 lines in `naming_status.py`. Risk: none; `lib_verify.py` already proves these names. Rules touched: none.
- First step: patch the tracker, then diff the full rec per address.

**A2. Add the Ego_d carved names as a third PDB name set. +249 done; 330 more targets become known.**
- Measured (`a6.py`, `a7.py`): `ghidra_out/egod_carved_names.tsv` has 30,852 Ego_d.pdb names that are in neither Ego_r nor FableWin, and
  the tracker does not read it. Using only its `demangles = 1` lines:
  - 249 `agree_unverified` addresses spell an Ego_d name (247 data symbols such as `?DELTA_NOTIONAL_ZERO@@3MB`, 2 code) -> agree_pdb;
  - 330 `split_unknown` addresses hold exactly one Ego_d name (238 `??_7` vftables, 86 data, 6 code) -> split_fixable, target known.
- Risk: Ego_d is a VC10 Dinkumware debug build, so its STL spellings differ; keep `@std@@` names out. Data and vftable names do not
  depend on the toolchain.
- Rules touched: a ruling that Ego_d counts as a PDB name set for the tracker and `descriptive_check.py`. EXTRA_RULES already lists
  "Ego_r / FableWin / Ego_d name sets" as name sources; the 2026-11-23 plan called Ego_d unused because it read only the 24-line
  `egod_pdb_names.tsv`.
- First step: the ruling; then the patch and a rec diff.

**A3. Bulk stale-line sweep, then a compiled census. +54 to +164 done; about 400 more targets simplified.**
- Method (`tok_nc.py`, `stale_nc.py`; list in `stale_candidates_nc.tsv`): for each binding line of a split target, map the witness
  site to its landed row (catalog address + length), take `owner_source()`, strip comments, and test for the spelling's key identifier.
- Measured: **1,055 candidate lines.**

  | If these are stale | -> agree_pdb | -> agree_std | -> agree_unverified | -> one stand-in | fewer spellings |
  |---|---:|---:|---:|---:|---:|
  | every candidate | 39 | 125 | 245 | 153 | 232 |
  | only "strong" (identifier in no source and no header) | 15 | 39 | 82 | 59 | 89 |
- Calibration on the two lines NF448 dropped: `??4CCountedPointer<CScriptThingImp>` reads live (right); `??0CDefClassBase` at 0x00430370
  reads as a candidate (wrong: a header emits it). So header-declared candidates have false positives and the census stays mandatory.
  All 11 of NF450's pending lines are in the list.
- The upper figures include lines the census will rewitness to another row instead of deleting; those close nothing.
- Effort: one census run over about 1,000 witness rows plus the rows that reference each target (NF448 compiled 2,600 objects for 52
  lines). No per-line judgment. Rules touched: none new (R-NF430-2 grants table-only deletes over unchanged rows).
- First step: census the strong and no-header lines first.

**A4 (optional; machine time only). Full regrade of every landed row.** `self_symbols.py --all` regrades in chunks of 600 at 5-6
minutes a chunk (its own doc): about 9 hours unattended for 57,170 rows. It refreshes every self symbol and finds rows that no longer
match from the repo (NF448 found five by accident). It reads only each row's graded self symbol, so an exact stale answer for all
38,609 table lines needs a relocation dump added to the same pass (`ref_scan.py` or the census tools).

### B. No compile; moves the best-guess count only
**B1. Register existing, consistent, non-PDB spellings. Up to 3,672 addresses agree_unverified -> agree_descriptive.**
- Measured (`a8.py`): from the 6,557 agree_unverified I set aside 249 Ego_d names (A2), 1,285 whose name and class exist in a PDB
  (C1), 493 `Class_Method` free stand-ins (they must become members) and 68 with no landed row. The other 4,462 went through
  `descriptive_check.py --extra` as a dry run (`BEST_GUESS_dryrun.tsv`, `dc.out`): **3,672 pass**; 790 fail (SYMBOL_ELSEWHERE 731,
  DUP_SYMBOL 512: names shared by several addresses, mostly past 0xC08AE0). The 3,672 by place: game 1,373; data 958; libpk
  (0xC08AE0-0xCA0000) 968; 0xCA0000-0xCD0000 296 (strmbase and CRT-side code: vendor source can prove those names, so they are not
  guesses); 0xCD0000-0x0120FFFF 45; 0x0121xxxx 32 (funclets and initializers of game code).
- Precedent: nf440 `acc` (14 rows registered as `one-source:caller-binding`). No source changes.
- Risk: the rule says "search first" and "as close to what retail would have named". Judge per class, not per row (530 classes in the
  finish plan's route 7), libpk first: the 2026-11-04 and 2026-11-12 owner rules already give no-PDB vendor code descriptive names and
  say not to hunt exact ones, so the 968 libpk lines are the safest.
- The proven count does not move. "Named incl. best-guess" would go from 54.30% to about 60.0% if all 3,672 were accepted.
- Rules touched: best-guess rule (2026-11-25); which basis value libpk names take (`style`) is one coordinator decision.
- First step: the 968 libpk lines grouped by class, each with the automated search result (absent from all four name sets) as its note.

### C. Compile, but scriptable in bulk
**C1. Access-letter / const fixes onto a unique PDB symbol. About 267 addresses.**
- Measured (`a5.py`): 1,245 agree_unverified addresses have exactly one PDB symbol with the same `?name@class@@`. Differences: access /
  virtual / const letters 646, parameter or return types 302, scope (missing namespace) 274, wchar 18, class key 4, mixed 1.
- Across all open statuses (`a9.py`) only **267** are a pure access letter or `const` (164 agree_unverified, 92 split_unknown, 5 standin,
  6 rtti_identified). **766** change virtuality (Q -> U): the model needs a vptr, so that is a port.
- Tool: `access_fix.py --unit-self` / `unit_fix.py` (self plus every caller as one unit). These are rejects of route 1 of the
  2026-11-23 plan, so expect less than the 84-88% acceptance of its first runs.
- 189 of the 267 are in classes that have an engine header: put those in ONE header batch so equiv_check runs once (D2).
- Second source: `order_match.py` (N=1) independently predicts the same PDB symbol for 261 of the 1,245.

**C2. T1 respells unlocked by A2: up to 330 targets.** The 238 vftable targets are the NF430 shape (`CVtable_<va>` lines; NF430 closed
99 addresses with table deletes alone), so many may fall to A3 instead.

**C3. `order_match` names. New names for 578 agree_unverified, 212 split_unknown, 115 standin.**
- Measured: N=1 with the size guard on Ego_r, leave-one-out precision 98.0% (5,060 right, 102 wrong), 2,072 open addresses named.
  N=2: 99.5%, 887 addresses. FableWin's order adds almost nothing (122 addresses at 91%).
- **The 98% does not hold on open addresses.** On split_fixable it agrees with the known PDB spelling in about 96%. On the 558
  rtti_identified addresses it predicts, it names the RTTI function in only 421 (75%); 134 are a different function, mostly
  compiler-generated members (`??_E` of a neighbouring class, `??4`). Of the new names, 229 are compiler-generated (`??...`) and 624
  are ordinary named functions. Treat a `??` prediction as unusable and quote about 75-96% for the rest until it is measured by shape.
- **Ruling question:** EXTRA_RULES line 76 lists "a PDB name-set entry plus donor order or size" as two sources; the order_match
  docstring calls a prediction one source. Under line 76 these are proven targets; otherwise they are `one-source:tu-order` guesses
  (170 registry lines already use that basis). Either way each needs a respell through the unit route.

### D. Process changes (no change to the evidence standard)
**D1. Let callee-only respells through the T3-queue hold.** 209 (NF442) and 248 (NF443) residue lines read "row: named in
T3_PORT_QUEUE col 2/3": the generator skips the row only because a queue line mentions it. A callee-only respell leaves the row's
self symbol and bytes unchanged, which objgate ALL_EQ proves, so it cannot disturb the port the T3 line describes. Proposal: skip the
hold when the change is callee-only and objgate is ALL_EQ. Touches the lander's T3 rule (the nf446 u4 hold was the same problem and
went to the owner). Up to about 450 residue lines; fewer addresses than lines.

**D2. One header batch per round.** equiv_check is a fixed 35 minutes per header batch. Collect a round's add-only opt-ins (C1's 189,
PeekClassList-style opt-ins, r3 header debt) into one `_hdr` batch and run it once, copy against copy.

**D3. Run the scripted pipeline without an agent.** `pipeline438.sh` covers generate, batch, prechecks, HEAD / new vland, objgate, pk,
link, finalize, units, xrcheck and screen. With the cap at 2, agents should take only residue that needs judgment. Put the rulings
already granted into every brief (NF438 q1 / q2, r2, r3, r449a, r449c, q442a's four checks, R-NF430-2) so no row waits for a ruling
that exists.

**D4. Lander fixes already written up in GOTCHAS:** cross-batch table lines applied by hand, SELF_SYMBOLS without a status column, a
head payload carrying new text. Each cost a round trip; each is mechanical.

**D5. Which generators exist (question 5).** All are libclang-driven, write candidate rows plus a PLAN and a RESIDUE, and feed
`pipeline438.sh`: `rebind438.py` / `rebind443.py` (member, static and free callee rebinds; 161 + 98 + 308 rows landed), `ctor449.py`
(SIGCTOR: a model ctor's `T*` -> `U&`), `dtor449.py` (DTORX: a destructor stand-in -> a qualified `Q::~Q()`), `peek448.py` (one
header opt-in plus its 48 callers), and the older `access_fix.py`, `unit_fix.py`, `flatten_pilot.py`, `alias_header.py`. The simple
families are already bulk-generated; the per-pass yield is falling (NF449 76 rows, NF450 14) because what is left is residue.

### E. What I would not do
- **Do not drop vland for callee-only respells.** objgate ALL_EQ compares HEAD with new, not HEAD with retail; NF448 found five landed
  rows that no longer match from the repo. vland is also cheap (about 1 s a row per side). pk is the largest gate (about 2 s a row),
  and it is the one that catches a new name exposing a pick-any copy, so it stays too.
- **Do not use BSim for names:** 44% precision at sig >= 5 and 75% at sig >= 20 (`naming_remainder.py`).
- **The FSE manifest cannot be joined:** 947 functions with names and scopes, no addresses (`labels_fse_addresses.tsv` has 39 lines).

## 3. Do first
1. A1 (+239 done) and the A2 ruling (+249 done, 330 targets known): coordinator time only, no compile.
2. A3: census the strong candidates, then the rest (+54 to +164 done).
3. D1 and D2, before the next T1 or header round.
4. B1 for the libpk classes (968 best guesses), and the C3 ruling.

Steps 1-2 are worth about +540 to +650 proven addresses (about +0.85 to +1.0 points). At today's +20 to +120 per agent round that is 5
to 30 rounds.

## 4. Where the 29,455 non-done addresses are (question 1)
| Bucket | Addresses | What closes it |
|---|---:|---|
| unmeasured | 340 | 239 tracker-only (A1); 78 library mismatches and 16 unmatched rows need a regrade |
| agree_descriptive | 187 | registered best guesses: closed, counted apart |
| split_fixable T1 | 1,482 | caller respells: 818 members / free functions, 260 ctors, 100 dtors, 304 operators / `??_G` |
| split_fixable T2 | 24 | the row's own access letters |
| split_fixable T3 | 2,562 | the self row is wrong: stand-in alias 699, signature 620, other 1,243 (self rename or port) |
| rtti_identified | 2,480 | name known (RTTI + PDB); the row must emit it: 566 same name with another signature, 385 `Class_Method` free functions, the rest ports |
| rtti_conflict, split_pdb_multi | 139 | identity rulings |
| agree_unverified | 6,557 | 249 Ego_d (A2); 1,285 with a PDB name+class (C1); 493 `Class_Method` stand-ins (ports); 3,672 registrable as is (B1); 790 fail the registry check; 68 without a landed row |
| standin | 8,676 | 2,605 shared-name (identity) and 6,071 marker. Game 4,386, libpk 2,026, text past 0xCA0000 1,334, data 930. 737 are CCountedPointer |
| split_unknown | 7,008 | Game 4,978, libpk 1,044, data 777, other 209. Templates: vector 785, _Rb_tree 464, CCountedPointer 309, CIntelligentPointer 178, CArray 119, list 113. 330 get a target from Ego_d (A2) |

A table-only change closes at most about 164 (A3). A catalog-only rename closes 0 (tier T0 is empty).

## 5. Name sources: used by the tracker or not (question 3)
| Source | Used by `naming_status.py` | What it could add |
|---|---|---|
| `egor_pdb_names.tsv`, `fablewin_pdb_names.tsv` | yes, as name sets (donor-keyed: no join by address) | - |
| `labels_rtti_port.tsv`, `labels_rtti_port_dtors.tsv` | yes (rtti_identified / rtti_conflict) | - |
| `self_symbols.tsv` | yes, MATCH / RELOCATION_MATCH lines only | A1 |
| `library_signatures.tsv` | only as the `is_std` symbol set | A1 |
| `egod_carved_names.tsv` | **no** | A2 |
| `egor_obj_functions.tsv` (object order) | no (`order_match.py` is separate) | C3 |
| decorated names in the obj_functions files that the name tsvs lack (1,605) | no | 2 open addresses: nothing |
| `refs/fse_api_manifest.json` | no | nothing: no addresses |
| BSim audits | no | nothing usable |
| retail strings, Aeon's Lua ports | no | not measured (section 8) |

No address map joins the PDB name tsvs to retail: they carry Ego_r / FableWin addresses (EXTRA_RULES line 17). The usable bridges are
object order (C3), RTTI vftable slots (already used) and BSim (too imprecise).

## 6. Best-guess candidates in standin / split_unknown (question 6)
| One-source basis | standin | split_unknown | What it takes |
|---|---:|---:|---|
| TU order (`order_match` N=1, 98%) | 115 | 212 | respell + registry line; proven instead if the C3 ruling says two sources |
| `Class_Method` label with one PDB member of that name | 178 | 74 | member port |
| CCountedPointer layout predictor (nf440: about 1 name in 4 wrong somewhere) | 737 | 309 | the nf440 bg pipeline; 184 already registered |
| libpk / GameCODA SDK names (R-NF430-1) | about 2,026 | about 1,044 | NF441 tied 43 classes; the CODAApiDbg.lib slot-body match has not been run |
| the one non-stand-in spelling already in use | - | 2,142 (886 where it is the row's own self) | respell the stand-in callers onto it: T1 shape with a guessed target. R-NF440-1 says a registry target is never T1, so this needs a ruling |

## 7. Numbers and how they were measured
| Number | Value | File / command |
|---|---:|---|
| statuses, tiers | section 0 | `naming_status.py --out st --dump-rec rec.json`; `a1.py` |
| region split | section 4 | `a3.py` (libpk bounds from EXTRA_RULES chunk 433; data = 0x01220000 and up) |
| unmeasured rows with a proven library symbol | 246 (239 pass `is_std`) | `a7.py`: `self_symbols.tsv` x `library_signatures.tsv` |
| Ego_d hits | 249 / 330 | `a7.py`, `demangles = 1` lines |
| stale candidates | 1,055 lines; 54-164 done | `tok_nc.py`, `stale_nc.py` -> `stale_candidates_nc.tsv` |
| registry dry run | 4,462 tried, 3,672 pass | `a8.py`; `descriptive_check.py --extra BEST_GUESS_dryrun.tsv` -> `dc.out` |
| unique PDB candidates by difference | 1,245: 646 / 302 / 274 / 18 / 4 / 1 | `a5.py` |
| access or const v virtuality; header present | 267 v 766; 189 | `a9.py` |
| order_match | 2,072 at 98.0% (N=1); 887 at 99.5% (N=2) | `order_match.py rec.json --anchors N` -> `preds_n*.tsv`, `predsfw_n*.tsv` |
| BSim precision; `Class_Method` census | 44% / 75%; 471 unique of 1,877 | `naming_remainder.py` (read-only run) |
| split_unknown with one non-stand-in spelling | 2,142 (886 self) | inline script on `rec.json` |
| gates, batch a1 (48 rows) | vland HEAD 48 s, vland new 47 s, objgate under 1 s, pk 92 s, /FORCE link 1 s: 3 min 8 s, about 4 s a row | file times of `vland_a1h.log`, `vland_a1n.log`, `og_a1.out`, `pk_a1.out`, `lk_a1.out` in `$S/w22/agentNF443/work` |
| generation and prechecks, a1 | about 4.5 min before the gates (libclang rebind, definers) | same folder, `a1_addr.txt` to `run_a1.out` |
| equiv_check | about 35 min per header batch | the brief; not re-timed |
| T1 residue reasons | 209 + 248 T3-queue holds; 445 + 218 special members | `RESIDUE.tsv` of nf442, nf443, nf449, nf450 |

## 8. Not measured
- **Aeon's Lua ports:** `work/aeon_lua_ports/` is not in this checkout.
- **Retail strings as a name source:** no join attempted (it needs each function's string references).
- **The true stale rate:** text pre-filter only; no compile was allowed, and the header-declared class has known false positives.
- **Gate times beyond one batch:** the per-gate times are from NF443's a1 only; name_gate, xrcheck, apply_table and reserved_check
  were not timed separately (they ran inside finalize), and equiv_check's 35 minutes is the brief's figure.
- **Acceptance for C1 and C3:** both pools are rejects of earlier scripted runs, so the yields are upper bounds.
- **Addresses D1 closes:** 457 is residue lines, not targets, and the two lists overlap.
- **The GameCODA slot-body match and the CCountedPointer predictor's remaining reach:** taken from the hand-backs, not rerun.
- **Generator reach:** I read the tool headers only (`$S/w22/agentNF449/tools/ctor449.py`, `dtor449.py`;
  `$S/w22/agentNF448/tools/peek448.py`; `rebind438.py` / `rebind443.py`), not their per-row results.
