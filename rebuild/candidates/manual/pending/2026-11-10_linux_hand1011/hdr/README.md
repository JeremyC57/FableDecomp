# chunk 1011 header overlay (FABLE_HDR_OVERLAY=$A/hdr)

Copies of rebuild/include headers (HEAD) plus new headers. Every change sits behind a new `#ifdef FABLE_RETAIL_*` opt-in; the
default path of each modified header is byte-identical to the repo copy (the only edit outside an opt-in block is the
`#if defined(...)` list that defines FABLE_STLP_NOINLINE in stlport/fable_containers.h, extended by one macro).

## Modified headers (opt-in -> what it adds -> rows)
- `stlport/fable_containers.h`: `FABLE_RETAIL_STLP_LIST_CLEAR_NOINLINE` -- `_List_base::clear()` keeps the STLport 4.6 body but is
  `__declspec(noinline)` (retail calls the instance out of line, 0x004B8050). Unlike the declaration-only `_CLEAR_OUTOFLINE`, c2 sees the
  body and knows the call leaves the head word alone. Rows 0x005FCBE0, 0x005FDDD0.
- `engine/CTCInventoryMap.h`:
  - `FABLE_RETAIL_CTCINVENTORYMAP_TELEPORT` -- private GetRegionTeleporter(const CRegion&) const 0x005FBEA0, private TeleportToActiveRegion()
    0x005FBFA0, public TeleportHeroToBoastingArea() const 0x005FB4B0. Rows 0x005FBEA0 / 0x005FBFA0 / 0x005FB4B0.
  - `FABLE_RETAIL_CTCINVENTORYMAP_FRAMEUPDATE` -- virtual FrameUpdate() (slot 9, 0x006061B0), protected UpdateVisitedRegions(long,long)
    0x00605CE0, private UpdateRegionsCorrespondances(long,long) 0x005FC0C0. Row 0x006061B0.
  - `FABLE_RETAIL_CTCINVENTORYMAP_PASTQUESTS` -- private ConstructPastQuestsList(CComponent*) 0x005FCBE0 and
    ConstructPastQuestsListPC(CComponent*, CComponent*) 0x005FDDD0. Rows 0x005FCBE0 / 0x005FDDD0.
- `engine/CTCHeroStats.h`: `FABLE_RETAIL_CTCHEROSTATS_ADDRENOWN` -- AddRenown(long) 0x0057D8BE. Row 0x005EB590 (parked).
- `engine/CThingSearchTools.h`: `FABLE_RETAIL_CTHINGSEARCHTOOLS_FIRSTTHINGBODY` -- same-TU `__declspec(noinline)` GetPFirstThing<Filter,T>
  body over PeekClassList() (declared protected when ALLTHINGSBODY is off). Rows 0x005FBEA0 / 0x005FBFA0.
- `engine/CTCTeleporter.h`: `FABLE_RETAIL_CTCTELEPORTER_TELEPORTHERO` -- TeleportHero() 0x007DFCF0. Row 0x005FBFA0.
- `engine/CWorld.h`: `FABLE_RETAIL_CWORLD_MOVEHEROTOREGION` -- HandleMoveHeroToRegionGameEvent(const CGameEvent&) 0x0049EAF0. Row 0x005FB4B0.
- `engine/CGameEvent.h`: `FABLE_RETAIL_CGAMEEVENT_INLINE` -- inline ctor (type, Player -1, EndPos 0, Valid true, Replacement false),
  AddData(long, const void*) 0x009F1760, inline AddFloat / AddUBYTE / SetPlayer; `#error` with `_CTOR`. Row 0x005FB4B0.
- `engine/CQuestManager.h`: `FABLE_RETAIL_CQUESTMANAGER_FAILQUEST` -- SetQuestAsFailed(...) 0x004B2280, DeactivateQuest(...) 0x004B43D0
  (declarations only). Row 0x006061B0.
- `engine/CTCQuestCard.h`: `FABLE_RETAIL_CTCQUESTCARD_SCRIPTQUESTNAME` -- inline GetScriptQuestName() const. Row 0x006061B0.
- `engine/NGameText_CDataBank.h`: `FABLE_RETAIL_NGAMETEXT_GROUPENTRY` -- NGameText::CGroupEntry (CIVCountedPointeeBase, CArray<long> Entries
  +0x1C) and GetPGroupEntry(unsigned long, bool) const 0x009C8890. Row 0x005EB180.
- `engine/NInventory_CTCInventoryBase.h`: `FABLE_RETAIL_CTCINVENTORYBASE_MINIMAPGRAPHIC` -- GetMiniMapGraphic(CRegion&) const 0x005BEB57. Row
  0x005FBAA0 (parked).
- `engine/CMap.h`: `FABLE_RETAIL_CMAP_WORLDACCESSORS` -- GetWorldSizeX/Y(), GetWorldPosition(). Row 0x005FBAA0 (parked).
- `engine/C2DCoordI.h`: `FABLE_RETAIL_C2DCOORDI_COPY` -- the FableWin user copy constructor. Row 0x005FBAA0 (parked).
- `fable_game.h`: `FABLE_RETAIL_CMAINGAMECOMPONENT_CUTSCENEFONT` (PeekCutSceneMainFont), `FABLE_RETAIL_CMAINGAMECOMPONENT_ENGINEGRAPHICS`
  (splits _pad_0x16154: CCountedPointer<CGraphicDataBank> PMainGraphicBank +0x161C8, public PeekEngineGraphics()). CUTSCENEFONT: row
  0x005EB180; ENGINEGRAPHICS: row 0x005FBAA0 (parked).

## New headers (declarations only unless stated)
- `engine/CFontManager.h` -- CFontManager::GetFont(const CCharString&) 0x009E2C80, `extern CFontManager* PFontManager` 0x013B838C. Row 0x005EA650.
- `engine/CTCInventoryQuests.h` -- CTCInventoryQuests : NInventory::CTCInventoryBase, NHeroInformationScreens::CBase (RTTI), StatGetInterfaceType
  (TCI 25), ConstructQuestList 0x0061B610, ConstructQuestListPC 0x0061E6D0, `enum QuestScreenType { }` (no enumerator names proven). No
  members past the bases (header debt; Ego_r sizeof 0x1B0). Rows 0x005FCBE0 / 0x005FDDD0.
- `engine/CTCHeroReceiveItems.h` -- CTCHeroReceiveItems : CTCBase, StatGetInterfaceType (TCI 203), GiveItem(CThingObject&, bool) 0x007CAB30.
  No members (header debt). Row 0x005EB590 (parked).
- `engine/CThingFilter_OverlappingCircle.h` -- Ego_r layout (SpherePos +0, Radius +0xC), inline ctor, operator() 0x0063EF50. Row 0x005EB590.
- `engine/CThingFilter_AICreatureHasProperty.h` -- unsigned long Property, inline ctor, operator() 0x006A75D0. Row 0x005FB4B0.
