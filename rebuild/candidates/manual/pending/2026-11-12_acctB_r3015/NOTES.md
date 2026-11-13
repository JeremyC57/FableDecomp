# Hand chunk 3015 (CEnginePrimitiveManagerStaticMeshesBase): 1 of 1 byte-exact

Row 0x00BA6FF0 CEnginePrimitiveManagerStaticMeshesBase::DrawSilhouette: RELOCATION_MATCH 1907v1907 (src/00ba6ff0.cpp, test/00ba6ff0.cpp PASS
under g++ and VC7.1, read-only vland RELOCATION_MATCH PASS with WARN RELOC_NAME only, lv/ deleted). Continuation of agent 2032's parked row.

## What matched (1)
- CEnginePrimitiveManagerStaticMeshesBase::DrawSilhouette: silhouette draw of a static mesh (capture block, LOD pick, mesh fetch, render info,
  stencil or z-buffer states, alpha box). Shape lessons: (1) GetAppropriateLOD as the real FableWin inline (FABLE_FORCEINLINE, score 182) with
  the body proven against retail's OOL copy 0x00B9C210 -- the TERNARY clamp `d - r > K ? d - r : K` is what schedules the radius chain after the
  fsqrt; (2) CalcDistanceFromCamera = one-line GetMagnitude (OOL 0x00B9C280; in this TU it gives retail's X, Z, Y squares); (3) the three
  render-info stores through a named `CEngineRenderInfoStaticMesh* render_info = primitive.GetRenderInfo();` (after the GetMesh check) make
  GetMesh re-read primitive.MeshInfo (0x00BA7184), so the LOD walk reuses ecx (`add ecx,44h`) and the FindLocalLights / ShowSilhouettes register
  roles and the SetStencilFunc test/or order all fall into place.

## Parked for Fable 5.1 (0)
None.

## Skipped
None. The row starts at a real function start (0x00BA6FF0 `sub esp,60h`) and ends at 0x00BA7762 (`ret 4`, then int3 padding to 0x00BA7770):
1907 bytes as the oracle says (no SPLITS.tsv).

## Label corrections
- 0x00B9C210 / 0x00B9C280 / 0x00B9C2B0 are unlabelled (no manifest row) unreferenced COMDAT copies of GetAppropriateLOD / CalcDistanceFromCamera /
  CalcDistanceFromCameraSquared (LABELS.tsv; bodies compile to the exact bytes).
- 0x00B90090 is CEnginePrimitiveManagerOcclusionBase::GetSilhouetteColour (landed as ?Get@T@@QAE_NHH@Z / labelled ReturnFalse_ret8); 0x0129BA3C
  reloc label EPSILON = ?DELTA_NOTIONAL_ZERO@@3MB (NAME_FIXES.tsv).

## Descriptive names
- Locals: centre, distance, radius, lod, mesh, render_info, box, colour; GetAppropriateLOD's parameter / local names (mesh_info, distance, radius,
  tolerance, nearest, lod_error) -- no PDB has them.
- The bodies of GetBoundingSphereRadiusWorldSpace, FindLocalLights, GetVisibilityRatiosValid, GetShowSilhouettes, GetLODErrorTolerance,
  GetLODVirtualScreenSizeFOVAdjusted (FableWin-only inlines; names are PDB spellings, bodies inferred from the retail expansion).
  GetAppropriateLOD and CalcDistanceFromCamera bodies are now PROVEN (OOL COMDAT byte match).
- `(ERenderMode)2` in DrawSilhouette: the enumerator name was not found.

## Header debt
- hdr/ is an unchanged copy of agent 2032's overlay (see hdr/README.md); the row uses its opt-ins, incl. FABLE_RETAIL_CENGINEPRIMITIVEMANAGERMESHBASE_APPROPRIATELOD.

# Agent 3015 log -- row 0x00BA6FF0 (continuation of prior/2032)

All grades with FABLE_HDR_OVERLAY=/opt/fable_lane/w22/agent3015/hdr (hdr = copy of prior/2032/hdr). Grade log: grades.log (tools/gg.sh).

## Measurements (in order)
- g01 = prior differ source regraded: DIFFER(1904v1907), 480/514 exact (tools/mdiff.py masked count).
- FOUND: retail has UNREFERENCED out-of-line COMDAT copies in the RepeatedStaticMeshes TU:
  0x00B9C210 = CEnginePrimitiveManagerMeshBase::GetAppropriateLOD (fastcall, ecx = mesh info, ret 0Ch),
  0x00B9C280 = CEnginePrimitiveRenderUtil::CalcDistanceFromCamera, 0x00B9C2B0 = CalcDistanceFromCameraSquared (no sqrt).
  Free check (tools/ool.sh on probe/b/cb4.cpp, file-scope address-taking probe): these bodies compile to the retail bytes exactly:
    CalcDistanceFromCamera: `return (position - GetCurrentCamera().PeekCameraPos()).GetMagnitude();` (r3013's body; the 2032 running sum
      gives a different OOL copy, Z/X/Y order differs)
    GetAppropriateLOD(mesh_info, distance, radius, tolerance): `float nearest = distance - radius > DELTA_NOTIONAL_ZERO ? distance - radius :
      DELTA_NOTIONAL_ZERO; float lod_error = nearest / (radius * GetLODVirtualScreenSizeFOVAdjusted()) * tolerance; unsigned long lod = 0;
      while (mesh_info->HasLOD(lod + 1) && !(mesh_info->GetLODError(lod + 1) > lod_error)) ++lod; return lod;`
      (lod_error lands in radius's dead home [esp+8] as retail; the landed SetupMesh's `errorTolerance *=` lands in [esp+0Ch] = NOT retail's OOL body)
- In DrawSilhouette (full-source diagnostic compiles, tools/chkf.sh, no retail diff): the GetAppropriateLOD expansion with the TERNARY nearest
  puts the radius chain after the fsqrt (residue 1 fixed); the if-clamp + lod_error form keeps it before. The one-line GetMagnitude body gives
  retail's X, Z, Y squares in the full TU (fragments differ: symbol count).
- g02 (GetAppropriateLOD with `tolerance *=`) and g03 (proven bodies, probe/q0.cpp): 486/514; residue (1) and (4) gone (the SetStencilFunc
  test/or order now matches retail in g03 without any header change).
- c2prio (tools/prio.sh): the LOD walk IV (#155) is coloured before the MeshInfo CSE temp (#67, lines radius/lod/GetMesh) and pays ecx cost 1
  -> edx. g04 DIAGNOSTIC (not deliverable): q0 with the GetMesh object re-read through a volatile pointer = RELOCATION_MATCH. So the ONLY
  remaining residue is that retail's GetMesh `this` is a separate load of primitive.MeshInfo (not CSE'd with the radius / GetAppropriateLOD load).
- Free diagnostics that did NOT break the CSE (still 1 load of +0x50): mesh local declared before GetAppropriateLOD or at the top; early named
  mesh_info local (before centre or after distance); radius via mesh_info->GetBoundingSphere().GetRadius()*Scale; statement order sweeps.
- Inline trace (tools/inl.sh, FUNC=?DrawSilhouette@, probe/q0i.cpp = q0 with plain `inline`): GetAppropriateLOD c1 91, f 0.25, score 182 > 74
  -> called; retail expands it, so FABLE_FORCEINLINE (CalcDistanceFromCamera 44 expanded by itself; ~CCountedPointer 94 stays called).
- Kill experiments (free, tools/chkf.sh loads of [ebp+50h]): a store through ANOTHER base (global pointer, k5) or an out-of-line call (k3)
  between the LOD and GetMesh makes the separate load (and retail's add ecx,44h); same-base stores (render info LOD, k1; ShadowMeshInfo, k6),
  stores to escaped locals (mesh ctor early/late, colour), inline out-param writes (k8/k9), an extra sqrt (k10), CCountedPointer(0) / = NULL
  with FABLE_RETAIL_CCOUNTEDPOINTER (k12/k13), loop spellings of GetAppropriateLOD (for / break / return; all keep the OOL bytes) and IL-size
  changes (x1..x3) do NOT. With kills both before and after the LOD (k11) there are 3 loads: no late merging of same-block loads.
- c2prio (q0): the walk IV (prio 28) is coloured before the MeshInfo temp (#67, prio 18) and gets costs {ecx: 1} -> edx; in the volatile
  diagnostic the MeshInfo temp ends at the walk init and the IV takes ecx with no cost. So retail's IL has a SEPARATE GetMesh load.
- differ/00ba6ff0.cpp = probe/q0.cpp (g03 source).
- g05 = probe/f4.cpp (q0 + named render_info after the GetMesh check): RELOCATION_MATCH. Free checks: f1 (render_info before radius), f2 (before
  centre), f3 (before the mesh local) give the identical object (md5); DEAD named locals (d1..d3, incl. an unused render_info pointer) do NOT
  -- the stores must go through the named pointer. (Mechanism not traced further: the named pointer is a different base symbol for the three
  stores; with them, c2 no longer carries the MeshInfo CSE temp to GetMesh.)
- g06 = gall.sh on src/00ba6ff0.cpp (final header text): RELOCATION_MATCH. tall.sh / tall.py PASS. vland (read-only) RELOCATION_MATCH PASS, WARN
  RELOC_NAME (GetSilhouetteColour -> ReturnFalse_ret8 label, DELTA_NOTIONAL_ZERO -> EPSILON label); lv/ deleted.
- CalcDistanceFromCameraSquared body also proven free (probe/b/csq.cpp = retail 0x00B9C2B0); not used by this row.

## Commands
- full grade: `bash tools/gg.sh <src> gNN "<note>"` (q.py + logs grades.log; tools/mdiff.py tmp/gNN.txt -v = masked diff)
- free full-source diagnostic (no retail diff): `bash tools/chkf.sh <src>` (radius order, [ebp+50h] loads, walk init, squares);
  `HDR=<overlay> CCFLAGS=<flags>` overrides
- free OOL body check: `bash tools/ool.sh <cprobe.cpp>` (prints GetAppropriateLOD / CalcDistanceFromCamera out-of-line bodies)
- c2 allocator trace: `bash tools/prio.sh <src> <tag>`; inline trace: `FUNC='?DrawSilhouette@' bash tools/inl.sh <src> <tag>`
