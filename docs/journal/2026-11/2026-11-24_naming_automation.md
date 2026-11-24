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

## In flight at the stop ("don't launch anything new")
- **NF325 hand-back, not landed yet:** 31 rows, 37 headers, 1 new header. Staleness was clean at 01:40.
  - NF325 ran equiv_check over the include closure (365 rows) only, because of load.
  - The coordinator started the full run: `$S/nf325_eq/` (`equiv_full.tsv`), comparing rebuild/include with `$S/nf325_tree`.
  - Land after it reports 0 differing: copy hdr/engine, apply_rows, recheck the rows plus includers, write the README, commit by rows.
- **Agents still running** (started before the stop):
  - NF324: flatten tool over 1,744 flattened virtual members;
  - NF326: the CScriptThing / ScriptedThing pick-any unit;
  - NF327: items unblocked by today's rulings.
  Their folders are `$S/w22/agentNF32{4,6,7}/`; land their hand-backs as above.
- **unit3 was stopped** before producing anything. Rerun it later with `unit_fix.py $S/unit3b` (fresh dir); 51 units were deferred to it.
- **Load:** three agents plus the coordinator's compiles drove the load average to about 38 on 4 cores. Run one heavy coordinator job at
  a time.
