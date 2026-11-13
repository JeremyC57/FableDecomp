# agentC1 overlay (acctC lane): copies of TODAY's rebuild/include headers + NEW opt-ins only

Built by tools/mkhdr.py + tools/spec.py from rebuild/include (2026-11-13) and the opt-in blocks of the chunk-4006 hand-back
(ref/005d1f0c/hdr_STALE, re-applied to the current text). No default path changed. FABLE_STATIC_ASSERTs that moved are pinned with
FABLE_STATIC_ASSERT_AT(<old line>, ...) by lane_merge/pin_asserts.py (CTCHeroStats.h 112, CTCInventoryItem.h 10, CTCHero.h 2,
CTCInventoryStats.h 1, COpinionReactionManager.h 1), so every typedef name is unchanged. equiv_check.py over all 56,187 landed rows
(sources + tests): preprocessed text identical with and without this overlay (0 differing rows).

| header | opt-in | what | evidence |
|---|---|---|---|
| engine/CTCInventoryStats.h | FABLE_RETAIL_CTCINVENTORYSTATS_PERSONALITY (new; 2 blocks) | `enum EHeroSexuality;` forward; BuildHeroClass 0x005CD4C4, BuildRenownString 0x005CD433, ConvertMap 0x005CD6B7, CreateTitle 0x005CCF0E, CreateTitlePC 0x005CCFFC, GetCreatureGroupAsString 0x005CC9AD, GetFatnessAsString 0x005CC862, GetHeroSexualityAsString 0x005CCBBD (GetTimeSpentInGame comes from the existing FABLE_RETAIL_CTCINVENTORYSTATS_TIMESPENT) | Ego_r / FableWin manglings (LABELS.tsv) + the call sites of both rows; landed rows for ConvertMap / GetFatnessAsString / GetCreatureGroupAsString |
| engine/CTCHeroStats.h | FABLE_RETAIL_CTCHEROSTATS_PERSONALITY (new; 2 blocks, inside the SERIALISE layout) | twelve out-of-line statistics getters (0x0057D23B .. 0x0057AAD9) and the inline one-line getters over the PDB members (GetAge / GetBestTrophyWitnesses come from the existing _AGE / _TROPHYWITNESSES opt-ins). GetNumberOfHousesOwned has a FITTED body `{ long houses = NumberOfHousesOwned; return houses; }` (see NOTES.md). NO GetNumberOfBuildingsOwned | PDB manglings; retail keeps this TU's inline getter COMDATs at 0x005CC58E..0x005CC685 (`mov eax,[ecx+off] ; ret`, header order) |
| engine/CTCHero.h | FABLE_RETAIL_CTCHERO_TITLEGUITEXT (new) | `CWideString GetTitleGuiText() const` 0x006CA0A0 | Ego_r / FableWin mangling + call site 0x005D29xx |
| engine/CTCInventoryItem.h | FABLE_RETAIL_CTCINVENTORYITEM_ISTROPHY (new) | `static bool __fastcall IsTrophy(long)` 0x005D9930 | PDB mangling + landed row 0x005D9930 |
| engine/COpinionReactionManager.h | FABLE_RETAIL_COPINIONREACTIONMANAGER_HEROSTATS (new) | `float GetHeroStatsInOpinionFormat(EOpinion) const` 0x007009D0 (the includer declares EOpinion) | PDB mangling + landed row 0x007009D0; enumerators: Anniversary Test PDB LF_ENUM EOpinion |
| engine/NUISystem_CManager.h | FABLE_RETAIL_CMANAGER_STATICSLIDERBIDIRECTIONAL (new) | `CreateStaticSliderBidirectional(const CWideString&, float, const C2DCoordI&)` 0x0042819A | PDB mangling + two call sites |
| engine/CTCHeroMorph.h | FABLE_RETAIL_CTCHEROMORPH_TATTOONAMES (new) + the class guard line extended `|| defined(FABLE_RETAIL_CTCHEROMORPH_TATTOONAMES)` | `void GetTattooNames(std::vector<CWideString>&)` 0x0071CE80 (non-const); `class CWideString;` forward | Ego_r / FableWin mangling + landed row 0x0071CE80; GetTC key TCI_APPEARANCE_MORPH 3 (`push 3 ; pop edi` 0x005D2853) is the existing class's |
| engine/GFPackFloat.h | FABLE_RETAIL_GFROUNDFLOATNEAR_GROUP_QUOTIENT (new `#elif`) | `return (GFFloatToLongNear(value * scale) / scale);` -- replaces the stale overlay's change of the SHARED default body | rows 0x005D1F0C / 0x005CE0E6: default body DIFFER (scheduler cut, 10 hunks), GROUP_PRODUCT 3 hunks (0x005D52F9 bank load), GROUP_QUOTIENT RELOCATION_MATCH. FITTED (no-code paren) |
