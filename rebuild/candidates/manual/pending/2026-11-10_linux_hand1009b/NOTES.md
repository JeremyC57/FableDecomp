# Chunk 1009b -- CTCInventoryTrade (11 rows) -- notes

All compiles and grades ran with `export FABLE_HDR_OVERLAY=$A/hdr` (gr.sh / pv.sh / c2.sh / gall.sh export it); the overlay
adds declaration-only opt-ins (hdr/README.md). Frame-pointer size TU for every row: `#pragma optimize("y", off)` + `("s", on)`.
Every relocation of the four MATCH rows was checked against rebuild/oracles/reloc_bindings.tsv targets (tools/relcheck.py: 0 mismatches).

## Results
| row | name (PDB) | grade | grades | lever / residue |
|---|---|---|---|---|
| 0x0059D270 | AddWantedItemToListPC | RELOCATION_MATCH | 1 | shared headers (CMenuEntry 151-153 slots) |
| 0x005A3E47 | ConstructBuyActionsList | RELOCATION_MATCH | 1 | shared headers; GetTC expanded, containable cap + affordability step-down loop |
| 0x005A79E6 | ConstructBuyActionsListPC | RELOCATION_MATCH | 1 | PC twin (PC_LIST_BUY_ACTIONS, 0.0f/1.0f, PC_MENU_ENTRY_TEMPLATE) |
| 0x005A29F4 | ConstructSpecialShopList | RELOCATION_MATCH | 3 | STLP_LIST_SIZE_DISTANCE; named CText* subtitle; block-scoped "Trade" CCharString |
| 0x0059FC17 | AlreadyHaveSpecial | DIFFER(725v716) | 2 | tattoo loop only (see below) |
| 0x0059FEE3 | AddBuyItemToList | DIFFER(1552v1556) | 1 | this vs inventory register choice |
| 0x005A6769 | ConstructNormalShopListPC | DIFFER(1336v1336) | 1 | wanted-sum operand order only |
| 0x005A6CA1 | AddBuyItemToListPC | DIFFER(1669v1662) | 1 | shared headers; same this/inventory residue as 0x0059FEE3 (+ slot shifts, arms not head-merged) |
| 0x005AD305 | ApplyTattooCard | parked | 0 | not reworked (header debt below); differ/ = prior chunk's local-model draft |
| 0x005ACEDA / 0x005A94BD | OpenInventory / OpenInventoryPC (pair, which-is-which unproven) | held | 0 | both open with PSoundManager (0x013B8394) +0xB8 SetGroupVolume (slot 46) calls -> CASoundManager slot names -> held |

Grades are fewer than the 10-per-row budget on the parked rows because each further grade would have been a byte-identical
re-run: every variant was first measured for free (tools/cmpr.py: local normalised diff against dis_all.txt; c2prio for the
colouring order) and none moved the residue: ~40 for 0x005A6769, ~25 for 0x0059FEE3 (plus the earlier c2v_* set), ~30 for 0x0059FC17.

## What the unmatched rows need next
- 0x0059FC17: shared-header source is instruction-identical except the tattoo loop. With a named `card` VC7.1 hoists
  `&card->BankIndex` (`lea edi,[ecx+34h]`) and spills card; with the CSE'd `tattooCardDef->` form there is no hoist but c2 colours
  the card temp (prio 16) before morph (prio 9) so card/morph swap esi/edi and the count lands in [ebp-10h] not [ebp-0Ch]. Retail
  needs morph coloured first. Untried: a different loop shape that changes the K of the loop blocks (e.g. the conflict test as a
  helper), or the original possibly testing `tattoos[i]` through an iterator copy.
- 0x0059FEE3 / 0x005A6CA1: retail colours the 0-initialised inventory pointer into ebx (its `xor ebx,ebx` also zeroes the entry
  handle) and splits `this` (memory home [ebp-0Ch] reused later as a CWideString temp, then esi/edi); ours: this ebx (prio 17),
  inventory 13. Tried: ternary, if/else, PeekTC, uninitialised inventory (worse), declaration order, `this` copy, bool-condition local.
  Next: find what raises inventory's priority (an extra use in a high-K block) -- once found, 0x005A6CA1 is a sed of the same source.
- 0x005A6769: only the commutative sum order; probe the K / tuple-key ordering with c2prio --rotation on the sum's expression temps.
- 0x005AD305: needs shared models for CTCAppearanceMorphBase (UploadTextureMorphsToEngine), CTCHeroMorph::AddTattoo /
  FrameUpdate / GetTattooThings and std::set<long> before a shared-header rewrite; the prior residue is a 3-way register cycle.

## Header debt
- engine/CTCHeroMorph.h does not exist outside _quarantine; the overlay adds a minimal opt-in class (hdr/README.md).
- CTCAppearanceMorphBase has no header (needed by 0x005AD305).
- CTCHero's tail members beyond HeroTitleObjectDef stay opaque.

## Callee identities used (all PDB spellings, reloc-checked on the MATCH rows)
CManager::GetInstance 0x0041E5F2, CreateMenuEntryComponent 0x00428494, CreateComponent, CMainGameComponent (0x013B86A0),
CPlayerManager::GetMainPlayer 0x00449960/0x00449970, CPlayer::PeekPControlledCreature 0x00487DD0 / GetPControlledCreature 0x00487DC0,
CTCInventoryItem::GetInventoryType 0x005D99D0 / GetHairCardDef 0x005D9A10 / GetTattooCardDef 0x005D9AA0,
NInventory::CTCInventoryBase::GetAmountOfItemsContainable 0x005BF30D / IsSpaceInInventoryForItem 0x005BF2ED,
CTCInventoryTrade::IsItemAffordable 0x0059DA10 / IsSpecialCase 0x0059C983 / AlreadyHaveSpecial 0x0059FC17,
map<CComponent*,CComponent*>::operator[] 0x005B3167, CTattooDef::IsConflicting 0x0059B679, CTCHeroMorph::GetTattoos 0x0071DD50 /
HasAPermanentTattooCoveringArea 0x0071C2D0, CTCHeroAttachableAppearanceModifiers::IsWearingAppearanceModifier 0x00706000.

## Tools written (tools/)
pc.py, pv.sh (probe compile), gr.sh (grade + log), sd.py (masked diff of a grade), cmpr.py + vcmp.sh (FREE local diff of
a probe / a batch of variants against dis_all.txt), variants.py, c2.sh, rdis.py, rstr.py, relcheck.py + rc.sh.
