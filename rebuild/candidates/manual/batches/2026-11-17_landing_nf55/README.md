# 2026-11-17 landing nf55: name-fix round 7, NF55 (58 rows: game-side tail, second half)

Owner: "Continue the naming fix queue with two agents until complete".

NF55 took the second half of the game-side tail of the queue (targets below 0x00800000): 39 small entries with 70 binder rows,
from NF41's probe and NF44's binder lists. NF54 had the first half. This is the last slice of the original queue. Brief, worklist
and records: `naming/2026-11-17_agentNF55/`.

## Rows (70 worklist rows, all terminal; STATUS.tsv)

| Status | Rows |
|---|---|
| ported | 51 worklist rows + 7 self rows of NF55's targets = 58 changed rows |
| not-a-binder | 2 (0x00661720: the e49 claim is rejected; 0x007F2DC0 already binds the claimed symbol, only the anonymous-namespace hash differs, and the compiler sets that per file) |
| blocked | 17 (unchanged; 15 blocked/port, 2 blocked/identity) |

The 7 self rows: 0x0065F17B, 0x0065F8CA, 0x006691C0, 0x006A7390, 0x0072AC40, 0x007445D0 (GetFOV) and 0x007AED10 (GetBossPhase).

**Blocked rows:**
- **blocked/port, 15:** 0x006666A0, 0x00691AE0, 0x00694430, 0x0069AD10, 0x006C7370, 0x006D53C0, 0x00713510, 0x0072E130,
  0x00799900, 0x008909F0, 0x0090FAE0, 0x0094B8B0, 0x00961880, 0x009627C0 and 0x0096C4F0. Each wrong spelling comes from a local
  model (a stand-in CThing, CPersistContext, CDefPointer, CFollowerList, ...) that must move onto its shared header first; STATUS.tsv
  names it. Nine carry only the probe grade, because the fix is a port of the whole file.
- **0x00799900 (4 port grades, DIFFER 351 v 351):** the binding is exact on engine/CTCShopKeeper.h, but the shared by-value
  `CIntelligentPointer(T*)` constructor loads its argument one instruction early. Both PDBs say that constructor takes its argument
  by value, so NF55 parked the row instead of inventing a reference overload.
- **blocked/identity, 2:** 0x006D1250 and 0x0075D8B0. 0x00616580 and 0x00B2EB20 are byte-identical 71-byte `set<long>` finds at
  two addresses, so they are two distinct symbols, and no evidence yet says which caller binds which (e221 / e195, REJECTED.tsv).

## Entries (APPLIED.tsv, 39)
- **22 done:** e144, e208, e169, e94, e52, e3, e70, e46, e62, nf44 56, e105, e51, e149, e79, e42, e108, e104, e65, nf44 9, e4,
  e152 and nf44 126. e105 is also done on NF54's half.
- **14 blocked:** e93, e53, e106, e81, e30, e11, e131, e47, nf44 57, nf44 95, e99, nf44 10, e132 and e31. Each waits on the
  blocked/port rows above (APPLIED.tsv names them).
- **3 rejected (REJECTED.tsv):**
  - e221 and e195: the binding conflict above; no symbol is proven.
  - e49: 0x0070C0E0 is the private const `CAnimateThing::IsAnimationFinished` (Ego_r 0x005752F0; FableWin's source order matches
    the retail neighbours), not `IsMainAnimFinished`. The proven spelling is queued for the self row and its callers.

## Rows and checks
- **Self renames (section 1, 16 rows; 12 file moves for sources and tests).** Among them:
  - CGameScriptInterface IsTimeOfDayBetween / PlayAVIMovie / EntityClearPersonalityOverride: the virtual members (slots 650, 369
    and 620), where the old leaves were free `__fastcall` stand-ins;
  - `list<CIntelligentPointer<CThingAICreature> >::size` 0x0065F17B (was catalogued `CBasicString<char>::CalcStringLength`);
  - `IS_THING_ALIVE<CIntelligentPointer<CThingBuilding> >` 0x0065F8CA (was `_global::IsThingAlive_0065f8ca`);
  - CThingCreatureBase ClearAttackedByCreature 0x006691C0 (was `CAIStateGroupWithBuilding_006691c0::Cleanup`) and the protected
    UpdateLastAttackingCreature;
  - CTCPerceiveThing::GetFOV 0x007445D0 (was `GetScaledDefValue`), CTCBoss::GetBossPhase 0x007AED10 (file GetCurrentPhase,
    definition DrawGetEnvironment), CTCSpecialAbilities::FadeOrKillEffect 0x0072AC40 (was `CTCPhysicalShieldSpell::FadeOutEffect`);
  - the public virtual `CTCPhysicsControlled::Move`, the protected `CTCContainerRewardHero::ReleaseItems`, the private
    `CTCTurncoatSpell::StopTurncoatingEffects`, the reference-parameter CCreatureAction_UnsheatheToShowToSheatheTrophy ctor and
    `CThingFilter_IsInFOV::operator()(const CThing&)`;
  - `CAIStateGroup_OpinionReactionRoot::WaitForTextToLoadThenSpeak` with its PDB parameter and result types. It keeps public QAE
    because its only landed caller 0x00982F70 (NF56's row) binds it so; the PDB access is private (queued).
- **Coordinator review:** section 1 has no descriptive stand-in owner, so it was applied as handed back.
- **Funclets:** no EH funclet includes a changed row. funclet_includes_check reports 1,355 checked, 0 broken.
- **Recheck (`logs/recheck_part0..2.log`):** all 58 changed rows graded from the repo after landing: 56 RELOCATION_MATCH and 2
  MATCH, every test PASS. NF55's own final run (`naming/2026-11-17_agentNF55/recheck_final.log`) agrees.
- **Checks:**
  - `naming/rebind.py` removed 309 binding lines and added 263;
  - bindings_multi_check is at its baseline (`NullCoord`).

## Headers (16 opt-ins in 15 files; lines added only, default path unchanged)
CTCInventoryAbilities.h, CVectorMap.h, CTCQuickAccessMenu.h, CUserProfileManager.h, CThingCreatureBase.h (2),
CTCCreatureOpinionOfHero.h, CInputProcessGameBase.h, CInputProcessQuickAccessItems.h, NParticleEngine_CParticleUpdateParameters.h,
CTCPhysicsStandard.h, CTCPhysicsControlled.h, CTCCombat.h, CTCPerceiveThing.h, CTCContainer.h and CTCContainerRewardHero.h.
Details: `naming/2026-11-17_agentNF55/hdr/README.md`. None of NF54's headers is overlaid.
- `FABLE_RETAIL_CTCPHYSICSSTANDARD_MOVE` declares the two virtuals CTCPhysicsStandard introduces (slots 97 / 98 of vftable
  0x01265F84, the row's `call [eax+184h]` / `[eax+188h]`), only under its macro.
- `FABLE_RETAIL_CUSERPROFILEMANAGER_SUBTITLING` names three fields inside its opt-in.

## Name fixes queued (`land1117fu`, 27)
From NF55's NAME_FIXES.tsv (26) plus one coordinator line:
- callers outside the worklist that still bind spellings NF55 proved wrong: IsQuitting (0x00690FD0), OnSpecialAbilityButtonPressed
  (0x00690030), FadeOrKillEffect (0x007BE0F9), StopTurncoatingEffects (0x007C75A0 / 0x007C7560), GetBossPhase (0x00954410, which
  calls it under the made-up name GetCurrentPhase);
- self rows whose access, constness or parameters are wrong: CTCQuickAccessMenu::IsQuitting 0x00625940, the
  CTCPhysicsStandard / CTCPhysicsControlled Move steps, CTCTalk IsTalking / PlayAISpeech, CTCContainer::AddItemToInventory,
  `CAnimateThing::IsAnimationFinished` 0x0070C0E0 and others;
- **coordinator:** row 0x00745120 (`CTCPerceiveThing::UpdateLineOfSight`) also defines `CTCPerceiveThing::GetFOV` in its own file.
  Now that 0x007445D0 is that function, the second definition should become a call or an inline on the shared header.

## Rule slips (NF55 report)
- In seven rows NF55 edited one declaration inside a local model that predates it, instead of porting the whole file: 0x00665190,
  0x007FBC90, 0x008C2240, 0x0095B020, 0x006A7390, 0x007C6DA0 and 0x007C75E0. 0x0068B7C0's call moved onto its existing local base
  class. No class was renamed and no new local model was added.
- The input-process macro was first named off-pattern; NF55 renamed it to the `FABLE_RETAIL_CINPUTPROCESSGAMEBASE_` form and
  regraded its row.
- `grades_used` in STATUS.tsv includes one-off grades of the original files, taken to fill RENAMES section 2 for four self rows.
