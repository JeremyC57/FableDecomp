# NF137 header overlay

Six overlays. The base copy of each is in ../orig/engine/ and is byte-identical to HEAD rebuild/include/engine/ (checked 2026-11-20 with cmp).
No other w22 agent (NF120-NF139) overlays any of these six headers.

| header | opt-in macro(s) | adds | pin_asserts |
|---|---|---|---|
| engine/CAIStateGroup_BriarRoseWillUse.h | FABLE_RETAIL_CAISTATEGROUP_BRIARROSEWILLUSE | retail class on CAIStateGroupBase (Update public virtual; PEnemy +0x14, NextWillUseFrame +0x1C; sizeof 0x20); flattened blob moved to #else | pinned 0 (warned about the new assert) |
| engine/CCreatureAction_BriarRoseShotFarInto.h | FABLE_RETAIL_CCREATUREACTION_BRIARROSESHOTFARINTO_CTOR (nested in the CLASS opt-in) | the (CThingCreatureBase&, CThingCreatureBase&) ctor declaration 0x00957170 | pinned 0 |
| engine/CCreatureAction_ThrowShotObject.h | FABLE_RETAIL_CCREATUREACTION_THROWSHOTOBJECT_CLASS | retail class on CCombatActionBase with the inline ctor 0x009566C0; sizeof 0x114; blob in #else | pinned 2 (the two default-path asserts became _AT 30 / 31, same conditions) |
| engine/CAIStateGroup_MazeRangedCombat.h | FABLE_RETAIL_CAISTATEGROUP_MAZERANGEDCOMBAT (turns on FABLE_RETAIL_CAISTATEGROUP_MAZEBASE, includes engine/_quarantine/CAIStateGroup_MazeBase.h) | retail class on CAIStateGroup_MazeBase (Update public virtual, IsStillRunnable protected 0x00954220, LastCompletedPhase +0x20; sizeof 0x24), the MazeTeleport pattern; blob in #else | pinned 0 |
| engine/CAIStateGroup_OpinionReactionRoot.h | FABLE_RETAIL_CAISTATEGROUP_OPINIONREACTIONROOT_COMMENTABOUTHERO (nested in _WIFEUPDATE, private) | declaration bool CommentAboutHeroIsRunnable() 0x009863E0 | pinned 0 |
| engine/CAIStateGroup_CreatureAdvanceInCover.h | FABLE_RETAIL_CAISTATEGROUP_CREATUREADVANCEINCOVER_GETCOVERTHINGLIST (nested, needs _CREATUREUSECOVERBASE_COVERLIST) | public virtual GetCoverThingList(std::vector<CThing*>&) 0x00978FD0 | pinned 1 (the default sizeof 0x4C assert became _AT 33, same condition) |

Default path: token-identical to HEAD for all six headers, apart from the pin_asserts respell of the existing static asserts to
FABLE_STATIC_ASSERT_AT (same conditions). Every addition sits inside a new member-specific #ifdef.

Landed includers (grep of rebuild/src/compiled + rebuild/tests): BriarRoseWillUse 4, BriarRoseShotFarInto 1, ThrowShotObject 2,
MazeRangedCombat 5, OpinionReactionRoot 16, CreatureAdvanceInCover 9. No landed file (rebuild/src, rebuild/tests, rebuild/include) defines any
of the six new macros, so the opt-ins change none of them.
