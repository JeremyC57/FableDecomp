# Chunk 2012 header overlay (grade with `FABLE_HDR_OVERLAY=/opt/fable_lane/w22/agent2012/hdr`)

Every change to an existing header is an additive opt-in block: with none of the new macros defined each file preprocesses to the repo's
default text. The files below marked "chunk 2007" were copied unchanged from `pending/2026-11-11_acctB_c2007/hdr/` (not yet in
rebuild/include; see that hand-back's hdr/README.md): CAnimSet.h, CAnimateThing.h, CAnimationEntry.h, CCreatureActionBase.h,
CCreatureAction_UnsheatheToShowToSheatheTrophy.h, CFactionManager.h, CInputProcessGameBase.h, CInputProcessTavernGame.h,
CMessageEventFilters.h, CMessageEventManager.h, CRightHandedSet.h, CTCCarryable.h, CTCCreatureExpression.h, CTCCreatureNavigation.h,
CTCTalk.h, CTCTavernGame.h, CTCTavernGameCoinBase.h, CTCTavernGameShoveHaPenny.h, CTCTrophy.h, CTavernGameShoveHaPennyDef.h,
CThingAICreature.h, CThingCreatureBase.h, CTrophyDef.h, Coin.h. Row 0x006DE280 uses CTCTrophy.h, CTrophyDef.h (FABLE_RETAIL_CTROPHYDEF),
CTCCarryable.h (NONCONST_CARRIER), CThingAICreature.h (STATEGROUPENABLED), CCreatureActionBase.h, CThingCreatureBase.h from that set.

## New headers (this chunk)
- `engine/CCreatureAction_SheatheItemToInventory.h` (row 0x006DE280): RTTI class, vtable 0x0125C83C (PerformAction 0x0069F4F0,
  FinishAction 0x0069F670, Clone 0x0069F480), over the retail CCreatureActionBase (0xA8); PFollowUpAction (Ego_r +0xB4 -> retail +0xA8),
  sizeof 0xB0. Evidence: Ego_r struct layout (188 = 0xB0 + 12 for the base's containers) + TerminateMinigame's temporary filling the end
  of its 0xC4 frame ([esp+24h]..[esp+0D4h]); ctor 0x0069FFD0 (creature, follow-up, priority) / dtor 0x0048C950 from both PDB name sets
  and the bindings.
- `engine/CTCHeroMorph.h` (row 0x006D1250): CTCBase-derived component, TCI_APPEARANCE_MORPH (3), GetTattooThings 0x0071DE20 (Ego_r / FableWin
  spelling; Ego_r order GetTattoos / GetTattooThings = retail 0x0071DD50 / 0x0071DE20). Declaration only.

## Opt-ins added to existing headers (this chunk)
- `CWorld.h` FABLE_RETAIL_CWORLD_REGIONLOADCONFIRMATION: inline `SetRegionLoadConfirmationAsGiven() { RegionLoadWaitingForConfirmation =
  false; }` (FableWin.pdb ?SetRegionLoadConfirmationAsGiven@CWorld@@QAEXXZ, COMDAT 0x0249D470 right after TerminateMinigame; retail
  `mov byte ptr [eax+130h],0` after GetWorld, 0x006DE89E; RegionLoadWaitingForConfirmation +0x130 in the header).
- `CTCHeroStats.h` FABLE_RETAIL_CTCHEROSTATS_TROPHYWITNESSES: inline Get/SetBestTrophyWitnesses (FableWin 0x021BC9D9 / 0x0249D490) and
  a layout branch with BestTrophyWitnesses +0xDC (retail 0x006DE571 / 0x006DE579); FABLE_RETAIL_CTCHEROSTATS_FATNESS: inline GetFatness
  (FableWin 0x01E65BE1) and a layout branch with Fatness +0x38 (row 0x006D1250, 0x006D180E). Both layout branches sit after every
  existing branch (an earlier opt-in wins).
- `CTCCreatureOpinionOfHero.h` FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_CLEARPERSONALITY: `ClearPersonalityOverride()` 0x00682310
  (Ego_r.pdb 0x00593490 after both SetPersonalityOverride overloads; no-argument calls 0x006DE402 / 0x0089C4FD). Declaration only.
- `NGameText_CDataBank.h` FABLE_RETAIL_NGAMETEXT_CONSTRUCTSTRING_LONGS3 (nested in CONSTRUCTSTRING_LONGS): fastcall
  ConstructString(CWideString&, long, long, long) 0x009C7BA0 (Ego_r J/JJ/JJJ run; `ret 8`).
- `CTCWife.h` FABLE_RETAIL_CTCWIFE_APPEARANCEBOOL: MS forward enum EWifeAppearanceCategories and the private inline
  `bool& AppearanceBool(EWifeAppearanceCategories c) { return BooleanHusbandAppearances[c - 12]; }` (FableWin COMDAT 0x02489F40 right
  after CheckAppearanceChanges; retail reads / writes +0xE9..+0xEB for categories 13..15). Used by the parked row only.
- `CTattooDef.h` FABLE_RETAIL_CTATTOODEF_MEMBERS (nested in FABLE_RETAIL_CTATTOODEF): the Ego_r members TattooName +0x28 .. CoversBodyAreaFlags
  +0x40 .. SpecificCoversBodyAreaFlags +0x48 (no containers; the parked row tests +0x40, 0x006D16CD). Used by the parked row only.
