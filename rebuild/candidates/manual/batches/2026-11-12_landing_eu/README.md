# 2026-11-12 landing eu -- author workflow wf11 (first Opus-only run): ten of ten rows landed

Source: `pending/2026-11-12_linux_handWF11/` (workflow `lane_tools/workflows/author_rows.js`, 53 agents, ALL Opus -- scout, author,
verify, repair -- under the owner's Opus-only rule; no row needed Fable). All ten rows byte-matched in about 95 minutes; nine
VERIFIED by both lenses, one with open findings fixed by the coordinator. Landed (all RELOCATION_MATCH, test PASS under VC7.1,
LANDVERIFY_STRICT_HEADERS=1): 0x008DA410 NCreatureMode::CModeScreamerAggressive::OnActivate, 0x0079CC80 CTCBulletTime::PlayEffect,
0x00AABD50 CSoundEmitterLH::PlaySoundEvent (the manifest said VSlot4), 0x009810B0 CAIStateGroupBase::AIAction_FindAGuardAndGiveMessage,
0x00A44F40 C3DSkeletalMorphTarget::CombineMorph (two author passes), 0x00D38D20 NScript::CQ_FireHeartScript::RunVillagerComments,
0x00704860 CGameScriptThingManager::CreateScriptThing, 0x00A5B370 CCachedFile::Read, 0x00E36750
NScript::CV_ArcheryCompetitionScript::DoCountDown, 0x00773EE0 CTCShop::GetSaleItemsToPlace.

**Coordinator fixes at landing (0x00AABD50, regraded: RELOCATION_MATCH, test PASS):** the pitch-fader loop was hand-hoisted
(`if (bits) { bit = f(); do {...} while (bits); }`); the verifier showed the plain `while` form, written like the gain loop, gives
the same bytes (VC7.1 hoists the invariant call itself), so the row and its test model use that. Retail quirk kept and commented:
the pitch loop asks FindFirstSetBit for the exhausted GAIN mask. FindFirstSetBit is now declared a descriptive name; two name fixes
the notes mentioned but did not hand back (0x00AAC870, 0x00AAB8B0) are queued.
**Mechanical checks:** reserved_check 0 (two comments mention reserved callees), purity clean, no local model of a header class in
a row source; carried bodies are landed rows' real bodies (0x00982210 in 0x009810B0, out of line; 0x00669580 in 0x0079CC80).
**Headers** (12 + 1 new; additions under new opt-ins, all unused elsewhere, defaults unchanged per hdrcheck2): CTCDParticleEmitter.h,
CModeStandBase.h, CTCCombat.h, CTCBulletTime.h, LHA_CEventPlayOptions.h, CCreatureActionBase.h, CThingSearchTools.h,
CCreatureAction_ShowFear.h, CAIStateGroupBase.h, CIntelligentPointer.h, CGameScriptThing.h, CV_ArcheryCompetitionScript.h; new
NCreatureMode_CModeScreamerAggressive.h.
Checks: bindings_multi_check at baseline; funclet_includes_check 1317 checked, 0 broken.
Queued (land1011eu): the rows' name fixes.
