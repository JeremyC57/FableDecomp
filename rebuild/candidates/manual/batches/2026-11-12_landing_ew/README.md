# 2026-11-12 landing ew -- author workflow wf12 (Opus only): nine of ten rows landed, one in repair

Source: `pending/2026-11-12_linux_handWF12/` (workflow `lane_tools/workflows/author_rows.js`, 65 agents, all Opus; no row needed
Fable). All ten rows byte-matched in about 90 minutes. Landed (all RELOCATION_MATCH, test PASS under VC7.1,
LANDVERIFY_STRICT_HEADERS=1): 0x009FF220 CGraphicInfoFrame::CFrameData::UploadFrame, 0x0082B7E0 CMiniMapDisplay::DrawMarkers,
0x0084D910 NCameraModes::CBase::UpdateCameraStretchAroundHero (two author passes), 0x006507D5
NPlayerGui::CDrawQuestInfo::CQuestInfoElementTimer::DrawAt, 0x007AF490 CTCBoss::KillHelpers, 0x007F9960
CTCTargetingSpirit::UpdateTargetList, 0x0096CB40 CAIStateGroup_EatAtTable::GetSeat, 0x007BDE31
NThingShot::CTCShotDefinitionFireballSpell::OnCollision, 0x007D2CC0 CTCTavernGameCoinGolf::EnableCourse.

**Not landed yet:** 0x004085F0 CUserProfileManager::ResetAssignedInputs matches, but both verifiers found real problems: the global
0x013B85F4 under a second name and type (landed as `bool g_bControlSchemeLeftClickIsKing`), a made-up global where the PDB inline
CStartupDefinitionManager::IsAllocated() belongs, and an unexplained base-qualified GetDef call. It is in a repair + two-verifier
workflow (`lane_tools/workflows/repair_and_verify_row.js`) and lands separately.
**Coordinator fixes at landing (0x009FF220, regraded: match):** a stale comment about a non-existent PeekGraphicHeader() accessor
reworded; the overlay's IsBump() could be defined twice when two opt-ins are combined -- guarded.
**Mechanical checks:** reserved_check 0 (comments cite reserved / landed siblings; bodies copied from LANDED rows only), purity
clean, no local model of a header class in a row source.
**Headers** (16 + 1 new; additions under new opt-ins, all unused elsewhere, defaults unchanged per hdrcheck2): CGraphicFrame.h,
CGraphicInfo.h, CGraphicInfoFrame.h, CManagedTexture.h, CGraphicDataBank.h, CFadeInFadeOutBase.h, CMiniMapDisplay.h, CPlayerGuiDef.h,
CGameCameraManager.h, CThingCreatureBase.h, CTCBoss.h, CCombatAction_GenericStrikeResponseKnockdown.h, CThingFilter_IsNotList.h,
CTCTavernTable.h, CFireballSpellLevelDef.h, CTavernGameCoinGolfDef.h, fable_string.h (one `#elif` opt-in:
CBASICSTRING_TEXT_EQ_NOINLINE); new CTCTargetingSpirit.h.
Checks: bindings_multi_check at baseline; funclet_includes_check 1317 checked, 0 broken.
Queued (land1011ew): the rows' name fixes.
