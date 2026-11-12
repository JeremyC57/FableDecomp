## 0x007F0F40

- engine/C3DVector.h: FABLE_RETAIL_C3DVECTOR_GETMAGNITUDEANDNORMALISE declares `float GetMagnitudeAndNormalise();` (0x00A14480, PDB ?GetMagnitudeAndNormalise@C3DVector@@QAEMXZ).
- engine/C3DVector.h: FABLE_RETAIL_C3DVECTOR_ISNONZERO_ZYX_SQUARED (with _ISNONZERO_ZYX) spells IsNonZero as `float squared=Z*Z; squared+=Y*Y; squared+=X*X;` -- retail 0x007F1017..0x007F1031 finishes the Z store/reload before loading Y (the one-line ZYX sum hoists the Y load). Default path unchanged.

## 0x006B08F0

engine/CWorld.h: FABLE_RETAIL_CWORLD_SAVEGAMEMARKERPOS (nested in FABLE_RETAIL_CWORLD) -- inline `const C3DVector& GetSaveGameMarkerPos() const` (FableWin.pdb ?GetSaveGameMarkerPos@CWorld@@QBEABVC3DVector@@XZ, inline in retail, reads the unaligned C3DVector at +0x141 in place). Full copy of the current repo header plus this opt-in only.
(engine/C3DVector.h overlay from attempt 1 no longer needed: the row uses the repo header's FABLE_RETAIL_C3DVECTOR_NONZERO_SQUARED_XYZ; the old overlay copy with probe-only opt-ins is parked in hdr_unused/.)

## 0x00731EA0

engine/CTCSpotLight.h: FABLE_RETAIL_CTCSPOTLIGHT_BASES -- real class with CTCBase + CAGraphicAppearanceComponent bases (Ego_r layout), nested CSpotLightInfo, PDB inline accessors GetColour/GetWidth/GetInnerRadius/GetOuterRadius (if/else bodies, by value), GetDirection decl (row 0x00731EA0).
engine/CSpotLightDef.h: FABLE_RETAIL_CSPOTLIGHTDEF -- real class on CSubDefClassBase (as CLightDef), Colour +0x28 CRGBColour (row 0x00731EA0).

## 0x009341C0

- engine/CAIStateGroup_MinionKidGroupTag.h: FABLE_RETAIL_CAISTATEGROUP_MINIONKIDGROUPTAG -- the class on its CAIStateGroupBase base with Ego_r members and PDB method declarations (row 0x009341C0).
- engine/CIntelligentPointer.h: FABLE_RETAIL_CINTELLIGENTPOINTER_FORCEINLINE_CTOR -- FABLE_FORCEINLINE on the (T*) ctor, both by-value IP argument sites of 0x009341C0 inline in retail.

## 0x00754050

engine/CTCCreatureModeManager.h: FABLE_RETAIL_CMODEBASE_DEFAULTANIMFILTER -- inline CModeBase::GetDefaultAnimFilter() const returning the WeaponBasedFilter at +8 (FableWin.pdb).
engine/CAnimationEntry.h: FABLE_RETAIL_CANIMATIONENTRY_GETROTATION -- declaration of GetRotationInAnimation(float&) const (0x005DBFD0).
stlport/fable_containers.h: FABLE_RETAIL_STLP_CONSTRUCT_BYVAL -- _Construct(T*, U v) takes its value by value (the inline push_back copy reads the argument into registers before storing; retail 0x007541E0).

## 0x00951D60

# Overlay opt-ins (wf13, row 0x00951D60 CAIStateGroup_JackDragon::IntoHover); full copies of the current repo headers, default paths unchanged
- engine/CJackDragonDef.h: FABLE_RETAIL_CJACKDRAGONDEF -- polymorphic CJackDragonDef : CSubDefClassBase (RTTI) with StatGetTypeName / Alloc / Transfer / Copy / GetSizeofClass and the Ego_r.pdb members +0x28..+0x74 (HoverBackOffOffset +0x54), sizeof 0x78.
- engine/CAIStateGroup_JackDragon.h: FABLE_RETAIL_CAISTATEGROUP_JACKDRAGON -- CAIStateGroup_JackDragon : CAIStateGroupBase (RTTI), packed, Ego_r.pdb typed members (PEnemy CIntelligentPointer<CThingCreatureBase>, markers CIntelligentPointer<CThing>, PInvisibleBodgeCreature CIntelligentPointer<CThingAICreature>, CurrentState EDragonState, C3DVector positions), the PDB member functions with their PDB access (StartFlying / IsStillRunnable private AAE), nested EDragonState with DESCRIPTIVE enumerators (names in neither PDB; from the Update 0x00952E80 switch). Needs FABLE_RETAIL_CAISTATEGROUPBASE(+_MEMBERS), FABLE_RETAIL_CINTELLIGENTPOINTER, FABLE_RETAIL_C3DVECTOR.

## 0x009CCEF0

- engine/CBankFile.h: FABLE_RETAIL_CBANKFILE_FILENAMEINDICES -- typed Symbols/RuntimeData/PackedDataType/FilenameIndices, nested CRuntimeData, inline GetEntrySymbol/GetEntryDataType, FindIndicesByFilename + ConvertPathToFilename decls.
- fable_string.h: FABLE_RETAIL_CCHARSTRING_GETASLOWERCASE -- GetAsLowerCase decl (0x0099ED10).
- stlport/fable_map.h: FABLE_RETAIL_STLP_MULTIMAP_EQUAL_RANGE -- multimap const_iterator + const equal_range forwarder.

## 0x00492973 (not landed)

engine/CSubtitleRenderer.h: FABLE_RETAIL_CSUBTITLERENDERER_SETTEXTINFO -- nested CSetTextInfo (ctor 0x0048DDF6 / dtor 0x0043E3D0) + GetTextFromTextTag / ProcessEmbeddedGraphicTags / ProcessEmbeddedHighlightTags / InitPlayerMode (private) + SetButton1OverrideText decls (row 0x00492973).
engine/CMessageEventManager.h: FABLE_RETAIL_CMESSAGEEVENTMANAGER_FINDALL -- const member template FindAllMessages(const TFilter&, list<const CMessageEvent*>&) returning const long, decl only (0x00493EAB).
engine/CMessageEventFilters.h: FABLE_RETAIL_CMESSAGEEVENTFILTERS_ISOFTYPE_CTOR -- the inline CMessageEventFilter_IsOfType(EMessageEventType) ctor alone.
engine/CTCHeroReceiveItems.h: FABLE_RETAIL_CTCHERORECEIVEITEMS_DISPLAYINGITEMS (inside _PICKEDUPITEMS) -- PCurrentItem +0x1C, PersistCurrentItem +0x24, ReceivingItems +0x25, DisplayingItems +0x26, ForceClear +0x27.
stlport/fable_deque.h: FABLE_RETAIL_STLP_DEQUE_PUSH_BACK_OUTOFLINE -- deque::push_back declared only (0x004941C0); FABLE_RETAIL_STLP_QUEUE -- STLport queue with inline push().

## 0x00881A40 (not landed)

engine/NScreenEffect_CScreenEffectPriorityStack.h: FABLE_RETAIL_CSCREENEFFECTPRIORITYSTACK_SCREENFILTERENTRY (FULL branch) -- defines CThingGraphicArgPasser (pack 1), CScreenFilterEntry {param +0x10, reset map +0x30} and its CThingGraphicByPassFlagState (ctor thing(0), inline Reset) (row 0x00881A40).
engine/NGraphicAppearance_CTCGraphicAppearance.h: FABLE_RETAIL_CTCGRAPHICAPPEARANCE_BYPASSCOLORFILTER (MOVEMENT branch) -- inline GetByPassColorFilter() const, SetByPassColorFilter(bool) decl 0x004BFB00 (row 0x00881A40).
stlport/fable_map.h: FABLE_RETAIL_STLP_TREE_FIND_MEMBER_TEMPLATE_NOINLINE -- tree member-template find with body visible but __declspec(noinline) (0x00883860); also _DECL (declaration only; not used by the match) (row 0x00881A40).

