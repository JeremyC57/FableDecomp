# Chunk 563 (agent563, 2026-10-29): 11 of 12 never-tried rows land (RELOCATION_MATCH), 1 parked

CTCInventoryMap::GetMenuEntryFromCategory 0x005FAA90 and its PC twin GetMenuEntryFromCategoryPC 0x005FAC20 (PC_MENU_ENTRY
strings), ~CTCQuickAccessMenu 0x00625A20, CPersistContext::Transfer<EOpinionAttitudeType> 0x00685CC0 (catalog said
<signed char>; RTTI of the text-arm wrapper vtable 0x0125BABC), ~CThingPlayerCreature 0x006AA230, CTCBuyableHouse::SetRented
0x006C1150, ~CTCCreatureNavigation 0x006CC1E0, ~CAnimateThing 0x0070BA90, the implicit
NTCScriptedControl::CActionTalkBase dtor 0x0071A430 (catalog C3DMeshStripifier) and CTCEmoteIcon dtor 0x0071B120 (catalog
CCreatureAction_DragonFireBlastBase), both named from the RTTI of the caller's vtable, ~CTCRandomAppearanceMorph 0x00727B40.

Parked (pending/2026-10-27_linux_hand563/differ): 0x0071BA60 CTCHeroMorph::GetMorphStrength, 624v624, frame `sub esp,14h`
v retail 10h (case 11's GFLimitReturn temporaries reuse a case-9 slot in retail).

NAME_FIXES.tsv: 23 rows, deferred (naming pass). Review: 0x0070B8A0, 0x0070BA00, 0x00527910 and 0x004378C0 carry several
unrelated bound spellings (possibly folded bodies).
