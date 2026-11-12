## 0x00868CE0

# Header overlay for row 0x00868CE0
- engine/CHitParameters.h: FABLE_RETAIL_CHITPARAMETERS_AUGMENTATIONS -- Augmentations as the real std::vector<EObjectAugmentationType> (+0x3C, same 12 bytes).
- engine/CHitParameters.h: FABLE_RETAIL_CHITPARAMETERS_SETWEAPON -- declares void SetWeapon(CThing& weapon) (0x00868CE0, Ego_r ?SetWeapon@CHitParameters@@QAEXAAVCThing@@@Z).
- engine/CTCObjectAugmentations.h: FABLE_RETAIL_CTCOBJECTAUGMENTATIONS_GETAUGMENTATIONS -- declares GetAugmentations(std::vector<long>&) const (0x00766920) in the default branch.

## 0x00E2EAC0

No header opt-in was added for 0x00E2EAC0 (pass 2 confirmed): the row uses the current repo headers as they are (engine/CV_AmbushScamScript.h, CScriptThing.h, CGameScriptInterfaceBase.h, fable_script_distance.h). A probe copy of engine/CCountedPointer.h used during pass 2 was removed again.

## 0x005590C0

# Header overlay of row 0x005590C0 (full copies of the current repo headers plus one opt-in each; default paths unchanged)
- engine/NUISystem_CScrollBarOutside.h: `FABLE_RETAIL_CSCROLLBAROUTSIDE_ONLEFTCLICKED` declares the override `virtual void OnLeftClicked();` (Ego_r.pdb / FableWin.pdb ?OnLeftClicked@CScrollBarOutside@NUISystem@@UAEXXZ, vtable 0x0124B54C slot 146). No layout or vtable change.
- engine/NUISystem_CManager.h: `FABLE_RETAIL_CMANAGER_GETMOUSECURSOR` adds the inline `CComponent* GetMouseCursor() { return PMouseCursor; }` (FableWin.pdb ?GetMouseCursor@CManager@NUISystem@@QAEPAVCComponent@2@XZ; inline in retail, `mov ecx,[eax+0B8h]` at 0x00559113; landed 0x0055A5D0 / 0x0055A5B0 spell it the same in local models). No layout change.

## 0x00541FD0

No header overlay needed for 0x00541FD0 (existing repo opt-ins only).

## 0x008C2AD0

No header overlay: row 0x008C2AD0 matches on the current repo headers (no new opt-in).

## 0x005F11A9

# Header overlay for row 0x005F11A9

No header was copied or changed for this row. The source uses the repo headers as they are, with opt-ins that already exist
(the same set as the landed sibling 0x005F1320): FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA, FABLE_RETAIL_CTCINVENTORYABILITIES_MAGIC,
FABLE_RETAIL_CTCINVENTORYABILITIES_ASSIGN, FABLE_RETAIL_CDELETION, FABLE_RETAIL_STLP_TREE_ERASE, FABLE_RETAIL_CTCBASE,
FABLE_RETAIL_CTCINVENTORYBASE, FABLE_RETAIL_CTCINVENTORYABILITIES, FABLE_RETAIL_CCHARSTRING.

## 0x006978D0

# Header overlay for row 0x006978D0 (full copies of the current repo headers plus declaration-only opt-ins)
- engine/CCamera.h: FABLE_RETAIL_CCAMERA_FOV -- declares CCamera::GetFOV() const (0x00A0BEB0) and SetFOV(float,float) (0x00A0BED0); declarations only.
- engine/CGameCameraManager.h: FABLE_RETAIL_CGAMECAMERAMANAGER_INTERPOLATEDCAMERA -- private GetInterpolatedCamera declaration in the FABLE_RETAIL_CGAMECAMERAMANAGER_LOOKAROUND (full class) branch; declaration only.

## 0x00EB3F40

No header overlay: this row uses the repo headers unchanged (no new FABLE_RETAIL_* opt-in).

## 0x008D5300

No header overlay needed for 0x008D5300: the row matches on the current repo headers (existing opt-ins only).

## 0x009ADC90 (not landed)

# Header overlay for row 0x009ADC90 (CDefinitionManager::InstantiateDef) -- full copies of the current repo headers plus opt-ins only
- engine/CDefinitionManager.h: `FABLE_RETAIL_CDEFINITIONMANAGER_INSTANTIATEDEF` (inside FABLE_RETAIL_CDEFINITIONMANAGER) -- nested CDefClassInfo / CInstantiatedDefInfo (Ego_r TPI), typed PSymbolMap / PDefinitionSymbolMap (CCountedPointer<CSymbolMap>) and InstantiatedDefs (CArray<CInstantiatedDefInfo*>) at the same offsets, declarations of GetPDefClassInfo 0x009AD580 and InstantiateDef 0x009ADC90. Needs FABLE_RETAIL_CCHARSTRING, FABLE_RETAIL_CDEFSTRINGTABLE, FABLE_RETAIL_CSYMBOLMAP, FABLE_RETAIL_CDEFPOINTER_POINTER_CTOR.
- engine/CDefString.h: `FABLE_RETAIL_CDEFSTRING_CTOR_SETSTRING` -- inline `CDefString(const CCharString&) { SetString(text); }` and the declaration of SetString 0x009D5060 (default: the body-less ctor declaration, unchanged).
- engine/CSymbolMap.h: `FABLE_RETAIL_CSYMBOLMAP_ADDSYMBOLDIRTY` (inside FABLE_RETAIL_CSYMBOLMAP) -- declarations of the two AddSymbolDirty overloads 0x009E5420 / 0x009E54E0.
- engine/CDefPointer.h: `FABLE_RETAIL_CDEFPOINTER_POINTER_CTOR` -- the FableWin.pdb `CDefPointer(T*)` constructor, inline: stores the pointer and AddRefs it with no null test (the expansion at 0x009ADD49).


## 0x009ADC90 (landing ej, after repair 2)

# Header overlay for row 0x009ADC90 (CDefinitionManager::InstantiateDef) -- full copies of the current repo headers plus opt-ins only
- engine/CDefinitionManager.h: `FABLE_RETAIL_CDEFINITIONMANAGER_INSTANTIATEDEF` (inside FABLE_RETAIL_CDEFINITIONMANAGER) -- nested CDefClassInfo / CInstantiatedDefInfo (Ego_r TPI), typed PSymbolMap / PDefinitionSymbolMap (CCountedPointer<CSymbolMap>) and InstantiatedDefs (CArray<CInstantiatedDefInfo*>) at the same offsets, declarations of GetPDefClassInfo 0x009AD580 and InstantiateDef 0x009ADC90. Needs FABLE_RETAIL_CCHARSTRING, FABLE_RETAIL_CDEFSTRINGTABLE, FABLE_RETAIL_CSYMBOLMAP, FABLE_RETAIL_CDEFPOINTER_POINTER_CTOR. Repair 2: the record constructor has the FableWin.pdb signature (`const CDefPointer<CDefClassBase>&` 4th parameter, 0x03061650) and the default constructor (0x0305D840) is declared.
- engine/CDefString.h: `FABLE_RETAIL_CDEFSTRING_CTOR_SETSTRING` -- inline `CDefString(const CCharString&) { SetString(text); }` and the declaration of SetString 0x009D5060 (default: the body-less ctor declaration, unchanged).
- engine/CSymbolMap.h: `FABLE_RETAIL_CSYMBOLMAP_ADDSYMBOLDIRTY` (inside FABLE_RETAIL_CSYMBOLMAP) -- declarations of the two AddSymbolDirty overloads 0x009E5420 / 0x009E54E0.
- engine/CDefPointer.h: `FABLE_RETAIL_CDEFPOINTER_POINTER_CTOR` -- the FableWin.pdb `CDefPointer(T*)` constructor (0x030644A0 for CDefClassBase), inline: stores the pointer WITHOUT counting it (adopts the creation reference; no `inc` after `mov esi,eax` at 0x009ADCD6). Repair 2 changed the body from store + AddRef.

