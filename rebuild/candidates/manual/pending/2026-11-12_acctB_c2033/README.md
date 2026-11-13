# acctB hand-back, chunk 2033 (2 reserved rows: 2 matched)

Authored by an Opus 5.5 agent, re-verified by the acctB local coordinator. Nothing is landed.

| address | result | notes |
|---|---|---|
| 0x0069AE80 CGameCameraManager::CGameCameraManager(const CWorld&) (vtable 0x0125C754 stored first) | RELOCATION_MATCH 1923v1923, test PASS (g++ and VC7.1) | levers: the whole STLport map-insert chain visible (existing repo opt-ins `FABLE_RETAIL_STLP_MAP` + `_MAP_ORIGINAL_INSERT_BODY`); the by-name GetDef<T> body visible (`FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_NAME_BODY`); a same-TU body of SetCameraManagerSetDef 0x0069AC90 under `#pragma auto_inline(off)` + `__declspec(noinline)` (inside `#ifdef _MSC_VER`) so the constructor's set-def local shares the dead parameter word (frame 0x28 -> 0x24); `1.0f / 3.0f` (0x3EAAAAAB). 3 grades |
| 0x00A4C5E0 C3DAnimationSequenceData::ReadCompressedFromFile(CDataInputStream&, CSimpleMemoryPool&) | RELOCATION_MATCH 1844v1844, test PASS (g++ and VC7.1) | plain C++, matched at the first grade. **True length 1844, not the reserved 1927** (SPLITS.tsv): `ret 8` at 0x00A4CD11, int3, then an uncatalogued 71-byte function at 0x00A4CD20 (no xrefs; possibly `C3DAnimationSequenceData::IsScalingNonUniform(const C3DVector&)` from Ego_r -- one source only, a candidate label) |

Coordinator re-verification (cl 13.10.3077 under Wine, `FABLE_HDR_OVERLAY=hdr/`): `coordverify/` (grades, both test runs,
read-only vland: 2 wins, no RELOC_IDENTITY / RELOC_CONFLICT; 3 RELOC_NAME warnings covered by NAME_FIXES.tsv). Purity and
padded-struct greps are clean.

Review items for landing:

- **Same-TU body of 0x0069AC90 in row 0x0069AE80** is there only to be visible to the constructor; the agent says it is
  NOT byte-exact with retail 0x0069AC90 (landed separately): grade only the constructor leaf, and decide whether a
  non-matching visible body is acceptable or the landed 0x0069AC90 body should be included instead.
- **Row-local class declarations (header debt):** NCameraModes::CFirstPerson, CLookAtFront, CSACCombat, CPCMouseLook,
  CZTargetNew (and CProjectileWeapon) have no shared header; the row declares only their nested `<Mode>Init` class and two
  inline statics, no data members. They want shared headers at landing.
- **Descriptive names:** `g_PreloadScriptedCameras` (0x013BA8D4) and `g_CameraManagerDebugFlag` (0x013BA8D5) -- no name
  source exists; NCameraModes::EMode values are written as `static_cast<EMode>(N)` (no enumerator names anywhere).
- **NAME_FIXES.tsv (7):** 0x00878A50 `NCameraModes::CProjectileWeapon::StatRegisterMode` (landed as
  `CameraState::ResetCameraInputFlag`); 0x0069BBA0 `CCountedPointer<NCameraModes::CBaseInit>::DeleteData`; 0x0069D900 the
  STLport `_Rb_tree<...>::insert_unique` for RegisteredModes; 0x00637650 / 0x004AB990 the by-name
  `CDefinitionManager::GetDef<T>` instances; 0x0069AC90 SetCameraManagerSetDef takes a const-T pointer; 0x00877160
  `NCameraModes::CTools::CTools(const CGameCameraManager&)` (landed as a CGameflowAssistanceScript ctor).

To land: `hdr/` (table in `hdr/README.md`): opt-ins `FABLE_RETAIL_CGAMECAMERAMANAGER_FULL` (full class, sizeof 0x160
asserted), `FABLE_RETAIL_CCAMERA_CTORS`, `FABLE_RETAIL_NCAMERAMODES_CTOOLS_CTOR`, `NCameraModes_CStringAndCage` `_INIT`,
`C3DAnimationSequenceData` `_READ` (this copy is based on r3001's overlay: merge after r3001); new
`NCameraModes_CBaseInit.h` and `CCameraManagerSetDef.h` (replaces the quarantined generated header, same offsets). Rebuild the
oracle from Fable.exe (no retail bytes are in this folder).
