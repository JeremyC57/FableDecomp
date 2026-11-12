# 2026-11-12 landing fh -- author workflow wf16 (Opus only): nine of ten rows landed, one not matched

Source: `pending/2026-11-12_linux_handWF16/` (workflow `lane_tools/workflows/author_rows.js`, 53 agents, all Opus). Nine rows
byte-matched in about 115 minutes. Landed (all RELOCATION_MATCH, test PASS under VC7.1, LANDVERIFY_STRICT_HEADERS=1): 0x007E4E30
CTCBetting::FrameUpdate (flag /GS), 0x007E8590 CTCOwnedEntity::SetPOwner, 0x00922010 CAIStateGroup_MinionWalkToMarker::Update,
0x00AF5090 NParticleEngine::CParticleSystem::ClearParticle (two author passes), 0x00EB2140
NScript::CV_MurderTwistFinalScript::CMurderTwistSheriffGuard::Main, 0x009F3B00 CSurface::CopyFromMemory, 0x005C71AA
CTCInventoryWeapons::ConstructWeaponDescriptionNoDamageNorWeight, 0x009735D0 CAIStateGroup_JackOfBladesBase::RushToPos,
0x00803D60 CTCTargetingPlayer::UpdateProjectileTargeting (flag /d2inlT68).

**Not matched:** 0x006D8F10 CHeroExperienceDef::Transfer, DIFFER(693v692) after three Opus author passes. Every instruction is
the same except where ebp is saved: retail pushes it mid-function (0x006D9064) and pops it inside the last vector transfer; VC7.1
here saves it in the prologue, one byte longer. Second row of the Opus-only workflows that Opus could not match, so it goes to ONE
Fable author pass (`args.authorModel = 'fable'`) with all notes.

**Inferred statement (0x00EB2140):** a dead conditional before the first wait loop, marked `Inferred:` in the source with the
retail bytes; behaviour is identical to retail.
**Coordinator fixes at landing (both rows regraded: match, test PASS):**
- 0x005C71AA carried its own copies of the GetSubDef<T> / GetDef<T> template bodies; it now takes them from
  engine/CDefinitionManager.h (GETDEF_GLOBAL_BODY, GETSUBDEF_BODY), as the evidence verifier had already proven byte-identical.
- 0x009735D0: the CCreatureAction_CastAssassinRush constructor takes `CThingCreatureBase&` as both PDBs spell it (the overlay had
  a pointer); the landed callee row's wrong leaf is handed back as a name fix.
- 0x00AF5090: module respelled `NParticleEngine::CParticleSystem` (the hand-back said `NParticleEngine = CParticleSystem`).
**Mechanical checks:** reserved_check 0 (0x00803D60 cites 0x00804020 in a comment only), purity clean, no local model of a header
class. Pragmas: `auto_inline(off)` around the real Lock body 0x009F33E0 in 0x009F3B00; `optimize("s", on)` in 0x005C71AA (that
TU's established spelling).
**Headers** (13; additions under new opt-ins, unused elsewhere, defaults unchanged per hdrcheck2): CTCBetting.h, CTCThingOwner.h,
CThing.h, fable_string.h, CParticle.h, CSurface.h, CAIStateGroup_JackOfBladesBase.h, CTCSolidToThing.h,
CCreatureAction_CastAssassinRush.h, C3DVector.h (NORMALISE_PAREN_SQUARES_ZYX as a new first arm of the Normalise chain: one
re-headed `#if` line, documented), CGameCameraManager.h, NCameraModes_CBase.h, CTCTargetingPlayer.h.
Checks: bindings_multi_check at baseline; funclet_includes_check 1317 checked, 0 broken.
Queued (land1011fh): 0x007E8310, 0x006C0330, 0x00921630, 0x009F2FC0, 0x0042B312, 0x004428F0, 0x008CE1B0, 0x00416225, 0x00668610,
0x00686D90.
