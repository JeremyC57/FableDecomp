# chunk F2 -- notes (6 of 6 matched)

Sources: src/<addr>.cpp, tests: tests/<addr>.cpp, overlay: hdr/ (additions only, see hdr/README.md). All six grade RELOCATION_MATCH
under `LANDVERIFY_STRICT_HEADERS=1` against the complete overlay; all six tests pass vt.py. The inherited drafts are in differ_inherited/.

**0x00406AE0 CUserProfileManager::GetManualSaveDisplayName** (22 grades, 1 probe)
- What made it match: the symbol is an implicit CCharString temporary of the return statement (`GetTextBySymbol("TEXT_...")`), not a named local; the inline GFIntToWideString body is in the TU (expanded in the first arm, called in the second by VC7.1 itself).
- Needs: hdr CUserProfileManager.h (FABLE_RETAIL_CUSERPROFILEMANAGER_SAVENAMES); no flags. Real inline body in the TU: GFIntToWideString 0x00405F00 (Ego_r / FableWin ?GFIntToWideString@@YI?AVCWideString@@J@Z).

**0x009D5BF0 CBankFileAsync::HandleFailedAllocation** (6 grades)
- What made it match: the port onto std::set (upper_bound, iterator ++) plus the STLport insert_unique / _M_insert bodies visible (FABLE_RETAIL_STLP_MAP + FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY): `data` stays in esi across the insert call.
- Needs: hdr CBankFileAsync.h, CBankFileAsyncData.h (friend + private FreeBuffer), stlport/fable_map.h (set / tree const upper_bound); no flags. Real inline body in the TU: CBankFileAsyncData::FreeBuffer (the landed 0x009D5410, expanded here in retail).

**0x00BE06B0 CEngineStaticMapFileBlockManager::AddRequest** (7 grades)
- What made it match: `CFileBlock fileBlock = { 1, lowPriority, data };` (aggregate initialisation = copy construction of the counted pointer) after `data` is default-constructed then assigned; insert bodies visible as above.
- Needs: hdr CEngineStaticMapFileBlockManager.h (AddRequest), CBankFileAsyncData.h (ChangeLoadingPriority); no flags. CBankFileAsyncEntry has no shared header (one-member local declaration).

**0x00909A00 CCombatSequence_WillForcePushLongRange::IsValid** (19 grades, 2 probes)
- What made it match: `if (PeekTC) { if (IsPoweredUp()) { ...; if (dot < 0) return true; } else return false; } return true;` -- two separate `return true` statements keep the dead compare alive; the view direction copied out of the GetRenderCamera(1.0f) temporary; the creature in a reference local.
- Needs: hdr CCombatSequence_WillForcePushLongRange.h (retail class branch); no flags. CCombatSequenceBase and CTCAICreatureWillPowerIndicator have no shared header (local, PDB names: Creature, PoweredUp / IsPoweredUp).

**0x00B939E0 CEngineSceneGrid::CheckVisibilityResults** (7 grades)
- What made it match: both cell lookups through the inline `GetCell(x, y)` (FableWin ?GetCell@CEngineSceneGrid@@QAEPAVCEngineSceneGridCell@@KK@Z) and a single `return visible` (if / else-if), instead of in-place indexing and an early `return`.
- Needs: hdr CEngineSceneGridCell.h (private CheckVisibilityResults + friend CEngineSceneGrid); no flags. CEngineSceneGrid has no shared header (local, Ego_r members).

**0x00BCFB00 CEnginePrimitiveManagerAnimatedMeshesBase::RenderPrimitiveBlocksIndexBuffer** (6 grades)
- What made it match: the PDB's inline accessors and inline SetBoneTransforms / DrawBuffersIndexed bodies, CShaderConstantRange with implicit copy constructor and destructor, and the draw's vertex count / primitive count / start index read into locals in that order.
- Needs: hdr C3DPrimitive2.h, N3DPrimitive_CStaticBlock.h, CShaderRenderManager.h, CShaderConstantRange.h, CVertexShaderConstantLayout.h, CEngineVSConstantLayoutLightsBones.h, CEngineResourceManager.h; no flags. The manager class has no shared header (local, Ego_r BoneMatrixBuffer).

## For the coordinator
- Descriptive (made-up) names: none in the rows. Parameter and local names are descriptive; every class, member, method and global name is from the PDB extracts or reloc_bindings (GPResourceManager / GPShaderRenderManager in NEngineInternal, g_HighLoadingPriority).
- 0x00B939E0 tests `layer & OccludedLayers` with the raw enumerator (`test ebx,[cell+40h]`), not GFGetOcclusionLayerBitMask(layer): that is what retail does.
- 0x00909A00: both outcomes of the camera test return true in retail; the test asserts that.
- NAME_FIXES.tsv has three landed spellings that the PDB extracts contradict (not respelled here).
- Follow-up (coordinator): 0x00B939E0 now includes engine/CBaseClassNonCopyable.h instead of a local stub (still RELOCATION_MATCH, strict; test PASS). The other local classes (CEngineSceneGrid, CEnginePrimitiveManagerAnimatedMeshesBase, CCombatSequenceBase, CTCAICreatureWillPowerIndicator, CBankFileAsyncEntry) have no file under rebuild/include/engine/.
