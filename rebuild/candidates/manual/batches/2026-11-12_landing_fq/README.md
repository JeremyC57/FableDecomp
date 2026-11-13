# 2026-11-13 landing fq -- author workflow wf21 (Opus only): nine of ten rows landed, one in repair

Source: `pending/2026-11-12_linux_handWF21/` (workflow `lane_tools/workflows/author_rows.js`, 60 agents, all Opus; no row needed
Fable). The run was cut by a container restart with all ten rows matched and was resumed from its journal. Landed (all
RELOCATION_MATCH, test PASS under VC7.1, LANDVERIFY_STRICT_HEADERS=1): 0x004CE1B0 CSubtitledMessage::AddMasterShotCameraTagFromText,
0x006A5610 CThingPhysical::GetFirstCylinderCollisionBetweenPoints, 0x007D71E0 CTCTavernGameCardPairs::ShuffleCards, 0x00E83230
NScript::CV_FisticuffsClubScript::CFisticuffsClubRing::SetRelevantRegion (was catalogued as a global HandleVillageSelection),
0x00B4AC10 CEnginePrimitiveRenderer2D::CEnginePrimitiveRenderer2D, 0x00973DC0 CAIStateGroup_JackOfBladesBase::SummonPillars,
0x00875B10 NCameraModes::CTools::TestMeshForOccupationAndGetType, 0x00E6B900 NScript::CV_ChapelOfEvilScript::TeleportAcolytes,
0x005EC270 CTCTavernGame::ProcessInput.

**Not landed yet:** 0x004093E0 CUserProfileManager::SetDefaultValuesVideo matches (three author passes); it carries inline callee
bodies whose provenance is not stated or proven against their own addresses, an ICF name question at 0x00406880, and two test
cases that cannot fail. It is in a repair + two-verifier workflow.

**Inferred statement (0x00E83230):** one assignment sits in a single-site `do { } while (0)` so VC7.1 expands the implicit
CScriptThing operator= at that site only, as retail does (the plain form matches only under /d2inlT100); marked `Inferred:`.
**Coordinator fixes at landing (regraded: match, test PASS):**
- 0x00B4AC10: the overlay said the CEnginePrimitiveManager2DParticleGroup constructor had no PDB symbol; FableWin names it
  (0x02D9D090, retail 0x00B4B690). Comment corrected; three name fixes with their evidence added to NAME_FIXES / NOTES.
- 0x00E6B900: the inline operator== is a respelling of its landed row (which uses the made-up `IsSameThing`), not a copy; the
  comment says so, an Inline-budget line covers the forced-inline empty destructor, and the landed 0x008AAE20 is handed back.
**Landing-verifier warning recorded:** 0x00B4AC10 binds `g_TwoPi` to 0x0128F608 (value 6.2831855). The landed 0x00B314E0 has the
two names swapped (`g_TwoPi` on 0x01238020 = 360.0, `g_DegreesPerTurn` on 0x0128F608); queued as a name fix. The `_$E2` warning
is a compiler-generated initialiser name shared by unrelated TUs.
**Mechanical checks:** reserved_check 0 (0x009B9E00, 0x00BE1E20 are cited in comments only), purity clean. `LOCAL MODEL` flag on
0x00E83230 is the out-of-class definition of the nested CFisticuffsClubRing that the shared header declares, not a second model.
Pragmas: `auto_inline(off)` around real same-TU callee bodies; `inline_depth(0)` around the script early returns in 0x00E6B900
(102 landed rows use that spelling).
**Headers** (11; additions under new opt-ins, unused elsewhere, defaults unchanged per hdrcheck2): CThingPhysical.h,
CTavernGameCardBaseDef.h, CEnginePrimitiveManager2DSprites.h, CEnginePrimitiveManager2DParticleGroup.h, CEngineDef.h,
CEnginePrimitiveRenderer2D.h, CAIStateGroup_JackOfBladesBase.h, NCameraModes_CTools.h, C3DVector.h (SCALE_NAMED_RESULT; 3-way
merge), CInputProcessTavernGame.h, CTCTavernGame.h.
Checks: bindings_multi_check at baseline; funclet_includes_check 1317 checked, 0 broken.
Queued (land1011fq): the rows' name fixes, 0x00B314E0 (swapped constant names), 0x007D7860 random_shuffle, 0x00A0D390, 0x00687D00.
