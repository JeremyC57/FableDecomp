# Codex update review: rows_00 (276 addresses)

275 new rows plus 1 rewrite of an existing row (00692940 `std::_Dest_val` became `CCreatureActionBase::GetWorld`). I checked every oracle byte string in the update against the retail `Fable.exe`: all 276 are identical and every function ends cleanly (00557960 ends in its embedded switch table).

## Counts (our VC7.1/Wine lane, `grades_00.tsv`; raw run in `grades_00.raw.tsv`)

| grade | rows | test PASS (`<addr>_TEST PASS`) | test FAIL |
|---|---|---|---|
| MATCH | 11 | 11 (00705040 passes once linked with its source .obj, as the verifier does) | 0 |
| RELOCATION_MATCH | 265 | 258 | 7 (only the pass string differs) |
| DIFFER / SRC_FAIL | 0 | - | - |

3 rows need per-row flags that the catalog records: 00456dfb `/O1`, 00890c80 and 00890d80 `/GS`. With no flags they grade DIFFER; with those flags they grade RELOCATION_MATCH.

## Rows that failed something

| addr | problem | cause |
|---|---|---|
| 00446220, 004573fd, 00457ca0, 004c00e0, 00697e20, 00699560, 006dc450 | test FAIL | The test prints its own pass line, not `<addr>_TEST PASS`, and the catalog PassPattern matches that line (4 rows use just `PASS`). Each exe exits 0 with its asserts active, so behaviour is fine. |
| 00456dfb, 00890c80, 00890d80 | DIFFER at default flags | Need `/O1` or `/GS`. `/O1` is outside the verifier's flag sweep. |
| 00705040 | LINK_FAIL in qx | A qx limitation: the test needs the source object. Linked with it, the test passes. |
| 00433fe0 | wrong call targets | See B1. |
| 00555390, 007464a0 | wrong call targets | See B1. |

## Static audit

**BLOCKER**
- **B1: wrong call targets that RELOCATION_MATCH hides.** RELOCATION_MATCH ignores which function a call actually goes to.
  - 00433fe0: the two `Transfer` overloads are swapped. Retail calls `0x431061`, which is `Transfer(float&)` (it uses `fldz`/`fld`), for fields +3c..+48, and calls `0x431102` (`Transfer(long&)`) for +60/+64. The source emits the opposite. Its comment also contradicts 00456dfb, which is correct.
  - 00555390 and 007464a0: the source calls `::operator new` (`??2`, the import at 0xBFEA1A), but retail calls the `malloc` import at 0xBFEA0E. 007464a0's own comment names BFEA0E.
- **B2: 27 tests never call the candidate.** They exercise a second, simplified model, and several just check their own assignments:
  - 00433fe0, 00434450, 004554c6, 00457537, 004585fe, 00459615, 004642fa, 004643cf, 004c3d30
  - 004de122, 00519b80, 0051a900, 0056e6b6, 0056fe7c, 0058376c, 00633420, 006338d0, 006339b0
  - 00687310, 007b7a70, 007e3c70, 00845bc0, 0084bd40, 008929d0, 00892a80, 008c35d0, 008d0ba0

  The other 249 tests embed the exact source and check it with `CHECK` or `assert`, which are active because tests compile with `/Od` and no NDEBUG.
- None of `__asm`, `naked`, `_emit` or `__declspec(align` appears anywhere. No source or test is missing, all `engine/*.h` includes exist, and there are no duplicate catalog or oracle rows.

**WARNING**
- **`volatile`:** used in 00458033, 004581f8 and 00557960. The project already allows it (72 baseline sources use it, and GOTCHAS documents it).
- **Hard-coded retail addresses** (vtables, globals, strings) appear in 00457537, 00458033, 004581f8, 0047756c, 004c3d30, 00557960, 00687310, 008929d0 and 00892a80. 114 baseline sources do the same.
- **`#pragma pack` with no comment justifying the layout:** 00434450, 004573fd, 00457ca0, 00460657, 00471a77, 004c00e0, 005dc8b0.
- **Local struct where a header exists:** 27 sources, for example CRegion (00519b80, 0051a900), CPlayer (006339b0), CWorldMap (0063a540, 0063a850), CGameScriptInterface (008929d0, 00892a80) and CParentDefClassBase (00457537, 00458033, 004581f8).
- **Retail address missing from the source text:** 42 sources (it is in the filename only), for example 00446220, 008d1330..008d3cf0 and 00697e20.
- **The same retail callee gets different names in different sources:**
  - 0x80C460 is `CreateThing` in one and `CreateExplosion` in another; 0x80CBF0 is `AddExcludedThing` / `AddImmuneThing`.
  - 0xA0D2D0 is `RemoveFromInterface` / `Shutdown`; 0x833A10 is `SuspendAI` / `SetSuspendAnimation`.
  - One mangled `GetEvent` symbol points at two different functions (0x8D4A90 in 008d1330, 0x8D4CD0 in 008d15b0). That would clash in a real link.
- Every `// Leaf:` symbol matches the function it names.

## Identity spot-check (25 rows; I checked xrefs and RTTI against the retail exe)

| addr | Codex name | prior label | check | verdict |
|---|---|---|---|---|
| 00404970 | CIMEFont::GetFontNameAndScale | same | - | OK |
| 004083e0 | CUserProfileManager::GetConfigOptionsDefaultsDef | CManager::GetIconsDef | name exists in the FableWin PDB; called from the options getters at 4084b0/408500 | plausible, owner not stated with evidence |
| 0041d8e7 | NUISystem::CManager::GetMiscThingsDef | CManager::GetIconsDef | slot 42 of the CManager vtable 0x1230134, matching the PDB order Misc then Icons | OK, but the source does not cite the slot |
| 004573fd | CSpecialAbilitiesBerserkDef::~ | ~CPatchTesselationEdgeStrip | vtable 0x12341FC slot 0 (0x4573E1) calls it | OK |
| 00458aa7 | CHeroCombatDef::CHeroCombatDef | CEngineVSConstantLayout… | RTTI vtable 0x1234CF4 is stored by this ctor | OK |
| 00460657 | CSpecialAbilitiesDivineWrathDef::operator= | CParentDefClassBase::operator= | vtable 0x123487C slot 19 jumps here | OK |
| 004642fa | CThingBaseDef::operator= | same | - | OK |
| 004c00e0 | CTCGraphicAppearance ctor | CEngineSubPrimitive2DClockSprite | vtable 0x123986C is `NGraphicAppearance::CTCGraphicAppearance` | OK (namespace missing from the name) |
| 004de122 | CTrophyDef::Transfer | rtti_port | - | OK |
| 004e29ba | CParticleAttachmentInfoVector::operator= | CParticleMorphs | its only caller is in 004e299b | OK (descriptive name) |
| 0053633e | NUISystem::CList::ScrollToLastChild | same | - | OK |
| 005500b0 | NUISystem::CMesh::GetDataFromDef | CScrollingViewport | both callers store the CMesh vtable 0x1249F9C | OK |
| 00541120 | CDynamicList::RemoveChild | DeleteChild | slot 64 of the CDynamicList vtable 0x1246E44 | OK |
| 006380c0 | NThingMap::CThingMapManager::Clear | CShaderResource::CloseShaderBank | its only caller is in Init 00638540 | OK |
| 00692940 | CCreatureActionBase::GetWorld | std::_Dest_val | the bytes call 0xA01B50 then jump to 0x4C7AA0 | OK |
| 006dc450 | GFSerialiseMapBinaryOut<…> | CHeroExperienceDef::WriteToStream | in the PDB names; 4 callers | plausible |
| 0073e1d0 | CAnimFilterOf3Groups::Accepts | vfunc_0 | slot 0 confirmed, but the PDB names it `operator()(const CAnimationEntry&) const` | WARNING: name contradicts the PDB |
| 00705040 | NativeMatrix3x3::TransformVectorPair | ConvertVectorToLocal | invented; the source says the owner is unknown | WARNING |
| 0078e1f0 | CPairedIntelligentPointerRecord::DeletingDestructor | scalar_deleting_destructor | invented; the source says the owner is unknown | WARNING |
| 0084dfe0 | NCameraModes::CBase::Reset | CFirstPerson::OnDie | no call, jump or pointer reference anywhere | WARNING: cannot be verified |
| 008857e0 | …::GetRenderFrameInterpolatedRadialBlurParameters | SetAsAlliedWithFaction | in the PDB names; 1 caller | plausible |
| 0089cd70 | CGameScriptInterface::CanThingBe_Heard_ByOtherThing | rtti_port | in the FSE manifest | OK |
| 008a24c0 | CGameScriptInterface::GetWanderCentrePoint | rtti_port | in the FSE manifest | OK |
| 008cbb90 | CCreatureAction_SummonerFlameSlice::Initialise | rtti_port | - | OK |
| 008d3450 | CGameScriptThing::MsgHitEnemyWithRangedWeapon | rtti_port | - | OK |

## Tools verdict (`land_pending.sh`, `test_land_pending.py`)

The new lander is safe and is an improvement.

- **No duplicate catalog rows.** Rows that are already landed are kept in the batch payload, but `verify_and_land.py` (unchanged) still skips any address already in the catalog (line 464), and the oracle append skips duplicates.
- **Staged inputs are kept on every failure path.** That covers the verifier crashing or not finishing, zero wins, a partial landing, and a row missing from the catalog or the oracle. Each case exits 1 before the `rm`. The old lander deleted the staged files even when rows were rejected.
- **The test suite is green:** 9/9 pass under `unittest`, run from a scratch copy (pytest is not installed).

Minor issues:
- The "everything already landed" shortcut checks the oracle TSV, while the verifier checks the catalog. A row that is in the oracle but not the catalog is reported as incomplete and can't be landed through this script. Nothing is lost.
- If the same address appears twice in one payload it still produces two catalog blocks. That bug is older than this change.
- The tests' fake verifier does not model skipping already-landed rows.

## Recommendation

Land all except the listed rows. Hold the 3 B1 rows (00433fe0, 00555390, 007464a0) until their callees are fixed. Hold the 27 B2 rows until their tests call the real candidate. Fix the pass string in the 7 test-FAIL rows before landing; that is a one-line change each and they otherwise pass. 00433fe0 is in both B1 and B2, so 36 distinct rows are held and 240 are clean to land. Also fix the 0073e1d0 name. The lander change is fine to take.
