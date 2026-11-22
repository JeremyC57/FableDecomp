# Landing nf244 (Account A, 2026-11-22): CIEngine slots 11-13 on their PDB declarations by default, the CEngine overrides opt-in, four CModeStandBase-family self leaves

Agent NF244 (entries q12680 steps 1-2, q12681; q12682 / q12679 / q12676 / q12659 not started, passed on). Hand-back:
`pending/2026-11-22_nf244_handback/` (NOTES = the 16-subset bisect table; work/bis*.txt = one verdict line per subset, dumps stripped).
- **q12680 step 1 (engine/CIEngine.h):** the four slot opt-ins do NOT act one at a time on 0x0058578B (CTCInventory::OpenInventory): SCREENPOS
  alone and CONVERTWORLDPOS alone DIFFER (the same 0x00585D7A swap), RENDER + CONVERTSCREENPOS DIFFER, two triples match; neutral controls
  match. The triple {SCREENPOS, CONVERTSCREENPOS, CONVERTWORLDPOS} (slots 11-13, the PDB declarations) becomes the default, as q12680
  directed ("make the declarations that do not move it the default"); slot 8 keeps `Virtual08_00b27d90` with its RENDER opt-in.
  pin_asserts 4 pinned, assert_names_check 0 mismatches. equiv_check over the 159-row closure: 108 rows differ; ten tests respelled
  (slots 11-13 overrides only).
- **q12680 step 2 (engine/CEngine.h):** add-only `FABLE_RETAIL_CENGINE_OVERRIDES` (ctor, virtual dtor, the 78 overrides the default branch
  lacks, OnPre/PostDeviceReset private as both PDBs; 84 vtable symbols each in two PDBs by a probe class). No landed file defines it.
- **q12681:** 0x0073DA00 `?IsAllowingPlayerSpecialMovementMoves@CModeStandBase@@UBE_NXZ` MATCH, 0x00861900 `?GetMovementSpeed@CModeStandBase@@
  UBEMABVC3DVector@@0W4EMovementType@@@Z` RELOCATION_MATCH (Module NCreatureMode::CModeCarryingStandBase -> CModeStandBase, files renamed;
  its HEAD comment said "AI creature", the compared type is 2 = THING_TYPE_PLAYER_CREATURE: corrected, bytes unchanged), 0x008D6850
  AnimUpdate MATCH, 0x00743CB0 `_gapscan::sub_00743cb0` / catalog `vfunc_11` -> NCreatureMode::CModeIceTrollGroundSmashIcicleAssault::
  IsRemoveOnDeactivate MATCH (files renamed). Three raw PDBs each; 2 -> 1 spellings each; a 15-object vtable link: 0 duplicates.
- **Recheck** (`logs/recheck.log`, landed tree, which also carries nf245's CStringParser.h): 114 rows (the 108 equiv-differing, the ten respelled tests, the four q12681 rows, 0x0058578B): 10 MATCH + 104
  RELOCATION_MATCH, all PASS; three RELOC_CONFLICT warnings (0x0064BFBA, 0x0064E978, 0x006B42F0) are identical at HEAD (agent no-overlay run).

## Coordinator rulings on NF244's questions
1. Step-1 set: {SCREENPOS, CONVERTSCREENPOS, CONVERTWORLDPOS} accepted (proved over all 108 differing rows; the other triple only on 0x0058578B).
2. Slot 8 (`Virtual08_00b27d90`) stays the last placeholder in CIEngine.h; re-tuning 0x0058578B under all four declarations is authoring:
   queued, not run (no authoring agent).
3. Step 3 queued as a header claim (CEngine.h typed-member mode: PreloadCameras / PreloadStats / VideoOptions / FrameMemoryPool / the three
   CArrays, a nested CEngine::CPreloadStats, and a gate so slot 1 binds CInitBaseClass::Validate 0x0099A340, not CIEngine::Validate); then
   re-grade 0x00B260B0 / 0x00B261D0. Their locks are released.
4. 0x00743CB0's local declaration of the header-less ice-troll class on the shared CModeStandBase base: accepted (same shape as the
   landed 0x00743FE0 / 0x008D6850).
5. OnPreDeviceReset / OnPostDeviceReset access in the default branch, and the self rows 0x00B26010 / 0x00B25D00: queued (NAME_FIXES).
Rule slips reported by the agent (one foreground `sleep 60`, one refused variable-path `rm -rf`): noted; the next brief repeats the rules.
