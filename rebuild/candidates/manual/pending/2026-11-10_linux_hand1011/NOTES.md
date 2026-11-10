# chunk 1011 notes (agent1011)

Every grade ran with `FABLE_HDR_OVERLAY=$A/hdr` exported (overlay documented in hdr/README.md). Grades are logged in ATTEMPTS.log
(SRC_FAIL compiles are not counted; one 005eb590 grade re-graded an unchanged source after a failed edit and is counted).

## Result: 8 RELOCATION_MATCH, 2 parked DIFFER, 3 fragments (no function), 3 not reached
| row | identity (2 sources) | grade | grades |
|---|---|---|---|
| 005ea650 | ?OnInitialActivate@CTCTavernGame@@UAEXXZ (vtable 0x012560BC slot 6; Ego_r/FableWin) | RELOCATION_MATCH | 2 |
| 005eb180 | ?SetStateAfterInstructions@CTCTavernGame@@IAEXKVCWideString@@W4ETavernGameStates@1@@Z (Ego_r/FableWin; header) | RELOCATION_MATCH | 2 |
| 006061b0 | ?FrameUpdate@CTCInventoryMap@@UAEXXZ (vtable 0x01257A9C slot 9; Ego_r) | RELOCATION_MATCH | 2 |
| 005fbfa0 | ?TeleportToActiveRegion@CTCInventoryMap@@AAEXXZ (Ego_r donor order; body) | RELOCATION_MATCH | 1 |
| 005fbea0 | ?GetRegionTeleporter@CTCInventoryMap@@ABEPAVCThing@@ABVCRegion@@@Z (Ego_r donor order; caller 0x005FBFA0 shape) | RELOCATION_MATCH | 1 |
| 005fb4b0 | ?TeleportHeroToBoastingArea@CTCInventoryMap@@QBEXXZ (Ego_r; landed caller 0x0061A360) | RELOCATION_MATCH (/GS) | 1 |
| 005fcbe0 | ?ConstructPastQuestsList@CTCInventoryMap@@AAEPAVCComponent@NUISystem@@PAV23@@Z (Ego_r 0x0064F5D0 / FableWin; "UI_TITLE_QUEST_PAST") | RELOCATION_MATCH | 4 |
| 005fddd0 | ?ConstructPastQuestsListPC@CTCInventoryMap@@AAEPAVCComponent@NUISystem@@PAV23@0@Z (Ego_r 0x00651F00 / FableWin; `ret 8`, PC_ strings) | RELOCATION_MATCH | 1 |
| 005eb590 | ?GivePrize@CTCTavernGame@@QAEXXZ (Ego_r/FableWin; header binds it) | DIFFER(631v631), 22 lines | 5 |
| 005fbaa0 | CTCInventoryMap::GetRelativePosOnMiniMap (prior agents; catalog) | DIFFER(634v634), 12 lines | 9 |
| 005eadd6 / 005eaf46 / 005eb08d | mid-function cuts of landed SetStateAfterYesNoAlt 0x005EADC0 / SetStateAfterYesNo 0x005EAF30 / SetStateAfterOK 0x005EB080 | FRAGMENT | 0 |
| 005eabc0 / 005ec270 / 005eca50 | PositionMesh / ProcessInput / DrawPreparePrimitivesForRendering | not reached | 0 |

## Callee names used (PDB spellings)
CFontManager::GetFont 0x009E2C80 (+ PFontManager 0x013B838C, descriptive), CGameEvent::AddData 0x009F1760, NGameText::CDataBank::GetPGroupEntry
0x009C8890, CWorld::GetFrame 0x0049D870, CPlayer::GetPControlledCreature 0x00487DC0, CTCBase::PeekPlayerManager 0x00686D50 /
PeekThingSearchTools 0x00686D90, CPlayerManager::GetMainPlayer() const 0x00449960 / GetMainPlayerNumber 0x00449700, CThing::GetPos 0x004C73D0,
CThingObject::Create 0x00703210, CTCHeroReceiveItems::GiveItem 0x007CAB30, CTCHeroStats::AddRenown 0x0057D8BE, CTCHeroOpinionDeedLog::PostDeed
0x006C6990, CTCInventoryQuests::ConstructQuestList 0x0061B610 / ConstructQuestListPC 0x0061E6D0, NUISystem::CManager::GetInstance 0x0041E5F2 /
CreateComponent 0x0041DB1D, CComponent slots 3 FindChild / 124 AddPositionChild, CList slot 145 AddNonScrollingChild, GTeleportOpenedFromMap
0x0137D76C. See NAME_FIXES.tsv (GetFont spelling in landed rows; the catalog's wrong names for the fragments and quest builders).

## Header debt
- engine/CTCInventoryQuests.h and engine/CTCHeroReceiveItems.h model no members (declarations only; Ego_r sizes 0x1B0 / unknown).
- QuestScreenType has no proven enumerator names (1 = current, 2 = past by use).
- The landed siblings 0x005FC2D0 / 0x005FC8A0 (ConstructQuestsScreen_CurrentQuests[PC]) are local models in an invented namespace; with this
  overlay they can be re-authored exactly like 0x005FCBE0 / 0x005FDDD0 (ConstructCurrentQuestsList[PC]: ActiveQuestCards, screen type 1).
- PositionMesh needs: a declaration in CTCTavernGame.h (static __fastcall, Ego_r ?PositionMesh@CTCTavernGame@@SIXPAVCThing@@ABVC3DVector@@ABVCRightHandedSet@@1MM@Z),
  CTCPhysicsBase virtual slot names 30/31/32/75 (+0x78/+0x7C/+0x80/+0x12C), CThing slots 27/28, CRightHandedSet::RotateAboutAxis 0x00A22F80.

## Parked rows: measured residue and next steps
- 005eb590 GivePrize (5 grades + c2prio): after hero_stats in its own `{}` (frame fixed) and the implicit "" conversion (lea push fixed), only a
  register tie remains: c2prio shows the def pointer's reload piece (#16, prio 46) coloured before `player` / `prize` (prio 32), so it takes
  esi and prize gets edi; retail has def in edi (load hoisted above the Create pushes) and prize in esi. Named def pointers are forwarded into
  def (no change). Next: try shapes that lower the def piece's block weight (e.g. AddRenown's def read moved) or raise player/prize's K.
- 005fbaa0 GetRelativePosOnMiniMap (9 grades): scale_y homed in the loop-index slot [esp+10h] (retail [esp+24h]); the final X*scale loads the
  scale first. One grade left.

## Not reached
005eabc0, 005ec270, 005eca50 were not attempted this pass (time went to the quest-list pair and GivePrize, which needed new shared headers).
Their prior drafts (prior/) are local models and need the header work listed above first.
