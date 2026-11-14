# acctC hand-back c40a (agentC40, 2026-11-14)

| address | name | bytes | grade | test |
|---|---|---|---|---|
| 0x00BD2D90 | CEngineInternalPrimitiveAnimatedMesh::GetBoneTransformations | 721 | RELOCATION_MATCH | PASS |
| 0x007DB2E0 | CTCTavernGameCardBlackjack::NewRound (catalog: CTCTavernGameCardBase_NewGame) | 723 | RELOCATION_MATCH | PASS |
| 0x00A3E830 | CSoundManagerLH::SetMonitorAllowedPostStopTailDurationMS (catalog: vfunc_35) | 24 | MATCH | PASS |
| 0x00A3E520 | CSoundManagerLH::StopMusic (catalog: vfunc_18) | 24 | MATCH | PASS |
| 0x00ED14E0 | NScript::CV_SingingStonesScript::CSingingStone::Init | 512 | RELOCATION_MATCH | PASS |
| 0x00BBDDD0 | CEnginePrimitiveManagerParticleSpriteTrailGroup::RenderPrimitive | 699 | RELOCATION_MATCH | PASS |

Verified by the coordinator with `lane/coordverify.sh` (read-only vland, strict headers): `coordverify.log`.

REVIEW ITEMS:
- Reserved row 0x00564D5D was re-cut to 0x00564D50 `std::char_traits<char>::copy` (42 bytes, MATCH / PASS): now staged in
  `2026-11-13_acctC_c40b/`; what was changed in the manifest is in `2026-11-13_acctC/RECUTS.md`.
- 0x00BD2D90: three named locals (`mesh_pointer`, `animation_manager`, `CMatrix4x4& transform`) are FITTED: VC7.1 orders the
  four x87 products of each matrix element by symbol numbering and a sweep of natural variants found the set that gives
  retail's order. `AlignedPreMultiplyTransposed` (FableWin helper) is force-inlined on byte evidence (precedent:
  `..._SETMULTIPLYTRANSPOSED_FORCEINLINE`).
- 0x00BBDDD0: `GetAlphaIncludingDistanceFade` is force-inlined although retail also has an out-of-line copy (0x00BA7B30):
  it contains the asm float-to-int kernel, and a plain inline with asm inside is called, not expanded. Byte evidence only.
- libpk sound interface (`libpk/CSoundSystem.h`): all names descriptive (no PDB covers libpk); landed rows spell slot 6 both
  `GetEventManager` (19 rows) and `GetPlayer` (6 rows): this header uses GetEventManager for slot 6 and GetPlayer for slot 5.
  Unknown slots keep the landed `vNN` spelling, which the made-up-names rule may not accept in a shared header.
- Catalog lengths of the two CSoundManagerLH rows (752 / 736) were wrong: each is a 24-byte function.
- Name fixes in NAME_FIXES.tsv / LABELS.tsv (incl. 0x00875690 = NCameraModes::CGeneralCircle::CGeneralCircle).
