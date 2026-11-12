# Chunk Q25 (six parked rows, one at a time, Opus 5.5; first chunk under the stay-on-task rule)

All 6 rows were reached, in 29 grades, 6 probes and about 160 tool calls. 5 matched and landed in landing dt:
- 0x00489D40 CPlayer::CreateCharacter: `init` declared before an inner block from the co-op offset to the end, so the holy-site and
  name slots share a frame slot. It defines the already-landed GetPControlledCreature 0x00487DC0 out of line with its real body.
- 0x00847520 CCreatureAction_PerformExpressionExtended::FinishAction: the 8-argument Extended ctor built from named locals.
- 0x004B5EE0 CQuestManager::SaveGameState: matched on the first grade after the port (new SAVEGAMESTATE opt-in).
- 0x007FC210 CTCKraken::CreateTentacles: the draft's IsLoading was really IsEditorActive; shared out-of-line GetDef<T> body.
- 0x009658B0 CAIStateGroup_RecoverStealableItem::Update: the draft had the actions wrong (0x00843700 is the PickUpGenericBox ctor,
  0x00842150 DropGenericBox).
DIFFER: 0x00BF0B20 CEnginePrimitiveRenderUtil::SetupOutlineStencilStates (1290v1290 after 15 grades): two instructions of prologue
scheduling (retail stores the first colour byte before `push esi`).
Local classes with no shared header (Ego_r members): CPlayerDef, CThingRespawnInfo, CTCKrakenTentacle, CCarrySlotDef,
CTCStealableItemLocation.
