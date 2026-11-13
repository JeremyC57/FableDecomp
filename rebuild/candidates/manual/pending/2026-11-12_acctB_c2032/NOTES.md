# Hand chunk 2032 (agent2032, 2026-11-12): 1 of 2 byte-exact

All grades were taken with `FABLE_HDR_OVERLAY=/opt/fable_lane/w22/agent2032/hdr` exported (g.sh / gall.sh). hdr/README.md lists every
overlay change: each one sits behind an opt-in and leaves the default paths untouched. The grade history is in tmp/grades.log.

## Matched (src/ + test/)
| address | identity | grades | lever |
|---|---|---|---|
| 0x009C47A0 | `?AddFile@CFileInstaller@@QAEXABVCCharString@@0PBV?$CArray@VCCharString@@@@_N@Z` (catalog label "AddFilteredFiles" was descriptive) | 8 (6 + 2 regrades) | iterator taken inside the `filters != NULL &&` guard; forced CWideString / CWideStringData `== L""` layers; STLP_MAP insert_unique body visible (fixes the 0x74 frame); inline STLport `operator==(basic_string, const C*)` |

Result: RELOCATION_MATCH 1915v1915. The read-only vland passed with WARN RELOC_NAME / RELOC_ALIAS only; the callee spellings are in
NAME_FIXES.tsv. Behaviour test test/009c47a0.cpp passes under g++ and VC7.1. lv/ has been deleted.

## Parked (differ/)
- 0x00BA6FF0 `CEnginePrimitiveManagerStaticMeshesBase::DrawSilhouette` -- DIFFER(1904v1907), 447/514 exact, 5 grades, **pass 1 only**.
  Passes 2 and 3 were not used because the context budget ran out; this is not a proof that the row cannot match.
  Remaining diff:
  1. The radius chain `fld [ecx+34h] ; fmul [ebp+58h] ; fstp [esp+0Ch]` is emitted before the squared-distance sum
     (ours 0x00BA70FD..0x00BA7107). Retail emits it after the fsqrt (0x00BA7119..0x00BA711F). This is the root cause: c2
     forwards the single-use `distance` past the radius statement.
  2. LOD walk. Ours keeps MeshInfo in ecx and walks edx (`lea edx,[ecx+44h]`). Retail walks ecx (`add ecx,44h`) and reloads
     MeshInfo for GetMesh (0x00BA7184 `mov ecx,[ebp+50h]`). This probably follows from (1).
  3. eax/ecx/edx role permutation in the FindLocalLights argument set-up (0x00BA71B5..0x00BA720E) and in the ShowSilhouettes read
     (0x00BA721A).
  4. SetStencilFunc bookmark block 0x00BA753D: retail `test eax,esi ; or esi,eax`, ours swapped. This is the known
     CRenderStateManager.h lane gap.

  Measured with free probes:
  - A second use of `distance` keeps the sqrt first but spills it.
  - Making radius an argument evaluated after distance (helper order) puts radius after the sqrt, but keeps it in an FP register,
    and FindLocalLights recomputes it.
  - Parentheses, statement-order sweeps, assignment-in-argument, address-taken radius and the GetAppropriateLOD helper (FableWin
    `?GetAppropriateLOD@CEnginePrimitiveManagerMeshBase@@SIKPBVC3DMeshInfo@@MMM@Z`, declaration in the overlay) did not move it.

  Next ideas:
  - Dump the c2 forwarding pass with tools/c2h/dumpblk.py to see what pins retail's distance.
  - Try GetAppropriateLOD(mesh_info, distance, radius, ...) with the real FableWin parameter order and an inline body.
  - Try the radius read as a `const float&` member access.

  Behaviour test: differ/00ba6ff0.test.cpp passes under g++ and VC7.1.

## Skipped
None.

## Names proven (LABELS.tsv) and label corrections (NAME_FIXES.tsv)
- The 0x009C47A0 catalog label AddFilteredFiles is really AddFile (Ego_r 0x00B13370 + FableWin).
- 0x009C3130 is the CFileInfo ctor (FableWin 0x03046BC0). It was landed as CFileInstallerEntry_009c3130.
- 0x009C6A70 / 0x009C6640 are CCountedPointer<const CFileInfo> ctor / DeleteData (Ego_r 0x00B149F0 / 0x00B149C0). They were landed as
  CCountedPointer_009c6640 / Destroy_009c6640.
- 0x00999C50 / 0x00999CB0 are CAFile::StartFileScan / ScanNextFile (Ego_r 0x00B018F0 / 0x00B01930, the CWideString& overloads). The
  reloc bindings name them free ScanFirstFile / ScanNextFile with a CFileScanState.
- 0x009C46B0 IsCompressedFilename is protected const (`IBE`, Ego_r 0x00B13080); the landed model is public non-const.
- 0x0099EE20 is CCharString::GetAsUpperCase (bound as ToUpper).
- 0x00403C60 is std::__char_traits_base<unsigned short,unsigned short>::compare (labelled MemCmp_Unsigned16).
- 0x00B90090 is CEnginePrimitiveManagerOcclusionBase::GetSilhouetteColour (Ego_r 0x00A4FCE0 + FableWin 0x02ED7FD0; donor order before
  IsClippedByNearPlane). It was landed as `?Get@T@@QAE_NHH@Z` in CThingFilterIsUnconscious_operator_00b90090.cpp.

## Descriptive names
- The local names in both rows: filtered_files, filename, centre, distance, radius, nearest, lod_error, lod, mesh, box, colour.
- The bodies of GetBoundingSphereRadiusWorldSpace, FindLocalLights, GetVisibilityRatiosValid, GetShowSilhouettes,
  GetLODErrorTolerance and GetLODVirtualScreenSizeFOVAdjusted. These are FableWin-only inlines; the names are PDB spellings, but each
  body was inferred from the retail expansion.
- `(ERenderMode)2` in DrawSilhouette: the enumerator name was not found.

## Header debt (all in hdr/, see hdr/README.md)
- fable_string.h is a whole-file overlay (opt-in `FABLE_RETAIL_CWIDESTRING_TEXT_EQ_STLP_INLINE`). Re-base it on HEAD at landing.
- stlport/fable_string.h has the include-path tweak (overlay only) plus `FABLE_RETAIL_STLP_CHAR_TRAITS_LENGTH`.
- Row 2's headers are copies of agent2028's overlay plus new opt-ins. Agent2028's CalcDistanceFromCamera body (a one-line
  GetMagnitude) differs from this row's running-sum body; reconcile the two when both land.
