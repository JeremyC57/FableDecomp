# Chunk 519 (agent519, 2026-10-28): 36 of 40 re-queued single functions landed (under 128 bytes)

Re-queue chunk of 40 chunk_history orphans under 128 bytes (address order, 0x00A787D0..0x00BFAA80): engine primitives,
particles, cloth, occlusion, texture and tree-filer code. One Opus 5.5 agent at high effort; landed after cleanup pass 1.

- **Landed:** 36 rows with `vland.py --land` (7 MATCH, 29 RELOCATION_MATCH; 0x00AA8400 and 0x00B84990 need /Gr, 0x00ABBD70
  /GS), every test PASS under g++ and VC7.1; no RELOC_IDENTITY, RELOC_CONFLICT or RELOC_DUPLICATE. The agent settled
  0x0099A2E0 `??1CBaseClass@@UAE@XZ` vs 0x0099A300 `??1CBaseClassNonCopyable@@MAE@XZ` (identical bodies, two functions).
- **Parked (4, pending/2026-10-27_linux_hand519/differ/):** 0x00B2F760 CEnginePrimitiveOcclusionManager ctor (101v95,
  temporary reload), 0x00B84190 `??_G` CEngineClothSimulationAnimatedMesh (36v75, base dtor not inlined), 0x00BBA3E0
  GetBoundingSphereWorldSpace (x87 scheduling), 0x00BC9B30 CEngineTreeFilerBase::FindNode (register swap).
- **Identities:** all 40 proven (PDB name sets, vtable / RTTI); one descriptive global, g_HighLoadingPriority 0x0139A728.

Label corrections (NOTES.md): 0x00AF0880 / 0x00AFEAD0 the CEnginePrimitive2DParticleGroup / ParticleMeshGroup ctors (both
catalogued as SetLocalizationMatrix), 0x00BE0420 SetBlockToHighPriority, 0x00BE08E0 / 0x00BE0930 STLport `_Rb_tree::find`
(bound as an invented CRetailBlockSet), 0x00B564C0 ~CVertexBufferWin32, 0x00BEB550 CEngineSubPrimitiveBase::operator
delete, landed 0x00B2FA40 is CEnginePrimitiveOcclusionManager::SetAsVisible (not CV_BanditTollScript::Init); globals
0x01436E5C / 0x01436E68 / 0x01436E9C are the occlusion, simulation and lighting managers.

Review items (LESSONS.md): insert_unique 0x00A819B0 now has V / U allocator and invented CRetailTree spellings to unify;
ConvertVectorToLocal is IBE in the PDB, bound QBE; CBaseClassNonCopyable ctor IAE vs QAE bindings.
