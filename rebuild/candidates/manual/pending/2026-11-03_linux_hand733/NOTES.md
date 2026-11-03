# Chunk 733 -- CUserProfileManager: six large never-attempted rows

Result: 2 matched (RELOCATION_MATCH, vland PASS on the final full mirror), 4 parked with their best source in differ/.

| Row | Function | Result | Attempts | Residue / lever |
|---|---|---|---|---|
| 0x004075C0 | RenderUIBackground | RELOCATION_MATCH | 6 | function-local statics; sum order sets the frame slots |
| 0x0040BCA0 | LoadGameState (/GS) | RELOCATION_MATCH | 11 | do-while(0) inlines the read chain; same-TU GetConfigOptionsDefaultsDef |
| 0x0040A7F0 | ApplyVideoValues | DIFFER(1925v1941) | 13 | frame 0x7C v 0x78 + register colouring |
| 0x00409FD0 | SaveGameState | DIFFER(1892v1892) | 24 | one `[ecx+esi]` v `[esi+ecx]` operand in the inline CCRC::Calc |
| 0x0040C790 | SetDefaultValues | DIFFER(1284v1284) | 16 | block layout: retail sinks the non-null-def arm |
| 0x00409B70 | SetDefaultValuesForNewProfile | DIFFER(1089v1105) | 12 | same layout residue (+16 bytes = long jumps + alignment it causes) |

## Parked rows
- **0x0040A7F0 ApplyVideoValues** (best v5). Once C2DExtentsI has its retail user copy ctor (required for the in-slot
  EnumerateDisplayModes arguments) the frame is 0x78 against retail's 0x7C, so every slot is off by 4; retail keeps the def
  in ebx and the chosen refresh rate in the frame, ours def in ebp and the rate in ebx. Tried: menu pointer at function
  scope, refresh declaration order, reference instead of copy for the mode, const-reference dims, both TU bodies
  (GetConfigOptionsDefaultsDef, ConvertFloatToVideoOptionQuality). Next: find retail's extra 4-byte local first.
- **0x00409FD0 SaveGameState** (best s8). Everything relocation-equal except the second (text) CRC loop of the inline seeded
  CCRC::Calc: retail `movzx edi, byte ptr [ecx+esi]`, ours `[esi+ecx]`. The seeded Calc body must keep matching the landed
  out-of-line copy 0x004014A0.
- **0x0040C790 / 0x00409B70 SetDefaultValues / SetDefaultValuesForNewProfile** (best a8 / b1). Same instructions; retail
  lays the null-def arm as the fall-through, then the merge, and places the non-null arm after the ClampResolution walk-down
  loop's back-edge (`jmp` back to ClampAntialiasing). In 0x00409B70 that also makes the three ClampResolution exits long
  jumps and aligns the mode loop (`mov edi,edi`), hence the 16-byte size difference. Tried on one or both: inverted test,
  `def.Get()` pointer, per-argument ternaries (both polarities), hoisted fallback locals, ternary Antialiasing after the arms,
  do-while(0) around either arm, no same-TU ClampVideoOption body, /Ox, /G6, /G7. All keep source order. Hypothesis: the
  sunk arm is a block VC7.1 creates late (jump threading of repeated `def` tests, or a tail-merged copy); not reproduced.
  0x00409B70 got 10 own attempts (b1..b10) on top of the 0x0040C790 exploration.

## Header overlay (hdr/; hdrcheck2: default=same for every file, each change behind one FABLE_RETAIL_<CLASS>)
- fable_user_profile.h (CUSERPROFILEMANAGER): `enum EEngineVideoOptionQuality;`, GetPathNameFromFileName(const CWideString&,
  bool) const 0x00408120, private ConvertFloatToVideoOptionQuality(float) 0x00406820, plus the earlier retail rewrite.
- fable_game.h (CMAINGAMECOMPONENT): rebased on the repo's 14:56 file; adds PeekPlayerManager() const and GetDisplayEngine().
- engine/CDataInputStream.h: ReadString(unsigned long) 0x00996BE0.
- engine/CFileDataInputStream.h (new CFILEDATAINPUTSTREAM branch): ctor(CAFile&, unsigned long) 0x00994700, virtual dtor
  0x00994780, Close, File / Buffer / BufferSize, size 0x24.
- engine/CDisplayManager.h (CDISPLAYMANAGER): EnumerateDisplayModes 0x009C02D0, EnumerateAntiAliasingModes 0x009C05C0,
  GetRefreshRate; GetBitDepth wrapped in `#ifdef FABLE_RETAIL_CPIXELFORMAT` (sibling 0x00B26360 needs it).
- engine/CDisplayEngine.h (CDISPLAYENGINE): `CIEngine& Get3DEngine()`.
- engine/C2DExtentsF.h (C2DEXTENTSF): Initialise(float, float). engine/C2DExtentsI.h (C2DEXTENTSI): user copy ctor.
- engine/C4DVector.h (C4DVECTOR): const operator+ / operator-. engine/CRGBFloatColour.h (CRGBFLOATCOLOUR): operator*=(float).
- engine/CTCInGameMenu.h (new CTCINGAMEMENU branch, CTCBase-derived): StatGetInterfaceType, IsOpen, VideoOptionsChanged
  0x0047E651, VideoOptionsNotAccepted 0x0047F30F, InGameMenuOpen +0xC, size 0xF8.
- New: engine/CEngineVideoOptions.h (Ego_r layout 0x34, inline ctor), engine/NUISystem_CUIScreenUnitConverter.h (inline
  static ResetDimensions; public statics 0x013B8768/876C/8770 in the landed `@@2` spelling), engine/CCRC.h (seeded inline
  Calc + declarations), engine/CDisplayModeDesc.h, engine/CActionInputControl.h, stlport/fable_deque.h; earlier edits to
  CPersistContext.h, CDataOutputStream.h, CMemoryDataOutputStream.h, CPixelFormat.h.
- SIBLINGS.tsv: 51 landed rows reaching the changed headers, all MATCH/RELOCATION_MATCH + PASS after the rebase.

## Names
- Rows: all six spellings are Ego_r.pdb + FableWin.pdb publics (LABELS.tsv).
- NAME_FIXES.tsv: 28 retail addresses whose landed / bound spellings are wrong (template Transfer<T> bound as plain members,
  QAE/QBE and PAV/AAV slips, CManager::GetInstance bound as CFrontEndManager_00404c00, ApplyValues bound as SoundMutedChanged,
  GFGetSystemManager as AddChildPrimitive, GetRenderTargetDimensions as Device::GetDims, and the 0x01375080/84/88 caps).
- Descriptive, not PDB: the config-detection globals 0x01375070..0x013750A0 (g_DetailA..D, g_MaxDetailA..D, g_MemoryA/B,
  g_MaxAntialiasing / g_MaxResolutionWidth / g_MaxRefreshRate: the landed 0x004017B0 spellings; 0x00409090 / 0x00408900 /
  0x00406890 bind second spellings g_ConfigLevel / g_FableConfigWidth / g_maxDisplayRefreshRate, listed in NAME_FIXES);
  the RenderUIBackground statics noFilterColour / noFilterSaturation / noFilterContrast / noFilterBrightness; the `flag`
  parameter of GetPathNameFromFileName.
- Not fixable from the PDBs: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020 (Ego_r spelling is Dinkumware,
  retail STLport), vector<CWideString>::erase 0x00412450 (same), CWideString PBG v the PDB's PB_W (repo-wide convention).
- vland RELOC_NAME warnings on 0x0040BCA0 (catalog labels Dtor_CBaseBase_00994780, Transfer<CActionInputControl>,
  Transfer<ETutorialCategory>, operator_char_const*) are catalog spellings, covered by NAME_FIXES.

## Tests
test/<addr>.test.cpp for all six rows; each prints `<addr>_TEST PASS` under g++; the two matched ones also pass in vland.
