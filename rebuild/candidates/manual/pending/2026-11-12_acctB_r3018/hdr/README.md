# hdr/ overlay (chunk 2016)

Grade with `export FABLE_HDR_OVERLAY=/opt/fable_lane/w22/agent2016/hdr`. Every change to an existing header is an opt-in macro (the
default path is untouched, so landed includers keep their bytes). Copies were taken on 2026-11-11 from HEAD or from the named
hand-back overlay; diff each against HEAD (and the other pending overlays) and re-apply the opt-in blocks when landing.

## New headers (row 0x007D6870, parked -- needed only by differ/007d6870.cpp)
| header | content | evidence |
|---|---|---|
| `engine/CCard.h` | CCard (class key V), Ego_r.pdb members Thing .. Index, LocalPos +0x27 (byte-packed C3DVector), Width +0x4C, Height +0x50, sizeof 0x58; `void Turn()` 0x008F0620 | Ego_r.pdb layout (88 bytes, natural layout reproduces every offset); retail reads +0x24/+0x25 (0x007D69E5..), +0x27/+0x2B/+0x2F/+0x4C/+0x50 (0x007D6C85..) |
| `engine/CTCTavernGameCardBase.h` | CTCTavernGameCardBase : CTCTavernGame; virtual slots 51..61 (DealCards, CardsMoving, CardsFinishedMoving, CardsTurning, TurnCardsBack, CardsMainGame, GetCardOrder, GetRandomCard, SelectCard, ShuffleCards, UpdateHighlightedCard) + ProcessInput override; GetCardDef 0x008F0B20; members GamePositionRHSet +0xF0 .. CurrentMovingCard +0x310, sizeof 0x318 (8-aligned) | Ego_r.pdb member list at PDB - 0x18 - 4 per earlier STLport vector; vtables 0x01288294 / 0x0126F5B4 / 0x0126F6FC slot map vs the Ego_r / FableWin override sets; landed Blackjack ctor starts at +0x318 |
| `engine/CTCTavernGameCardPairs.h` | CTCTavernGameCardPairs : CTCTavernGameCardBase; ProcessInput / SelectCard overrides; AlternateSelectedCard +0x318 .. PreviousDirection +0x328, sizeof 0x330 | Ego_r.pdb members at PDB - 0x20; landed CardsMainGame 0x007D62D0 (+0x318); 0x007D6B55 / 0x007D6CFF / 0x007D6D22 |

## Copies from earlier hand-backs (unchanged)
- `engine/CTCTavernGame.h`, `engine/CInputProcessGameBase.h`, `engine/CInputProcessTavernGame.h`, `engine/CRightHandedSet.h`: chunk 2007
  overlay (pending/2026-11-11_acctB_c2007/hdr), byte-identical copies (row 0x007D6870 only).

## Opt-ins added to existing headers
| header (copied from) | opt-in | content | default path |
|---|---|---|---|
| `engine/CThing.h` (c2008 overlay) | `FABLE_RETAIL_CTHING_CONSTRUCTFROMLEVELSCRIPT` (inside the c2008 CONSTRUCTFROMBINARYSAVE block, i.e. inside LOADSINGLE) | protected `void CreateSerialiseCreatableTCs(const CCharString&)` 0x004CA230 | unchanged |
| `engine/CThingManager.h` (c2008 overlay) | `FABLE_RETAIL_CTHINGMANAGER_LEVELBEINGLOADED_OUTOFLINE` (inside LOADSINGLE) | GetLevelBeingLoaded() const declared only (the CThing TU calls 0x0051E2C0) | inline body (unchanged) |
| `engine/CInventoryDef.h` (HEAD) | `FABLE_RETAIL_CINVENTORYDEF_FULL` | the remaining 374 Ego_r.pdb members after MeshAreaBRPos, typed at retail offsets (GenericFont +0xCC, QuestCardInfoTLPos +0x7A0, QuestCardInfo2TLPos +0x7A8, QuestCardInfoTextOffset +0x7BC; sizeof 0xA34), static asserts; forward `enum EInventoryIdentifier;` | partial class (unchanged) |
| `engine/NInventory_CTCInventoryBase.h` (c2002 overlay) | `FABLE_RETAIL_CTCINVENTORYBASE_DRAWTEXT` | `NInventory::ETextLayout` (descriptive enumerators LEFT/CENTRE/RIGHT = 0/1/2), static __fastcall Draw2DText 0x005BCAFE, DrawInventoryText 0x005BCE49 | unchanged |
| `engine/CTCInventoryQuests.h` (c2002 overlay) | `FABLE_RETAIL_CTCINVENTORYQUESTS_TESTQUESTS` (inside OPENPC) | GetTestQuestsToDisplay(std::vector<CTestQuest>&) 0x0061A8A0, DrawActiveTestQuests 0x0061AD60; includes engine/CTestQuest.h | unchanged |
| `engine/CTestQuest.h` (HEAD) | `FABLE_RETAIL_CTESTQUEST_NAMECOMPARE` | class CTestQuestNameCompare with declared `bool operator()(const CTestQuest&, const CTestQuest&) const` (Ego_r.pdb ??RCTestQuestNameCompare@@QBE_NABVCTestQuest@@0@Z) | unchanged |
| `engine/CWorld.h` (HEAD) | `FABLE_RETAIL_CWORLD_ACTIVETESTQUESTS` (needs FABLE_RETAIL_CWORLD) | inline `const std::vector<CTestQuest>& GetActiveTestQuests() const` (FableWin.pdb 0x01F7D6A0) | unchanged |
| `engine/CRenderManager2D.h` (HEAD) | `FABLE_RETAIL_CRENDERMANAGER2D_BOXCOLOURS` | `bool Draw2DBoxColours(const C2DBoxF&, const CRGBColour colours[], unsigned long)` 0x009DDA50 (array parameter = QBV) | unchanged |
| `engine/CQuestManager.h` (HEAD) | `FABLE_RETAIL_CQUESTMANAGER_ISQUESTACTIVE` | `bool IsQuestActive(const CCharString&) const` 0x004AF610 | unchanged |
| `stlport/fable_containers.h` (HEAD) | `FABLE_RETAIL_STLP_DESTROY_AUX_FASTCALL` | `__destroy_aux(I, I, const __false_type&)` __fastcall (spelled `__fastcall` under `_MSC_VER`, since rebuild_abi.h may not be included yet) without the AUGMENTATION_PARTICLES side effects | cdecl (unchanged) |

Note on the stlport overlay: repo headers include `"stlport/fable_containers.h"` relative to rebuild/include, which wins over the -I
overlay; row 0x0061AD60 therefore includes `stlport/fable_containers.h` FIRST so the overlay copy is the one loaded (include guard).
Folding the opt-in into rebuild/include removes the need.

## Chunk 3018 (continuation of row 0x007D6870)
No header changes kept. Probe-only copies (C3DVector.h, CDefPointer.h with diagnostic opt-ins) moved to probe/hdr_probes/; the hand-in
differ/007d6870.cpp grades 566/571 with this overlay as it stands.

## Chunk 3018 session 2
No header changes: src/007d6870.cpp grades RELOCATION_MATCH with this overlay unchanged. probe/hdrA/ (C3DVector.h with FABLE_PROBE_*
operator- / operator^ / Dot texts) is probe-only and must not be landed.
