# 2026-11-13 landing fx -- author workflow wf22 (Opus only): eight of ten rows landed, two still differ

Source: `pending/2026-11-12_linux_handWF22/` (workflow `lane_tools/workflows/author_rows.js`, 53 agents, all Opus; no row used
Fable). Landed (all RELOCATION_MATCH, test PASS under VC7.1, LANDVERIFY_STRICT_HEADERS=1): 0x007E1FC0 CTCAmbientBird::FrameUpdate,
0x004BD6E0 NGraphicAppearance::CTCGraphicAppearance::SetPrimitive, 0x00A24520 C3DMeshLODInfo::BuildMesh, 0x0054FBC0
NUISystem::CText::SetValue, 0x004C0560 NGraphicAppearance::CTCGraphicAppearance::GetHelperPoint, 0x0063B3F0
CWorldMap::CreateLineSegment, 0x00F3A890 CRangeCompressor::CalcBestCompressionForBlock, 0x009C9280
NGameText::CDataBank::GetTextEntryFromGroup.

**Not matched (three Opus author passes each; best sources and notes in the pending folder):**
- 0x00646F67 NPlayerGui::CDrawExperience::Draw -- DIFFER 843 v 843, 7 hunks: only the order of the end-cap argument setup at
  0x0064721B..0x00647250 (retail computes the end X before the two zero uv temporaries).
- 0x00F3BA70 CTriangleStripifier2::BuildCacheOptimizedVertexList -- DIFFER 834 v 853, 8 hunks, all in the inlined push_back
  growth path (register choice for len / the list, and a loop-head pad).
Neither has had a Fable pass; that is the owner's call per row.

**Coordinator fixes at landing (regraded: match, test PASS):**
- 0x004C0560: the two same-TU inline bodies (GetAllMeshes, IsResourcesLoadedForMesh) are copies of the bodies in landed
  0x004BE3E0; the source header now says so (it cited 0x004BD420, a different spelling, and the unlanded 0x004BC090).
- 0x009C9280: two existing comment lines of NGameText_CDataBank.h that the overlay had rewritten are restored and the correction
  is added as new lines. The test now checks that a loaded first pick queues no load request (the mutant that drops the loaded
  fast path passed before; it fails now).
- C3DMeshStats.h was changed by two rows (0x00A24520 MESHGENERATORS, 0x004C0560 ISANIMATED): merge conflict resolved by keeping
  both opt-in blocks.

**Landing-verifier warning recorded:** RELOC_CONFLICT on `std::vector<long>::push_back`: 0x009C9280 binds it to 0x005178A0
(through CArray<long>), landed 0x0057CB2A binds it to 0x00485FF3. They are two different bodies (0x00485FF3 has a frame pointer).
Ego_r.pdb has a Lionhead member `?push_back@?$CArray@K@@QAEXABK@Z`, so 0x005178A0 is probably CArray's own push_back (folded for
long / unsigned long) and 0x00485FF3 the STLport one. 0x005178A0 already carries four other spellings in the bindings. Queued as a
binding conflict (land1011fx); the existing binding was kept.
**Mechanical checks:** reserved_check 0 (0x00A22F80, 0x00A24D70, 0x00A26AD0 are cited in comments only), purity clean. Pragmas:
`auto_inline(off)` around real same-TU callee bodies in 0x00F3A890.
**Headers** (25; additions under new opt-ins, unused elsewhere, defaults unchanged per hdrcheck2): CTCAnimatingObject.h, C3DVector.h
(NORMALISE_PAREN_SQUARES_XYZ; the existing `#if ..._ZYX` became the `#elif` after it, every existing flag preprocesses the same;
the existing Normalise opt-ins were tried at landing and do not match this row), stlport/fable_map.h, C3DAnimationInterpolator.h,
NGraphicAppearance_CTCGraphicAppearance.h, CTCHeroAttachableAppearanceModifiers.h, C3DAnimationBlendState.h, CBankFile.h,
C3DPrimitive2.h, C3DMeshStats.h, CResourceList.h, C3DMesh2.h, CBankFileAsyncData.h, C3DMeshLODInfo.h, CMeshDataBankEntry.h,
CResource.h, NUISystem_CText.h, CSubtitleRenderer.h, C3DMeshObject.h, CBankFileAsync.h (one `#if` condition gained a third
`defined()`), GFRandom.h, CIVCountedPointer.h, NGameText_CDataBank.h, fable_string.h.
Checks: bindings_multi_check at baseline; funclet_includes_check 1317 checked, 0 broken; 12 landed users of the touched headers
rechecked, all match (logs/recheck.log).
Queued (land1011fx): 0x00706860, 0x013B8A18, 0x00A36100, 0x009D68B0, 0x00646D90, 0x00646DA7, and the push_back conflict.
