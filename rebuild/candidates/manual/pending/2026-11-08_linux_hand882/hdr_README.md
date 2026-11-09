# Chunk 882 header overlay (row 0x0061E6D0 CTCInventoryQuests::ConstructQuestListPC)

Every change to an existing header is an additive `#ifdef FABLE_RETAIL_*` block; the default path is unchanged
(`diff rebuild/include/<f> hdr/<f>` shows only the opt-in). Two headers are new.

## New headers
- `engine/CTCInventoryQuests.h` -- CTCInventoryQuests : NInventory::CTCInventoryBase (+0) + NHeroInformationScreens::CBase (+0x130)
  (RTTI, same bases as CTCInventoryMap). Ego_r.pdb layout minus 0x18 (retail CTCInventoryBase 0x130) and 4 per STLport container:
  AvailableQuests +0x16C (map<CThing*,CComponent*>, row `lea ecx,[ebx+16Ch]` insert_unique 0x0061F122), DisplayQuests +0x178
  (vector<CIntelligentPointer<CThing>>, `mov edi,[ebx+17Ch]` 0x0061F15F), PreviousScreenType +0x168 / InventoryScreen +0x14D
  (landed CloseInventoryPC 0x00621720), sizeof 0x188. Global enum QuestScreenType (Ego_r `W4QuestScreenType@@`); enumerator
  names are in neither PDB, so QUEST_SCREEN_TYPE_AVAILABLE / _CURRENT / _OTHER (0/1/2) are descriptive placeholders.
- `engine/CQuestCardDef.h` -- CQuestCardDef : CSubDefClassBase, members from Ego_r.pdb minus 4 after RewardObjects (STLport vector):
  RegionName +0x38, IsCore +0x58, IsVignette +0x59, IsExclusive +0x5A, Prerequisites +0x5C (map<EQuestCardPrerequisite,long>),
  NumBoasts +0x68, sizeof 0x74 (row reads +0x59 / +0x5C / +0x60 / +0x68 / +0x38). Requires FABLE_RETAIL_DEF_CLASS_BASE.
  EQuestCardPrerequisite (Ego_r global enum); QUEST_CARD_PREREQUISITE_RENOWN = 0 is a descriptive placeholder.

## Opt-ins added to existing headers
- `engine/CBankFile.h` FABLE_RETAIL_CBANKFILE_FINDINDEX: `FindIndexBySymbol(const CCharString&) const` 0x009CCDC0 (Ego_r 0x00B1A780 /
  FableWin 0x02F97650).
- `engine/CGraphicDataBank.h` FABLE_RETAIL_CGRAPHICDATABANK_BANKFILE: CGraphicDataBank derives CBankFile at +0 (RTTI
  CGraphicDataBank : CBankFileAsync : CBankFile) so the sprite bank answers FindIndexBySymbol; sizeof 0x30C kept.
- `engine/CMainGameComponent.h` FABLE_RETAIL_CMAINGAMECOMPONENT_GETWORLD (inside the WORLD block): inline `CWorld& GetWorld()`
  (FableWin.pdb ?GetWorld@CMainGameComponent@@...).
- `engine/CQuestManager.h` FABLE_RETAIL_CQUESTMANAGER_REGIONSTARTED: `bool IsActiveQuestStartedInRegion(const CCharString&) const`
  0x004B1270.
- `engine/CTCHeroStats.h` FABLE_RETAIL_CTCHEROSTATS_GETRENOWNLEVEL (inside the SERIALISE layout): inline `long GetRenownLevel() const`
  (FableWin.pdb ?GetRenownLevel@CTCHeroStats@@QBEJXZ 0x01E6E164; not in Ego_r's out-of-line set; row reads +0x70 in place).
- `engine/CTCInventoryMap.h` FABLE_RETAIL_CTCINVENTORYMAP_QUESTMARKERS: CreateMiniMapMarkers 0x00603000, CreateMapMarkers 0x005FF190,
  GetRegionsToDisplay 0x005FD410 (Ego_r + FableWin names).
- `engine/CTCQuestCard.h` FABLE_RETAIL_CTCQUESTCARD_ACCESSORS: IsCore 0x00702280, IsOptional 0x007022C0, GetQuestName 0x00702310,
  GetQuestSummary 0x00702400, GetQuestObjective 0x00702470, GetRegionName 0x007025A0 (out of line), inline GetPDef() (FableWin
  0x01C543E0) and the FableWin-only inline accessors GetScriptQuestName / GetGoldReward / GetRenownReward. Includes engine/CQuestCardDef.h.
- `engine/NInventory_CTCInventoryBase.h`: forward `class CRegion;` plus FABLE_RETAIL_CTCINVENTORYBASE_MINIMAPGRAPHIC
  `unsigned long GetMiniMapGraphic(CRegion&) const` 0x005BEB57.
- `engine/NUISystem_CAction.h` FABLE_RETAIL_CACTION_INLINE_CTOR: inline `CAction() : ActionType(0), Valid(true) {}` (retail expands
  the ctor at most sites and calls the COMDAT 0x0042BE50 once its inline budget is spent).
- `engine/NUISystem_CUIScreenUnitConverter.h` FABLE_RETAIL_CUISCREENUNITCONVERTER_VECTOR_INLINE: ConvertVector as an inline body
  (the landed 0x0052E580 body; expanded at the row's first site 0x0061EA22, called at the second 0x00620F0A).
- `engine/NUISystem_CUIStateDef.h` FABLE_RETAIL_CUISTATEDEF_IMPLICIT_DTOR: no user virtual dtor (retail's dtor is implicit and
  inlined), with the Transfer / Copy / GetSizeofClass overrides declared instead.
- `engine/NUISystem_CZoomingComponent.h` FABLE_RETAIL_CZOOMINGCOMPONENT_ZOOM: vtable slots 142 Pan, 143 SetMinZoom (+0x23C),
  144 SetMaxZoom (+0x240), 145 SetZoomCenter (+0x244), 146 GetZoomCenter (FableWin vtable order).

Diagnostic-only overlays tried and removed (not needed by differ/0061e6d0.cpp): CDefPointer plain inline, CThing GetTC plain inline,
an extra STLport allocator level -- none moved the inline budget the right way.

## Session 2026-11-09
- `stlport/fable_containers.h` (new overlay copy of the current repo file, three additive blocks): FABLE_RETAIL_STLP_ALLOC_PROXY_REBIND
  (STLport 4.6 `__stl_alloc_rebind` and `_STLP_alloc_proxy::allocate` / `deallocate` through it) and
  FABLE_RETAIL_STLP_LIST_CREATE_CONSTRUCT (`_M_create_node` builds the value with `_Construct`). Same text as chunk 883's opt-ins
  (row 0x006096E0, where they put the scroll-action group 4 at retail's inline budget). Same code; one more inline layer each, which
  adds ~360 to this row's caller size g by the take block. differ/0061e6d0.cpp defines both. Default path unchanged
  (`diff rebuild/include/stlport/fable_containers.h hdr/stlport/fable_containers.h` shows only the three #ifdef blocks).

## Session 2 (2026-11-09): tried and removed
Measured as budget consumers on probe b1 and removed again (differ/0061e6d0.cpp uses none of them; hdr/ is back to the session-1
state): FABLE_RETAIL_CRGBCOLOUR_CTOR4_INITIALISE (engine/CRGBColour.h, FableWin ??0CRGBColour@@QAE@EEEE@Z through
?Initialise@CRGBColour@@QAEXEEEE@Z: identical code, +90 g), FABLE_RETAIL_STLP_ALLOC_CREATE (stlport/fable_containers.h, STLport 4.6
__stl_alloc_create in the _List_base ctor: identical code) and an empty release CCountedPointer::Validate() called by operator-> /
operator* (probe only, unproven body): all three together +170 g of the 724 needed.
