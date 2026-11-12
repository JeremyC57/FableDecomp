# acctB hand-back, chunk 2032 (2 reserved rows: 1 matched, 1 parked after pass 1)

Authored by an Opus 5.5 agent, re-verified by the acctB local coordinator. Nothing is landed.

| address | result | notes |
|---|---|---|
| 0x009C47A0 **CFileInstaller::AddFile** (the catalog label "AddFilteredFiles" is made up; `?AddFile@CFileInstaller@@QAEXABVCCharString@@0PBV?$CArray@VCCharString@@@@_N@Z`, Ego_r + FableWin) | RELOCATION_MATCH 1915v1915, test PASS (g++ and VC7.1) | `// Inline-budget:` line: header opt-ins `FABLE_RETAIL_CWIDESTRING_FORCEINLINE_EQ` + `..._DATA_TEXT_EQ` force both layers of `CWideString ==` at the `filename == L""` site (no `/d2inlT` fits: push_back scores 77 and the CFileInfo ctor 76 and must stay calls) -- review at landing. Other levers: iterator taken inside the `filters != NULL &&` guard (loop at frequency 0.25), same-TU STLport `insert_unique` body (retail's 0x74 frame), inline STLport `operator==`. 8 grades |
| 0x00BA6FF0 CEnginePrimitiveManagerStaticMeshesBase::DrawSilhouette | parked, DIFFER 1904v1907, 447 of 514 instructions exact | root cause: the radius chain (`fld [ecx+34h] ; fmul [ebp+58h] ; fstp [esp+0Ch]`) is computed before the squared-distance sum in ours (0x00BA70FD..07), after the fsqrt in retail (0x00BA7119..1F) -- the compiler moves the single-use `distance` past the radius statement; the LOD walk (0x00BA7184 reload) and the eax/ecx/edx roles in the FindLocalLights setup (0x00BA71B5..0x00BA721A) follow; plus the known `test` / `or` operand swap of SetStencilFunc at 0x00BA753D (CRenderStateManager.h gap). 5 grades; **passes 2 and 3 not used (agent context)**. Source + passing test in `rows/parked/`. Queued for the acctB near-miss lane |

Coordinator re-verification (cl 13.10.3077 under Wine, `FABLE_HDR_OVERLAY=hdr/`): `coordverify/` (grade, both test runs,
read-only vland: 1 win, no RELOC_IDENTITY / RELOC_CONFLICT; RELOC_NAME / RELOC_ALIAS warnings covered by NAME_FIXES.tsv).
Purity and padded-struct greps are clean. Parked row regraded: DIFFER(1904v1907).

Review items for landing:

- **NAME_FIXES.tsv (10),** each checked by the agent against the PDB name dumps: 0x009C47A0 AddFile; 0x009C3130 the
  `CFileInstaller::CFileInfo` ctor; 0x009C6A70 / 0x009C6640 `CCountedPointer<const CFileInfo>` ctor / DeleteData;
  0x00999C50 `CAFile::StartFileScan` and 0x00999CB0 `CAFile::ScanNextFile` (landed as free functions); 0x009C46B0
  IsCompressedFilename is protected const; 0x0099EE20 `CCharString::GetAsUpperCase` (landed ToUpper); 0x00403C60
  `__char_traits_base<unsigned short>::compare`; 0x00B90090 `CEnginePrimitiveManagerOcclusionBase::GetSilhouetteColour`
  (landed in `CThingFilterIsUnconscious_operator_00b90090.cpp` as `?Get@T@@QAE_NHH@Z`).
- **`hdr/fable_string.h` is a whole-file overlay** of `rebuild/include/fable_string.h` (14 diff lines against HEAD today:
  one new opt-in `FABLE_RETAIL_CWIDESTRING_TEXT_EQ_STLP_INLINE`): rebase it on HEAD at landing. `hdr/stlport/fable_string.h`
  adds `FABLE_RETAIL_STLP_CHAR_TRAITS_LENGTH` and an include-path fix only the overlay needs.
- **Header conflict:** c2028's CalcDistanceFromCamera body is a one-line GetMagnitude; the parked row needs a running sum
  (X, Z, Y order). Only one can be the header body -- unresolved until 0x00BA6FF0 matches.
- Descriptive names: locals; inline bodies that exist only in FableWin (inferred from retail); `(ERenderMode)2`.

To land 0x009C47A0: `hdr/fable_string.h`, `hdr/stlport/fable_string.h`, `hdr/engine/CAFile.h` (`_INSTALLERPATHS`),
`hdr/engine/CFileInstaller.h` (`_CFILEINFO_CTOR`); the other overlay files serve the parked row (copies of c2028's
overlay plus the opt-ins listed in `hdr/README.md`). Rebuild the oracle from Fable.exe (no retail bytes are in this folder).
