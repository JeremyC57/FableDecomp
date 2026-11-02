# Chunk 713 notes

10/10 RELOCATION_MATCH + PASS under g++ and VC7.1. No parked rows, no retail_asm, no volatile, no stand-in classes
dropping members. All extents checked against the padding (ret/jmp then int3 to the next 16-aligned start;
0x00681850's jump/byte tables end exactly at 0x006818A0): no extent fixes.

## Names and evidence (all proven from >= 2 sources; none descriptive)
- 0x008E3720 `??1CModeJackOfBladesFlash@NCreatureMode@@UAE@XZ`: RTTI vtable 0x012857A8, slot-0 ??_G 0x008E3700; FableWin ??1 0x028496A0.
  Catalog said ~CAIStateGroup_CarryCrateAndFollow (byte twin propagation). Built on chunk 710's FABLE_RETAIL_CMODESTANDBASE model.
- 0x008E37C0 `??1CModeScorpionKingTailPlunge@NCreatureMode@@UAE@XZ`: vtable 0x01285860, ??_G 0x008E37A0; FableWin ??1 0x02853510.
  Same 59-byte shape as 0x008E3720 but different members; each named from its own vtable. pack(1) only on C3DVector and the
  CIntelligentPointer members (+0x89/+0x95 vectors, +0xA1/+0xA9/+0xB1 pointers).
- 0x00702A50 `??1CTCQuestCard@@UAE@XZ`: vtable 0x01264C44 slot-0 ??_G 0x00702A30; FableWin ??1 0x025534C0. Catalog said ~CTCDExplosion.
- 0x00CB6E60 STLport `vector<pair<CCharString,CScriptInfo>>::_M_clear` (IAE): callers _M_insert_overflow 0x00CB6FD0 and push_back 0x00CB7210;
  body = landed 0x00AF36C0 STLport text. Catalog said CCombatWheel::ResetRings.
- 0x00E32310 / 0x00E398A0 / 0x00E542B0 / 0x00ED39F0 `??1CV_AmbushScamScript` / `CV_AssassinAttacksScript` / `CV_BeardyBaldyScript` /
  `CV_SingingStonesScript` (NScript, UAE): RTTI-less vtables 0x012E2428 / 0x012E35B0 / 0x012E5790 / 0x012F2B0C -> ctors 0x00E2AB60 /
  0x00E376D0 / 0x00E4F5B0 / 0x00ED0E60 -> Alloc 0x00E322C0 / 0x00E39850 / 0x00E54260 / 0x00ED39A0 (`push` = sizeof 0x94 / 0xB8 / 0xA4 / 0x9C)
  -> registration tags S_VAS / S_VAA / S_VBB / S_VSS -> refs/script_recovery/conversion_queue.tsv. Members from Ego_r at offset - 0x14
  (retail CScriptBase 0x40), STLport 12-byte vectors. Catalog said CParticleEmitter.
- 0x0049BA20 `?Uninitialise@CFrameRateSmoother@@QAEXXZ`: caller CMainGameComponent::Shutdown passes this+0x16178 (the FrameRateSmoother
  member); Ego_r + FableWin name sets. Catalog said CThingCreatureBase::EditWorldUpdate; landed caller binding said CEditWorld::Shutdown.
- 0x00681850 `?IsFactionEnemyToHeroOverriddenOnOpinion@CTCCreatureOpinionOfHero@@QBE_NXZ`: Ego_r + FableWin donor order
  (0x006817F0 IsWillingToAttackHeroOnOpinion < 0x00681850 < 0x006818A0 IsWillingToStartCombatWithHeroOnOpinion). The landed binding was already
  correct; the catalog label IsWillingToAttackHeroOnOpinion belongs to the sibling 0x006817F0.

## Label corrections (see NAME_FIXES.tsv)
- Landed ??_G rows 0x00E322F0, 0x00E39880, 0x00E54290, 0x00ED39D0 are named CLandscapeBackgroundPatch_vectordeletingdestructor: they are the
  ??_G of the four village scripts above.
- Landed descriptive ctors CVtable_012E2428 (0x00E2AB60) and CVtable_012f2b0c (0x00ED0E60) are the CV_AmbushScamScript / CV_SingingStonesScript
  PDB ctors.
- 0x00701840 GetThingAttitudeToPlayer is spelt five-plus ways in landed callers (pointer / long / non-const / COpinionManager); the PDB form takes
  `const CThing&` and returns EOpinionAttitudeType, const member.
- 0x0099A330 is CInitBaseClass::IsValid (landed as CPlayer::IsInitialised); 0x0099A360 is protected IAE CInitBaseClass::Uninitialise.
- 0x00CBD510 / 0x00A01C10 / 0x00686830 are ??1CScriptBase / ??1CBaseIntelligentPointer / ??1CTCBase (many local stand-in names landed).

## Header debt
- engine/CLevelScriptBase.h default path declares `virtual ~CLevelScriptBase();` out of line; retail's is implicit. Flipping the default
  (remove the declaration) needs a recheck of 1,062 includers; done here as opt-in FABLE_RETAIL_CLEVELSCRIPTBASE.
- engine/CWorld.h retail branch flattens POpinionReactionManager into `_Data`/`_Info` words; should be CIntelligentPointer<COpinionReactionManager>
  (0x00681850 reads `.._Data`).
- COpinionReactionManager / EOpinionAttitudeType have no shared header; 0x00681850 declares the one member it calls locally (no layout).
  Enumerator spellings for the attitude values 6..17 are unknown (table of names at 0x013BABD0 not yet mapped), so the switch is written
  numerically.
- The CModeStandBase-family helper classes (CTCCreatureModeManager, CAnimFilterBase, CAnimFilterOfGroup, CBaseIntelligentPointer) have no
  shared engine/ header; chunk 710's local models were reused.
- Village-script template argument CQ_SunnyvaleMasterData is unproven for these four scripts (it does not affect bytes of the implicit dtor).
- CTCQuestCard / CTCCreatureOpinionOfHero generated default branches are Ego_r layouts without the vptr; the retail branches are opt-in.

## Tooling note
tall.py writes to $S/wine/t713 and q.py to $S/wine/q/<addr> (brief-provided tools, outside the agent directory).
