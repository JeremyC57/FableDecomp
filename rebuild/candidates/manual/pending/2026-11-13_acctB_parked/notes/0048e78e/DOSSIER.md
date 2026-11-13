# DOSSIER 0x0048E78E CSubtitleRenderer::DrawButtonGraphics (2662 B, identity scout, bwf1)
Sources: xdis = `$W/xdis_scout.txt` (`xdis.py 0048e78e 0xa76`), prior = `$W/prior/` (NOTES.md, NAME_FIXES.tsv, scout/outline.md,
scout/headers.md, scout/residue_1.md, grades.log 95 grades). Nothing here contradicts prior; it is re-verified + condensed.

## 1. Retail shape (xdis lines 1-10, 913)
- `push ebp ; lea ebp,[esp-68h] ; sub esp,16Ch` then two global loads, THEN `push ebx/esi/edi`; epilogue `add ebp,68h ; leave ; ret 10h`
  (0x0048F1F1). __thiscall, 4 dword args ([ebp+70h] info, +74h handles, +78h &pos, +7Ch bool). No fs:0 / SEH, no /GS cookie -> no `// Flags:`.
- BIASED frame = `#pragma optimize("s", on)` ALONE (prior scout/outline.md s.1; same as landed DrawBox 0x0048FAA0, Update 0x004930D7,
  IsTextReadyForClickPast 0x0048E072, IsCutsceneSkippable 0x0048DE44). Most other TU siblings use ("y",off)+("s",on) because their retail
  frame is `mov ebp,esp` -- do NOT copy that here. The bias size 0x68 depends on frame content (probe: plain arrays gave `mov ebp,esp`).
- Size idioms: `push 45h ; pop edx` (0x0048E9CF), `push ecx` arg slots, cross-jumped `fstp [esp]` (0x0048E81B -> 0x0048E801),
  cross-jumped push before one `call [eax+5Ch]` (0x0048E98F..0x0048E99F).
- One function, no cold code; extent 0x0048E78E..0x0048F1F3 verified (DrawBox ends `ret 8` at 0x0048E78B; Draw2DText at 0x0048F1F4).

## 2. Name / signature (two sources) -- manifest label is RIGHT
- `?DrawButtonGraphics@CSubtitleRenderer@@AAE_NABVCEnginePrimitivePrepareInfo@@AAV?$vector@VCEnginePrimitiveHandle@@V?$allocator@VCEnginePrimitiveHandle@@@std@@@std@@ABVC2DVector@@_N@Z`
  = `private: bool __thiscall DrawButtonGraphics(const CEnginePrimitivePrepareInfo&, std::vector<CEnginePrimitiveHandle>&, const C2DVector&, bool)`.
  Evidence: ghidra_out/egor_pdb_names.tsv (Ego_r 0x00497852) + FableWin.pdb 0x01A67310 (prior/LABELS.tsv); `ret 10h` = 4 args. Non-virtual (not in
  rtti-vtables.tsv). Param / local names descriptive (no PDB locals).
- Only caller: 0x00490818 in DrawPreparePrimitivesForEngine 0x00490494 (xref.py; NOT landed): `push 0 ; push &local C2DVector ; push this+0xA0
  (ButtonPrimitiveHandles) ; push info` -> in_text_box = false there.

## 3. Callees (landed file / landed spelling / PDB) -- flags = disagreement
| retail | landed row (rebuild/src/compiled/..) | landed spelling | PDB spelling | |
|---|---|---|---|---|
| 0x00434870 | 00/43/CFadeInFadeOutBase_Update_00434870.cpp | ?Update@CFadeInFadeOutBase_Methods@@QAEXM@Z | ?Update@CFadeInFadeOutBase@@QAEXM@Z | FLAG class |
| 0x00434840 | 00/43/CFadeInFadeOutBase_Open_00434840.cpp | ?Open@CFadeInFadeOutBase@@QAEXMMVCRGBColour@@@Z | same | ok |
| 0x0048DA30 | 00/48/CFadeInFadeOutBase_IsOpen_0048da30.cpp | `long CFadeInFadeOutBase_Methods::IsOpen()` | ?IsOpen@CFadeInFadeOutBase@@QBE_NXZ | FLAG class/const/bool |
| 0x00438570 | 00/43/CPlayerGui_PeekGuiDef_00438570.cpp | Leaf "PeekGuiDef" | ?PeekGuiDef@CPlayerGui@@SIABVCPlayerGuiDef@@XZ (static fastcall) | check |
| 0x009E2C80 | 00/9e/CFontManager_GetFont_009e2c80.cpp | ?GetFont@CFontManager@@QAE?AV?$CCountedPointer@VCFontBank@@@@ABVCCharString@@@Z | same | ok (manifest sig garbage) |
| 0x004057A0 | 00/40/Destval..._004057a0.cpp | ??1?$CCountedPointer@VCFontBank@@@@QAE@XZ | (dtor of GetFont's temporary; out of line) | ok |
| 0x0048E062 | 00/48/global_operatornew_0048e062.cpp | ?operator_new@@YGPAXIPAUPool@@@Z | ?GetTextWidthFromText@CSubtitleRenderer@@ABEMABVCWideString@@ABVCFontBank@@@Z | FLAG wrong fn |
| 0x009E1E10 | 00/9e/global_GFVXToAX_009e1e10.cpp | ?GFVXToAX@@YIMM@Z | same | ok (DrawTextEmbeddedGraphics row declares it __stdcall GFVXToAX_009e1e10: FLAG) |
| 0x009BEDC0 | 00/9b/CDisplayManager_GetRenderTargetDimensions_009bedc0.cpp | CDisplayManagerRenderTargetDimensionsView::.. const | ?GetRenderTargetDimensions@CDisplayManager@@QAE?BVC2DExtentsI@@XZ (NON-const, const result) | FLAG view class/const |
| vt+0x5C (bank) | CGraphicDataBank slot 23 0x00A014D0 | GetGraphicMaxWidth(unsigned long) const | ?GetGraphicMaxWidth@CGraphicDataBank@@UBEKK@Z | ok |
| 0x004348D0 | 00/43/CFadeInFadeOutBase_GetFadeInOutAlpha_004348d0.cpp | non-const, made-up members | ?GetFadeInOutAlpha@CFadeInFadeOutBase@@QBEMXZ | FLAG const + member names |
| 0x00493AC2 | 00/49/CEnginePrimitive2DSprite_..._00493ac2.cpp | ??0CEnginePrimitive2DSprite@@QAE@_KABVC2DVector@@W4EEngine2DLayer@@KK...11@Z | (pos ctor) | ok |
| 0x0041BEB0 | 00/41/CEnginePrimitive2DSprite_..._0041beb0.cpp | (no Leaf) | ??0CEnginePrimitive2DSprite@@QAE@_KABVC2DBoxF@@W4EEngine2DLayer@@KKW4EEnginePrimitiveBlendMode@@W4EEngineInterpolationSet@@MMEEEEKABVC2DVector@@5@Z | use header ENUM_CTORS |
| vt+0x5C (engine) | CIEngine.h slot 23 | SetPersistentPrimitive(CEnginePrimitiveHandle&, const CEnginePrimitive&, unsigned long) | ?SetPersistentPrimitive@CEngine@@ | ok |
| 0x0099B150 | 00/99/CWideString_operator_0099b150.cpp | ??9CWideString@@QBE_NPBG@Z | same | ok |
| 0x0048DE44 | 00/48/CSubtitleRenderer_IsCutsceneSkippable_0048de44.cpp | bool IsCutsceneSkippable() const | ?IsCutsceneSkippable@CSubtitleRenderer@@QBE_NXZ | ok |
| 0x0048E072 | 00/48/CSubtitleRenderer_IsTextReadyForClickPast_0048e072.cpp | IsTextReadyForClickPast | ?IsTextReadyForClickPast@CSubtitleRenderer@@QBE_NXZ | ok (reloc_bindings IsTextFullyShown: FLAG) |
| vt+0x0C / +0x04 | CEnginePersistentPrimitiveBase slots 3 / 1 (Clear expansion 0x0048EE4B..0x0048EE84) | -- | ?Clear@CEnginePrimitiveHandle@@QAEXXZ (inline) | ok |
| 0x00449970 | 00/44/CPlayerManager_GetMainPlayer_00449970.cpp | ?GetMainPlayer@CPlayerManager@@QAEAAVCPlayer@@XZ | same (non-const overload) | ok |
| 0x00644D54 | 00/64/NPlayerGuiCDrawQuestInfo_GetMiniGameType_00644d54.cpp | -- | ?GetMiniGameType@CDrawQuestInfo@NPlayerGui@@QBE?AW4EMiniGameType@@XZ | check (reloc_bindings non-const long) |
| 0x00643CA1 | 00/64/NPlayerGui_GFAnchorToTopRight_00643ca1.cpp | ?GFAnchorToTopRight@NPlayerGui@@YI?AVC2DVector@@ABV2@@Z | same | ok |
Inline (no call in retail): IsCutsceneActionModeEnabled (+0xE2 byte, FableWin ?IsCutsceneActionModeEnabled@CSubtitleRenderer@@QBE_NXZ),
CWorld::IsPaused (+0xA4, CWorld.h:130/545), CMainGameComponent::Get / PeekWorld (CMainGameComponent.h:175/221), CFadeInFadeOutBase::IsActive
(byte [ebx]), Close (`mov [ebx+0Dh],0 ; mov [ebx+0Ch],1`), CEnginePrimitiveHandle::Clear, GetPlayerRespawnDelay. Globals: PMainGameComponent
0x013B86A0, PFontManager 0x013B838C, GPDisplayManager 0x013B8390, player-gui 0x013B8790. NAME_FIXES.tsv (prior) already carries all respellings.

## 4. Members (retail offsets; Ego_r struct_layouts are 0x10 lower for CSubtitleRenderer -- retail VC6 deque is 0x28)
- CSubtitleRenderer (rebuild/include/engine/CSubtitleRenderer.h; struct_layouts_egor.tsv): Button1Text..Button2OverrideText +0x64..+0x74
  (CWideString), Button1Alpha +0x78 / Button2Alpha +0x79 (uchar), DisplayFlags +0x7C (ulong; bits 0x800/4/0x48/2 numeric, type ESubtitleFlag
  only in FableWin), InCutscene +0xDE, CutsceneActionMode +0xE2, Button1TextOverridden +0xE3, Button2TextOverridden +0xE4 (bool),
  ButtonPulseTimer +0x104 (CFadeInFadeOutBase, 0x1C; Active +0, Closing +0xC, Opening +0xD). CWideString typing + timers + the
  method declarations exist ONLY in the overlay macro FABLE_RETAIL_CSUBTITLERENDERER_BUTTONGRAPHICS ($W/hdr, not in repo).
- CPlayerGuiDef (+ebx): TextBoxTextAreaBRPos.X 0x5AC, TextBoxButtonGraphicOffset 0x5C0/0x5C4, TextBoxButtonOffsetY 0x5C8,
  TextBoxButtonInBoxOffsetY 0x5CC, TextBoxButtonGapWidth 0x5D0, TextBoxButtonTextFontName 0x5D4 (CCharString), TextBoxConfirmGraphic 0x5D8,
  TextBoxCancelGraphic 0x5DC, TextBoxThirdGraphic 0x5E0, TextBoxHugeMouseButtonGraphic 0x5F4, TextBoxHugeMouseButtonLeftGraphic 0x5F8,
  TextBoxHugeButtonOffset 0x5FC/0x600, TextBoxHugeButtonLeftOffset 0x604/0x608, TextBoxFishingReelTextPos 0x60C, TextBoxFishingReelGraphicOffset
  0x614. Named by overlay-only FABLE_RETAIL_CPLAYERGUIDEF_TEXTBOX ($W/hdr/engine/CPlayerGuiDef.h:422).
- CEnginePrimitivePrepareInfo: Interpolate.Current (+0) / PausedCurrent (+0x10) via FABLE_RETAIL_CENGINEPRIMITIVEPREPAREINFO (repo).
- Draft models NO class locally (all via shared headers) -- good. Only free decls in the row: GFVXToAX, NPlayerGui::GFAnchorToTopRight,
  `extern CDisplayManager* GPDisplayManager`.
- Macro availability (draft's 43 defines): IN REPO already: CWIDESTRING, CCHARSTRING, CRGBCOLOUR, C2DVECTOR, C2DBOXF(+_CORNERS,+_EXTENTS),
  DEF_CLASS_BASE, CENGINEPERSISTENTPRIMITIVEBASE, CIVCOUNTEDPOINTER, CENGINEPRIMITIVEHANDLE, CENGINEPRIMITIVE2DSPRITE_ENUM_CTORS,
  CENGINEPRIMITIVEPREPAREINFO, CIENGINE(+_RENDER/_CONVERTSCREENPOS/_CONVERTWORLDPOS), CFONTBANK, CFADEINFADEOUTBASE_ISACTIVE, CPLAYERGUI(+_CTOR,
  _GRAPHICBANK, _QUEST_DRAW, _PEEKGUIDEF), CGRAPHICDATABANK_GRAPHICSIZE, CMAINGAMECOMPONENT(+_WORLD,_PLAYERMANAGER,_GETPLAYERMANAGER),
  CWORLD_UPDATEINPUT, CDISPLAYMANAGER, SCOPED_PTR. OVERLAY ONLY (must be added to repo headers at landing): CRGBCOLOUR_CTOR_INITIALISE,
  CIVCOUNTEDPOINTER_FORCEINLINE_RELEASE, CENGINEPRIMITIVEHANDLE_FORCEINLINE_CLEAR, CFONTBANK_VSTRINGWIDTH, CFADEINFADEOUTBASE_FADE,
  CFADEINFADEOUTBASE_ISOPEN_OUTOFLINE, CSUBTITLERENDERER_BUTTONGRAPHICS, CPLAYERGUIDEF_TEXTBOX, CDRAWQUESTINFO_MINIGAMETYPE,
  CMAINGAMECOMPONENT_SCOPED_ACCESSORS, CPLAYER_RESPAWNDELAY.
- STALE overlay headers (HINTS): C2DVector.h (repo gained _ASSIGN3D/_ISNONZERO_FORCEINLINE/_ROTATE; overlay adds NAMED_RESULT_ADD/SUB/MUL, which
  the draft does NOT use -> could simply drop the overlay copy), CMainGameComponent.h (overlay adds _SCOPED_ACCESSORS/_ENGINEGRAPHICS; repo
  gained _INITLUA), NPlayerGui_CDrawQuestInfo.h (overlay adds _MINIGAMETYPE; repo gained _UPDATEMINIGAME), plus CGameCameraManager.h,
  CTCInGameMenu.h, CNavigatorManager.h (not used by this row). Re-base by hand before hand-back.

## 5. Callee visibility (same TU = subtitle TU 0x0048DA30..0x00494xxx, unaligned COMDAT-free run)
- Same TU, defined EARLIER and still called out of line: IsOpen 0x0048DA30 (23i at /Od; overlay ISOPEN_OUTOFLINE = declaration only),
  IsCutsceneSkippable 0x0048DE44, GetTextWidthFromText 0x0048E062 (4 calls), IsTextReadyForClickPast 0x0048E072, DrawBox 0x0048E0A2 (not called).
  If the author defines any of these in the row, they must be prevented from inlining (auto_inline(off) as IsTextReadyForClickPast row does).
- Out-of-line library/other-TU: CCountedPointer<CFontBank> dtor 0x004057A0 (called right after GetFont: the temporary is destroyed
  immediately, `font` keeps the raw Data word in [ebp+2Ch]); CWideString::operator!= 0x0099B150; both sprite ctors.
- Expanded inline in retail: CEnginePrimitiveHandle::Clear (+ CIVCountedPointer Reset/Release) at 0x0048EE4B..0x0048EE84 -> overlay
  FORCEINLINE macros (fitted; needs an `// Inline-budget:` line before landing per prior NOTES). No STLport map/tree helpers involved;
  `primitive_handles[i]` is plain vector indexing (inline).

## 6. Siblings (same TU, landed) to copy idioms from
- 00/48/CSubtitleRenderer_DrawBox_0048faa0.cpp: `#pragma optimize("s", on)` only.
- 00/48/CSubtitleRenderer_IsTextReadyForClickPast_0048e072.cpp: ("s",on) + `auto_inline(off/on)` around a same-TU helper body.
- 00/49/CSubtitleRenderer_Update_004930d7.cpp: ("s",on) only.
- 00/48/CSubtitleRenderer_DrawTextEmbeddedGraphics_0048f230.cpp, CSubtitleRenderer_Draw2DText_0048f1f4.cpp (neighbours after this row):
  ("y",off)+("s",on) because their frames are `mov ebp,esp`; DrawTextEmbeddedGraphics keeps a CWideString across calls with no EH frame ->
  the TU is not /EHsc (no Flags line). GetGraphicFromEmbeddedTag 0x00490BE0 uses CWIDESTRING_TEXT_EQ / _FORCEINLINE_EQ (for `!= L""` checks,
  but here operator!= is called out of line, so do not take those).

## 7. Draft model errors v disassembly (draft = c39, 821/913; best c67 = 858/913, prior/scout/residue_1.md)
- (A) pos.Y init 0x0048E85E..0x0048E898: retail `mov ecx,[eax+4] ; fld [eax+4] ; mov eax,ecx ; mov [54h],eax ; mov [64h],ecx` then
  `fadd [off] ; fstp [5Ch]` and `fld [64h];fadd;fstp [64h]`, `fld [54h];fadd;fstp [54h]`. Slots: button1_pos = [58h]/[5Ch],
  button2_pos = [60h]/[64h], button3_pos = [50h]/[54h]. So ONLY button1.Y is a float assignment (fld/fstp); button2.Y and button3.Y are
  INTEGER copies (struct/member copy semantics), button3 from button2's register. Draft constructs all three with C2DVector(0.0f, pos.Y)
  (three float inits). Candidate the notes do not list: button1 float-init, then button2/button3 copied from something whole-word
  (e.g. `button2_pos.Y = pos.Y` typed as integer via a struct copy of `pos` then X overwritten) -- X initial stores are dead in retail
  (no store of 0 to [58h]/[60h]/[50h]), consistent with any init whose X is later overwritten.
- (B) box block 0x0048E9C6: retail loads DisplayFlags into ecx right after GetFadeInOutAlpha (`mov ecx,[esi+7Ch] ; test ch,8`), 0x45 into
  edx, layer stored into [ebp+54h] (button3_pos.Y slot reused) -- draft's c39 has flags in eax; c67 (conversion inside each width arm) fixes
  this but costs +6 bytes in the width block (retail = ONE cross-jumped `call [eax+5Ch]` with `fild` once, 0x0048E98F..0x0048E9AA, i.e. the
  if/else-into-`unsigned long` of the draft). Prior analysis: one integer candidate too many in that block (likely the ButtonPulseTimer
  address `lea ecx,[esi+104h]` at 0x0048E9B2 shared with the top section).
- (C) arm A 0x0048EA97: retail keeps `lea eax,[ebx+5FCh]` / `lea ecx,[ebx+600h]` address temps (C2DVector operator+= on the box corners, or
  Move-by-offset through a reference) v draft's four scalar `+=`.
- (D) tail 0x0048F0B7 operand order `fld [eax] (GFAnchorToTopRight result) ; fadd [edi]` -- record-keyed: +4..12 dead IL units flipped it
  (prior residue_1 s.2); FableWin /Od body is ~36-45 instructions longer than the draft => original has a few more statements/named temps.
- Frame size/slots already equal retail in the draft (0x16C via the bare block around button3_box -- FITTED).
