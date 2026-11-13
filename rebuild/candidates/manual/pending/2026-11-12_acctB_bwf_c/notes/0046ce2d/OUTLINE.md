# OUTLINE 0x0046CE2D CPlayerGuiDef::operator=(const CPlayerGuiDef&) -- implicit (compiler-generated) copy assignment

## Frame facts
- thiscall, `ret 4`, returns this (eax = ebx). No frame pointer, no SEH/EH states, no /GS cookie, no locals, frame size 0.
- Saved callee order: push ebx ; push ebp ; (mov ebp,[esp+0Ch] = &other) ; push esi ; push edi. Pops: edi, esi, ebp, ebx.
- Registers: ebx = this, ebp = &other, eax = scratch for every dword copy, esi/edi only for the C3DVector movsd.
- Single straight-line block, no branches, no jump tables, no cold code. Last instruction 0x0046EABA `ret 4`.
- Pragma: `#pragma optimize("s", on)` (def TU; siblings 0x0046F43E / 0x00465EB3). No `optimize("y",off)`.

## Source form (recommended)
Do not write operator= by hand: declare the class with the members of $W/members.tsv in offset order (real types), derived from
CParentDefClassBase (FABLE_RETAIL_DEF_CLASS_BASE), and define only
    void CPlayerGuiDef::Copy(const CDefClassBase* def) { *this = *static_cast<const CPlayerGuiDef*>(def); }
so VC7.1 emits the implicit ??4CPlayerGuiDef@@QAEAAV0@ABV0@@Z (that is this row; Copy itself is 0x0046CE28, a separate landed row).
Container op= must be declared out of line (FABLE_RETAIL_STLP_VECTOR_ASSIGN or _VECTOR_C); map gets _Rb_tree::operator= (out of
line, fable_map.h:112) through its implicit op=. The memberwise result is exactly the list below. If the implicit route
misbehaves, the explicit memberwise text (CParentDefClassBase::operator=(other); then each member in this order; return *this)
is the fallback, as CGlobalDef 0x00465EB3 does.

## Block list (retail order; address = first instruction of the copy run, or the `call` instruction)
0x0046CE2D  prologue; this->CParentDefClassBase::operator=(other)  -> call 0x00431ED0 at 0x0046CE38 (push ebp = &other first)
0x0046CE40  // 6 dword copies +0x3c..+0x53 : InventoryNoMovementSound .. BacklightColour = other.<same>;  (scalars / C2DVector / CRGBColour, members.tsv order)
0x0046CE61  DiffuseVector = other.DiffuseVector;   // C3DVector, lea esi/edi +0x54 ; movsd x3
0x0046CE6D  // 60 dword copies +0x60..+0x14f : GuildSealPos_X .. StealthEyeQuestionMarkGraphic = other.<same>;  (scalars / C2DVector / CRGBColour, members.tsv order)
0x0046D117  call vector<long>::operator=                        StealthEyeGraphicFrames = other.StealthEyeGraphicFrames;   // +0x150
0x0046D122  // 12 dword copies +0x15c..+0x18b : StealthEyeTextOffset_X .. DPadRingsExtendGraphic = other.<same>;  (scalars / C2DVector / CRGBColour, members.tsv order)
0x0046D1B9  call vector<long>::operator=                        DPadRingsChargeGraphics = other.DPadRingsChargeGraphics;   // +0x18c
0x0046D1CB  call vector<C2DVector>::operator=                   DPadRingsChargeOffsets = other.DPadRingsChargeOffsets;   // +0x198
0x0046D1DD  call vector<float>::operator=                       DPadRingsChargeAngles = other.DPadRingsChargeAngles;   // +0x1a4
0x0046D1E8  // 107 dword copies +0x1b0..+0x35b : DPadRingsFavouriteGraphic .. CombatLockedOffset_Y = other.<same>;  (scalars / C2DVector / CRGBColour, members.tsv order)
0x0046D6F3  call vector<long>::operator=                        CombatLockedGraphics = other.CombatLockedGraphics;   // +0x35c
0x0046D6FE  // 7 dword copies +0x368..+0x383 : CombatRingOffset_X .. CombatCurrentTextOffset_Y = other.<same>;  (scalars / C2DVector / CRGBColour, members.tsv order)
0x0046D759  call vector<C2DVector>::operator=                   CombatCountOffsets = other.CombatCountOffsets;   // +0x384
0x0046D764  // 1 dword copies +0x390..+0x393 : CombatCountEmptyGraphic .. CombatCountEmptyGraphic = other.<same>;  (scalars / C2DVector / CRGBColour, members.tsv order)
0x0046D777  call vector<long>::operator=                        CombatCountGraphics = other.CombatCountGraphics;   // +0x394
0x0046D789  call vector<long>::operator=                        CombatRingGraphics = other.CombatRingGraphics;   // +0x3a0
0x0046D794  // 38 dword copies +0x3ac..+0x443 : CombatRingBlackGraphic .. CrimeBarBorderOffset_Y = other.<same>;  (scalars / C2DVector / CRGBColour, members.tsv order)
0x0046D963  call vector<C2DVector>::operator=                   CrosshairOffsets = other.CrosshairOffsets;   // +0x444
0x0046D975  call vector<long>::operator=                        CrosshairGraphics = other.CrosshairGraphics;   // +0x450
0x0046D987  call vector<long>::operator=                        CrosshairGreenGraphics = other.CrosshairGreenGraphics;   // +0x45c
0x0046D992  // 30 dword copies +0x468..+0x4df : MiniGameFishingLeftGraphic .. MoneyChangeTextSpeed = other.<same>;  (scalars / C2DVector / CRGBColour, members.tsv order)
0x0046DB01  call CCharString::operator=                         StatBarsFontName = other.StatBarsFontName;   // +0x4e0
0x0046DB0C  // 23 dword copies +0x4e4..+0x53f : SecondsForStatsBarsToAppear .. StatUpdateMoralityEvilEffect = other.<same>;  (scalars / C2DVector / CRGBColour, members.tsv order)
0x0046DC27  call vector<CRGBColour>::operator=                  StatUpdateColours = other.StatUpdateColours;   // +0x540
0x0046DC39  call vector<long>::operator=                        StatUpdateGraphics = other.StatUpdateGraphics;   // +0x54c
0x0046DC4B  call vector<long>::operator=                        StatUpdateEffects = other.StatUpdateEffects;   // +0x558
0x0046DC56  // 28 dword copies +0x564..+0x5d3 : AutoPickupPos_X .. TextBoxButtonGapWidth = other.<same>;  (scalars / C2DVector / CRGBColour, members.tsv order)
0x0046DDAD  call CCharString::operator=                         TextBoxButtonTextFontName = other.TextBoxButtonTextFontName;   // +0x5d4
0x0046DDB8  // 4 dword copies +0x5d8..+0x5e7 : TextBoxConfirmGraphic .. TextBoxSkipCutsceneGraphic = other.<same>;  (scalars / C2DVector / CRGBColour, members.tsv order)
0x0046DDEF  call CCharString::operator=                         TextBoxButtonASoundCriteria = other.TextBoxButtonASoundCriteria;   // +0x5e8
0x0046DE01  call CCharString::operator=                         TextBoxButtonBSoundCriteria = other.TextBoxButtonBSoundCriteria;   // +0x5ec
0x0046DE0C  // 21 dword copies +0x5f0..+0x643 : TextBoxHugeButtonGraphic .. TextBoxBettingRightOffset_Y = other.<same>;  (scalars / C2DVector / CRGBColour, members.tsv order)
0x0046DF0F  call CCharString::operator=                         ReceiveItemsSoundCriteria = other.ReceiveItemsSoundCriteria;   // +0x644
0x0046DF1A  // 48 dword copies +0x648..+0x707 : ControllerAGraphic .. HotBarMainGraphic = other.<same>;  (scalars / C2DVector / CRGBColour, members.tsv order)
0x0046E161  call vector<long>::operator=                        HotBarEndGraphics = other.HotBarEndGraphics;   // +0x708
0x0046E16C  // 5 dword copies +0x714..+0x727 : HotBarOffset_X .. HotBarItemWidth = other.<same>;  (scalars / C2DVector / CRGBColour, members.tsv order)
0x0046E1AF  call vector<CQuickAccessItemButtonGuiDef>::operator=  QuickAccessItemButtons = other.QuickAccessItemButtons;   // +0x728
0x0046E1C1  call vector<CQuickAccessItemButtonGuiDef>::operator=  ContextSensitiveItemButtons = other.ContextSensitiveItemButtons;   // +0x734
0x0046E1CC  // 47 dword copies +0x740..+0x7fb : TargetingCursorSectionEnemyEmptyGraphic .. InteractButtonTooltipOffset_Y = other.<same>;  (scalars / C2DVector / CRGBColour, members.tsv order)
0x0046E407  call vector<CAbilityButtonGuiDef>::operator=        AbilityButtons = other.AbilityButtons;   // +0x7fc
0x0046E419  call vector<CAbilityButtonGuiDef>::operator=        SpecialAbilityButtons = other.SpecialAbilityButtons;   // +0x808
0x0046E424  // 6 dword copies +0x814..+0x82b : DigitPos_X .. DigitHeroOffset_Y = other.<same>;  (scalars / C2DVector / CRGBColour, members.tsv order)
0x0046E473  call vector<long>::operator=                        DigitGraphics = other.DigitGraphics;   // +0x82c
0x0046E47E  // 13 dword copies +0x838..+0x86b : RegionDisplayPos_X .. HeroSprintBarInnerGraphicOffset_Y = other.<same>;  (scalars / C2DVector / CRGBColour, members.tsv order)
0x0046E521  call vector<CRGBColour>::operator=                  HighlightStartColour = other.HighlightStartColour;   // +0x86c
0x0046E533  call vector<CRGBColour>::operator=                  HighlightEndColour = other.HighlightEndColour;   // +0x878
0x0046E545  call vector<float>::operator=                       HighlightStartWidthWorldSpace = other.HighlightStartWidthWorldSpace;   // +0x884
0x0046E557  call vector<float>::operator=                       HighlightEndWidthWorldSpace = other.HighlightEndWidthWorldSpace;   // +0x890
0x0046E569  call vector<float>::operator=                       HighlightStartWidthScreenSpace = other.HighlightStartWidthScreenSpace;   // +0x89c
0x0046E57B  call vector<float>::operator=                       HighlightEndWidthScreenSpace = other.HighlightEndWidthScreenSpace;   // +0x8a8
0x0046E58D  call vector<long>::operator=                        HighlightFadeSteps = other.HighlightFadeSteps;   // +0x8b4
0x0046E598  // 14 dword copies +0x8c0..+0x8f7 : NumberOfSecondsForContainerItemToReachFullSize .. ScreenMessageTextDropShadowColour = other.<same>;  (scalars / C2DVector / CRGBColour, members.tsv order)
0x0046E647  call CCharString::operator=                         ScreenMessageFont = other.ScreenMessageFont;   // +0x8f8
0x0046E652  // 5 dword copies +0x8fc..+0x90f : ScreenMessageNextMessageTextOffset_X .. NumberOfCharactersScreenMessageAddsPerFrame = other.<same>;  (scalars / C2DVector / CRGBColour, members.tsv order)
0x0046E695  call vector<CCharString>::operator=                 TutorialText = other.TutorialText;   // +0x910
0x0046E6A7  call vector<CCharString>::operator=                 TutorialLogBookText = other.TutorialLogBookText;   // +0x91c
0x0046E6B9  call vector<long>::operator=                        TutorialGuildSealGraphics = other.TutorialGuildSealGraphics;   // +0x928
0x0046E6CB  call map? (CRetailTree_00432bab)::operator=         ScriptSprites = other.ScriptSprites;   // +0x934
0x0046E6DD  call map? (CRetailTree_0046ec57)::operator=         GameActionValues = other.GameActionValues;   // +0x940
0x0046E6EF  call map? (CRetailTree_00432bab)::operator=         MiniMapGraphics = other.MiniMapGraphics;   // +0x94c
0x0046E701  call vector<long>::operator=                        MiniMapMarkerGraphics = other.MiniMapMarkerGraphics;   // +0x958
0x0046E70C  // 15 dword copies +0x964..+0x99f : MiniMapScreenOrigin_X .. MaxMinimapThemeColours = other.<same>;  (scalars / C2DVector / CRGBColour, members.tsv order)
0x0046E7C7  call vector<CRGBColour>::operator=                  MinimapZoneColours = other.MinimapZoneColours;   // +0x9a0
0x0046E7D9  call vector<CRGBColour>::operator=                  MinimapThemeColours = other.MinimapThemeColours;   // +0x9ac
0x0046E7E4  // 27 dword copies +0x9b8..+0xa23 : RegionChangeTLOffset_X .. RegionChangeGuildSealBorderOffset_Y = other.<same>;  (scalars / C2DVector / CRGBColour, members.tsv order)
0x0046E92F  call vector<long>::operator=                        RegionChangeGuildSealGraphics = other.RegionChangeGuildSealGraphics;   // +0xa24
0x0046E93A  // 4 dword copies +0xa30..+0xa3f : LoadProgressBarLeftGraphic .. LoadProgressBarInnerGraphic = other.<same>;  (scalars / C2DVector / CRGBColour, members.tsv order)
0x0046E971  call vector<long>::operator=                        LoadProgressBackdropGraphics = other.LoadProgressBackdropGraphics;   // +0xa40
0x0046E97C  // 18 dword copies +0xa4c..+0xa93 : TavernGameBlockTLOffset_X .. TavernGameBlockBRGraphic = other.<same>;  (scalars / C2DVector / CRGBColour, members.tsv order)
0x0046EA5B  call CCharString::operator=                         RegionGraphFileName = other.RegionGraphFileName;   // +0xa94
0x0046EA66  // 7 dword copies +0xa98..+0xab3 : MouseCursorScaleX .. MouseOverlayGraphic = other.<same>;  (scalars / C2DVector / CRGBColour, members.tsv order)
0x0046EAAE  epilogue (pop edi/esi interleaved with the +0xAB0 store); return *this (mov eax,ebx) ; ret 4

Notes:
- Each call is `lea eax,[ebp+off] ; push eax ; lea ecx,[ebx+off] ; call`: arg first, then this-pointer -> normal thiscall member
  op= on a member subobject (map: ecx = &map = &map._M_t).
- C2DVector members copy as two dword moves (X then Y), CRGBColour as one dword, the two `unsigned long` (MaxMinimapZoneColours
  +0x998, MaxMinimapThemeColours +0x99C) as dwords -- they live inside the 0x0046E70C run.
- Container callee names: vector<long> 0x00454886, vector<C2DVector> 0x0046EABD, vector<float> 0x0045EF2D, CCharString 0x0099EFB0,
  vector<CRGBColour> 0x0045BAAA, vector<CQuickAccessItemButtonGuiDef> 0x0046ED15, vector<CAbilityButtonGuiDef> 0x0046F06A,
  vector<CCharString> 0x00432EE9, _Rb_tree<CCharString,pair<const CCharString,long>..> 0x00432B38,
  _Rb_tree<CCharString,pair<const CCharString,EGameAction>..> 0x0046EBE4.
