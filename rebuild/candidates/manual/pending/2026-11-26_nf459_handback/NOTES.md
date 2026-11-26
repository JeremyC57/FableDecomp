# NF459 (2026-11-26): r457a, r457b, creature-action families (r454b)

Scratch: `$S/w22/agentNF459`. Tools were copied from NF457 and repointed (`tools/`). New tools:
- `tools/drectve.py`: one-line VC7.1 TUs, prints `.drectve`.
- `tools/calledit.py`: caller-side `(CThingCreatureBase*)` -> `&` respell, with each construction changed to `X(*p)`.

`sandbox/` holds copies of the 3 oracle TSVs and descriptive_check.py. They were refreshed from the repo right before the final gates. The repo oracle md5s are in work/repo_oracles_md5.txt and matched after every sandbox step.
- reloc_bindings.tsv changed in the repo at 00:56 UTC during the run (650 diff lines: another agent or a landing, not NF459; none of NF459's symbols are in it).
- The final gates ran on a fresh copy taken after that change.
- Every batch's orig/ was rechecked equal to the HEAD working tree at hand-back.

## Batches (NF457 layout: rows/, orig/ = exact HEAD copies incl. tests, head/payload.json + oracle.tsv, RESULTS, SELF_SYMBOLS, UNITS, CHECKS, TABLE_CHANGES, HEADERS, BEST_GUESS, evidence/)
| batch | rows | units | verdict | what |
|---|---:|---:|---|---|
| b7_heroRunningStop_identity | 1 | 1 | ACCEPT (r457a) | See b7 notes below. |
| b8_particlegroup_nocontainers | 7 | 1 | ACCEPT pending header equiv + r459b | See b8 notes below. |
| b9_creature_ctor_units | 25 | 12 | ACCEPT | See b9 notes below. |

**b7:**
- 0x0085BBA0 -> `??0CCreatureActionHeroRunningStop@@QAE@AAVCThingCreatureBase@@@Z` (NF457 b5x row text).
- The test is respelt to the real class and PASSes.
- RENAMES.tsv is in NF452's format, with real new file names. Those names are free; NF452 kept its stand-in names, and NF459 deviates on purpose so that no stand-in file name is left.
- CATALOG_CHANGES.tsv: the Module line only. The new Source and TestSource file names are in RENAMES.tsv (`new_source` / `new_test_source`).
- Also for the coordinator: rebuild/manifest/functions.tsv still carries the old name, module and parameter types (`CCombatAction_GenericResponseToDecapitation *,CThingCreatureBase *`) for 0x0085BBA0; regenerate it or respell it with the catalog.
- NAME_FIXES.tsv.
- evidence/vland_a7r.log is a third run with the catalog rename simulated (payload module/name, oracle name). STANDIN_LEAF is gone there.
- Census: the 5 landed HeroRunningStop siblings were compiled at HEAD (work/hobj). None DEFs or UNDs a one-argument ctor of the class, and no source declares one.
- Link (row + 5 siblings): 0 duplicates; unresolved 1 -> 1 (v00 stand-in renamed). See r459a.

**b8:**
- r457b redo. New add-only view `FABLE_RETAIL_CENGINEINTERNALPRIMITIVEPARTICLEGROUP_NO_CONTAINERS` in engine/CEngineInternalPrimitiveParticleGroup.h (HEADERS.tsv).
- The 6 deleting destructors are on it. `.drectve` is unchanged (no libcp), and objproof is ALL_EQ including `.drectve`.
- WARN set strictly smaller: RELOC_NAME and SHARED_HEADER_AVAILABLE are gone.
- 0x00BBA140 = NF456's edit, still on its local model: r459b.
- TABLE_CHANGES 1 add (NF456's).

**b9:**
- r454b units:
  - TentacleStrikeLoop 0x008CF990 (two-argument, `J`) + TentacleStrikeOutOf 0x008CFA10 + caller 0x008CF7D0.
  - The 11 one-argument ctors with a landed caller and a table line, each with its caller:
    - 00690db0 / 00690d30
    - 00854660 / 0066fc90
    - 008bcd10 / 0072c930
    - 008bd110 / 0072cd00
    - 008bd280 / 0072ce20
    - 008ca800 / 0091c3d0
    - 008cbf00 / 0096fcd0
    - 008e43a0 / 008de620
    - 008e4420 / 008de6a0
    - 008f5020 / 008e0730
    - 00982c10 / 009211d0
- One Tentacle unit (owner 0x008CF990) holds 008cf990, 008cfa10 and 008cf7d0, because 008cf7d0 respells both ctors.
- Name ties (evidence/ties_b9.txt, tools/ties.py): every target stores its own RTTI vftable. All end `ret 4`, except 0x008CF990, which ends `ret 8`.
- TABLE_CHANGES: 12 replace.
- BEST_GUESS: 4 FableWin-only (00690db0, 008e43a0, 008e4420, 008cfa10). Ego_d does not help here: its name set (egod_pdb_names.tsv) has no entry for any ctor of this family, the proven ones included.
- SELF_SYMBOLS: 20 lines. The 5 callers without a self_symbols line at HEAD (0072c930, 0091c3d0, 0096fcd0, 008de6a0, 009211d0) get none; their self is unchanged.

Gates (every row):
- vland `--recheck-landed` HEAD v new at one LANDVERIFY_WORK path (tools/runpair.sh; b8 uses runpair_hdr.sh, with FABLE_HDR_OVERLAY on the new side only): RELOCATION_MATCH PASS both sides, tests PASS.
- WARN sets: identical (b9, up to the PAV -> AAV spelling); class-swapped (b7); strictly smaller (b8).
- objproof (tools/objproof.py on vland's own objects): ALL_EQ under only the intended renames; `.drectve` compared as bytes.
- nf.py objects, compiled at separate paths, showed an anonymous-namespace hash rename in 0072cd00 / 0072ce20. It is a build-path artifact and is absent at the one vland path.
- Link HEAD set v new set:
  - b7: 0 duplicates.
  - b8: 0 duplicates; unresolved 7 -> 1.
  - b9: 1 pre-existing COMDAT duplicate, renamed only (0x008CF7D0's in-class TentacleStrikeOutOf v the strong 0x008CFA10, r457d class); unresolved identical.
- Sandbox (fresh copies):
  - all SELF_SYMBOLS applied (28);
  - TABLE_CHANGES `--dry` 0 misses, then applied (multi-target 1 -> 1);
  - descriptive_check 2026 lines, 0 FAIL with BEST_GUESS as `--extra`;
  - clash check 0 over 28 symbols.

## Census notes (b9)
- xref over every section at current HEAD. 0x00854660 also has a call at 0x00629AF0, which lies in no catalog row (nothing to respell).
- 0x008CBF00's "abs" hit at 0x00E872BF is coincidental bytes inside 0x00E87030 (a jcc displacement followed by a `mov edi, imm32`), not a reference. So 0x00E87030 is not a caller.
- 0x008BCD60 is not a model of 0x008BCD10: it is its own row, the 4-argument CastWillFireballSpell ctor `(PAV,J,_N,J)`, a two-argument-form target for later. Lock released.
- 0x008CF700 models an invented one-argument TentacleStrikeLoop ctor. It is left out of the unit: r459c.

## Holds / not done (resume point)
1. Rulings r459a..r459d (RULINGS_NEEDED.tsv).
2. **BanditKing holds**, as expected:
   - 0x008C8C30 models `CCreatureAction_BanditKingUnstuck(CThingCreatureBase*, bool)`; no PDB (Ego_r, FableWin, Ego_d) spells a `(CThingCreatureBase&, bool)` form.
   - The PDBs have only `(CThingCreatureBase&)` (FableWin) and `(CThingCreatureBase&, CThing&)`.
   - The family (008c94d0 / 008c95b0 / 008c9620 with 008c8c30 / 008c8f70 / 008c91e0) needs 008c8c30 re-modelled first. Not locked.
3. Two-argument forms: not started. Census lines with more parameters are in NF456's t5_creature_census.tsv; 008bcd60 4-arg is one. Regenerate at HEAD first.
4. r457c QuestCard (0x00702A50 thin row on 0x007026A0): not started (context budget).
5. 006cd890 CGroundNavigatorExternalInterface: NF453-locked, not touched.

## Locks (NF459)
33 rows, all in batches/*/rows.txt:
- b7: 0085bba0 (taken over from COORD_HOLD_NF457);
- b8: 00bba140 00bba240 00bbf180 00bc2240 00bc3be0 00bc53e0 00be1b30 (taken over from COORD_HOLD_NF457);
- b9: 25 rows.

008bcd60 was taken and released. No other locks.
