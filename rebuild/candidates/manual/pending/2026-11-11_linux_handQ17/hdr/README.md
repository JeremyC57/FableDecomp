# agentQ17 header overlay (grade with FABLE_HDR_OVERLAY=$A/hdr)

Q17 inherited Q16's overlay. The Q15 and Q16 tables below are now byte-identical to the repo HEAD copies (they landed); only the
Q17 table (last) is new. Every Q17 change is behind a new `#ifdef FABLE_RETAIL_*` opt-in or is a new header; the default path of
each changed header is unchanged (two guards were widened/narrowed only on opt-in macros: CCombatManager.h and CTCSpecialAbilities.h,
see the table).


Q16 inherited Q15's overlay; the Q15 rows (first table) are now byte-identical to the repo HEAD copies (they landed), so only the
Q16 rows (second table) are new. Every Q16 change is behind a new `#ifdef FABLE_RETAIL_*` opt-in or is a new header; the default
path of each changed header is byte-identical to HEAD.

## Q15 rows (identical to HEAD now)

Every change is behind a new `#ifdef FABLE_RETAIL_*` opt-in; the default path of each header is byte-identical to HEAD.

| header | opt-in | what | row |
|---|---|---|---|
| engine/CTCSleep.h | FABLE_RETAIL_CTCSLEEP_FASTTRACK | `GetFastTrack()` decl (0x0092C3B0) + inline `GetIsReadyToSleep()` (Ego_r.pdb names) | 0x0092E130 |
| engine/CAIStateGroup_MinionSleep.h | FABLE_RETAIL_CAISTATEGROUP_MINIONSLEEP | retail class deriving CAIStateGroup_GoToBed (0x64, WasCarryingWeapon +0x60) | 0x0092E130 |
| engine/NEntityEvents_CEventHitBy.h | FABLE_RETAIL_NENTITYEVENTS_CEVENTHITBY_TYPE | inline `StatGetType()` (EVENT_HIT_BY = 1) + `GetType()` slot decl (Ego_r / FableWin names); needs _CEVENTBASE_VSLOTS3 | 0x00738A80 |
| engine/NEntityEvents_EventFilters.h | FABLE_RETAIL_NENTITYEVENTS_CISHITEVENTHITBY | new filter class CIsHitEventHitBy (Ego_r layout HitBy +0; ctor `(const CThing&)` and inline operator() from the name set) | 0x00738A80 |
| engine/COptimisedPrimitive.h | FABLE_RETAIL_QUADRIC_COPTIMISEDPRIMITIVE_METHODS | declarations of GetFaceNormal, FindTextureTangent, FindFaceTextureTangents, PerformContraction, DetectFoldOver, PopFrontContraction, UpdateCost, Optimise (Ego_r name set) | 0x00AC7CB0, 0x00AC7300, 0x00ACD960 |
| engine/QuadricOptimiserTypes.h | FABLE_RETAIL_QUADRIC_COPTIMISEDFACE | COptimisedFace (Ego_r, 0x48) | 0x00AC7CB0, 0x00AC7300 |
| engine/CVectorMap.h | FABLE_RETAIL_CVECTORMAP_GETKEYLIST | `GetKeyList(std::vector<K>&) const` decl (Ego_r name; 0x0086D240) | 0x0086B9D0 |
| engine/CRandomAppearanceMorph.h | FABLE_RETAIL_CRANDOMAPPEARANCEMORPH_SKELETAL | GetRandomSkeletalMorph / AddSkeletalMorphToMorphSet decls (Ego_r, protected const) | 0x0086B9D0 |
| engine/CThingFilter_IsValidGossipee.h | (new header, no default path) | CThingFilter_IsValidGossipee from Ego_r layout + name set | 0x0090C900 |
| engine/CTCVillage.h | FABLE_RETAIL_CTCVILLAGE_CURFEW | IsCurfewTime() const decl (Ego_r; 0x00658D5D) in the FABLE_RETAIL_CTCVILLAGE class | 0x0090C900 |
| engine/NTCScriptedControl_CActionFollowCreature.h | (new header) | CActionFollowCreature : CTCScriptedControl::CActionBase (Ego_r layout, name set) | 0x00713D50 |
| engine/CTCStealth.h | FABLE_RETAIL_CTCSTEALTH_SETSNEAKING | inline StatGetInterfaceType (TCI_STEALTH) and SetSneaking (FableWin names; expanded in retail) | 0x00713D50 |

## Q16 rows (identical to HEAD now)

| header | opt-in | what | row |
|---|---|---|---|
| engine/CAIStateGroup_MinionBehaviourCrowdBase.h | (new header, no default path) | CAIStateGroup_MinionBehaviourCrowdBase : CAIStateGroupBase (Ego_r layout: PCrowdThing CIntelligentPointer<const CThing> +0x14, PTaskCrowd +0x1C, MyMessageEvent +0x24, DesignatedCrowdPosition C3DVector +0x2C, IsPartOfCrowd +0x38, sizeof 0x3C); virtuals Initialise / Cleanup / IsRunnable / Update and the pure leaves IsRunnableLeaf / UpdateLeaf / CleanupLeaf / InitLeaf (slots 14-17 from vtable 0x0128C494 and the derived PodiumEmoteNeutral vtable 0x0128D16C -> 0x0096F4A0 / 0x0096F570 / 0x0096F510 / 0x0096F490) | 0x00957DB0 |
| engine/CTCPerceiveThing.h | FABLE_RETAIL_CTCPERCEIVETHING_AWAREOFTHING | inline StatGetInterfaceType (shares the JACK_STAGE1 branch), inline `IsAwareOfThing()` (AwarenessTimer != 0.0f, expanded at 0x00957E30) and the out-of-line `IsAwarenessOfThingDirect() const` decl (0x00744430, Ego_r name) | 0x00957DB0 |
| engine/C3DPrimitive2.h | FABLE_RETAIL_C3DPRIMITIVE2_INDEXBUFFER | IndexBuffer typed `boost::scoped_ptr<CIndexBuffer>` (Ego_r) + private `CreateDegenerateTriangleIndexBuffer()` decl; needs FABLE_RETAIL_SCOPED_PTR (+ _RESET) and FABLE_RETAIL_CINDEXBUFFER | 0x00A90260 |
| engine/CArray.h | FABLE_RETAIL_CARRAY_SIZE_CTOR_OUTOFLINE | `explicit CArray(unsigned int n);` declaration only (called out of line, 0x00A96100 = FableWin ??0?$CArray@K@@QAE@I@Z); checked before FABLE_RETAIL_CARRAY | 0x00A90260, 0x00F3ABF0 |
| engine/CRangeCompressor.h | (new header, no default path) | CRangeCompressor with nested CScriptEntry (24 bytes: Flags, BitCount, RangeBias, RangeShift, ShuffleMask, OrMask; ctor zeroes Flags) and the Ego_r / FableWin member declarations incl. private CalcCompressionScript / CalcBestCompressionForBlock | 0x00F3ABF0 |
| engine/CThingTrackNode.h | FABLE_RETAIL_CTHINGTRACKNODE_LINK | `void SetAsLinkedTo(CThingTrackNode* other);` decl (Ego_r / FableWin, public) | 0x008378D0 |

Rows 0x00774730 (CTCShop::SellItem) needed no header change: it uses the HEAD opt-ins FABLE_RETAIL_CDEFINITIONMANAGER,
FABLE_RETAIL_CRESOURCELIST, FABLE_RETAIL_CTCTHINGOWNER, FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE and
FABLE_RETAIL_STLP_TREE_FIND_MEMBER_TEMPLATE.

## Q17 rows

| header | opt-in | what | row |
|---|---|---|---|
| engine/CAIStateGroup_CreatureUseCoverBase.h | FABLE_RETAIL_CAISTATEGROUP_CREATUREUSECOVERBASE | retail class over CAIStateGroup_EnemyCreatureBase (Ego_r names): MoveIntoCover, DuckBehindCover, HasDiveAnim +0x1c, TimingCritical +0x1e, PCoverThing +0x1f, CoverList (opaque), ValidPositionsFound +0x48 | 0x009711D0 |
| engine/CAIStateGroupBase.h | FABLE_RETAIL_CAISTATEGROUPBASE_PROJECTILESTRAFE (needs _AIACTIONS) | `AIAction_ProjectileAttackAndStrafeToPos(EAIActionRepeat, C3DVector, CIntelligentPointer<const CThing>, bool&, EMovementType, bool)` decl, 0x0097F3A0 (Ego_r) | 0x009711D0 |
| engine/GFWorldDistance.h | FABLE_RETAIL_GFGETWORLD2DDISTANCE | inline `GFGetWorld2DDistance(pos1, pos2)` (FableWin name; named `sqr_dist` local keeps fsqrt after the pops) | 0x009711D0 |
| engine/CCreatureAction_DuckAndCover.h | (new header, no default path) | `: CCombatActionBase`; ctor `(CThingCreatureBase&, const C3DVector&)` 0x009717A0 (Ego_r ??0 spelling); FacingDirection +0x114; sizeof 0x120 | 0x009711D0 |
| engine/CCreatureActionBase.h | FABLE_RETAIL_CCREATUREACTIONBASE_GETACTIONID / _HASPERFORMEDACTION (when !EXPRESSION_FRAME) / _GETDEPENDENTTHINGS (inside FULL_MEMBERS) | `long GetActionID() const` 0x00692620; `bool HasPerformedAction() const` 0x006925A0; protected `GetDependentThings()` | 0x00966030, 0x00965640, 0x008BF500 |
| engine/CAIStateGroup_CarryCrateAndFollow.h | FABLE_RETAIL_CAISTATEGROUP_CARRYCRATEANDFOLLOW | retail class: public virtual OnReplace; PCrate, PPreviouslyCarriedItem, PThingToFollow; GotActionPoint +0x2c, ActionPointIndex +0x30, PickUpActionID +0x34 | 0x00966030 |
| engine/CAIStateGroup_RecoverStealableItem.h | FABLE_RETAIL_CAISTATEGROUP_RECOVERSTEALABLEITEM | PStealableItem, PTargetPoint (const), PPreviouslyCarriedItem, GotActionPoint, ActionPointIndex, ReachedDestination +0x34, PickUpActionID +0x38, NeedsCleanup +0x3c | 0x00965640 |
| engine/CAIStateGroup_SeekScriptTarget.h | FABLE_RETAIL_CAISTATEGROUP_SEEKSCRIPTTARGET | public virtual Update; private `IsStillRunnable() const` 0x0093BFA0; private PEnemy | 0x0093C030 |
| engine/CThingCreatureBase.h | FABLE_RETAIL_CTHINGCREATUREBASE_FREETOFINISH | `bool IsFreeToFinishAction() const;` 0x006621C0 (Ego_r QBE) | 0x009711D0 |
| engine/CombatDialogueManager.h | FABLE_RETAIL_COMBATDIALOGUEMANAGER_SPEECH | SaySpeech, PlayBattleCry (0x008AF850) decls, inline GetPDialogueDef | 0x009711D0, 0x0093C030 |
| engine/CCombatManager.h | FABLE_RETAIL_CCOMBATMANAGER_DIALOGUE (guard widened to `LEADER_INITIALISE || _DIALOGUE`), _UNSHEATH, _ISATTACKING | GetCombatDialogueManager; `CreatureUnsheathWeapon(CThingCreatureBase&, EUnsheatheWeaponType)` 0x006EBDD0; `IsCreatureAttacking(const CThingCreatureBase&)` 0x006E92C0 (landed binding) | 0x009711D0, 0x0093C030 |
| engine/CTCSpecialAbilities.h | guard narrowed: DIVINE_FINALCLEANUP's FadeOrKillEffect decl is skipped when FABLE_RETAIL_CTCUNHOLYPOWER_CANCELFORCUTSCENE is also defined (CTCUnholyPower.h declares it) | avoids the duplicate declaration; default and DIVINE-only paths unchanged | 0x00795090 |
| engine/CThing.h | FABLE_RETAIL_CTHING_LEVELPERSISTENCE | `SetAsLevelPersistent(bool)` (+ IsToKillOnLevelUnload unless LOADSINGLE && !ORIGINAL_VTABLE already declares it) | 0x0095B820 |
| engine/CTCBoss.h | FABLE_RETAIL_CTCBOSS_SETHELPER | `SetHelperCreature(CThingAICreature&)` 0x007AF310 | 0x0095B820 |
| engine/CCreatureAction_QueenHornetShootBabies.h | FABLE_RETAIL_CCREATUREACTION_QUEENHORNETSHOOTBABIES | `: CCombatActionBase`; ctor `(CThingCreatureBase&, CThingCreatureBase&)` 0x0095CB80; sizeof 0x114 | 0x0095B820 |
| engine/CAIStateGroup_HoverHornetQueen.h | FABLE_RETAIL_CAISTATEGROUP_HOVERHORNETQUEEN | `: CAIStateGroup_HoverMovementBase`; protected IsStillRunnable 0x0095A420, Spawn; members; sizeof 0x68 | 0x0095B820 |
| engine/CAIStateGroup_HoverMovementBase.h | (new header, no default path) | empty `: CAIStateGroup_EnemyCreatureBase`, sizeof 0x1c | 0x0095B820 |
| engine/CTCWeapon.h | FABLE_RETAIL_CTCWEAPON_READYPROJECTILE | `void ReadyProjectile();` 0x005DA4E0 | 0x008BF500 |
| engine/CCreatureAction_GenericFireProjectileWeapon.h | FABLE_RETAIL_CCREATUREACTION_GENERICFIREPROJECTILEWEAPON | retail class: ctor, Initialise, PerformAction (slot 3, 0x008BF280), FinishAction (slot 4) | 0x008BF500 |
| engine/C2DVector.h | FABLE_RETAIL_C2DVECTOR_ROTATE | `void Rotate(float revolutions);` 0x00A141D0 | 0x00A87700 |
| engine/CSteeringAgent.h | FABLE_RETAIL_CSTEERINGAGENT_ACCESSORS | GetTarget, IsPlayer, GetPosition, GetRadius(bool), IsWandering, GetAvoidDynamicObstacles | 0x00A87700 |
| engine/CKinematic.h | (new header, no default path) | `C2DVector Vector` (private); ctors; GetVector; SetVector | 0x00A87700 |
| engine/CSBConstraint.h | (new header, no default path) | CSBInput (Input +0x0c, SetInput) and CSBConstraint (MoveBeforeOrientation +0x14, GetMoveBeforeOrientation) | 0x00A87700 |
| engine/CSB.h | FABLE_RETAIL_CSB | `virtual bool Process(const CSteeringAgent*) = 0`; GetOutput; ShouldIgnoreAgent; GetDistanceBetweenAgents; protected `CKinematic Output` +4 | 0x00A87700 |
| engine/CSBCAgents.h | FABLE_RETAIL_CSB | `: CSBConstraint`; virtual Process; `std::vector<const CSteeringAgent*>* Agents` +0x18 | 0x00A87700 |
| engine/NUISystem_CScrollableList.h | FABLE_RETAIL_CSCROLLABLELIST_VISIBLELIST | protected HideComponent 0x00555440 / UnhideComponent 0x005556F0 / UpdateVisibleList 0x00555A90 decls (Ego_r IAE spellings) | 0x00555A90 (parked) |

Rows 0x00795090 (CTCUnholyPower::FinalCleanup) also uses the HEAD opt-ins FABLE_RETAIL_CTHING_ADDANDCOPYTC, FABLE_RETAIL_CTCSOUNDPLAYER and
FABLE_RETAIL_CTCUNHOLYPOWER_CANCELFORCUTSCENE; 0x00966030 / 0x00965640 use FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE with
engine/CDefClassBase_GetSubDef.h; 0x0095B820 uses FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREABODY.
