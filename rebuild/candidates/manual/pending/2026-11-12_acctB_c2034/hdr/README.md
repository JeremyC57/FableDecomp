# Chunk 2034 header overlay (`FABLE_HDR_OVERLAY=/opt/fable_lane/w22/agent2034/hdr`)

Every change is an opt-in (`#ifdef FABLE_RETAIL_...`) or a new header only this chunk's row includes; each copied repo header's default
path is unchanged. Rows must include engine/CAIStateGroupBase.h first.

## Copied unchanged from earlier hand-backs (not yet in rebuild/include)
- From pending/2026-11-11_acctB_c2026/hdr/engine/ (all files; see its README): C3DVector.h was then replaced by the c2029 copy,
  CAIStateGroupBase.h by the c2029 copy (superset of c2017 / c2023 / c2026: PERFORMACTION_NOWAIT, MOVETOPOSPROXIMITY, FILTER).
- pending/2026-11-11_acctB_c2017/hdr/engine/CTCBoss.h (GENERICCOUNTER, SETHELPERCREATURE, EFFECTS), then extended below.

## Opt-ins added (chunk 2034, row 0x0097BDA0)
| header | opt-in | change | evidence |
|---|---|---|---|
| engine/CAIStateGroupBase.h | FABLE_RETAIL_CAISTATEGROUPBASE_PUMPCOMBATACTIONS | protected `AIAction_PumpCombatActions()` decl, 0x0097D980 | Ego_r ?AIAction_PumpCombatActions@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@XZ; donor order PerformAction 0x00848680 / PumpCombatActions 0x00848740 / CombatPerformActionAndWait 0x00848800 = retail 0x0097D8B0 / 0x0097D980 / 0x0097DA40 |
| engine/CAIStateGroup_ScorpionKing.h | FABLE_RETAIL_CAISTATEGROUP_SCORPIONKING | retail class : CAIStateGroupBase, Ego_r members at retail offsets (PArenaCentre +0x14, TrapThings +0x1C, bIsRangeAttack +0x28, ChargeCount +0x2C, PEnemy +0x30, EnemyKnockDownFrame +0x38, HealthAtLastTailStrike +0x3C), methods with Ego_r access | RTTI base (rtti_bases.tsv) + Ego_r TPI/name set; retail reads +0x2C/+0x30/+0x38/+0x3C in 0x0097BDA0 |
| engine/CCreatureAction_ScorpionChargeInto.h | FABLE_RETAIL_CCREATUREACTION_SCORPIONCHARGEINTO | retail class : CCombatActionBase, ChargeDistance +0x114, PObstruction +0x118, ctor decl 0x008C4E20, implicit dtor | Ego_r ctor name + members; landed ctor 0x008C4E20 stores +0x114 / builds +0x118; dtor COMDAT 0x0097C900 |
| engine/CCreatureAction_ScorpionUnblockable{Left,Right}TurnAttack.h | FABLE_RETAIL_CCREATUREACTION_SCORPIONUNBLOCKABLE{LEFT,RIGHT}TURNATTACK | retail class : CCombatActionBase, ctor decl 0x0097C760 / 0x0097C830 | Ego_r ctor names (AAVCThingCreatureBase@@0@Z) + landed same-TU ctors |
| engine/CCreatureAction_ScorpionPrepareTailPlunge.h | FABLE_RETAIL_CCREATUREACTION_SCORPIONPREPARETAILPLUNGE | retail class, TargetPos +0x114, ctor decl 0x0097C920 | Ego_r ctor name + member; landed ctor |
| engine/CTCBoss.h | FABLE_RETAIL_CTCBOSS_STRIKES | inline SetCountingStrikesSinceHitHero(bool), ResetStrikesSinceHitHero() decl 0x007AED50 | FableWin ?SetCountingStrikesSinceHitHero@CTCBoss@@QAEX_N@Z (no Ego_r/retail copy; stored in place 0x0097BFDD); Ego_r donor order 0x007B06A0 after GetStrikesSinceHitHero = retail 0x007AED50 (`mov [ecx+14h],0`) |
| engine/CThingCreatureBase.h | FABLE_RETAIL_CTHINGCREATUREBASE_GETCOMBATTYPEDEF | `const CCombatTypeDef& GetCombatTypeDef() const` decl 0x00666D10 | Ego_r QBEABVCCombatTypeDef@@XZ + catalog label + use (FrontNearLimit +0x74) |
| engine/CTCPhysicalShieldSpell.h | FABLE_RETAIL_CTCPHYSICALSHIELDSPELL_ALLOWREACTIONS (default branch) | inline StatGetInterfaceType (147), AllowReactionsOnHitFromAttacker(CThing*, bool) decl 0x0079E580 | Ego_r name + donor order (0x007A3910 after OnHitAtPos) + catalog; UsingComponents test 0x80000 at +0x30 (= bit 147) |

## New headers
- engine/CScorpionKingBattleDef.h: : CParentDefClassBase (rtti_bases.tsv), Ego_r members from +0x3C (retail: NumChargesBeforeTailStrike
  +0x3C compared at 0x0097C22B, DamageToCauseTailStrike +0x54, ClawObstructionRadius +0x5C), inline StatGetTypeName (Ego_r/FableWin name;
  the landed GetSubDef<CScorpionKingBattleDef> 0x008C5A50 builds that literal).
- engine/NEntityEvents_CEventCreatureCharging.h: NEntityEvents::CEventCreatureCharging : CEventBase (8 bytes, vtable 0x012989D4) with
  the inline ctors (FableWin ??0CEventBase@NEntityEvents@@QAE@XZ, ??0CEventCreatureCharging@NEntityEvents@@QAE@XZ); retail expands
  `mov [eax+4],0 ; mov [eax],012989D4h` (0x0097C304). Guarded CEventBase definition (FABLE_ENGINE_NENTITYEVENTS_CEVENTBASE_DEFINED as c2015).
