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

## Row 3 (0x0060EEB0, DIFFER) opt-ins, re-based from the bwf1 / agent4008 overlay onto today's headers (tools/rebase.py; no default text changed)
| header | opt-in | what | evidence |
|---|---|---|---|
| engine/CTCInventoryMap.h | FABLE_RETAIL_CTCINVENTORYMAP_MAPMEMBERS (new; 3 blocks) | pmap_real CZoomingComponent* +0x188, MapList CList* +0x214, MapRegions map<long,CRegion> +0x218 typed (#else = the old opaque bytes) | Ego_r.pdb member types; retail stores / calls on those offsets (comment in the block) |
| engine/CTCInventoryMap.h | FABLE_RETAIL_CTCINVENTORYMAP_GUILDSEALRECALLREGION (new) | `long GetGuildSealRecallRegion() const` private, 0x005FA990 | Ego_r + FableWin mangling, landed row |
| engine/CWorldMap.h | FABLE_RETAIL_CWORLDMAP_REGIONCOUNT (new) | `long GetRegionCount() const` 0x004FB470 | Ego_r + FableWin mangling |
| engine/CTCQuestCard.h | FABLE_RETAIL_CTCQUESTCARD_TELEPORTERREGIONNAME, _ISVIGNETTE (new) | GetTeleporterRegionName 0x007021F0, IsVignette 0x007022E0 | Ego_r + FableWin manglings |
| engine/CRegion.h | FABLE_RETAIL_CREGION_ASSIGN, _WORLDMAPNAMEGRAPHIC (new) | operator= 0x00519B80, GetWorldMapNameGraphic 0x006BC380 | reloc_bindings + both PDBs |
| engine/CWorld.h | FABLE_RETAIL_CWORLD_GUILDSEALRECALLPOS (new) + the two existing `#ifdef FABLE_RETAIL_CWORLD_TYPED_SAVE` lines of the GuildSealRecallPos member and its assert extended `|| defined(FABLE_RETAIL_CWORLD_GUILDSEALRECALLPOS)` | inline `const C3DVector& GetGuildSealRecallPos() const` | FableWin.pdb name; retail reads +0x154..0x15C in place |
| engine/NUISystem_CUIState.h | FABLE_RETAIL_CUISTATE_IMPLICIT_COPY (new `#ifndef` around the declared copy ctor) | implicit member-wise copy | retail copies the pair<const unsigned long, CUIState> member by member (0x00611349..0x0061140C) |
Moved asserts pinned (CWorld.h 120, CWorldMap.h 37, CRegion.h 20, CTCQuestCard.h 19, CTCInventoryMap.h 8, NUISystem_CUIState.h 1).
equiv_check.py over all 56,187 landed rows with the WHOLE overlay: 0 differing rows. Rows 1 / 2 regraded after these were added:
RELOCATION_MATCH both; read-only vland: 2 wins, 0 RELOC_IDENTITY / CONFLICT / NOT_GENUINE.
| engine/CCountedPointer.h | FABLE_RETAIL_CCOUNTEDPOINTER_RESET_ELSE_NULL (new, agentC4) | Reset body with the count block from the argument and `else Info = 0;` (c1 108 v 95) | row 2 0x005FF190 inline split of Reset (9 sites) reproduced exactly; FableWin Reset 69i v 44i. No assert moved (the only assert is line 59, above). FITTED inline-cost text: coordinator to judge vs the landed 0x00429C6F default text |
| stlport/fable_map.h | FABLE_RETAIL_STLP_MAP_FORCEINLINE_DTOR (new, agentC4) | explicit FABLE_FORCEINLINE ~map() {} (the includer must include fable_compiler.h) | row 0x005FF190 expands its map<long,pair<long,long>> destructor (0x00600136..); the implicit one is never scored by c2 (inline trace). Inline-budget device (EXTRA_RULES 2026-11-08 pref. 3). The size assert moved 699 -> 706: pinned FABLE_STATIC_ASSERT_AT(699, ...) |
| engine/CTCInventoryAbilities.h | FABLE_RETAIL_CTCINVENTORYABILITIES_PCMAGIC (new, agentC5; 3 lines inserted before the _LEARNEDEXPRESSION block) | declares `NUISystem::CComponent* ConstructPCMagicList(NUISystem::CComponent* parent);` 0x005F1B10 (public) | Ego_r.pdb + FableWin.pdb mangling ?ConstructPCMagicList@CTCInventoryAbilities@@QAEPAVCComponent@NUISystem@@PAV23@@Z; only caller 0x00422D85 in landed CManager::Action. The 22 FABLE_STATIC_ASSERTs below moved +3 lines and are pinned FABLE_STATIC_ASSERT_AT(<old line>, ...) by lane_merge/pin_asserts.py (219 and 329..349). equiv_check NOT run for this addition (declaration under a new macro only). |
(agentC5) hdrx/ is a DIAG-only overlay (Reset text experiments FABLE_DIAG_RESET_*, Validate {} body): never hand it back.
(agentC7) No header change. hdrx/ is a DIAG-only overlay (Reset / operator-> hooks): never hand it back.
(agentC9) No header change. Row 0x005F1B10 uses only existing opt-ins (adds FABLE_RETAIL_STLP_MAP, an existing repo opt-in, to its TU defines).
(agentC10) No header change.

## agentC11 additions
| engine/CExpressionSubDef.h | FABLE_RETAIL_CEXPRESSIONSUBDEF_STATGETTYPENAME (new, nested in FABLE_RETAIL_CEXPRESSIONSUBDEF) | `static CCharString __fastcall StatGetTypeName()` inline, returns CCharString("CExpressionSubDef") -- lets the shared GetSubDef body (FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY) instantiate for CExpressionSubDef | FableWin.pdb ?StatGetTypeName@CExpressionSubDef@@SI?AVCCharString@@XZ (0x01E049C0) + the landed 0x005DA240 row's spelling. The two FABLE_STATIC_ASSERTs after the class moved 5 lines: pinned as FABLE_STATIC_ASSERT_AT(16, ..) / (17, ..) (their old lines) |

## agentC12 (row 0x005EEFF2 CTCInventoryAbilities::GetMagicListUIComponent) -- coordinator decision: STLport bodies out of the row file
stlport/fable_containers.h = a copy of TODAY's rebuild/include/stlport/fable_containers.h with two NEW opt-ins (each a new first
branch: the existing `#ifdef X` line became `#elif defined(X)`; no default text, no existing opt-in body changed; the header has no
FABLE_STATIC_ASSERT, so nothing to pin).
| header | opt-in | what | evidence |
|---|---|---|---|
| stlport/fable_containers.h | FABLE_RETAIL_STLP_VECTOR_SIZECTOR_VISIBLE_NOINLINE (new, ahead of FABLE_RETAIL_STLP_VECTOR_N_OUTOFLINE) | `explicit vector(size_type n)` with the default body, `__declspec(noinline)` | row calls vector<EHeroAbility>::vector(size_type) 0x005F8EC0 out of line; with the body visible c2 homes the allocator temporary in the live argument slot as retail does (row RELOCATION_MATCH; declaration-only N_OUTOFLINE does not). The COMDAT itself is DIFFER(50v47): retail calls __fastcall uninitialized_fill_n 0x005F8220 |
| stlport/fable_containers.h | FABLE_RETAIL_STLP_LIST_BASE_VISIBLE_NOINLINE (new, ahead of FABLE_RETAIL_STLP_LIST_BASE_OUTOFLINE) | `_List_base(const _Alloc&)` / `~_List_base()` with the default bodies, `__declspec(noinline)` | row calls list<EHeroAbility> COMDATs 0x005F88ED / 0x005F8908 (both RELOCATION_MATCH from this TU) |
