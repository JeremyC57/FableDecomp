# NF234 header overlay (2026-11-22): add-only, member-specific opt-ins (entries 1 and 4)

Base copies: `orig/engine/{CCountedPointer,CFrontendGameComponent,CEngineInterpolationInfo}.h` (the repo tree with nf233 landed). Other overlays
(`ls $S/w22/*/hdr/engine/<h>`): CCountedPointer.h only in older NF62 / NF73 / NF76 / NF222 / NF225 copies (all landed since), CFrontendGameComponent.h
in NF226 (landed), CEngineInterpolationInfo.h in NF233 (landed = repo); none in AUTH23 / C2T2. `grep -rlw` of the five new macros over rebuild/ and every
other `$S/w22/*/hdr` (work/macros.txt): no hit, except SHUTDOWN in NF233's two drafts (pending/2026-11-22_nf233_handback/work/e2/). Default paths are token-identical (every change is inside a new `#ifdef` / `#if defined`
term; the old text is the `#else`); no layout change.

- `FABLE_RETAIL_CFRONTENDGAMECOMPONENT_SHUTDOWN` (engine/CFrontendGameComponent.h): NF233's drafted layout branch (pending/2026-11-22_nf233_handback/
  work/e2), unchanged: Ego_r.pdb 0x0049E380 + FableWin.pdb 0x01A41790 ?Shutdown@CFrontendGameComponent@@AAEXXZ (private) and the typed members
  PTempGraphics +0x1DFC, MainFont +0x1E04, ProfileNames / SaveNames / WorldNames (STLport vector<CWideString>), the two offsets, SaveToLoad.
- `FABLE_RETAIL_CCOUNTEDPOINTER_CGRAPHICDATABANK_RELEASEDATA_SIZE_BODY` (engine/CCountedPointer.h, the approved form): explicit
  specialization of ReleaseData<CGraphicDataBank> under the existing size block's optimize("s", on) (the Data-first RELEASEDATA body) =
  owner 0x00419108 (owner row sets optimize("s", on) + FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA). Mirrors the landed CONSTCFONTBANK one.
- `FABLE_RETAIL_CCOUNTEDPOINTER_CGRAPHICDATABANK_RESET_SIZE_BODY` / `..._CONSTCFONTBANK_RESET_SIZE_BODY` (CCountedPointer.h; beyond the
  approved pair: decision 1): explicit specializations of the default Reset body in the same block = owners 0x004194D1 / 0x0042FC13 (59 B
  each, calling ReleaseData 0x00419108 / 0x0042F948 and storing DeleteData 0x00419036 / 0x0042F864; owner rows set optimize("s", on)).
  The size mode keeps ReleaseData called in the copy while the /O2 caller still expands both.
- `FABLE_RETAIL_CENGINEINTERPOLATIONINFO_NESTED_SET` (engine/CEngineInterpolationInfo.h; the header claim of q12646 / q12649): the set class
  nested in CEngineInterpolationInfo, as all three PDBs spell it (Ego_d.pdb + FableWin.pdb ??0CEngineInterpolationInfoSet@CEngineInterpolationInfo@@QAE@XZ
  and its operator=, ??4..@@QAEABV01@ABVCInterpolationInfoSet@CInterpolationInfo@@@Z; the type name CEngineInterpolationInfo::CEngineInterpolationInfoSet
  in Ego_r / FableWin / Ego_d; no top-level VCEngineInterpolationInfoSet@@ in any PDB), plus
  `typedef CEngineInterpolationInfo::CEngineInterpolationInfoSet CEngineInterpolationInfoSet;` so CNewFrontendGameComponent.h's m_LastInterp
  keeps compiling. The NF233 CTOR opt-in is repeated inside the nested class. Same members and offsets.

Users: rows/00495780.cpp (SHUTDOWN + RELEASEDATA + CONSTCFONTBANK_RELEASEDATA_SIZE_BODY + the three NF234 CCountedPointer opt-ins +
DELETEDATA_DECL_ONLY), rows/0042ea8f.cpp (NESTED_SET). Each includes the overlaid header itself before any header that includes it (NF213 lesson).

Proof (work/):
- pin_asserts.py rebuild/include hdr (three headers): pinned 0 each (work/pin.log); the only assert in a changed region, CEngineInterpolationInfo.h's,
  is already FABLE_STATIC_ASSERT_AT(24, ...).
- assert_names_check.py rebuild/include hdr: mismatches 0 (work/names.log).
- equiv_check.py rebuild/include v work/t/include (= rebuild/include + this overlay; work/t/src links rebuild/src) over all landed rows:
  0 of 57,023 landed rows differ (sources + tests; work/equiv.log, work/equiv.tsv empty), so no landed includer changes (direct landed includers:
  CCountedPointer.h 8,392, CFrontendGameComponent.h 7, CEngineInterpolationInfo.h 6 (work/hdr_includers.txt); none defines a new macro).
