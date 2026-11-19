# 2026-11-19 header pass: q11441 and q11272 (coordinator, claimed header items)

Advisor decision (owner delegated): the round-8 header items are the coordinator's, done as one claimed pass between landings
(COORDINATION_CLAIMS 2026-11-19; B cleared q11441 in COORDINATION_LOG and had no overlay of either header).

- **q11441, engine/CGameNavigatorManagerExternalInterface.h:** the default branch's slot-1 placeholder `virtual void Initialise();` is
  retired; `GetNavigationInformation(const C2DBoxF&, CNavigationAreaUpdate&) const` (pure in CNavigatorManagerExternalInterface, the
  0x006B2960 override in CGameNavigatorManagerExternalInterface) is now unconditional. Evidence (hand3c b7..b12): Ego_r field list 0x32FC4
  (vftable offset 4, pure intro virtual), retail vtable 0x01238C58 slot 1 = 0x006B2960, FableWin
  `?GetNavigationInformation@CGameNavigatorManagerExternalInterface@@UBEXABVC2DBoxF@@AAVCNavigationAreaUpdate@@@Z`. The old opt-in macro is
  now a no-op. No landed row named `Initialise` through this interface.
- **q11272, engine/CTexture.h `FABLE_RETAIL_CTEXTURE_LEVELBYTELENGTH`:** the declaration is now `long GetLevelByteLength(long) const`
  (`?GetLevelByteLength@CTexture@@QBEJJ@Z`): Ego_r.pdb, Ego_d.pdb and FableWin.pdb all spell it so (string search of the three PDBs on
  2026-11-19); the opt-in's `QBEKK` claim was wrong. Its other user 0x009FB750 defines the same function in its TU and now spells it the
  same way (the arithmetic is on the unsigned D3DSURFACE_DESC fields either way). No row defines both LEVELBYTELENGTH and GETLEVELBYTELENGTH.

## Checks
- The ten rows the edits touch directly (0x009FB750, 0x009F9E80, 0x00B2BD80, 0x009FD830, 0x006B2960, 0x004A6E30, 0x004AB210,
  0x004A9260, 0x0049CDD0, 0x00A175A0): RELOCATION_MATCH, every test PASS (`logs/direct_rows.log`).
- `lane_merge/assert_names_check.py`: 0 mismatches. `naming/rebind.py` on those rows; `bindings_multi_check.py` at baseline.
- Includer recheck, direct and transitive (default branch changed, so the token-identity shortcut does not apply): 686 rows, 685 good;
  0x007E2A60 is the known on-hold row.
  `logs/includer_recheck.txt`.
