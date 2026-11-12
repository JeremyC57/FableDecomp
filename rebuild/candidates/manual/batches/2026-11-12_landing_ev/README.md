# 2026-11-12 landing ev -- author workflow wf10 (Opus only): nine of ten rows landed, one held back

Source: `pending/2026-11-12_linux_handWF10/` (workflow `lane_tools/workflows/author_rows.js`, 56 agents, all Opus; no row needed
Fable). All ten rows byte-matched in about 105 minutes. Landed (all RELOCATION_MATCH, test PASS under VC7.1,
LANDVERIFY_STRICT_HEADERS=1): 0x00808E40 CThingFilter_IsValidTargetForPlayerCreature::operator(), 0x0070CE10
CAnimSet::UpdateDistanceMoved, 0x00949380 CAIStateGroup_GoToPub::ThankBarmaid, 0x006E3580 CTCTalk::PlaySpeech (two author passes),
0x00966AF0 CAIStateGroup_TavernServer::GetBar, 0x006992A0 CGameCameraManager::PreparePrimitivesForRendering, 0x008F0EB0
CTCTavernGameCardBase::CardsTurning, 0x00929340 CAIStateGroup_BanditPlayDice::Update, 0x009F4390 CSurface::CopyFromMemory.

**Held back:** 0x00641C30 CTCLight::OnAppearanceDraw matches only with a body of CTCLight::DrawLight 0x006414A0 visible; that body
is an unproven reconstruction AND the function is reserved by the other account, so it cannot be authored here. It waits in
`held_back/00641c30/` until 0x006414A0 lands from the other lane. Both verifier lenses flagged it; the workflow's repair agent
confirmed the row is DIFFER(502v490) without the body.
**Coordinator fix at landing (0x00808E40):** the made-up global `g_bPlayerMeleeTargetingCheck` 0x013B85F4 said the wrong thing: its
second reader (CUserProfileManager::ResetAssignedInputs 0x004085F4) tests it before loading the def
"FABLE_PC_CONTROL_SCHEME_GDD_LEFT_CLICK_IS_KING". It lands as `bool g_bControlSchemeLeftClickIsKing` (descriptive, declared), and the
in-flight row 0x004085F0 (wf12) must use the same name and type when it lands.
**Mechanical checks:** reserved_check 0 on the nine landed rows, purity clean (0x006E3580 carries the landed real bodies of
CBasicString::Compare 0x00411570 and std::__find 0x004B8FF0 out of line), no local model of a header class in a row source.
**Headers** (16; additions under new opt-ins, all unused elsewhere, defaults unchanged per hdrcheck2): CTCStealth.h,
stlport/fable_containers.h (VECTOR_FILL_NOINLINE), CAnimateThing.h, CAnimSet.h, C3DAnimationInfo.h, CTCAIScratchpad.h,
CTCSoundPlayer.h, CTCTalk.h, CMainGameComponent.h, NSpeechGainManager.h, CAIStateGroup_TavernServer.h, CGameCameraManager.h,
NCameraModes_CBase.h, CCreatureAction_UnsheatheWeapons.h, CAIStateGroup_BanditPlayDice.h, CPixelFormat.h, CSurface.h.
Checks: bindings_multi_check at baseline; funclet_includes_check 1317 checked, 0 broken.
Queued (land1011ev): the rows' name fixes.
