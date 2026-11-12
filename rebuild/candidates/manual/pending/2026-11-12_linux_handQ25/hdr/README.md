# chunkQ25 header overlays

Each file here is a full copy of the repo header (rebuild/include/...) with additions only. Every addition sits behind a new
`#ifdef FABLE_RETAIL_*` opt-in, so rows that do not define the macro compile exactly as before (`diff` against the repo shows
only `>` lines, except CTCEmoteIcon.h, where one `#ifdef` becomes an `#if defined(..) || defined(..)`).

| File | Opt-in | What it adds | Used by |
|---|---|---|---|
| fable_string.h | FABLE_RETAIL_CCHARSTRING_ASSIGN_CHAR | `const CCharString& operator=(char c)` (??4CCharString@@QAEABV0@D@Z, 0x0099F040) | 00489d40 |
| engine/CPlayer.h | FABLE_RETAIL_PLAYER_CREATECHARACTER | includes CDefPointer.h; private `LocateCharacterStartHolySite(const CThingHolySite**) const` 0x00488B20 and `CreateCharacter(const CCharString&)` 0x00489D40; `CDefPointer<const CPlayerDef> PlayerDef` +0x21c typed in place of `PlayerDef_Object` (offsetof assert guarded to match) | 00489d40 |
| engine/CWorldMap.h | FABLE_RETAIL_CWORLDMAP_SETPLAYERPOS | `void SetPlayerPos(long, const C3DVector&, ELoadType)` 0x005063E0 | 00489d40 |
| engine/CThingPlayerCreature.h | FABLE_RETAIL_CTHINGPLAYERCREATURE_CREATE | `static CThingPlayerCreature* __fastcall Create(long, const C3DVector&, long, const CThingPlayerCreatureInit&)` 0x006AC910 | 00489d40 |
| engine/CCreatureAction_PerformExpressionExtended.h | FABLE_RETAIL_EXPRESSION_FRAME (new branch; old model under `#else`) | retail class `: CCreatureActionBase`: 8-arg ctor 0x00843310, PerformAction, FinishAction, Clone, IsToReplaceItemIfRemoved, PreventSneaking, virtual dtor 0x0084ADC0; private PExpressionDef, PPerformToThing, ExpressionItemDefIndex, EmoteIconAdded, FrameCount, ReplacedAction, SoundEffectID; sizeof 200 | 00847520 |
| engine/CCreatureActionBase.h | FABLE_RETAIL_CCREATUREACTIONBASE_GETFRAMESRUNNING | `long GetFramesRunning() const` 0x006925C0 | 00847520 |
| engine/CTCSoundPlayer.h | FABLE_RETAIL_CTCSOUNDPLAYER_RELEASELOOPINGSOUND | `void ReleaseAttachedLoopingSound(unsigned long)` 0x0066AD90 | 00847520 |
| engine/CTCEmoteIcon.h | FABLE_RETAIL_CTCEMOTEICON_STATTYPE | the inline StatGetInterfaceType also under this macro (`#if defined(FABLE_RETAIL_CTHINGGAMEOBJECT_ONHIT) \|\| defined(FABLE_RETAIL_CTCEMOTEICON_STATTYPE)`) | 00847520 |
| engine/CQuestManager.h | FABLE_RETAIL_CQUESTMANAGER_SAVEGAMESTATE (with _LOADGAMESTATE + _ACTIVEQUEST) | CRuntimeQuestPersistence::SetQuestScriptPersistScript 0x004B4E30 / SaveToGameStateScript 0x004AF450; `friend class CQuestManager` in the ACTIVEQUEST CActiveQuest; GetPActiveQuest guarded `#ifndef _SAVEGAMESTATE` (PQuestCurrentlyRunning is not in the LOADGAMESTATE layout); `_pad_0x2c` typed as `std::vector<CCharString> RegisteredQuestNames` +0x2c and `std::list<CCountedPointer<CActiveQuest> > ActiveQuests` +0x38; `SaveGameState(CCharString&)` and private `SaveMasterScriptDataToGameState(CScriptDataBase&, const CCharString&)` 0x004B5030 | 004b5ee0 |
| engine/CCreatureAction_PickUpGenericBox.h | FABLE_RETAIL_CCREATUREACTION_GENERICBOX (new branch) | retail class `: CCreatureActionBase`, ctor `(CThingCreatureBase&, CThing&)` 0x00843700, PerformAction, Clone, protected FrameUpdate, private `CIntelligentPointer<CThing> PThingToPickUp` +0xa8, sizeof 0xb0 | 009658b0 |
| engine/CCreatureAction_DropGenericBox.h | FABLE_RETAIL_CCREATUREACTION_GENERICBOX (new branch) | retail class `: CCreatureActionBase`, ctor `(CThingCreatureBase&)` 0x00842150, PerformAction, Clone, sizeof 0xa8 | 009658b0 |
| engine/CAIStateGroup_RecoverStealableItem.h | FABLE_RETAIL_CAISTATEGROUP_RECOVERSTEALABLEITEM_UPDATE | `virtual void Update()` 0x009658B0 | 009658b0 |
| engine/CEnginePrimitiveRenderUtil.h | FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_OUTLINESTENCILSTATES (nested in _OUTLINESTENCIL) | `void SetupOutlineStencilStates(const CEngineMeshEffectOutline&, float)` 0x00BF0B20 | 00bf0b20 (DIFFER) |

Grading note: rows 00847520, 004b5ee0, 007fc210, 009658b0 and 00bf0b20 were graded with an overlay dir that has no
fable_string.h (`hdr_r2/`, whose `engine` is a symlink to `hdr/engine`): MSVC's quoted-include search walks up the includer chain,
so a root-dir header that includes "fable_string.h" found the repo copy as well as the overlay copy and the class was defined twice
(C2011). Once the overlays are merged into the repo there is a single copy, and the issue does not arise. 00489d40 was graded with
the full `hdr/` (it needs ASSIGN_CHAR).
