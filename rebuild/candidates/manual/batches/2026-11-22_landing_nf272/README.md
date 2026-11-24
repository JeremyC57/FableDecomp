# Landing nf272 (Account A, 2026-11-22): the CWaterPatchMesh unit and the 0x00BFDD40 split (6 rows, 3 tests, 2 headers)

Agent NF272 (entry 1 done; entry 2, D4, not started: context). Hand-back: `pending/2026-11-22_nf272_handback/`.
- **First measurement:** 0x00BF4220's HEAD object binds both calls to 0x00BEAA00 as `?ClearTextures@CWaterPatchMesh@@QAEXXZ` (local model in
  candidates/landscape_patch_update_foreground_compiled.h; no table line).
- **0x00BEAA00** -> `?ReleaseBuffers@CWaterPatchMesh@@QAEXXZ` (catalog CTextureOwner_00beaa00::void ClearBlock -> CWaterPatchMesh::ReleaseBuffers),
  ported onto engine/CWaterPatchMesh.h, objsec ALL_EQ. Binder 0x00BF4220 respelled in place through the new candidates opt-in
  `FABLE_RETAIL_LANDSCAPE_PATCH_UPDATE_FOREGROUND_RELEASEBUFFERS` (10 asserts pinned; objsec ALL_EQ, so its DIFFER(615v634) is its unchanged
  HEAD grade, a July behaviour lift); its test stub respelled.
- **0x00BFDD40** -> `?DeleteForegroundVertexBuffer@CLandscapeLayerMesh@@QAEXXZ` (catalog CTextureOwner_00bfdd40::void ClearBlock ->
  CLandscapeLayerMesh::DeleteForegroundVertexBuffer); binder 0x00BF38F0 off `?ClearBlock@CTextureOwner@@QAEXXZ`. Both rows on a local
  CLandscapeLayerMesh (its only header is quarantined) built on the shared retail CMovableResource; objsec ALL_EQ, only that name moves.
- **0x00BEAAA0** -> `??1CWaterPatchMesh@@UAE@XZ` (catalog was CEngineScreenEffectOutlineGlow::FinishWithDynamicTextures; file now
  `CWaterPatchMesh_CWaterPatchMesh_00beaaa0.cpp`), off candidates/outline_glow_compiled.h onto the shared header, with the new add-only opt-in
  `FABLE_RETAIL_CWATERPATCHMESH_OPERATORDELETE` (inline class operator delete through FreeVirtual; FableWin `??3CWaterPatchMesh@@SIXPAX@Z` + the
  body of retail 0x00BEB4D0). **0x00BEB4D0** -> `??_GCWaterPatchMesh@@UAEPAXI@Z`, a fold row including 0x00BEAAA0's file (the nf251 0x00B2AB10
  pattern); relocation-zeroed leaves equal HEAD's (71 = 71, 100 = 100), the vtable-pointer stores become relocations to tabled vtables; pk:
  `??_GCWaterPatchMesh` = 0x00BEB4D0, `??_GCMovableResource` = 0x009D5110, the fold's `??1` = 0x00BEAAA0; both emitted vtables slot-for-slot
  equal 0x00BEB490's HEAD tables. Both tests rewritten on the shared headers and link the row (`??_G` through slot 0, flags 0 / 1).
  candidates/outline_glow_compiled.h has no includers left.
- **Headers:** equiv_check over both headers' closure (1,383 rows): 0 differ; planted-token control flags exactly the 9 includer lines.
- **Census / link:** each address one spelling, none grows, no new split; ReleaseBuffers / DeleteForegroundVertexBuffer / ~CWaterPatchMesh one
  body each. New objects + HEAD 0x00BEB490: 0 duplicates; + the fold object: 1 LNK4006 on an identical `??1` (nf251 ruling 1 shape).
  funclet_includes_check after the renames: 1,357 checked, 0 broken.
- **Table:** the `?ClearBlock@CTextureOwner@@QAEXXZ` line replaced by the DeleteForegroundVertexBuffer line. The optional ReleaseBuffers line
  is not added (HEAD had none; its only witness is the DIFFER row 0x00BF4220).
- **Recheck** (`logs/recheck.log`, landed tree): the 6 rows + 0x00BEB490, 0x00BDA0D0, 0x00BF5A30, the CLandscapeLayerMesh siblings 0x00BFDE20 /
  0x00BFDD70 / 0x00BFDE80 and every includer of the two overlaid headers (0x00B751F0 / 0x00B761D0 / 0x00B77370 / 0x00BEAB20 / 0x00BEAB40 /
  0x00BEACA0 / 0x00BEAE50 / 0x00BF5B50 / 0x00BF5BB0) (21 rows): 20 PASS (17 RELOCATION_MATCH, 3 MATCH), 0 RELOC_CONFLICT; 0x00BF4220 DIFFER(615v634) PASS, its HEAD grade (objsec ALL_EQ against HEAD).

## Coordinator rulings on NF272's questions
1. Q1 (the `??_GCWaterPatchMesh` copy in 0x00BEB490), option (a): accepted as carried HEAD debt and recorded. After this landing the name has
   two different bodies: 100 bytes in the fold row 0x00BEB4D0 (retail's) and the 30-byte HEAD pick-any copy in the unchanged ctor row 0x00BEB490
   (calls ??1, then `::operator delete`, the wrong deallocator); link order decides which slot 0 gets (`pending/.../evidence/link_order_GCWaterPatchMesh.txt`).
   At HEAD the 30-byte copy was the only body, so no link order is worse. Option (b) (defining the opt-in in 0x00BEB490) is not taken: it would put
   a new, unmeasured copy in the ctor row that cannot reach the 100-byte form without the dtor body. Queued as the next agent's first entry, a
   measurement: does folding 0x00BEAAA0's file into 0x00BEB490 keep the ctor leaf's bytes, relocations and both vtables and make its `??_G` copy EQ?
2. Q2: the local CLandscapeLayerMesh models on the shared CMovableResource are accepted (the quarantine rule allows the real class token); the
   port of the siblings 0x00BFDE20 / 0x00BFDD70 / 0x00BFDE80 off their local CMovableResource is queued.
3. Q3: the three sets land together.
4. Carried debt now visible (vtable slots 1-3 and CMovableResource slot 2 have no landed definer; self rows 0x00BEAB20 / 0x00BEAA50 /
   0x009D5100 / 0x009D50F0 define stand-ins) and the NAME_FIXES (0x00BEAA30 OnPostRelocation catalog, 0x00BEAB20 EAE) are queued.
5. D4 to the next agent.
