# Chunk 3015 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- (0x00BA6FF0) Unreferenced out-of-line COMDAT copies of header inlines sit in OTHER TUs too: GetAppropriateLOD 0x00B9C210, CalcDistanceFromCamera
  0x00B9C280, CalcDistanceFromCameraSquared 0x00B9C2B0 (RepeatedStaticMeshes TU, no manifest rows). Compile a candidate body out of line (file-scope
  `&Class::Fn`) against them for free before using it in a row: it fixed two wrong bodies inherited from a parked row (running-sum distance,
  `tolerance *=` LOD).
- (0x00BA6FF0) GetAppropriateLOD's distance clamp is a ternary `d - r > K ? d - r : K`; the if-clamp form compiles to the same out-of-line bytes but,
  expanded in DrawSilhouette, schedules the radius chain (`fld [ecx+34h] ; fmul ; fstp`) BEFORE the squared-distance sum instead of after the
  fsqrt (0x00BA7119). Two spellings with identical OOL bytes can expand differently: grade the expansion too.
- (0x00BA6FF0) A reload of a member pointer before a call (`mov ecx,[ebp+50h]` at 0x00BA7184, the LOD walk reusing ecx with `add ecx,44h`) came
  from writing three later stores through a NAMED pointer (`CEngineRenderInfoStaticMesh* render_info = primitive.GetRenderInfo();`) instead of
  `primitive.GetRenderInfo()->X` three times; a dead named local does nothing. Diagnose such reloads for free: a volatile re-read of the member
  (diagnostic only) grading RELOCATION_MATCH proves the reload is the whole residue; c2prio shows the walk IV paying `ecx: 1` while the CSE temp
  spans the loop.
- (0x00BA6FF0) c2 CSE kill facts measured (free): an out-of-line call or a store through ANOTHER base pointer (global) kills a member-load CSE;
  same-base stores at other offsets, stores to escaped locals, inline out-param writes and an extra sqrt do not.
- (0x00BA6FF0) A one-line GetMagnitude distance gives retail's X, Z, Y squares in the full TU but Z, Y, X in a fragment probe (the per-function
  key of r3004): judge x87 term order only on full-source diagnostic compiles.

Review at landing:
- The landed SetupMesh 0x00BCF2E0 (CEngineInternalPrimitiveAnimatedMesh_SetupMesh_00bcf2e0.cpp) defines GetAppropriateLOD locally with
  `errorTolerance *=` and an "(no out-of-line copy in Fable.exe)" note: both are wrong (OOL copy 0x00B9C210; its body keeps a separate lod_error,
  which lands in radius's dead home [esp+8]). It still matches its own row; reconcile the header body (this row's) when both are in rebuild/include.
- Agent 2028's CalcDistanceFromCamera body (one-line GetMagnitude) is the right one (OOL 0x00B9C280); chunk 2032's running-sum body is superseded.
- The row needs FABLE_FORCEINLINE on GetAppropriateLOD (score 182) and FindLocalLights (98), with the `// Inline-budget:` line.
- hdr/ is agent 2032's overlay unchanged (fable_string.h whole-file overlay etc.; see its README).
