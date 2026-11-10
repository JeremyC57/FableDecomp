# chunk 1011b notes (agent1011b)

Every grade ran with `FABLE_HDR_OVERLAY=$A/hdr` exported (overlay: engine/CTCInventoryMap.h, opt-in
FABLE_RETAIL_CTCINVENTORYMAP_QUESTREVIEW, documented in hdr/README.md). Grades are in ATTEMPTS.log (chunk 1011's five GivePrize grades and
nine GetRelativePosOnMiniMap grades are in ATTEMPTS_chunk1011.log). c2prio / frame-packer probes and their objects (wk/) are not grades.

## Result
| row | identity | grade | grades | lever / residue |
|---|---|---|---|---|
| 005eb590 | ?GivePrize@CTCTavernGame@@QAEXXZ (Ego_r/FableWin; header binds it) | RELOCATION_MATCH | 7 (5 in 1011) | named `const CPlayerManager& player_manager = PeekPlayerManager();` |
| 005fbaa0 | CTCInventoryMap::GetRelativePosOnMiniMap (Ego_r 0x0064CBE0) | DIFFER(634v634), 2 lines | 10 (9 in 1011) | x87 operand order of the last X * MiniMapScale |
| 005fc2d0 | ?ConstructQuestReviewList@CTCInventoryMap@@AAEPAVCComponent@NUISystem@@PAV23@@Z | RELOCATION_MATCH (rows/) | 2 | TC pointer zero-initialised |
| 005fc8a0 | ?ConstructQuestReviewListPC@CTCInventoryMap@@AAEPAVCComponent@NUISystem@@PAV23@0@Z | RELOCATION_MATCH (rows/) | 1 | twin of 005fc2d0 |
| 005eabc0 / 005ec270 / 005eca50 | PositionMesh / ProcessInput / DrawPreparePrimitivesForRendering | not reached | 0 | header debt below |

## Re-authored landed rows (rows/, for the coordinator to rename the landed files)
- 0x005FC2D0 is `CTCInventoryMap::ConstructQuestReviewList(NUISystem::CComponent* parent)`, module CTCInventoryMap. Evidence: Ego_r.pdb
  0x0064C6C0 and FableWin.pdb 0x02088CA0 carry `?ConstructQuestReviewList@CTCInventoryMap@@AAEPAVCComponent@NUISystem@@PAV23@@Z`; the
  CTCInventoryMap name sets (both PDBs) have exactly two private `(CComponent*)` quest-list builders, ConstructPastQuestsList (proven
  at 0x005FCBE0) and this one, and FableWin places them next to each other; retail's ConstructQuestsScreen calls 0x005FC2D0 (0x005FE231)
  right after 0x005FCBE0 (0x005FE115); `ret 4`; the body reads CQuestManager::ActiveQuestCards (+0x5C) where the past list reads
  CompletedQuestCards, uses "UI_TITLE_QUEST_CURRENT" and screen type 1. Landed as `_global::ConstructQuestsScreen_CurrentQuests` (file
  global_ConstructQuestsScreenCurrentQuests_005fc2d0.cpp, invented namespace NScreen005fc2d0) -> NAME_FIXES.tsv.
- 0x005FC8A0 is `CTCInventoryMap::ConstructQuestReviewListPC(NUISystem::CComponent* container, NUISystem::CComponent* parent)`: Ego_r.pdb
  0x0064CE90 / FableWin.pdb 0x020AEC70 `?ConstructQuestReviewListPC@CTCInventoryMap@@AAEPAVCComponent@NUISystem@@PAV23@0@Z`, the only
  other `(CComponent*, CComponent*)` builder besides ConstructPastQuestsListPC 0x005FDDD0 (FableWin neighbours), called from
  ConstructQuestsScreenPC (0x006027E9), `ret 8`, PC_ strings, CTCInventoryQuests::ConstructQuestListPC 0x0061E6D0.
- Shape: the past lists copy the completed cards by value; the review lists walk `CQuestManager::Get().GetActiveQuestCards()` by const
  reference (no copy, frame 0x24 / 0x28), and retail zero-initialises the CTCInventoryQuests pointer (`mov [esp+18h],ebp` with ebp = 0
  before the UsingComponents test). Without the `= 0` the build is 553 v 562 (no zero store, and the count loop head loses its alignment).
  No STLP_LIST_CLEAR_NOINLINE (no list copy). Tests: rows/<addr>_test.cpp (g++ and VC7.1 /Od PASS).

## 0x005EB590 GivePrize: how it matched
c2prio (`--trace --blocks`): `hero` (prio 34, key 5) was coloured before `player` / `prize` (32, keys 95 / 58); when the split pass then cut
the def pointer, its piece (prio 46; it covers the prize block and, through the no-deed-log path, the release at the end) took esi (table
order) and the prize was left with edi. Retail: def edi (`mov edi,[esp+10h] ; ... mov ecx,[edi+180h]` before the Create pushes,
0x005EB677), prize esi (`mov esi,eax` 0x005EB69B). Diagnostic: removing the PeekPlayerManager inline's reference-return local from
block 1 (K 6 -> 5) dropped hero to 30 and matched (probe only; it needs protected member access). Natural spelling: a named call-result
reference for the player manager in the prize block (`const CPlayerManager& player_manager = PeekPlayerManager(); long player =
player_manager.GetMainPlayerNumber();`) adds one candidate to that block (K 8 -> 9): player/prize 36 tie hero 36 and win on key, both take
esi, the def piece takes edi. Same instructions otherwise. Probes that did not move it: named def pointer / reference (forwarded by c1),
RELEASE_POINTEE, orientation/position order, hero typed CThingPlayerCreature*, split player-ref statement (hero 38), loop spellings
(while, cached end, named creature), named owner / physics (no K change), named search tools (moves the call: code change), early-return
form (659 bytes), zero-initialised receive_items, named GetTC bool.

## 0x005FBAA0 GetRelativePosOnMiniMap (parked at 10 grades, 2 lines)
- Frame (fixed, grade 10): the frame-packer trace (hand886 tools8/phook.py driven through c2dbg.probe, wk/m0.frame.json) shows the separate
  `float scale_y` joining the loop index's slot. Retail's frame (i 10h, top 14h, top-left 18h, bottom-right 20h with Y in ebp, scale.Y 24h,
  maps 28h) is an 8-byte `C2DVector scale(1.0f, 1.0f)` sharing the bottom-right slot; `relative_pos *= scale` (C2DVector::operator*= under
  FABLE_RETAIL_C2DVECTOR) and the field-wise form give the same bytes.
- Remaining: the final `relative_pos.X * region.GetMiniMapScale()` loads the scale first (ours `fld [ebp+2Ch] ; fmul [esi]`, retail
  `fld [esi] ; fmul [ebp+2Ch]`, 0x005FBCC7). c2prio: the scale read is CSE temp #45 over the four final statements; X is through the
  parameter pointer (#4). Probes that leave it unchanged: operand swap, outer swap, extra parentheses, named scale local (worse, 638),
  compound `X *= scale` (same), scale-first everywhere (same), C2DVector forms of the offset add (643) and of the final pair (650).
  Next: find which IL operand c2 puts first for two non-frame memory operands (the offset lines keep retail's order `fld [ebp+30h] ;
  fmul [ebp+2Ch]`, so it is not simply the larger id), e.g. a micro-TU with c2prio `--rotation`.

## Not reached (header debt)
- 0x005EABC0 PositionMesh (static __fastcall, Ego_r `?PositionMesh@CTCTavernGame@@SIXPAVCThing@@ABVC3DVector@@ABVCRightHandedSet@@1MM@Z`):
  calls CThing slot 27 (+0x6C) and slot 28 (+0x70). Retail vtable 0x01239ABC: slot 27 = 0x004C7790 `?DrawIsDrawable@CThing@@UBE_NXZ`,
  slot 28 = 0x004C8E50 `?SetAsDrawable@CThing@@UAEX_N@Z` (LABELS.tsv). To declare them in engine/CThing.h, slots 17..26 are needed in order:
  17 0x004C7B80 IsToBePlacedInMap, 19 0x004C8050 WorldUpdate, 20 0x004C8BA0 WorldUpdateBulletTime, 23 0x004C7DA0 GetChecksum, 24 0x004C7B00
  GetInfo (labels), but 18 0x004C7470, 21 0x004C7460, 22 0x004C7870, 25 0x004C74B0, 26 0x004CD1D0 have no proven name (and slots 2, 5-7,
  13, 14 are only partly named in FABLE_RETAIL_CTHING_LOADSINGLE). CTCPhysicsBase.h already carries the PDB virtuals (SetPosition etc.);
  CRightHandedSet::RotateAboutAxis 0x00A22F80 needs a declaration; the prior draft (prior/005eabc0.cpp, local model) is DIFFER(495v511)
  with an x87 temporary residue.
- 0x005EC270 ProcessInput: retail calls `?IsEventButtonPressed@CInputProcessGameBase@@IBE_NABVCInputEvent@@@Z` (protected in both PDBs) on
  the CInputProcessTavernGame* argument, so the access path (friend?) is unproven; 0x00687D00 is a `mov ecx,[ecx+24h] ; jmp 0x00445C30`
  forwarder with the (EGameAction, const CInputEvent&, C2DVector&) shape of FableWin
  `?GetControlMovementFromGameActionEvent@CInputProcessGameBase@@QBE_NW4EGameAction@@ABVCInputEvent@@AAVC2DVector@@@Z` (lead, one source);
  0x00A0D390 (`mov byte [ecx+0A8h],0 ; mov dword [ecx+4],2`) is a CProcessedInput setter, FableWin candidates SetAsPerformedEvent /
  Clear (lead). CTavernGameDef names Quit/Yes/No (+0xFC..) and the ETGS_* enumerators exist in the shared headers.
- 0x005ECA50 DrawPreparePrimitivesForRendering: not attempted (1280 bytes; local-model prior DIFFER(1185v1176)).
