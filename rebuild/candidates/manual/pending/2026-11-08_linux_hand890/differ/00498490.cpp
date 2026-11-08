// CProgressDisplay::DrawRetailDisplay(const CCharString& textTag, float progress, bool useProgress) -- draw the retail loading / auto-save screen: advance the stage count (or follow the installer's progress), pick the next loading picture at half way, draw the background picture or the GUI frame, the progress bar, the guild seal animation and the tip / save text
// Grade: NOT GRADED -- partial decode only, 0 grades (session budget spent on row 0x00551EA0); not compiled. Residue unknown.
// Next: write engine/CProgressDisplay.h (retail layout below) in hdr/, finish the decode from 0x00498972 on (work/r3.ann has the
//   annotated listing), then grade; expect x87 order and box-copy residues (C2DBoxF temporaries copied dword by dword through eax/ecx/edx).
// Leaf: ?DrawRetailDisplay@CProgressDisplay@@AAEXABVCCharString@@M_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00498490, 5,101 bytes (`ret 0Ch` at 0x0049987A, int3 to 0x0049987F, next 0x00499880 =
//   CProgressDisplay::RenderProgress, the caller). Frame `push ebp ; mov ebp,esp ; and esp,-8 ; sub esp,5Ch` (parameters via ebp).
// Name: FableWin.pdb 0x018AEEB0 / Ego_r.pdb 0x004A2EC0 ?DrawRetailDisplay@CProgressDisplay@@AAEXABVCCharString@@M_N@Z (private;
//   12 bytes of arguments = `ret 0Ch`). The landed caller 0x00499880 binds 0x00498490 as ?DrawProgress@CProgressDisplay@@QAEXABVCCharString@@M_N@Z
//   (NAME_FIXES.tsv).
// Retail layout (Ego_r.pdb member order; STLport vector 0xC; the doubles 8-aligned, sizeof 0x88). Every offset the row touches fits:
//   vptr +0x00; vector<CTexture> ProgressTextures +0x04 (empty test +4 == +8); GuildSealTextures +0x10 (+0x10 / +0x14);
//   LoadingScreenTextures +0x1C (size = (+0x20 - +0x1C) >> 3, sizeof CTexture 8); long LoadingScreenIndex +0x28; TextTags +0x2C;
//   TextTagsPC +0x38; TextTagIndex +0x44; CWideString Text +0x48; CCharString TextTag +0x4C; CCharString LastTextDisplayed +0x50
//   (compared with "" by operator!= 0x0099E960); long GuildSealFrameIndex +0x54; long StageCount +0x58; double LastUpdateTime +0x60,
//   LastScreenUpdate +0x68, LastProgressUpdate +0x70 (`fstp qword [ebx+70h]` of GFGetTime); bool DebugDisplay +0x78, Initialised +0x79,
//   FullScreen +0x7A, BlackScreen +0x7B, DisplayText +0x7C, DisplayResetProgressTipText +0x7D, ForRelaunch +0x7E, ForFrontEnd +0x7F,
//   ForFileInstaller +0x80, FileInstallerFinished +0x81, IgnoreNextBeginRenderCall +0x82, IgnoreNextEndRenderCall +0x83.
// Globals: g_bUseCompiledWorldFiles 0x013B8618, g_bUseCompiledGlobalThings 0x013B8609, g_WADBuildEnabled 0x01375459,
//   g_bAutoSaveProgressShown 0x013B89C1, g_bSaveLoadFailed 0x013B89C0, g_AutoSaveProgressEndTime 0x013B89C8 (double),
//   g_screenWidth / g_screenHeight 0x013961E8 / 0x013961F0 (double), CPlayerGui global 0x013B8790, GPDisplayManager 0x013B8390,
//   the progress font CCountedPointer 0x013B8998 (CProgressDisplay::PFont), g_bProgressDisplaySuppressed 0x013B89D3 (labels: check PDB).
// Callees (manifest labels, verify): GFGetSystemManager 0x009A4EC0, CDisplayManager::GetRenderTargetDimensions 0x009BEDC0 (x12),
//   GFAXToVX 0x009E1DA0 / GFAYToVY 0x009E1DD0, GFRoundVXToNearestPixel 0x009E1D00 / VY 0x009E1D50, the render manager's
//   Draw2DScaledGraphic 0x009DD210 (x10), Draw2DTexture 0x009DC6E0 (x7), Draw2DBox 0x009DE3F0, Draw2DGraphic 0x009DCA80,
//   CPlayerGui 0x00437570 / PeekGuiDef 0x00438570 (graphics table at +0xA40 of the GUI def), GFGetTime 0x009E1BC0, __ftol2,
//   NGameText::CDataBank::GetTextBySymbol 0x009C95E0, CCharString::GetSubString 0x0099EC70, GFABoxToVBox 0x009E2020,
//   FormatTextForVWindow 0x00497F80, DisplayCenteredText 0x00497FD0, GetVStringExtents 0x00437210, WordWrap_SetCallback 0x00A0FD90,
//   GetAutoSaveDisplayName 0x00406C20, ConstructString 0x009C7980.
// Strings: "_PC", "_TITLE", "TEXT_GUI_SAVE_SAVING_EXTENDED_10", "TEXT_GUI_INVENTORY_BUTTON_TEXT_EXIT", L"A - ", "TEXT_GUI_MENU_INVALID_SAVE".
// Decoded so far (0x00498490 .. 0x00498972):
//   CDisplayManager* display = GFGetSystemManager()->[+0x60]; renderer = display->[+8];
//   if (FullScreen) {
//       long total = g_bUseCompiledWorldFiles ? 130 : 465;
//       if (!g_bUseCompiledGlobalThings) total += 347;
//       if (g_WADBuildEnabled) total += 1788;
//       if (g_bAutoSaveProgressShown) total = 300;
//       ++StageCount;
//       if (g_bAutoSaveProgressShown) StageCount = (long)(K0 - (g_AutoSaveProgressEndTime - GFGetTime()) * K1);   // doubles 0x0122ED68 / 0x01238880
//       long zero = 0;
//       float fraction = (float)(StageCount < 0 ? zero : StageCount > total ? total : StageCount) / total;   // retail selects a
//           // POINTER (&zero / &total / &StageCount) then `fild [ptr] ; fidiv total`: a std::min / std::max by reference
//       bool restarted = false;
//       if (useProgress) {
//           fraction = progress;
//           if (!(progress < 1.0f)) {
//               fraction = 1.0f;
//               if (!FileInstallerFinished) { FileInstallerFinished = true; ForFileInstaller = false; StageCount = 0; fraction = 0.0f; restarted = true; }
//           }
//       }
//       if (!g_bAutoSaveProgressShown && (StageCount == total / 2 || restarted)) {     // `total / 2` = cdq ; sub ; sar (signed)
//           if (++LoadingScreenIndex == LoadingScreenTextures.size())
//               LoadingScreenIndex = 0;
//       }
//       if (g_bAutoSaveProgressShown) {       // VC7.1 threads the not-taken path of the first test straight to the else arm
//           if (CPlayerGui global) { 0x00437570; six Draw2DScaledGraphic tiles of the GUI def's graphics [+0xA40][0..5]: 256 x 256 /
//               128 cells scaled by the render-target size (x / 640, y / 480) -- C2DBoxF temporaries built and copied per call }
//       } else {
//           Draw2DTexture(C2DBoxF(0, 0, g_screenWidth, g_screenHeight), CRGBColour(255,255,255,255), LoadingScreenTextures[LoadingScreenIndex], 0);
//       }
//       if (ProgressTextures.empty() && !g_bAutoSaveProgressShown) goto done_bar;   // 0x00498FDF
//       if (g_bSaveLoadFailed) goto after_bar;                                          // 0x00498FE3
//       ... progress bar: GetRenderTargetDimensions, GFAXToVX(width*0.5) - GFAXToVX(96), y = height + 380 - 480, rounded to pixels,
//           left cap / filled middle / right cap pieces (each either the GUI-def graphic when auto-saving or a ProgressTextures entry)
//   }
//   after_bar (0x00498FE3): BlackScreen -> Draw2DBox over the whole screen; ForFrontEnd -> guild seal frames (GuildSealFrameIndex
//   wraps at GuildSealTextures.size()); then the text: TextTag / LastTextDisplayed tests, GFGetTime into LastProgressUpdate,
//   DisplayText / DisplayResetProgressTipText choose the tip ("<tag>_PC", "<tag>_TITLE") or the save texts, word-wrapped into a
//   virtual window and drawn centred with the progress font.
