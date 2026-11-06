# Chunk 792 notes

Two rows matched (RELOCATION_MATCH + PASS in the read-only vland run), two parked as DIFFER with residue, two analysed only
(no graded draft: the budget went to the four rows above). Grades were taken with FABLE_HDR_OVERLAY=hdr.

## 0x00AB6E40 CJoystickDX::UpdateEvents -- RELOCATION_MATCH (src/00ab6e40.cpp)
- Shape: aligned ebp frame; `count = 1` is declared before the CInputEvent local (its store precedes the ctor call); Poll and
  GetDeviceData each retry once after DIERR_INPUTLOST; the loop re-fetches one DIDEVICEOBJECTDATA per pass.
- Two-level switch on dwOfs (`cmp 3Bh`): byte table 0x00AB780C, dword table 0x00AB77C4 with 18 entries. Case values:
  0 DIJOFS_X -> XAxis, 4 DIJOFS_Y -> YAxis, 8 DIJOFS_Z -> XAxis2, 0x14 DIJOFS_RZ -> YAxis2 (each `(float)(long)data*2/65536 - 1`),
  0x0C DIJOFS_RX and 0x10 DIJOFS_RY -> an explicit empty case (index 3), 0x30..0x3B DIJOFS_BUTTON0..11 -> buttons 1..12
  (indices 5..16), every other offset 0..0x3B -> default (index 17). Index 3 and 17 both point at the re-fetch 0x00AB76B9.
- Key spellings: the button arms only build the event (Pressed if dwData & 0x80 else Released) and set a flag; the one
  AddEventToStore + ProcessMaintainedEvents block sits after the switch under `if (addEvent)` (retail has a single shared
  block 0x00AB76A1; per-case calls tail-merge only partially). The RX/RY case stores `addEvent = false` explicitly: without a
  store VC7.1 folds it into the default and the table loses an entry (2564 vs 2568 bytes).
- After the loop: SetAsJoystickPosition(XAxis, YAxis, 0.0) + AddEventToStore, SetAsJoystickPosition2(XAxis2, YAxis2, 0.0) +
  AddEventToStore, UpdateMaintainedEventPositions, AddMaintainedEventsToEventStore. UpdateAcquired is expanded in place.
- Callees (proven: PDB spelling + retail evidence): CInputEvent() 0x00A04410, ~CInputEvent 0x00A03B30, EmptyEventStore 0x009E41D0,
  GFGetSystemManager 0x009A4EC0, IsApplicationActive 0x009A57B0, HasInputFocus 0x009A4FB0, SetHasInputFocus (vtable slot 16),
  SetAsJoystickButtonPressed 0x00A04090, SetAsJoystickButtonReleased 0x00A04110, SetAsJoystickPosition 0x00A04030,
  SetAsJoystickPosition2 0x00A04060, AddEventToStore 0x009E41E0, ProcessMaintainedEvents 0x009E4470,
  UpdateMaintainedEventPositions 0x009E42E0, AddMaintainedEventsToEventStore 0x009E4360. Four landed spellings are wrong
  (NAME_FIXES.tsv); the label of 0x009A4FB0 reads 'CSystemManager::IsApplicationInputFocus' (PDB: HasInputFocus).

## 0x00AB8E10 CStaticFontBank::CStaticFontBank(CFontDataBank*, unsigned long) -- RELOCATION_MATCH (src/00ab8e10.cpp, /GS)
- Shape: memset of GlyphBanks (rep stosd 0x800), CArray<unsigned char> entry buffer, CMemoryDataInputStream, Name =
  ReadNullTerminatedString, then inline reads FontHeight, Weight, Italics (ReadEBOOL), MaxHeight, TextureSize.X/Y, MinChar,
  MaxChar, the bank count (count-down loop), per bank index/FirstGlyph/NoGlyphs, `new CGlyph[n]` (operator new[] 0x00BFEB22:
  declare it or VC emits scalar new -> RELOC_IDENTITY) and CGlyph::Read per glyph; then the image size, an 18-byte
  TargaHeader, a second CArray, `pixels = image.GetAsCArray()` taken BEFORE `if (size - 18 > 0) Read`, two GFGetSystemManager
  calls (the first result is unused in retail too), CPixelFormat(8,8,8,8), GetBestCompressedAlphaTextureFormat(16, fmt),
  `new CTexture(TextureSize, 1, fmt, 0, SURFACE_POOL_MANAGED)` (inline), and
  `PTexture->GetSurfaceLevel(0).CopyFromMemory(&C2DBoxI(0,0,X,Y), image.GetAsCArray(), &sourceRect, sourceFormat, X*4, -1)`.
- The destination box has to be a temporary built in the call (address of a temporary, a VC7.1 extension): its stores then
  sink between the pushes like retail; a named box (any ctor, either order) leaves 30-47 difflines.
- New callee names: CGlyph::Read 0x00AB96B0 and CSurface::CopyFromMemory 0x009F4390 (the requested 0x009F4390), both Ego_r.pdb.

## 0x00B5A3F0 CEngineScreenEffectColourFilter::ApplyColourFilter -- DIFFER(2923v2916), 23 difflines (differ/00b5a3f0.cpp)
- ~40 graded spellings (ATTEMPTS.log). Matched: the float block (`C4DVector(Settings.Tint * contrast * 0.5f)` from the
  CRGBFloatColour chain gives retail's temporaries), the GFLimit clamps, capture block, shader enables, the setters.
- Residue: the third SetPixelShaderConstant's `UpdateFlags |= 2` (retail `or [eax+3D8h],ecx`), the CRenderStateManager bookmark
  test/or operand flip one block late (whole-function IR effect: removing the contrast clamp or the fourth constant gives
  retail's pattern), and the loop nop that follows from the size gap. Uses GFFloatToLongNear (retail_asm helper): C++ spellings
  tried for the rounding are in ATTEMPTS.log (Y-series); none emits the bare `fstp/fld/fistp`.

## 0x00B5AF80 CEngineScreenEffectColourFilter::RenderColorFilteredQuad -- DIFFER(2863v2865) (differ/00b5af80.cpp)
- The oracle row holds TWO functions: RenderColorFilteredQuad ends at 0x00B5BAB1 (2865 bytes), then 15 int3 and a 10-byte
  setter at 0x00B5BAC0 (`mov eax,[esp+4]; mov [ecx+0Ch],eax; ret 4`, no xrefs, no PDB name in the class's set). I graded
  against a cut oracle; the coordinator needs to split the row (finish.py would otherwise compare 2890 bytes).
- Residue: x87 keeps A*c instead of R*c (retail spills G/B/A), plus the same bookmark flip. ~22 spellings.

## 0x009A25D0 CStreamingFontBank::Render -- analysed only
- Callee names: 0x009A1140 = CStreamingFontBank::GlyphLookup (?GlyphLookup@CStreamingFontBank@@IBEPAUCStreamingGlyphLookup@@G@Z;
  bound today under the stand-in class CStreamingFontBankX), 0x009A23E0 = GFUpdateFontTextureCache
  (FableWin.pdb ?GFUpdateFontTextureCache@?A0xfe46a155@@YIXXZ, anonymous namespace, __fastcall, no args: called with no setup
  when byte 0x013C5A70 is set; it locks the cache texture 0x013C5A80 and copies dirty cells). Other callees: GetRenderTargetDimensions
  0x009BEDC0, SetVertexFormat 0x00A0AA30 (0x144 / 0x244), DrawAsIndexedTris 0x00A0AF40 x2, floor, __ftol2, wide-string accessors
  0x0099AF00 / 0x0099AF10, vtable slot 1 GetStringExtents (+4) and slot 4 Preload (+0x10).
- Plan: start from engine/CStreamingFontBank.h's retail branch; model the inline SetTexture cache (DeviceState +0x3D00 stage 0,
  +0x3D04 stage 1, +0x3CB8 the highest stage used) and the seven texture-stage setters with the shared CRenderStateManager
  header; expect the same bookmark test/or flip residue as the colour-filter rows.

## 0x00715A70 NTCScriptedControl::CActionTalkBase::SetUpCamera -- analysed only
- Code ends at 0x00716739 (`ret 4`, 3276 bytes); the 20 table entries are two tables: 7 at 0x0071673C (CameraZoomType 1..7,
  `dec eax; cmp eax,6`) and 13 at 0x00716758 (CameraView 0..12, `cmp eax,0Ch`; forced to 11 when byte +0x8D is set).
- Callees all bound: CThing::GetPos 0x004C73D0, GetCameraSetupPositions 0x00712340, CThingSearchTools::GetAllThingsInRoughArea
  0x007197C0, GetHeadPos 0x00661B40, NCameraModes::CScriptModuleBase() 0x005ED070, CScriptModuleMoveBetweenLookFromAndLookTo
  0x008525F0, CCountedPointer::Reset 0x0069C3D0, CGameCameraManager::SetGameScriptedCameraMode 0x00697B70, 0x00686A70 and
  0x00A01B10 (labels are placeholders: `_helper_prep`, MazePointerBase ctor).
- Header debt: no CActionTalkBase / CSubtitledMessage header; the Ego_r layout (CArray 16 bytes) must be re-offset to retail's
  12-byte vectors first (retail touches +0x6D, +0x74, +0x80, +0x8C, +0x8D).

## Header debt
- engine/CBankFile.h (HOLD): add ?GetEntryDataSize@CBankFile@@QBEKK@Z (0x009CC240) and ?ReadEntryData@CBankFile@@QAEXKKPAXK@Z
  (0x009CC2A0); row 0x00AB8E10 declares a local CBankFile with only those two.
- Landed 0x00AB6AD0 / 0x00AB6AE0 (GetXAxis / GetYAxis) report test FAIL under vland --recheck-landed with or without my overlay
  (their exes print PASS when run directly): pre-existing, not from this chunk.

## Descriptive names
None in the landed rows. The 10-byte setter at 0x00B5BAC0 has no name (no PDB match).
