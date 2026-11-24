# 2026-11-24 — naming: scripted routes widened, agents NF321-NF327 (Account A)

Owner instructions this session:
- "have 3 agents at once total";
- "make sure that any that are possible to do correctly with automatic tools are done that way";
- "usage ran out but is back now continue agents and tasks";
- latest: "Dont launch anything new for now. And make sure all your tools and progress are pushed and committed".

## Tracker
`naming_status.py`, run after the unit2 landing:

| Status | 2026-11-23 (after NF320) | 2026-11-24 |
|---|---:|---:|
| agree_pdb | 22,988 | 23,193 |
| agree_unverified | 6,213 | 6,105 |
| rtti_identified | ~4,7xx | 4,551 |

Every landing below came with a recheck on the landed tree. Callers moved by a unit or binder landing keep their own symbol, so they do not
show up in agree_pdb.

## Landed (all on FullDecomp)
| Batch | What | Rows | Proof summary |
|---|---|---:|---|
| `2026-11-24_landing_unit1` | unit route: self + every landed caller of the old spelling, together | 181 (74 units) | 181 PASS, 74 table replaces |
| `2026-11-24_landing_unit2` | unit route, wave 2 (shared sources taken in waves) | 93 (39 units) | 93 PASS, 39 table replaces |
| `2026-11-24_landing_nf322` | flat-header classes onto generated polymorphic models, 40 header opt-ins | 60 | 114 PASS (rows + includers), slot probes |
| `2026-11-24_landing_nf321` | body / callee ports, 6 header opt-ins, 3 table replaces | 9 | 403 PASS (rows + includers) |
| `2026-11-24_landing_nf323` | 14 Clone rows (slot-for-slot vftable proof) + 6 declaration rows, 14 header opt-ins | 20 | 50 PASS |
| `2026-11-24_landing_dangling1` | rows still referencing a spelling an earlier table replace removed | 7 | 7 PASS |
| `2026-11-24_landing_rename1` | callers that called the right function by a made-up identifier | 7 | 7 PASS, 7 table replaces |
| `2026-11-24_landing_flatten4` | flatten route rerun (switch mode) | 5 | 5 PASS |

## Tools (all committed under `tools/decomp_pipeline/linux_lane/lane_tools/`)
- **access_fix.py / flatten_pilot.py / alias_header.py:** HEAD and new now compile at ONE path, removing the `?A0x` false rejects.
- **access_fix.py**, new modes:
  - `--rename`: same scope and tail, different identifier, on the caller side;
  - `--unit-self`;
  - `--pairs FILE`: explicit caller edits.
- **unit_fix.py (new):** the unit route.
  - Finds every landed caller of the old spelling by compiling every landed source that names the identifier (cap 60).
  - Edits the callers with `--pairs`, and accepts a unit only when the self and every caller pass.
  - Runs in waves: units sharing a source are deferred to the next run.
- **ref_scan.py (new):** which landed rows' objects reference a symbol.
- **land_scripted.py:**
  - units land whole or not at all;
  - a row whose landed source changed since its run was generated is held (stale). On first use this caught 2 rename rows and 1 unit2 unit.
- Agent helpers committed under `naming_batch/agent_tools/nf321/`, `nf322/`, `nf323/`.

## Defects found and fixed in earlier work
- **Table replaces.** `reloc_bindings.tsv` keeps one witness per (symbol, target), so a `replace W -> P` can strand other landed rows that
  still reference W. ref_scan over every replace since 2026-11-20 (268 spellings) found 5 stranded spellings: binders1 ×3, nf286, nf305.
  - dangling1 fixed 7 rows.
  - Still open: 0x0079EAA0 (T3-reserved) and 0x00B84190 (a constructor reference outside the tool's scope).
  - **39 widely used spellings (over 80 sources each) are NOT yet scanned.** That is about 5,000 compiles, for a quiet machine:
    `ref_scan.py <work> <syms> --cap 1200`, with the symbol list from `$S/refs1/REFS.tsv` TOO_MANY lines.
  - The rule is now in BRIEF_NF_BATCH.md ("Table replaces").
- **rename1 held three units** whose old spelling other rows still reference: 0x00AF5900, 0x008FAF70, 0x007E9360. They need the unit route.

## Rulings recorded
- NF321 Q1-Q3 and NF322 Q1-Q3 are in BRIEF_NF_BATCH.md.
- NF323 Q1-Q7 are in `batches/2026-11-24_landing_nf323/README.md`.
- NF325 Q1-Q4 (my calls, not yet in the brief; they go into NF325's landing README):
  - **Q1:** a friend under an add-only opt-in is allowed (NF309 Q2).
  - **Q2:** base slots declared `= 0` from retail `_purecall` plus the derived override spellings are allowed under the opt-in, with the
    probe.
  - **Q3:** land the new MazeBase header now; SHARED_HEADER_AVAILABLE is a warning.
  - **Q4:** CCombatSequenceBase pure slots 1 / 2 / 15 go to a later unit.

## After the stop: the running agents finished and were landed (owner: "Let the current agents finish, then land their work")
| Batch | What | Rows | Proof summary |
|---|---|---:|---|
| `2026-11-24_landing_nf325` | 25 CCombatSequence rows on generated models, primitive-base / EnemyCreatureBase defects fixed, MazeBase header | 31 | full equiv_check (57,146 rows, path-only lines only), 41 PASS with nf327 |
| `2026-11-24_landing_nf327` | 6 Clone rows on new opt-in headers, EndCutFade (`__fltused`), ServeFood, GetPossibleCollisionSet | 10 | cloneproof, closure equiv_check, 41 PASS with nf325 |
| `2026-11-24_landing_nf324_b1`..`_b7` | flattened virtual members back on their class (flatten_pilot v4, 122 header opt-ins) | 678 | land_scripted staleness + 678 PASS |
| `2026-11-24_landing_nf326` | CScriptThing pick-any unit: 10 members onto their PDB symbol, 101 copy rows | 111 | selfproof 10 / copyproof 101 OK on the landing objects, 111 PASS |

Tracker after these landings: **agree_pdb 23,920** (from 23,193 this morning, 22,988 on 2026-11-23). rtti_identified fell from 4,551 to
3,829.

## Open and queued (owner, 2026-11-24: continue the naming queue; up to 2 naming agents at once)
- **unit3: landed** (`batches/2026-11-24_landing_unit3`): 59 rows, 26 table replaces; agree_pdb 23,942. Wave 4: 26 units deferred
  again (shared sources); run `unit_fix.py` in a fresh directory. The unit 0x004166E2 / 0x00417001 is held for the third time, by the
  RP1 lock and the open 2026-11-20 CVsyncCallbackTimer claim line.
- **RP1 landed** (`batches/2026-11-24_landing_rp1`, advisor decision, owner delegated): the 2026-11-20 CVsyncCallbackTimer header re-port,
  never landed then, re-based (27 rows as returned, 2 three-way merged), 29 / 29 recheck good. Its claim is closed and its 30 locks are released, so
  wave 4 can take the 0x004166E2 unit. Stale session locks released as well: 8 NF214, 71 acctB (owner released those rows on 2026-11-21).
- **The 39-spelling ref_scan:** old spellings over 80 sources each, about 5,000 compiles.
- **unit4 landed** (`batches/2026-11-24_landing_unit4` + `_unit4b`): 25 rows, 12 table replaces; agree_pdb 23,951. 15 units deferred to wave 5.
  `land_scripted.py` now honours "newest claim line per item wins" (it held 0x004166E2 on the landing's own claim line).
- **NF324 Q1:** a generated opt-in derived-model route for the CCombatActionBase (~156) and CCombatSequenceBase (~143) families.
- **NF324 Q2:** 79 rows whose own tests call the flattened name, which need the tests rewritten by script.
- **NF324 table check:** 2 callers still bind a non-virtual spelling of members nf324 moved to virtual (0x00858CC0 CreatureOnBlowBlocked,
  0x00B47980 FrameEnd). They are queued in T3 `nf324` and need the binder route.
- **NF326 Q4:** the ~CScriptThing / copy-constructor pick-any unit. Also the 0x00DF6010 GetDataString debt.
- **Held units:** 0x00860290 (Clone with binders), 0x009680D0 (PickUpJug destructor shape), 0x008601B0 / 0x008601C0 (GetUpFromSitting
  macro), 0x008BFFB0 (no HEAD payload), and rename1's three units.
- **flatten_pilot.py v4** is in `naming_batch/agent_tools/nf324/`. Promote it over `lane_tools/flatten_pilot.py` after a review.
- **Stale lock:** one NF314 lock (0x00DC0300) is still live. NF314 is long finished, so release it at the next landing.
- **unit5-unit9 landed** (scripted waves, 24 rows); the loop was stopped before unit10's run (one unit per wave left); 3 deferred units remain.
- **Agents launched 2026-11-24 (owner: "You can run 2 naming queue agents at the same time as long as they don't interfere"):**
  - NF328: the ~CScriptThing / copy-constructor pick-any unit (NF326 Q4), with a stop condition on any new vftable / ctor / `??_G` emission.
  - NF329: NF324 Q1, ruled wanted as its own pass (advisor decision, owner delegated): the CCombatActionBase, then CCombatSequenceBase,
    flat-header rows on generated opt-in derived models under the NF322 Q1 / Q2 rules. Stays off NF326's rows and anything emitting the NF328 members.
- **B's BanditCamp pick-any warning** (COORDINATION_LOG): a coordinator audit, not an agent, because its fixes touch the CIsThingAlive /
  MovieBase headers B24A2 is working on.
- **NF329 landed** (`batches/2026-11-24_landing_nf329_b3`, `_b4`, `_b1`, `_b2`): 301 NF324 Q1 rows onto their RTTI-placed PDB members,
  118 new opt-in class models, 0 held. `land_scripted.py --release-t3 <tag>` lands rows an agent was sent into from T3 and closes their
  lines. Open: the 95 thin-remainder rows, the neutral CCombatActionBase guard, and rebasing the parked overlays (D-e).
- **NF328 handed back** (`pending/2026-11-24_nf328_handback/`): the ~CScriptThing / copy-ctor unit, 9 rows proven and held on decisions
  D1-D6. Advisor ruling: land as one unit, with the 21 differing copies as debt, after the D5 census holes are compiled.

## Evening: agents NF378-NF388 (Account A)
- **Landed:** nf378 b1/b1h/b2/b3 (25 stand-in selves + 4 binders, 11 header opt-ins, whole-tree equiv 0 differing); nf379 b1-b3 (CThingNoiseDef and JackDragon option S units, 0x0044C1BB); nf380 b1/b2; nf381 b1/b2 (78 generated Def-ctor binders onto `??0CSubDefClassBase@@QAE@XZ`, table line deleted after a compiled census of all 150 callers); nf382 b1/b2/b4; nf383 b1-b3; nf384 b1; nf385 b1. Option S landing tools (4f72f62cb2), tracker shared-name fix (347ece5c58).
- **Rulings (advisor decision, owner delegated):** retail has no ICF (identical template COMDATs at separate addresses; GOTCHAS rewritten); NF379 Q1-Q3; NF378 R1-R5; NF380 #1-#3; NF381 (census strict, reason-column mentions do not reserve); NF383 R1 (objsec exception for destructor thunks) and R2 (a full vector body with wrapper callers is one source); NF382 p1 (NoDuplicates v Any = new duplicate), p2 (wrong-body resolution may drop), p6 (ctor vs traits ctor not proven); NF385 R1 (CRetailTree pairs held), R2 (literal fix as its own batch).
- **Found:** the grader passes a row whose string literal differs from retail (0x00B1AE00); 38 `Unwind<addr>` funclets have no funclet_offset in the recheck payload.
