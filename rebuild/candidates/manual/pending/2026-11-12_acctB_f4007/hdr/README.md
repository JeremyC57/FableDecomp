# hdr/ overlay of chunk 4007 (row 0x0061B610 CTCInventoryQuests::ConstructQuestList)

Every file is HEAD's header plus opt-in `#ifdef FABLE_RETAIL_*` blocks; with no define the text is HEAD's, so no landed row changes
and nothing needs a regrade. Copied from the PC twin's overlay (pending/2026-11-11_linux_handH40f/hdr, its README lists the eleven
re-based opt-ins): engine/CTCInventoryQuests.h, CQuestCardDef.h (new there), CBankFile.h, NUISystem_CAction.h, CTCInventoryMap.h,
CQuestManager.h, NUISystem_CUIScreenUnitConverter.h, CTCQuestCard.h, NUISystem_CZoomingComponent.h, NUISystem_CUIStateDef.h,
CTCHeroStats.h, stlport/fable_containers.h (HEAD + H40f's FABLE_RETAIL_STLP_ALLOC_PROXY_REBIND / FABLE_RETAIL_STLP_MEM_FUN blocks).

New in this chunk:
- engine/CTCInventoryQuests.h, inside FABLE_RETAIL_CTCINVENTORYQUESTS_QUESTLIST: `class CTCQuestCard;` and the declaration
  `bool IsQuestCardAvailable(const CTCQuestCard& quest_card) const;` (Ego_r.pdb / FableWin.pdb, retail 0x0061A260). USED by
  differ/0061b610.cpp, which defines the inline body itself.
- engine/NUISystem_CDeletion.h: FABLE_RETAIL_CDELETION_INLINE (the two user-declared constructors with in-class bodies). Probe only
  (d1: byte-neutral); NOT used by the differ source -- drop it at landing unless a later grade needs it.
- stlport/fable_containers.h: FABLE_RETAIL_STLP_LIST_CLEAR_OUTOFCLASS (`_List_base::clear()` declared in the class, defined after
  it, STLport 4.6 _list.c shape). Probe only (rc: no decision moved); NOT used -- drop it at landing unless needed.

Second session (the match) -- USED by src/0061b610.cpp unless said otherwise; each is an opt-in, default text unchanged, no regrade:
- engine/CRGBColour.h (new overlay): FABLE_RETAIL_CRGBCOLOUR_CTOR4_INITIALISE -- CRGBColour(r,g,b,a) forwards to Initialise
  (FableWin ??0CRGBColour@@QAE@EEEE@Z 18 instructions).
- stlport/fable_map.h (new overlay): FABLE_RETAIL_STLP_TREE_INSERT_UNIQUE_HINT_BODY -- the STLport 4.6 hinted
  insert_unique(iterator, const V&) body (text of the landed row 0x0042D2C6), for a TU that must see it.
- engine/CMainGameComponent.h (new overlay): FABLE_RETAIL_CMAINGAMECOMPONENT_SMART_POINTER_ACCESSORS -- PTextBank / PWorld typed
  (CCountedPointer<NGameText::CDataBank>, boost::scoped_ptr<CWorld>; same sizes and offsets) and GetTextBank / GetWorld / PeekWorld /
  IsTextBankInitialised written through them (FableWin GetWorld / GetTextBank 10 instructions = one call more than the word read).
- engine/CThing.h (new overlay): FABLE_RETAIL_CTHING_PEEKTC_ORIGINAL_CHAIN -- one-argument PeekTC(T**) through the const GetTC(id)
  reference and the checked cast, the const twin of the repo's FABLE_RETAIL_CTHING_GETTC_ORIGINAL_CHAIN (FableWin 27i/74b SAME).
- engine/CTCInventoryMap.h: FABLE_RETAIL_CTCINVENTORYMAP_STATGETINTERFACETYPE -- the inline static StatGetInterfaceType
  (FableWin 0x01A526E0; TCI_INVENTORY_MAP).
- engine/CTCQuestCard.h: FABLE_RETAIL_CTCQUESTCARD_STATGETINTERFACETYPE -- the same inline static the _QUESTNAME opt-in declares, for
  a TU that takes the _ACCESSORS block (the two blocks both declare GetQuestName, so they cannot be combined).
- Repo opt-ins the row now also defines (no overlay needed): FABLE_RETAIL_CDEFPOINTER_RELEASE_SELFDELETING_POINTEE,
  FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES, FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY, FABLE_RETAIL_CTHING_ADDANDCOPYTC,
  FABLE_RETAIL_CTHING_GETTC_ORIGINAL_CHAIN.
- Still unused probe blocks from the first session (FABLE_RETAIL_CDELETION_INLINE, FABLE_RETAIL_STLP_LIST_CLEAR_OUTOFCLASS): drop at landing.
