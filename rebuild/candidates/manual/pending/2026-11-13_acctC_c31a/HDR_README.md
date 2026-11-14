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
| engine/CTCInventoryMap.h | FABLE_RETAIL_CTCINVENTORYMAP_MAPLIST (new, agentC14; 6 lines after the PCMAPLIST block; no assert moved: all already FABLE_STATIC_ASSERT_AT) | declares `NUISystem::CComponent* ConstructMapList(NUISystem::CComponent* parent, bool check_only, bool* has_regions)` 0x0060C900 | Ego_r 0x0065F950 + FableWin 0x0208A2A0 mangling, row 0x0060C900 RELOCATION_MATCH |
## agentC14 additions for row 0x00603000 CreateMiniMapMarkers (DIFFER; first found by chunk 729 / pending 2026-11-03_linux_hand729, re-applied to TODAY's headers as new opt-ins; asserts pinned with pin_asserts.py: CPlayerGui.h 131, CThingCreatureBase.h 39, CMiniMapDisplay.h 18)
| header | opt-in | what | evidence |
|---|---|---|---|
| engine/CThingFilter_HasInformation.h | NEW header | empty filter, `bool operator()(const CThingAICreature&) const` 0x006A7140 | Ego_r 0x005A2B70 + FableWin 0x01F937E0 ??RCThingFilter_HasInformation@@QBE_NABVCThingAICreature@@@Z (reloc_bindings' CThing spelling is a landed local model: NAME_FIXES) |
| engine/CDefString.h | FABLE_RETAIL_CDEFSTRING_INLINE_EQ (new) | inline `operator==(const char*)` with the text of the landed out-of-line 0x005B3440 | FableWin ??8CDefString@@QBE_NPBD@Z; 0x006045F9 GetStringAsCharArray + `repe cmpsb` against the literal |
| engine/CPlayerGui.h | FABLE_RETAIL_CPLAYERGUI_GETMINIMAP (new) | `CMiniMapDisplay& GetMiniMap()` 0x00437CE0, declaration | Ego_r / FableWin ?GetMiniMap@CPlayerGui@@QAEAAVCMiniMapDisplay@@XZ |
| engine/CThingCreatureBase.h | FABLE_RETAIL_CTHINGCREATUREBASE_ISDEMONDOOR (new) | inline `bool IsDemonDoor() const { return DemonDoor; }` (out-of-line copy 0x005FA7F0 for mem_fun_ref) | Ego_r 0x0053C6A0 / FableWin 0x01D9FA60 ?IsDemonDoor@CThingCreatureBase@@QBE_NXZ |
| engine/CMiniMapDisplay.h | FABLE_RETAIL_CMINIMAPDISPLAY_GETNONAUTOMARKERS (new) | `void GetNonAutoMarkers(std::list<const CMarkerInfo*>&) const` 0x0082A390, declaration | Ego_r 0x00732D70 mangling, landed row |
equiv_check.py over all landed rows (sources + tests) with the WHOLE agentC14 overlay (merged tree tmp/tree/include, ../src linked): 0 differing rows after pinning the inherited CExpressionSubDef.h asserts (first pass: 004DE881 + 1,317 rows that only failed to resolve ../src in the copy; rechecked).
| engine/CExpressionSubDef.h | (agentC14 fix) the agentC11 STATGETTYPENAME block had moved 3 FABLE_STATIC_ASSERTs unpinned (equiv_check: landed 0x004DE881 preprocessed differently); pinned with pin_asserts.py (FABLE_STATIC_ASSERT_AT 44/45/46), opt-in body unchanged | - | equiv_check |
(agentC15) No header change. Row 0x00603000 now also uses the EXISTING repo opt-in FABLE_RETAIL_STLP_FIND_NOINLINE_BODY (with
FABLE_RETAIL_STLP_FIND_RANDOM_DISPATCH) and pastes the landed same-TU body of GetRelativePosOnMiniMap (0x005FBAA0) into the row file.
DIAG-only overlays live in tmp/hdr_* / tmp/hdrR* (Reset text experiments): never hand them back.

## agentC21 addition (FITTED inline-budget text; used by NO handed-back row; the coordinator may drop it)
| header | opt-in | what | evidence |
|---|---|---|---|
| engine/CCountedPointer.h | FABLE_RETAIL_CCOUNTEDPOINTER_RESET_ELSE_KEEPS_INFO (new `#elif` after RESET_CALLS_DTOR; no assert moved: the only assert is line 59, above) | Reset = the default text plus `else Info = Info;` | Emits the default bytes wherever retail can be compared: /O1 own body = 0x00429C6F (59 bytes), expansions on a local = the default expansion (tools/vt3.py, text u5), ReleaseData untouched. Its own /O2 COMDAT is 114 bytes (default 103), and c2 prices an inline callee by the BYTES of its own COMDAT (agentC21 NOTES: sz 53/103/116/89 = objdump sizes), so the price is 106 instead of 95. Retail's price for Reset<CComponent> in the CTCInventoryMap TU is 108 or 109 (rows 0x00603000, 0x005FF190, 0x00605570); no text with all constraints reaches 116/117 bytes (~560 texts measured), so the rows would still need `/d2inlT72`. Siblings 0x0060C900 / 0x0060EEB0 and 0x00605570 (with its /d2inlT107) regraded RELOCATION_MATCH after the header change. NOT the original text. |
(agentC21) tmp/hdrD is a DIAG-only overlay (FABLE_DIAG_RESET_TEXT): never hand it back.

## agentC23 additions (declarations only; no default text changed; equiv_check NOT run)
| header | opt-in | what | evidence |
|---|---|---|---|
| engine/CTCInventoryMap.h | FABLE_RETAIL_CTCINVENTORYMAP_QUESTSSCREENPC (new, after the QUESTSSCREEN block) | `NUISystem::CComponent* ConstructQuestsScreenPC(NUISystem::CComponent* screen);` public, 0x00601310 | Ego_r.pdb 0x00655BC0 + FableWin.pdb 0x020ABAB0 mangling; reloc_bindings.tsv binds it at 0x00601310 (caller 0x0041E6D3). No assert moved (all already _AT) |
| engine/CTCInventory.h (copied from today's repo) | FABLE_RETAIL_CTCINVENTORY_OPENINVENTORY (new, first block of the class body) | `virtual void OpenInventory();` 0x0058578B | Ego_r 0x005F4D20 + FableWin 0x0212841C ?OpenInventory@CTCInventory@@UAEXXZ; vtable 0x01250DE4 slot 30 = base OpenInventory slot. 38 asserts pinned (pin_asserts.py) |
| engine/CThingCreatureBase.h | FABLE_RETAIL_CTHINGCREATUREBASE_ISCHILD (new, first block of the class body) | `bool IsChild() const;` public, 0x00661F70 | Ego_r / FableWin ?IsChild@CThingCreatureBase@@QBE_NXZ (the default branch declares it). pin_asserts: 0 moved |
| engine/CInputManager.h (copied from today's repo) | FABLE_RETAIL_CINPUTMANAGER_JOYSTICKS (guard-line extension of the three `#ifdef FABLE_RETAIL_CTHINGGAMEOBJECT_ONHIT` lines to `|| defined(...)`, as in the acctB stale overlay) | joystick accessors for 0x0058578B | acctB stale overlay text; no line moved |
(agentC23) hdrx/ is a DIAG-only overlay (tools/mkx.py: dead-arm FABLE_RETAIL_ASSERT in CCountedPointer accessors): never hand it back.

## agentC26: NO header change (hdr/ = the agentC23 overlay, byte for byte)
(agentC26) hdrx/ is a DIAG-only overlay (tools/mk2.py: live / dead assert forms in fable_assert.h and assert bodies in engine/CCountedPointer.h for the FableWin size
checks; differ/CCountedPointer_fablewin_asserts.spec.py is the FableWin-sized member set, differ/CCountedPointer_diag_get.h + differ/fable_assert_diag.h one sample): never hand it back.
No `FABLE_RETAIL_CCOUNTEDPOINTER_ASSERTS` opt-in was added: the FableWin-backed set does not reproduce the rows (NOTES.md). The fitted RESET_ELSE_NULL / RESET_ELSE_KEEPS_INFO
opt-ins are still used only by the parked map-row differs; NOTES.md shows they are not needed to explain retail (frequency lead).
- engine/NUISystem_CUIScreenUnitConverter.h (NEW in this overlay, copy of the current repo header + one addition; no FABLE_STATIC_ASSERT in the file, no default path changed, the existing FABLE_RETAIL_CUISCREENUNITCONVERTER_VECTOR_INLINE body is kept byte for byte under a new `#else`): `FABLE_RETAIL_CUISCREENUNITCONVERTER_VECTOR_INLINE_CONVERTXY` (agentC28, row 0x0061E6D0) -- the inline ConvertVector written through the class's per-axis converters ConvertX / ConvertY (Ego_r.pdb / FableWin.pdb ?ConvertX@CUIScreenUnitConverter@NUISystem@@SIMM@Z, ?ConvertY@..@@SIMM@Z). Evidence: FableWin ConvertVector 28i/81b v this text 27i/77b v the direct-global text 29i/95b (plain /Od); the out-of-line copy 0x0052E580 is RELOCATION_MATCH from this text (probe/fw/cvo.cpp); row 0x0061E6D0 is RELOCATION_MATCH with it and differs only in the SIB operand order at 0x0061FB58 / 0x0061FB5E without it. ConvertX / ConvertY bodies are Inferred (FableWin 15i/45b each; this text 13i/42b). Needs FABLE_RETAIL_CUISCREENUNITCONVERTER_INLINE + _VECTOR + _VECTOR_INLINE.

## agentC31 additions (row 0x00601310 CTCInventoryMap::ConstructQuestsScreenPC, RELOCATION_MATCH)
- stlport/fable_containers.h (no FABLE_STATIC_ASSERT in the file; default paths untouched; no existing opt-in body edited):
  - FABLE_RETAIL_STLP_MALLOC_ALLOC_ALLOCATE (new): `allocator<T>::allocate` forwards to the malloc-based `__sgi_alloc` layer
    (`n != 0 ? (T*)__malloc_alloc_allocate<0>::allocate(n * sizeof(T)) : 0`, callee `void* __result = malloc(__n); return __result;`),
    the mirror of the existing FABLE_RETAIL_STLP_MALLOC_ALLOC_DEALLOCATE. Evidence: STLport 4.6 stl/_alloc.h text (retail has no
    `_S_oom_malloc` test, so that line is left out); same bytes, measured +14 caller tuples per allocate site (48 sites in the row);
    the class template `__malloc_alloc_allocate` is a DESCRIPTIVE name (STLport's is `__malloc_alloc<0>::allocate`; a separate
    template so the existing DEALLOCATE block stays untouched). INFERRED text.
  - FABLE_RETAIL_STLP_LIST_BASE_CONVERT_ALLOCATOR (new): `_List_base(const _Alloc& a) : _M_node(a, (_List_node<_Tp>*)0)`; evidence:
    STLport 4.6 _list.h `_M_node(_STLP_CONVERT_ALLOCATOR(__a, _Node), (_Node*)0)` with `_STLP_CONVERT_ALLOCATOR(__a, _Tp) == __a` for a
    compiler with member templates (VC7.1); same bytes, measured -5 caller tuples per list constructor (32 in the row).
  - Both are per-row opt-ins like REBIND / CONSTRUCT_HELPER; the matched sibling 0x0061E6D0 (which does not define them) regrades
    RELOCATION_MATCH on this overlay.
