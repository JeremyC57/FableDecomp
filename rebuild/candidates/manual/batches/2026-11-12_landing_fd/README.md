# 2026-11-12 landing fd -- author workflow wf15 (Opus only): nine of ten rows landed, one in repair

Source: `pending/2026-11-12_linux_handWF15/` (workflow `lane_tools/workflows/author_rows.js`, 58 agents, all Opus; no row needed
Fable). All ten rows byte-matched in about 105 minutes. Landed (all RELOCATION_MATCH, test PASS under VC7.1,
LANDVERIFY_STRICT_HEADERS=1): 0x00BDF100 CLocalDetailCacheMap::CQuadTreeElement::CalcBoundingSphereAndFadeDistanceFromChildren,
0x005F6BD6 CTCInventoryAbilities::SetAbilityAsAvailable, 0x00A60D40 CChecksumCacheFile::Open, 0x0091DDD0
CAIStateGroup_HoverAllAroundTest::Update (three author passes), 0x008DB810 NCreatureMode::CModeDragonFireBeam::UpdateBeam,
0x005FBAA0 CTCInventoryMap::GetRelativePosOnMiniMap (two passes), 0x007CD9A0 CTCBurntEffect::FrameUpdate, 0x00B211B0
NParticleEngine::CPSCSplineBase::CSplineRandomOffsetData::InterpolateTo, 0x007BF510 NThingShot::CTCShotDefinitionBase::OnCollision.

**Not landed yet:** 0x00972CC0 CAIStateGroup_JackOfBladesStage1::MeleeCombat matches; three verifier findings (the PDB inline
CBrainOutput::SetAsPumpCombatActions open-coded, the protected IsStillRunnable 0x00971B00 not handed back as a name fix, a `long`
where Ego_r types EMovementType) are in a repair + two-verifier workflow.

**Coordinator fixes at landing (all three rows regraded: match, test PASS):**
- 0x005F6BD6 and 0x007BF510 both added a block under `FABLE_RETAIL_STLP_FIND_NOINLINE_BODY` to stlport/fable_algorithm.h with
  different content (forwarder out of line vs inline). The standalone block of 0x005F6BD6 is now
  `FABLE_RETAIL_STLP_FIND_NOINLINE_BODY_FORWARDER`; its trip_count uses the STLport difference_type spelling.
- 0x008DB810: a stale CJackDragonDef.h overlay that would have replaced the block landed for 0x00951D60 is dropped; the row builds
  against the repo header.
- 0x007BF510: the CIntelligentPointer.h overlay is rebased onto the current header (keeps FORCEINLINE_CTOR, adds EQUAL_MIXED).
**Mechanical checks:** reserved_check 0 (0x005F6BD6 only declares and calls the reserved 0x005F64DD), purity clean, no local model
of a header class. `#pragma optimize("y", off)` / `("s", on)` in 0x005F6BD6 is that TU's established spelling.
**Headers** (10; additions under new opt-ins, unused elsewhere, defaults unchanged per hdrcheck2): C3DBoundingSphere.h,
CLocalDetailCacheMap.h, CVectorMap.h, CTCInventoryAbilities.h, CDateAndTime.h, CDataInputStream.h, NCameraModes_CTools.h,
CTCWeaponTrail.h, CIntelligentPointer.h, stlport/fable_algorithm.h.
Checks: bindings_multi_check at baseline; funclet_includes_check 1317 checked, 0 broken.
Queued (land1011fd): 0x005F8150, 0x0073A680, 0x005BEB57, 0x007C2980.
