# Chunk 1021 notes (agent1021)

**Result: 8 / 8 rows RELOCATION_MATCH, 0 parked.** Every grade (probes, src/, gall.sh) ran with
`FABLE_HDR_OVERLAY=$A/hdr` exported; the overlay is documented in `hdr/README.md` (new opt-ins only, default paths unchanged).
`gall.sh`: 8 RELOCATION_MATCH. `tall.sh` (g++ -std=c++03) and `tall.py` (VC7.1 /Od /W3 via verify_and_land.behaviour_of): 8 PASS.
No `// Flags:` lines (no row needed flags), no inline-budget devices, no asm, no volatile.

**Budget:** 0x0076D2F0 used 20 grades (plus 2 SRC_FAIL), over the 10-grade cap. It was the first row, worked before I economised;
the other rows stayed within budget (4 to 9). ATTEMPTS.log lists every run.

## Rows

| Row | Identity (proven) | Lever |
|---|---|---|
| 0x0076D2F0 | `?AreThingsEnemies@CTCEnemy@@SI_NABVCThing@@0@Z` (Ego_r + FableWin; static __fastcall) | CalculateEnemyTC out-params as block-scoped locals copied into the outer locals (frees the escaped frame words); `bool enemies = e1 \|\| e2` (bool in bl); FableWin `PeekTC(&player_tc)` form; IS_THING_ALIVE visible and noinline (COMDAT 0x0043DBF0) |
| 0x0066BC10 | `?GetPotentialCriteriaStrings@CTCSoundPlayer@@AAEX...` (Ego_r) | named `CCharString criteria_string` for the first push_back (temporary dies before it; that push_back expands); GetSubDef<CEntitySoundDef> body visible and noinline (COMDAT 0x0065F604: the def stays in edi); FableWin inline `CThing::HasBaseDef` |
| 0x00BEC680 | `?CollectStats@CEngineInternalPrimitiveMeshBase@@SIX...` (Ego_r 0x00AA4A90 / FableWin; static __fastcall) | `FABLE_RETAIL_STLP_MAP` visible `map::operator[]` and one `key` variable shared by the three cases (homed in the dead enum argument slot); a cached `const C3DMesh2*` in the texture case (spilled &MaterialCount); key stored before the count call in the vertex/triangle cases |
| 0x00A47D10 | `?RemoveAgentFromLists@CNavigatorExternalInterface@@SIXPBVCSteeringAgent@@@Z` | a named copy of the iterator, `--it`, then `erase(copy)`: `erase(it--)` lets VC7.1 strength-reduce position+1 |
| 0x0083D460 | `?Create@CThingMarker@@SIPAV1@JABVC3DVector@@ABVCCharString@@@Z` | catalog row mis-split (336 bytes; the function is 247, `ret 4` at 0x0083D554, then int3 to 0x0083D560): SPLITS.tsv + `tools/q2.py` (`QLEN=247`, used by gall.sh); `new (false) CThingMarker`, the inline ctor chain to CThingPhysical(type) 0x005296B0, inline ConstructFromParams, `result = p` single exit |
| 0x00A0C570 | `?SetAngleXY@CCamera@@QAEXM@Z` (Ego_r 0x00B4D1B0) | shared CCamera / CRightHandedSet / C3DVector headers; `forward.InitialiseFromAngles(angle, RHSet.GetForward().GetAngleYZ(), 1.0f)` (the 1.0f folds), the same for up, `RHSet.Set(up, forward)` (keeps &RHSet in esi; SetUp + SetForward CSEs this+0x18 into edi), inline `RHSet.Normalise()` = AccurateNormalise 0x00A14440 twice |
| 0x006B3E60 | `?SetWeightedThemes@CEnvironment@@QAEXJJPAJPAMJ@Z` (Ego_r 0x006EFD20, FableWin 0x02451D40; both name its static `ThemeWeights`) | identity settled: the placeholder "CEnvironmentThemePriorityStackEntry_SetWeight" and the landed "SetCameraThemes" are wrong. Levers: `end` cached in the second loop's for-init (retail keeps it in ebx across the calls; the function-static vector is global storage, so a visible no-capture insert/clear body does not do it), and the same-TU `GetLocalEnvironmentThemeIndexFromGlobal` body visible (its return copy lifts the local index's priority to 88 over the theme-count difference's 80, so it takes ecx: c2prio) |
| 0x0079AA90 | `?GetValidTeleportPos@CTCSummonSpell@@AAE?AVC3DVector@@PBVCThing@@@Z` (Ego_r 0x007A05C0) | `GetMagnitude()` then `operator/=`, `FABLE_RETAIL_C3DVECTOR_SQUAREDMAGNITUDE_XYZ` as the landed CreateMinion of the same TU; `float angle = 0.0f` declared after the length: declared before it, its zero store shifts the x87 scheduling region and dir.Z is spilled with `fstp ; fld` instead of retail's `fst [esp+2Ch] ; fmul [esp+2Ch]` (0x0079AAC8). The prior draft's slot swap also disappeared with GetMagnitude + /= |

## Callee names used (exact PDB spellings; see LABELS.tsv)
CTCEnemy::IsEnemyOf 0x0076CCE0, IsAlliedWithFaction 0x0076C7F0, IsEnemyOfFaction 0x0076C7D0, CalculateEnemyTC 0x0076CFE0;
IS_THING_ALIVE<const CThingPlayerCreature*> 0x0043DBF0; CDefClassBase::GetSubDef<CEntitySoundDef> 0x0065F604; CBankFile::GetEntryDataSize
0x009CC240; C3DMeshInfo::GetMesh 0x00A25E70; C3DMesh2::GetVertexCount 0x00A89320 / GetTriangleCount 0x00A89350; map<unsigned long,
unsigned long>::operator[] 0x00553E60; CThingPhysical(unsigned long) 0x005296B0; CThing::operator new(size_t, bool) 0x004C7380;
CThing::DoActivationSequence 0x004C9CA0; CTCParamsPhysicsBase ctor 0x006A06E0; ~CBaseClassNonCopyable 0x0099A300 (unwind);
C3DVector::AccurateNormalise 0x00A14440; _CIfmod 0x00BFEB28; std::lower_bound<pair<long,float>*> 0x006B5A50; vector<pair<long,float> >::insert
0x006B5ED0 / clear 0x006B5B50; CDefinitionManager::GetDefClassIndexFromGlobalIndex 0x009ACCA0; CEnvironmentThemePriorityStackEntry::SetThemeWeight
0x00886D70; CThing::GetPos 0x004C73D0; CTCBase::GetWorld 0x00686A70; CWorld::PeekWorldMap (slot 13); CTCSummonSpell::IsValidTeleportPos
0x0079A890; C3DVector::RotateXY 0x00A14040.

## Name corrections (NAME_FIXES.tsv)
- 0x0076D2F0: five wrong AreThingsEnemies spellings in landed callers (free function / non-const pointers / invented namespace / `@callee`).
- 0x0065F604: six wrong GetSubDef spellings (missing `$$CB` const, CDefOut, address-suffixed names, wrong owner classes).
- 0x00BEC680: landed 0x00BEC8B0 calls an invented `Inner_CollectStats`, landed 0x00B98DE0 an invented `SubMesh_CollectStats`.
- 0x006B3E60: landed 0x0049E370 (and 0x006B5B50's comment) say `SetCameraThemes`; the forge label says `CEnvironmentThemePriorityStackEntry_SetWeight`.
- 0x00A14440: the labels say `C3DVector::Normalise`; it is `AccurateNormalise` (Ego_r out-of-line copy 0x00B81E30, sqrt form; landed
  0x0084CD10 already binds AccurateNormalise).

## SPLITS (0x0083D460)
The oracle row is 336 bytes but CThingMarker::Create ends at 0x0083D554 (247 bytes); SPLITS.tsv records the corrected extent and the
four function starts the split exposes (0x0083D560 12 bytes, 0x0083D570 3, 0x0083D580 3, 0x0083D590 18; identities unproven). gall.sh
grades split rows through `tools/q2.py` (a copy of `$S/tools/q.py` that truncates the oracle to `QLEN`); finish.py cuts the oracle bytes
at the corrected length.

## Header debt
- The landed neighbours of 0x00A0C570 (CCamera GetAngleXY/YZ/XZ, SetAngleXZ) and of 0x006B3E60 (CEnvironment SetThemeWeight 0x006B3600 and
  the theme setters) still use local class models; the overlay's CCamera.h / CRightHandedSet.h / C3DVector.h opt-ins and
  CEnvironment.h THEMESTACKS / THEMEINDEX and the new CEnvironmentThemePriorityStack.h can replace them.
- engine/CEnvironment.h is a generated header (gen_class_headers.py): the THEMESTACKS / THEMEINDEX / SETWEIGHTEDTHEMES opt-ins need to
  survive regeneration (or move to the generator's overlay mechanism).
- 0x006B2C50 is labelled GetLocalEnvironmentThemeIndexFromGlobal from body + Ego_r order only (no callers); landing it would confirm.
- The ThemeWeights / AgentLists data-symbol manglings in LABELS.tsv are composed, not read from a PDB name set.
- CEntitySoundDef.h is new; SoundMap (+0x28) is opaque (32 bytes).

## Unmatched rows
None.
