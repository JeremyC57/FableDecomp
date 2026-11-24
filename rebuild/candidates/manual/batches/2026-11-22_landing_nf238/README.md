# Landing nf238 (Account A, 2026-11-22): nf236 follow-ups (held p27 bindings, IsLocal, GetWinPhrase, one anonymous-namespace line)

Agent NF238 (beside NF237 / NF239). Hand-back: `pending/2026-11-22_nf238_handback/`. q12670 / q12671 done; q12668 / q12669 partly;
q12672 (CEngine ctor / dtor) and q12659 (0x00418DCA) not started.
- **Recheck** (`logs/recheck.log`, landed tree): 9 rows, all RELOCATION_MATCH + PASS, 0 RELOC_CONFLICT.
- **Header (add-only):** engine/NCreatureMode_CModeSitFloor.h `FABLE_RETAIL_NCREATUREMODE_CMODESITBENCH_ALLOC` (one static assert pinned).
  Base unchanged; equivalence over the header's 22 includers (`logs/equiv_includers.tsv`): only 0x00742790, which uses the opt-in;
  assert_names_check 0.
- **0x01436E28:** five binders (0x00B26360, 0x00B40260, 0x00B40BD0, 0x00B625E0, 0x00BEAB40) on
  `?GPDefinitionManager@NEngineInternal@@3PBVCDefinitionManager@@B` (all three PDBs): 2 -> 1; the held p27 line restored.
- **0x00740040 / 0x00742790:** CModeSitBench::Alloc ported off its local model onto the header; its leaf takes its binder's spelling
  (0x00742790 2 -> 1); the ctor binding moves to the PDB form, the held line restored (0x00740040 3 -> 2: the ctor's self row still
  emits the old leaf).
- **0x006F2C00:** 0x006F3530 (std::for_each) on CGetBestFreeProngForAttacker.h / CDefendingCombatantInfo.h; CRotateThing was a different
  real functor (stand-in, not an alias): 2 -> 1 with the `?A0x` hash stripped.
- **0x004C7E30** renamed CThing::IsLocal (`?IsLocal@CThing@@QBE_NXZ`; Ego_r / FableWin order IsLocal after GetTypeName, the body reads the
  bool at +9 of the record GetTypeName 0x004C7E10 names, both binders bind it): 2 -> 1. **0x007D7C10** renamed
  CTCTavernGameSpotTheAddition::GetWinPhrase (vtable 0x0126F814 slot 42 = GetWinPhrase in all seven tavern vtables; Ego_r 0x007D4EC0 /
  FableWin 0x02753360). Both: files, catalog module / name and manifest renamed here; new tests.
- **reloc_bindings:** 3 deleted, 3 added; no address grows. Link (agent): 0 LNK2005, 0 LNK4006.

## Coordinator rulings on NF238's questions
1. 0x01296F14: one unit, after the ScreamerIntoAggressive slot rows (0x0096DBA0, 0x0096DBB0, 0x0096E3F0) are named; queued for the agent
   that takes those rows.
2. 0x00740040's self row: queued as a header port (it emits the class vtable, so no leaf-only respell).
3. The 0x004C7E30 / 0x007D7C10 renames: accepted and applied.
4. CIEngine.h placeholders (Virtual08_00b27d90, Virtual11_00b23cf0, Virtual12_00b24400, Virtual13_00b242d0): queued as the first step
   of the CEngine unit (0x00B260B0 / 0x00B261D0).
