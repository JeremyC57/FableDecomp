# 2026-11-20 landing nf124: PRIORITY slice 7 remainder, first hand-back (3 rows)

Records: `naming/2026-11-20_agentNF124/` (selfcheck 0 problems). 7 entries (NF120's leftovers): 1 done (q6326 0x0076D270), 6 open, moved to
NF126 (q4747, q4802, q6408, q6453, q6454, q6455; NOTES has the resume census of the heavy CModeBase rows).

## Rows (3): 3 RELOCATION_MATCH, every test PASS (`logs/recheck.log`)
- 0x0089C100: binds `?AddAllyThing@CTCEnemy@@QAEXABVCThing@@@Z` and `?RemoveAllyThing@CTCEnemy@@QAEXABVCThing@@@Z` through the shared
  CThing.h / CTCEnemy.h (both PDBs), closing the existing splits on 0x0076D270 and 0x0076C850 (nf120 had queued the 0x0076C850 name).
- 0x008D6580 CModeSitBase::OnActivate and 0x008D8EA0 CModeBlock::FrameUpdate: leaf-only self renames QAE -> UAE (PDB virtual overrides;
  the exe scan finds no landed row referencing either address; coordinator decision, recorded in NF126's brief). Both now bind the
  q6453 / q6455 claim at 0x0073A680 and 0x008D6580 the q6454 claim at 0x0073AAC0.

## Headers (add-only opt-ins, default path token-identical; assert_names_check 0 mismatches)
CTCEnemy.h ADDALLYTHING / REMOVEALLYTHING (pin_asserts pinned its one assert to its existing tag, line 59); CTCCreatureModeManager.h
FABLE_RETAIL_CMODEBASE_GETNOFRAMESBEFOREPLAYINGCOMBINATIONIDLEANIMS; NCreatureMode_CModeSitFloor.h FABLE_RETAIL_NCREATUREMODE_CMODESITBASE_ONACTIVATE.

## Checks
bindings_multi_check 1; funclet_includes_check 1355 / 0. 201 rows rechecked: 201 good / 0 bad. No-new-split gate 0. Table spelling sets:
0x0073A680 9 -> 8, 0x0076C850 2 -> 1, 0x0076D270 2 -> 1; none grew.
NAME_FIXES queued: 0x0089C100 own leaf private const (ABE; moves with its caller 0x008A7EC0), 0x0073AC50 CModeBase::FrameUpdate UAE,
0x0073A6F0 nested-form GetNoFramesBeforePlayingCombinationIdleAnims (0x00862EC0 still binds the old form).
