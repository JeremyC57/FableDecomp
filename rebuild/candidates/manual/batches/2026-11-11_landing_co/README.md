# 2026-11-11 landing co -- chunk Q9 (parked near misses, Opus 5.5): 4 rows, five new headers

vland --land: 4 wins, every test PASS.
- **0x009F3010 CSurface::ReadOnlyLock** (MATCH).
  - Written like its landed byte-twin CSurface::Lock 0x009F33E0, with GetDimensions defined in the same file.
  - The failure arm builds C2DExtentsI through Initialise (new opt-in FABLE_RETAIL_C2DEXTENTSI_INITIALISE).
- **0x006AB430 CThingPlayerCreature::GetMovementBandTypeFromJoytickMagnitude** (RELOCATION_MATCH).
  - An else-if chain ending `else return 3;`, and the `PeekTC(id, &ptr)` out-parameter form.
  - FABLE_RETAIL_CTHING_ORIGINAL_VTABLE puts the virtual call at slot 81.
- **0x00E1CAE0 NScript::CQ_WizardBattleScript::MonitorMazePhases** (RELOCATION_MATCH).
  - A pure inline-budget residue: `// Flags: /d2inlT138` plus a scoped `#pragma inline_depth(0)`, owner ladder, noted on line 2.
  - New header: CQ_WizardBattleScript.h.
- **0x0092B900 CAIStateGroup_OnHeroPush::Update** (RELOCATION_MATCH).
  - Moved off a local model, with new headers CAIStateGroup_OnHeroPush.h and CTCOnHeroPush.h.
  - New opt-ins C3DVECTOR_NORMALISE_LOCALS and CCREATUREACTION_PLAYANIMATION_IMPLICIT_DTOR.
  - vland's RELOC_ALIAS warning is the MSVCR71 `_CIfmod` import thunk.

**Headers**
- hdrcheck2 reports default=same for every changed header, and each is a line-superset of the repo copy.
  - The two lines it reports as "removed" in C3DVector.h are conditions extended for MAGNITUDE2D; the old opt-ins still read same.
  - Every opt-in that DIFFs is new, and no landed row uses it.
- New headers: NFractal_CNoise.h, CTCOnHeroPush.h, CAIStateGroup_OnHeroPush.h, CPlayerMovementDef.h, CTCHeroMorph.h,
  CQ_WizardBattleScript.h.
  - NFractal_CNoise.h and CTCHeroMorph.h are used only by differ rows.

**Names**
- Retail global 0x0138CBA8 had two landed descriptive names. `g_LibraryLookTurnSpeed` (0x0092AE70, 0x0092A8A0) is respelled to
  `g_LookAtTurnSpeed` (already used by 0x0092AFE0).
  - Neither PDB names it. Every use is the turn speed passed to a LookAt (map, leaderboard, practice, library).
  - Both rows recheck RELOCATION_MATCH with tests PASS (`logs/vland_recheck_global_respell.log`). The bindings now hold one name.
- NAME_FIXES_QUEUE (land1011co), all three in both PDBs:
  - 0x00F35E00 ?InitPerlinNoise@CNoise@NFractal@@AAEXXZ
  - 0x009F2F90 ?GetDimensions@CSurface@@QBE?AVC2DExtentsI@@XZ
  - 0x009F33E0 ?Lock@CSurface@@QAE?AVCSurfaceLockedRect@@K@Z
  - The last two landed under the `CSurface_Methods` stand-in.
- Labels: ghidra_out/labels_landing_2026-11-11co.tsv.

**Header debt:** CTCOnHeroPush.h and CQ_WizardBattleScript.h can replace the local models in landed 0x008328D0 and
0x00E1C4D0 / C580 / CD40 / CFD0.

The chunk record is in pending/2026-11-11_linux_handQ9: 36 near misses, the closest 0x0092ACE0 GoToLibrary at 389v389 with 2 hunks.
