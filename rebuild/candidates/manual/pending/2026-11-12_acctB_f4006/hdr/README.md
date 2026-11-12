# chunk 4006 overlay headers (rows 0x005D1F0C / 0x005CE0E6, CTCInventoryStats personality page)

Use with `export FABLE_HDR_OVERLAY=/opt/fable_lane/w22/agent4006/hdr`. Every change is an opt-in block; the default text of each
repo header is unchanged (diff against rebuild/include: additions only). Row 0x005D1F0C grades RELOCATION_MATCH with these headers (continuation agent); the declarations
are backed by the PDB manglings and by one-to-one relocation pairing (tools/rel.py g60: 179 symbols on 179 retail targets, 0
inconsistencies; read-only vland RELOCATION_MATCH PASS).

| file | opt-in | what | evidence (two sources) |
|---|---|---|---|
| engine/CTCInventoryStats.h (+22) | FABLE_RETAIL_CTCINVENTORYSTATS_PERSONALITY | BuildHeroClass 0x005CD4C4, BuildRenownString 0x005CD433, ConvertMap 0x005CD6B7, CreateTitle 0x005CCF0E, CreateTitlePC 0x005CCFFC, GetFatnessAsString 0x005CC862, GetHeroSexualityAsString 0x005CCBBD, static __fastcall GetTimeSpentInGame 0x005CCC91 | Ego_r / FableWin manglings (LABELS.tsv) + the retail call sites of 0x005D1F0C (argument shapes); ConvertMap and GetFatnessAsString are landed rows |
| engine/CTCHeroStats.h (+74) | FABLE_RETAIL_CTCHEROSTATS_PERSONALITY (inside the SERIALISE layout) | twelve out-of-line statistics getters (0x0057D23B .. 0x0057AAD9) and about 45 inline one-line getters over the PDB members, GetMaxMoney = `std::max(MaxMoney, Money)` | PDB manglings (FableWin.pdb keeps out-of-line copies of the inline ones) + the retail `[edi + offset]` reads of 0x005D1F0C per label (the behaviour test compares them statement by statement) |
| engine/CTCHero.h (+5) | FABLE_RETAIL_CTCHERO_TITLEGUITEXT | `CWideString GetTitleGuiText() const` 0x006CA0A0 | reloc_bindings spelling (bound "same") + call site 0x005D29xx |
| engine/CTCInventoryItem.h (+5) | FABLE_RETAIL_CTCINVENTORYITEM_ISTROPHY | `static bool __fastcall IsTrophy(long)` 0x005D9930 | PDB mangling + landed row CTCInventoryItem_IsTrophy_005d9930.cpp |
| engine/COpinionReactionManager.h (+6) | FABLE_RETAIL_COPINIONREACTIONMANAGER_HEROSTATS | `float GetHeroStatsInOpinionFormat(EOpinion) const` 0x007009D0 (the includer declares enum EOpinion) | PDB mangling + three call sites with 0 / 4 / 2 |
| engine/NUISystem_CManager.h (+5) | FABLE_RETAIL_CMANAGER_STATICSLIDERBIDIRECTIONAL | `CreateStaticSliderBidirectional(const CWideString&, float, const C2DCoordI&)` 0x0042819A | PDB mangling + two call sites |
| engine/CTCHeroMorph.h (new) | none (new file) | CTCAppearanceMorphBase : CTCBase (StatGetInterfaceType = TCI_APPEARANCE_MORPH), CTCHeroMorph with `void GetTattooNames(std::vector<CWideString>&)` 0x0071CE80 | PDB mangling + the GetTC lookup key 3 at 0x005D2855 |
| engine/GFPackFloat.h (copy of the repo file, ONE line changed, NOT an opt-in) | none: the shared GFRoundFloatNear body | `return (GFFloatToLongNear(value * scale) / scale);` -- the return expression parenthesised (one no-code PAREN tuple after the divide) | row 0x005D1F0C: unparenthesised 5611 / 5617 instructions, parenthesised 5616 / 5617 (scheduler region cuts 0x005D4EB4 .. 0x005D5824); regraded under the overlay: landed includer 0x00AFBCE0 RELOCATION_MATCH (before and after), out-of-line GFPackFloat copy of the overlay header RELOCATION_MATCH at 0x00AF8D50 (probe/ool_pack.cpp). FITTED (`(float)(...)` gives the same bytes) |

Continuation agent's changes:
- engine/GFRoundFloatNear.h (the first agent's new header) is DELETED: the helper already exists in the shared engine/GFPackFloat.h.
  Inline-budget numbers (c2 inline trace, helper as plain `inline`): size 48, penalty 0, frequency 13.5 at each of the five sites,
  refused without a score (size-TU limit 15) -- tools/inl/plain.txt.
- engine/CTCHeroStats.h: + `long GetNumberOfBuildingsOwned() const { return GetNumberOfHousesOwned() + GetNumberOfShopsOwned(); }`
  inside FABLE_RETAIL_CTCHEROSTATS_PERSONALITY. FITTED, DESCRIPTIVE NAME (in no PDB). Evidence: retail 0x005D4698 and 0x005D00AB
  `lea edx,[eax+ecx]` (houses base, shops index) need one more inline level on the houses side (NOTES.md "g57 / g58").
- engine/CTCInventoryStats.h: + `CWideString GetCreatureGroupAsString(unsigned long)` 0x005CC9AD (FableWin mangling + landed row),
  for the same-TU ConvertMap body.

Debt / to check at landing:
- The local `enum EOpinion { OPINION_MORALITY, OPINION_RENOWN, OPINION_SCARINESS, OPINION_AGREEABLENESS, OPINION_ATTRACTIVENESS }`
  lives in the row source (values 0 / 2 / 4 are what retail pushes; the other two enumerator names are fitted to fill the gaps).
- Regrades: the opt-in headers leave defaults untouched (new macros). The one NON-opt-in change (GFPackFloat.h) was regraded against its
  only landed includer 0x00AFBCE0 (RELOCATION_MATCH with and without the overlay) and out of line against 0x00AF8D50.

Third agent (row 0x005CE0E6 ConstructPersonalityList, RELOCATION_MATCH): NO header change. The console row uses the same opt-ins as the PC
row plus the existing repo header engine/NUISystem_CActionParamState.h (FABLE_RETAIL_CACTIONPARAMSTATE, as the landed 0x005D6748) in
place of the ULONG / SLONG parameter headers. Regraded under this overlay after the row matched: 0x005D1F0C RELOCATION_MATCH (read-only
vland PASS), landed 0x00AFBCE0 RELOCATION_MATCH, landed 0x00B027D0 RELOCATION_MATCH.
