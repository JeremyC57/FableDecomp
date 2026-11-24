# Landing nf240 (Account A, 2026-11-22): CurrentRenderFrame, IsThingCarryingRangedWeapon, the CTCInventoryWeapons pair (12 of 15 rows)

Agent NF240 (beside NF241). Hand-back: `pending/2026-11-22_nf240_handback/`. q12673 done but for three blocked 0x006A1620 binders; L38 / L39
done; q12674 HELD (below); q12675 step 1 surveyed; q12676 / q12659 not started.
- **Recheck** (`logs/recheck.log`, landed tree): 12 rows, all RELOCATION_MATCH + PASS, 0 RELOC_CONFLICT.
- **0x0143B8D8** `?CurrentRenderFrame@CEngineInternalPrimitiveBase@@0GA` (private static, three PDBs): reached through the public inline
  accessors SetAsRendered / IncrementFrameCounter (FableWin + Ego_d; FrameStart's `inc word ptr`). Header: add-only
  `FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_CURRENTRENDERFRAME` (agent's equivalence over the 220 rows of its include closure: 0
  differ). 0x00B32660 / 0x00B92C20 on the header; 0x00B91760 / 0x00B92500 / 0x00B92790 / 0x00B943F0 in place (their local models gain the
  two accessors; ruling 1); 0x00B3AAA0's self leaf `?WasRendered@CEngineInternalPrimitiveBase@@MBE_NXZ` with a linked test. The two new
  inline copies equal retail's out-of-line copies 0x00B31D30 / 0x00B91620. Table: 2 -> 1 spelling; the held p27 line restored.
- **0x006A1620** `?IsThingCarryingRangedWeapon@CTCCarrying@@SI_NABVCThing@@@Z`: 0x00664A50, 0x0068F9D0, 0x0091F050 on CTCCarrying.h
  (comments calling it the melee function corrected); table 6 -> 4 spellings, the held p27 line restored (ruling 2). Blocked:
  0x00664A10 (needs CThingCreatureBase.h), 0x00890250 / 0x00890D80 (Entity / CThingNative stand-ins: re-author).
- **0x005C2DED / 0x005C2DF5** renamed CTCInventoryWeapons::IsMeleeWeaponDrawn / IsRangedWeaponDrawn (from the `Forward_...` placeholders):
  files, catalog, manifest; linked tests; HEAD's LNK4006 on their shared stand-in name is gone.

## HELD: q12674 (0x00740040 CModeSitBench ctor + slot rows 0x00739E70 / 0x00739E90)
It grades (the ctor's ??_G equals 0x007400E0 under the new NCreatureMode_CModeSitFloor.h `_IMPLICIT_DTOR` opt-in; all 12 vtable slots on PDB
names), but `?IsAllowingPlayerSpecialMovementMoves@CModeBase@CTCCreatureModeManager@@UBE_NXZ` would then bind 0x00739E90 here and
0x00739EA0 in nine local-model rows that swap vtable slots 9 / 11 (retail: CModeCarryingWeaponStand::IsAllowing 0x008E35F0 at slot 9,
CModeStandBase's in all 70 vtables): one name at two addresses. It lands as one unit with that nine-row swap (queued). Its rows and header
stay in the hand-back folder.

## Coordinator rulings on NF240's questions
1. In-place accessors on the four local-model rows: accepted (their only new COMDATs equal retail's copies; the in-place rule).
2. 0x006A1620's held line: restored now (count-reducing, 6 -> 4).
3. CAnimFilterBase's ??_G v 0x0041CCE4 (`pop ecx` v `add esp,4`): debt, identical in landed 0x00743600 (goes with q12674).
4. 0x00740040's linked test: queued with q12674.
5. CIEngine.h PDB declarations as the default: yes, as one header unit with the CEngine ctor / dtor (148 includers, ten tests renamed).
6. q12674: held until the nine-row slot-9 / slot-11 swap lands with it (above).
