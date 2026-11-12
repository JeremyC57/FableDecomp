## 0x008B0B60

engine/CCombatSequenceGeneric.h: FABLE_RETAIL_CCOMBATSEQUENCEGENERIC_ONCREATESPECIAL declares the virtual OnCreateSpecial (slot 19, 0x008B0B60) in the retail class; default path unchanged.
engine/CCombatActionBase.h: FABLE_RETAIL_CCOMBATACTIONBASE_GETPCOMBATACTIONFROMENUM declares the static factory GetPCombatActionFromEnum (0x00857430) in the default class branch; declaration only, default path unchanged.

## 0x009486D0

- engine/CTCTavernTable.h: `FABLE_RETAIL_CTCTAVERNTABLE_FREESEAT` declares `void CTCTavernTable::FreeSeat(long, long)` (0x007B8AA0, Ego_r/FableWin `?FreeSeat@CTCTavernTable@@QAEXJJ@Z`), row 0x009486D0. Full copy of the repo header plus this one block.

## 0x0094F480

- engine/CAIStateGroup_BalverineJumpAtFleeingEnemy.h: `FABLE_RETAIL_CAISTATEGROUP_BALVERINEJUMPATFLEEINGENEMY` (row 0x0094F480) -- the real class (derives CAIStateGroupBase, PTarget as CIntelligentPointer<CThingCreatureBase> +0x14, WaitUntilWorldFrameForNextAttempt +0x1C, virtuals IsRunnable / Update / InitStateGroup from vtable 0x0128C0F8 slots 11-13) instead of the flat generated struct; full copy of the current repo header, default path unchanged. Needs FABLE_RETAIL_CAISTATEGROUPBASE + _MEMBERS and FABLE_RETAIL_CINTELLIGENTPOINTER defined by the includer.

## 0x005F6793

No header overlay needed for 0x005F6793: repo headers only (FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NOINLINE already exists in stlport/fable_containers.h).

## 0x00847DF0

# wf7 row 0x00847DF0 header overlay (full copies of the current repo headers + opt-ins; default paths unchanged)
- engine/CCreatureActionBase.h: FABLE_RETAIL_CCREATUREACTIONBASE_SETANIMATION -- protected `void SetAnimation(const CAnimationEntry&, float, float)` 0x00693030 (Ego_r ?SetAnimation@CCreatureActionBase@@IAEXABVCAnimationEntry@@MM@Z), declaration only.
- engine/CCreatureActionBase.h: FABLE_RETAIL_CCREATUREACTIONBASE_GETTOTALTIMEWORLDFRAMES -- public `long GetTotalTimeWorldFrames() const` 0x006925B0 (Ego_r ?GetTotalTimeWorldFrames@CCreatureActionBase@@QBEJXZ), declaration only.
- engine/CAnimFilterOfGroup.h: FABLE_RETAIL_CANIMFILTEROFGROUP_INLINE_CTOR -- inline `CAnimFilterOfGroup(const CCharString&)` (retail expands it in place; out-of-line copy 0x0073E8F0) and inline virtual destructor.
- engine/CTCActionUseBase.h: FABLE_RETAIL_CTCACTIONUSEBASE_ANIMGROUPNAME -- `CCharString GetActionUseAnimationGroupName() const` 0x007EABA0, declaration only.
- engine/CCreatureAction_UseLamp.h: FABLE_RETAIL_CCREATUREACTION_USELAMP -- the real class on the retail CCreatureActionBase (PObjectToUse CIntelligentPointer<CThing> +0xa8, the four Ego_r virtuals + ctor), declarations only.

## 0x0097D6F0

- engine/CThingCreatureBase.h: `FABLE_RETAIL_CTHINGCREATUREBASE_ISPOSINVIEWANGLE` -- declares `bool IsPosInViewAngle(const C3DVector&, float, bool, bool) const` (0x006650E0, Ego_r.pdb + FableWin.pdb `?IsPosInViewAngle@CThingCreatureBase@@QBE_NABVC3DVector@@M_N1@Z`); declaration only, default path unchanged (row 0x0097D6F0).

## 0x006AD3C0

engine/CThingPlayerCreature.h: FABLE_RETAIL_CTHINGPLAYERCREATURE_ACCELERATIONMAP -- types RelativeMovementAccelerationComponents (+0x1a8) as std::map<EGameAction, std::pair<C3DVector,long> > (Ego_r member type) and includes stlport/fable_map.h; default path unchanged (row 0x006AD3C0).

## 0x007735D0

- engine/CTCHeroStats.h: `FABLE_RETAIL_CTCHEROSTATS_TRADESKILL` declares `float GetTradeSkill() const` (0x0057C239, PDB ?GetTradeSkill@CTCHeroStats@@QBEMXZ). Declaration only.
- engine/COpinionReactionManager.h: `FABLE_RETAIL_COPINIONREACTIONMANAGER_ATTITUDETOPLAYER` declares the PDB `GetThingAttitudeToPlayer(const CThing&) const` (0x00701840) without the rest of FABLE_RETAIL_CTCWIFE_MARRY. Declaration only.

## 0x00773790

engine/CTCHeroStats.h: FABLE_RETAIL_CTCHEROSTATS_TRADESKILL -- declares ?GetTradeSkill@CTCHeroStats@@QBEMXZ (0x0057C239), declaration only (row 0x00773790)

(attempt 2: no new opt-in; the matched row 0x00773790 needs only the CTCHeroStats.h line above.)
(repair pass: the overlay engine/CTCHeroStats.h is the CURRENT rebuild/include/engine/CTCHeroStats.h plus only the TRADESKILL block
above -- inserted after line 35; diff against the repo header is exactly those 5 added lines. An earlier copy had been taken from an
older header and lacked the FABLE_RETAIL_CTCHEROSTATS_PROPERTYOWNED branch; that is fixed.)

## 0x007FD650

# Header overlay for row 0x007FD650 (wf7)
- engine/CTCDPhysicalObstruction.h: `FABLE_RETAIL_CTCDPHYSICALOBSTRUCTION_FRAMEUPDATE` -- types the 0x18 `Lines` bytes as `CEnginePrimitiveHandle line1, line2, line3` (Ego_r.pdb names, same layout), declares the slot 9 override `virtual void FrameUpdate()` and the private `void UpdateAdditionalImpulse()` (0x007FD580), and includes engine/CEnginePrimitiveHandle.h. Default path unchanged.
- engine/CThingManager.h: `FABLE_RETAIL_CTHINGMANAGER_ISDRAWCOMBATCOLLISIONDEBUG` -- exposes the existing inline `IsDrawCombatCollisionDebug()` accessor alone (it was only reachable through FABLE_RETAIL_CTHINGGAMEOBJECT_ONHIT, which also switches engine/CEnginePrimitiveLine.h to the plain-inline constructor). Default path unchanged.

