# Chunk 829 notes

Summary: 1 RELOCATION_MATCH (0x0068BC70), 5 parked DIFFER (0x00B5AF80 2865v2865 md 14, 0x0068CB90 2369v2339 md 27,
0x00439070 2611v2613 md 125, 0x00435530 2571v2617 md 401, 0x00678760 3554v3579 md 1215). Every row has at least 10 logged
attempts (ATTEMPTS.log). Names marked (descriptive) are not PDB-proven.

## 0x0068BC70 CInputProcessInventoryQuestsScreen::ProcessInput -- RELOCATION_MATCH (src/, test/)
- Shape: `if (!inventory->IsInventoryScreen()) { one if / else-if chain over event type 0x11 / 0x12 / pressed / held /
  released / game action 0x47 / 0x65, no early returns } else { released A/B/Y/White handlers; base ProcessInput }`.
  Early returns (a1) let VC7.1 merge the SetAsPerformedEvent tails: DIFFER(2190v2481). `// Flags: /GS` (the CGameEvent
  32-byte array gives the cookie frame, as siblings 0x0068B7C0 / 0x0068AE30).
- Callees: IsEventButtonPressed 0x00687A90 (type 0x13), **IsEventButtonHeld 0x00687B30 (type 0x14)**, **IsEventButtonReleased
  0x00687BD0 (type 0x15)** -- reloc_bindings has the last two swapped (NAME_FIXES). PanMap 0x005FB140 takes C2DVector by value.
  GetMouseWheelMovement 0x00A03BB0, floor(float) 0x00689370, ScrollMenu 0x0041DA2D, GetInstance 0x0041E5F2,
  AddGameEvent 0x0041649C, LowerBound 0x0040F020, GetPControlledCreature 0x00487DC0.
- vland (read-only, overlay): RELOCATION_MATCH PASS; warnings SHARED_HEADER_AVAILABLE for CProcessedInput, CManager,
  CMainGameComponent, CTCInventoryMap, CThing, CPlayer, CInputProcessGameBase (local models kept, `this` class has no header);
  RELOC_CONFLICT on IsEventButtonReleased vs landed 0x006896C0 = the swapped binding in NAME_FIXES.
- Local models (header debt): CTCInventoryBase vtable slots 38-67, NHeroInformationScreens::CBase, CVectorMap Find as
  __forceinline, CGameEvent.

## 0x0068CB90 CInputProcessControlFreeCamera::ProcessInput -- DIFFER(2369v2339) md 27 (differ/)
- Shape: no PointCameraAtAngles helper (FableWin lists none): every arm is open-coded; a __forceinline GET_ROUNDED_ANGLE wraps
  each angle to [0,1); the direction comes from C3DVector::InitialiseFromAngles (hdr opt-in); height keys use
  `Move(C3DVector(0, 0, +-2.5f))`; forward / up are copied with `C3DVector v; v = ...` (implicit block copy); the mouse
  factor must be parenthesised `(X * 0.0027777778f) * 0.5f` to keep two fmuls.
- Residue: InitialiseFromAngles keeps a cos*len copy for Y and pops it (5x3 lines), the cross-product copy order (8 lines),
  the 0x12 arm homes y in the event argument slot (2 lines).
- Names: GetKey 0x00A03B70, GetJoystickButton 0x00A03B90, GetJoystickPosition 0x00A043A0, GetMouseMovement 0x00A043D0
  (C3DVector by value), GetType 0x00A03B40, SetAsPerformedEvent 0x00A0D390 (all FableWin.pdb; NAME_FIXES for the wrong
  bindings). CCamera GetAngleXY 0x00A0C380, GetAngleYZ 0x00A0C3E0, PointAt 0x00A0C2B0, MoveForward 0x00A0C250, MoveSideways 0x00A0C1B0.
  FreeCameraTurnSpeed / MoveSpeed / StepDistance / HeightStep at 0x0137EE6C / 68 / 70 / 74 are (descriptive).

## 0x00435530 CDisplayEngine::DoRender -- DIFFER(2571v2617) md 401 (differ/)
- Which overload: Ego_r.pdb holds exactly one `?DoRender@CDisplayEngine@@IAEXPBVCInterpolationInfo@@_N1@Z` (protected,
  0x004490E0, ret 0Ch) followed by `Render`; retail 0x00435530 is the big body (ret 0Ch) and 0x00435F70 the public wrapper,
  so 0x00435530 = DoRender (protected IAEX -- the class must declare it `protected:` or the obj symbol is QAEX and the grader
  falls back to "largest function"; a1-a9 did that) and 0x00435F70 = Render (landed as DoRender: NAME_FIXES).
- Shape: fade readiness; MainWindow filled by four direct field stores through `C2DBoxF* window = &MainWindow` (TLX, BRX,
  TLY, BRY; a temp + copy or placement new adds code); camera; fog off; view window; television area; BeginRender; clear;
  draw-game block; debug drawer; free-cam debug; letterbox; RenderHeroFuckShit / RenderHeroDrunkenness; fade timer
  (`t = t + info->Time`); screen fade update (`float time = 0; if (!count) time = info->Time;`); debug map + track names
  (count hoisted; GDefStringTable.GetString(...).ToWideString() inline so the CCharString temp dies before the call);
  "Recording!", "Frames behind %d", "Skipping unrecorded frames!", console, build version, 2D flush, screenshots
  ("data\\movies\\shot" + zero pad + frame + ".tga" at 640x480, or "wishworldshot" with --g_ScreenshotCount), gamma, swap, ++Frame.
- Residue: retail frame 0xcc vs ours 0x90 (retail does not share slots between the late blocks; the camera sits at frame
  0x94 in retail), so nearly every slot offset differs; the track-name loop's register choice.
- Callees with evidence: SetAboutToRun 0x00434CD0, EndScreenFadeOut 0x00434C90, RenderHeroFuckShit 0x00435000 (PDB; label was
  GiveHeroGold), RenderHeroDrunkenness 0x00435070, GetAllThingsInRoughArea<CThing,...> 0x00436D60 (PDB; label was
  GetPBestThingInRoughArea), Draw3DScaledText 0x009DBFF0 (PDB, hdr opt-in), CDefStringTable::GetString 0x009D49B0,
  GFIntToWideString 0x00405F00, CConsole ctor 0x009ECD80, CConsole::Render 0x009EA550 (label was Nop_ret4; argument type from
  the call site), SaveAsTGA 0x009F4590, CCamera ctor 0x00A0BF20, CEngineManager::OnRender 0x00640500.
- (descriptive): CTrackNodeInfo / PeekNameIndex (CThing +0x70, vtable slot 10), the debug drawer at CSystemManager +0xf8 and its
  Draw 0x009F8D60, g_013BA85C / g_013B8A0C / g_013B8A10, g_Render* / g_Draw* / g_Screenshot* globals, CSurfaceSaveView.
- Header debt: CDisplayManager's render-target surface (+0x2c) is private and CSurface lacks SaveAsTGA; the row uses a local
  view `PeekRenderTargetSurface(GPDisplayManager)`. Wanted: `CSurface* PeekRenderTargetSurface()` accessor in
  engine/CDisplayManager.h and `bool SaveAsTGA(const CWideString*, const C2DExtentsI*, bool)` in engine/CSurface.h (0x009F4590).

## 0x00439070 CPlayerGui::DrawPreparePrimitivesForEngine -- DIFFER(2611v2613) md 125 (differ/)
- PDB name confirmed in FableWin.pdb. Layout: Ego_r CPlayerGui members at Ego_r offset - 8 for the counted-pointer block
  (PDrawHealth +0x140 ... PDrawNPCName +0x248, stride 8); GuiMode +0x294, ShowTime +0x29d, ShowMoneyBag +0x29e, Alpha +0x2a0,
  DrawingDebugTGA +0x2b1, DebugTGATexture +0x2b4, ForceAllowSubtitleToRender +0x2bd, LiveGuiMode +0x2be, illustrators
  +0x2c0/+0x2c4, PCrime +0x2ec, DebugTGAPrimitiveHandle +0x31c, DisplayBottomHalfGUI +0x330.
- Spellings that mattered: /Gr makes std::for_each __fastcall (mem_fun_t with a ctor: hidden result in ecx, first in edx);
  CRGBFloatColour(r,g,b,a) evaluated right to left; colour locals block-scoped; the sprite is 0x54 bytes (Ego_r) for the 0x94
  frame; PeekGuiDef() inline with a "PLAYER_GUI_PC" CCharString (Get 0x0044C6B0, GetDef 0x0043FF30).
- Callees: SetMoneyAsDisplaying 0x004376B0 (bound as SetCrimeAsDisplaying: NAME_FIXES), SetCrimeAsDisplaying 0x00437770,
  CDrawMorality::Draw 0x006448A4 and CDrawSpiritScores::Draw 0x00644424 (labels), ClearEffects x5 (bound as ClearDisplay: NAME_FIXES).
- Residue: the fourth colour int temp shares a slot (retail uses [esp+0x18]), shifting slots by 4; the zero C2DVector temps in the
  TGA block are stored before the calls (retail after the pushes); handle Reset uses ecx (retail eax).
- (descriptive): the draw classes' Drawn flag (+0x18), SubtitleRenderer Bool_de / Bool_e5 / Flags7c, IsFrozen (CWorldMode slot 395),
  SetGuiColours (engine vtable +0x10c), AddPrimitive (+0x5c).

## 0x00678760 CPersistTraits<std::list<CReactionMatch*> >::TransferIn -- DIFFER(3554v3579) md 1215 (differ/)
- Exact PDB spelling (Ego_r.pdb 0x006AA810, FableWin.pdb):
  `?TransferIn@?$CPersistTraits@V?$list@PAVCReactionMatch@@V?$allocator@PAVCReactionMatch@@@std@@@std@@@@QAEXAAVCPersistContext@@AAVCStringParser@@AAV?$list@PAVCReactionMatch@@V?$allocator@PAVCReactionMatch@@@std@@@std@@@Z`
- Shape: `.Add` / `(` / type through ::TransferIn 0x009E2590 / `,` / SkipWhitespace / name = ReadAsStringUntilString("(") / `(`;
  then five shapes by CCharString == (CBasicString::operator== 0x004115A0): Sphere (COpinionSet of 5 floats + radius,
  ctor 0x0067C2F0), MoralityRenownBlob (7 floats, 0x00677760), AsymmetricPyramid (2 EOpinion + 6 floats, 0x00677620),
  SecondaryAxisMRShifted (0x00677540) / SecondaryAxisCentredMRShifted (0x006775B0) (EOpinion + 6 floats); unknown name ->
  PersistError(msg 0x0125B840 + name + "'!"); list push_back 0x0067BCC0. Float traits 0x0040F7E0 and EOpinion traits 0x00679910
  sit in the TU under auto_inline(off) + noinline (the sibling recipe).
- Residue: retail keeps PStringData in esi across the compares and threads the null path (five repe cmpsb, direct je); not
  reproduced with const, a const reference, a classifier inline, or a ternary operator== (helps the chain, breaks the singles).
- 0x00679910 is an ICF fold (landed as CPersistTraits<EInputKey>; FableWin has the EOpinion instantiation this row calls).

## 0x00B5AF80 CEngineScreenEffectColourFilter::RenderColorFilteredQuad -- DIFFER(2865v2865) md 14 (differ/, retry of chunk 792)
- Size parity needs the tint*contrast product written with a folded rvalue contrast (`scaledContrast * 1.0f` or `+ 0.0f`:
  no extra fmul is emitted, but VC7.1 then computes R last as retail does). This is an artificial spelling: flagged on line 2
  of the differ file; the plain spelling is md 29, member-assignment md 23.
- Of the 14 md lines, 8 are relocation-equivalent table+8 operands; the real residue is one test/or operand flip in the inline
  render-state bookmark setter at 0x00B5B52F (the same flip ApplyColourFilter parks).
