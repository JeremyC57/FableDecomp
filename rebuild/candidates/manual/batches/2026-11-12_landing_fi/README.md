# 2026-11-12 landing fi -- author workflow wf17 (Opus only): eight of ten rows landed, one in repair, one not matched

Source: `pending/2026-11-12_linux_handWF17/` (workflow `lane_tools/workflows/author_rows.js`, 59 agents, all Opus). Nine rows
byte-matched in about 130 minutes. Landed (all RELOCATION_MATCH, test PASS under VC7.1, LANDVERIFY_STRICT_HEADERS=1): 0x007E2A60
CTCFineDialogue::GlobalUpdate, 0x0075B000 CTCPhysicsRigidBody::InitialiseBody, 0x008548B0
CCreatureAction_SpiritAttack::CreateAttackEffect, 0x00958840 CAIStateGroup_MinionReactToCrime::Update (two author passes),
0x00B08250 NParticleEngine::CPSCSingleSprite::SetTrailPositionsFromSpline, 0x006CF530 CTCWife::FindAppropriateGift, 0x0079D7E0
CTCBulletTime::UnPause, 0x00977E00 CAIStateGroup_Guildmaster::Update.

**Not landed yet:** 0x0075A7A0 CTCPhysicsRigidBody::ResolveCollisions matches but models CTCPhysicsRigidBody locally although a
shared header exists (with a placeholder virtual name and a stale C3DVector.h overlay); it is in a repair + two-verifier workflow.
**Not matched:** 0x00854DD0 CCreatureAction_SpiritAttack::PerformAction, DIFFER(710v720, 9 hunks) after three Opus author passes:
an x87 operand order that VC7.1 only emits for an lvalue radius (the PDB getter returns by value) and a subtraction order that
needs a vector whose address is never taken. Third row Opus could not match; it goes to ONE Fable author pass.

**Coordinator fixes at landing (rows regraded: match, test PASS):**
- engine/CMessageEventManager.h: 0x007E2A60 and 0x00958840 each added a block at the same place (merge conflict); both blocks kept.
- 0x00958840: the named CCharString in its own block now carries the retail order that needs it (ctor 0x0095893D before the &def
  push, destructor 0x00958960 before `test bl,bl`; a temporary gives 721 bytes).
- 0x0075B000: the Ego_r size in a header comment corrected (0x1c4); the landed 0x0075BA30 operator= is handed back as the nested
  `CRigidBody::CRigidBodyConfig` spelling.
- 0x0079D7E0: the overlay adds `SCREEN_EFFECT_PRIORITY_SPELL = 2` to the enum line of an EXISTING opt-in branch (ARGPASSER) of
  NScreenEffect_CScreenEffectPriorityStack.h. Approved (additive; same enumerator and value as the FULL branch); the four landed
  rows that reach that branch were rechecked: 0x006B83E0, 0x0079CF30, 0x0079D7C0 RELOCATION_MATCH, 0x0079DC80 MATCH, tests PASS
  (`logs/recheck_argpasser_users.log`).
- 0x007E2A60: stale CThing.h overlay rebased by 3-way merge (keeps the SENDMESSAGEW / GETSCRIPTDATA blocks landed by wf16).
**Mechanical checks:** reserved_check 0 (reserved 0x00A358F0, 0x004C1060, 0x0097FEA0 are only declared and called), purity clean,
no local model of a header class in a landed row. Two documented re-headed `#if` lines (CThing.h HASTC_MATERIALISED,
CDisplayEngine.h STARTSCREENFADEOUT): default and existing paths token-identical (hdrcheck2).
**Headers** (21 + new engine/CRigidBody.h; additions under new opt-ins, unused elsewhere, defaults unchanged):
CMessageEventManager.h, CTCVillage.h, CThing.h, CDisplayEngine.h, CTCHeroStats.h, C3DMeshObject.h, CPhysicsMeshObject.h,
CTCPhysicsRigidBody.h, NParticleEngine_CParticleUpdateParameters.h, CVectorMap.h, CCreatureAction_SpiritAttack.h,
CThingBuilding.h, CTCCreatureOpinionOfHero.h, CAIStateGroupBase.h, CTCShop.h, CTCBulletTime.h,
NScreenEffect_CScreenEffectPriorityStack.h, CTCCarrying.h, CAIStateGroup_Guildmaster.h, CCombatAction_WillShieldInto.h.
Checks: bindings_multi_check at baseline; funclet_includes_check 1317 checked, 0 broken.
Queued (land1011fi): 0x006590BF, 0x006590A0, 0x00686DF0, 0x00A358F0, 0x0075BA30, 0x0076B120, 0x0079CE80, 0x0079DE00, 0x005B16FF,
0x004DF885.
