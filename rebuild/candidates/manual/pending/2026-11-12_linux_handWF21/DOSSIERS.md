## 0x004093E0

# DOSSIER 0x004093E0 CUserProfileManager::SetDefaultValuesVideo (841 bytes)

## 1. Retail shape (xdis 004093e0 0x359)
- __thiscall, plain `ret` (no args), no frame pointer (`sub esp,10h; push esi; push edi`), no SEH/EH frame even
  though a CDefPointer local is released at the end -> TU built without /EHsc (same as sibling GetMax*Detail 0x004084B0).
- Frame 0x10: frame+0 = colour-depth local (stored [esp+10h] after push ebx/ebp), frame+4 = the CDefPointer result slot
  (GetConfigOptionsDefaultsDef hidden ret; reloaded at the end for the release `dec [ecx+4]; jne; call [eax+4]`),
  frame+8..0x10 = ONE C2DExtentsI temp shared by both GetRenderTargetDimensions calls.
- esi=this, edi=raw def pointer (loaded once at 0x4093FE, kept all function), ebx=width, ebp=height (`or ebx,-1; or ebp,-1`
  AFTER the ClampAntialiasing call, before GFGetSystemManager), ebx/ebp pushed only at 0x4094B3 (shrink-wrapped).
- Order: Gamma=1.0f stored BEFORE the GetConfigOptionsDefaultsDef call. `fild [0137507C]` (first call's limit arg) is
  hoisted above `test edi,edi`. Null-def arm is fall-through (0x40940A); def arm is OUT OF LINE at 0x4095D4 and jumps back
  to the shared tail 0x4094AD (both arms end `lea eax,[esi+38h]; mov [eax],X` -- null arm stores edi(=0)).
- ClampAntialiasing called with `&Antialiasing` computed as `lea eax,[esi+38h]` reused from the store.
- Display-mode search 0x409543: `Resolution=0` (xor edx), guard `begin==end -> skip`, rotated loop, `inc dword [esi+48h]`
  in memory, end reloaded from [esi+84h] each iteration; compares X, Y, BitsPerPixel(depth local in ecx).
- 0x40957E..0x4095D2 + 0x409677 = INLINED ClampResolution(Resolution, g_MaxResolutionWidth) (size via sar 4, unsigned jb;
  unrotated step-down loop `cmp [eax],edi; jle; dec ecx; sub eax,10h; jmp`; store back only when moved).
- 0x40967A..0x4096C8 refresh pick: RefreshRate=0 in memory, i in ecx 1..3 (`jb` unsigned), reads [esi+4Ch] each pass,
  abs via cdq/xor/sub (intrinsic abs), `if (abs(r-t[i]) < abs(r-t[RefreshRate]) && t[i] <= r) RefreshRate=i`.
  NOT peeled (ours peeled it).
- 0x4096CA..0x4096FC = INLINED ClampRefreshRate(RefreshRate, g_MaxRefreshRate) (identical bytes to the unreferenced
  out-of-line copy 0x004069A0, 52 bytes, manifest-gaps tail of row 0x004068D0).
- Tail: ScreenAspectRatio(+74h)=0, VerticalSyncEnabled(+50h)=1, ApplyValues(), then CDefPointer release.

## 2. Identity (label is RIGHT)
- Ego_r.pdb (egor_pdb_names.tsv / egor_signatures.tsv 0x004073D0) and FableWin.pdb (_fablewin_api.tsv 0x01881E20):
  `?SetDefaultValuesVideo@CUserProfileManager@@QAEXXZ` = public: void __thiscall CUserProfileManager::SetDefaultValuesVideo(void).
- Caller: 0x004211D7 (inside 0x0041E6D3 NUISystem::CManager::Action per functions.tsv).
- Same TU (Ego_r address order, all before 0x004073D0): ApplyValues 0x405900 (IAE protected), ClampVideoOption 0x405520,
  ClampRefreshRate 0x4055E0, ClampResolution 0x405DA0, GetConfigOptionsDefaultsDef 0x406C90, ClampAntialiasing 0x407340,
  GetDisplayModes 0x4054D0.

## 3. Callees
| retail | landed row | PDB mangled | draft spelling issue |
|---|---|---|---|
| 0x004083E0 | 00/40/CUserProfileManager_GetConfigOptionsDefaultsDef_004083e0.cpp | ?GetConfigOptionsDefaultsDef@CUserProfileManager@@QBE?AV?$CDefPointer@$$CBVCConfigOptionsDefaultsDef@@@@XZ | draft returns a local `CDefPointer` struct (non-template) -> wrong; use engine/CDefPointer.h `CDefPointer<const CConfigOptionsDefaultsDef>` |
| 0x004068D0 x4 (x8 sites) | 00/40/CUserProfileManager_ClampVideoOption_004068d0.cpp | ?ClampVideoOption@CUserProfileManager@@AAEXAAMMMMM@Z = private void(float&,float,float,float,float) | draft (and landed row) use `float*` -> PDB says `float&`, private |
| 0x00409140 | 00/40/CUserProfileManager_ClampAntialiasing_00409140.cpp | ?ClampAntialiasing@CUserProfileManager@@AAEXAAKJ@Z = private void(unsigned long&, long) | draft/landed use `unsigned long*` -> `unsigned long&` |
| 0x00407370 | 00/40/CUserProfileManager_ApplyValues_00407370.cpp | ?ApplyValues@CUserProfileManager@@IAEXXZ (protected) | ok |
| 0x009A4EC0 x3 | 00/9a/global_GFGetSystemManager_009a4ec0.cpp | ?GFGetSystemManager@@YIPAVCSystemManager@@XZ (__fastcall) | ok |
| 0x009E3820 | 00/9e/CPixelFormat_GetColourDepth_009e3820.cpp | ?GetColourDepth@CPixelFormat@@QBEKXZ -> unsigned long, const | draft returns `long` -> `unsigned long` |
| 0x009BEDC0 x2 | 00/9b/CDisplayManager_GetRenderTargetDimensions_009bedc0.cpp | ?GetRenderTargetDimensions@CDisplayManager@@QAE?BVC2DExtentsI@@XZ -> `const C2DExtentsI` BY VALUE (hidden ret) | draft spells it as `const C2DExtentsI& (C2DExtentsI& out)` -> wrong; engine/CDisplayManager.h line 63 declares the right one |
| inline 0x004069A0 body | NOT landed (unreferenced COMDAT-like copy, manifest-gaps) | ?ClampRefreshRate@CUserProfileManager@@AAEXAAKJ@Z | draft hand-wrote it |
| inline 0x00406D90 body | 00/40/CUserProfileManager_ClampResolution_00406d90.cpp | ?ClampResolution@CUserProfileManager@@AAEXAAKJ@Z | draft hand-wrote it; landed row uses `unsigned long*` |
| def vtable slot 1 | CDefPointeeBase::Delete (engine/CDefPointer.h Release) | | draft local CDefBase model |

## 4. Members
CUserProfileManager (struct_layouts_egor.tsv; retail = PDB before +0x54, PDB-8 after the two input vectors):
Gamma float +0x1C, Antialiasing ulong +0x38, TextureDetail float +0x3C, ShadowDetail +0x40, MeshDetail +0x44,
Resolution ulong +0x48, RefreshRate ulong +0x4C, VerticalSyncEnabled bool +0x50, EffectsDetail float +0x70 (PDB 0x78),
ScreenAspectRatio ulong +0x74 (PDB 0x7C), DisplayModes vector<CDisplayModeDesc> +0x80/+0x84 (PDB 0x88).
- rebuild/include/fable_user_profile.h with FABLE_RETAIL_CUSERPROFILEMANAGER (used by GetMax*Detail rows) names
  Gamma..StereoReversed (+0x1C..+0x51) but +0x52..+0xCB is OPAQUE (`_pad_0x52`) -> EffectsDetail, ScreenAspectRatio,
  DisplayModes are not named anywhere shared; author must add an opt-in (or the engine/CUserProfileManager.h blocks).
- engine/CUserProfileManager.h has FABLE_RETAIL_CUSERPROFILEMANAGER_DISPLAYMODES (std::vector<CDisplayModeDesc>
  + GetDisplayModes 0x00406880) but its other layout blocks each carry own pads ("not combinable"); no video block.
- CConfigOptionsDefaultsDef (retail = PDB-4): Antialiasing +0x3C, ResolutionWidth +0x40, ResolutionHeight +0x44,
  BitDepth +0x48, TextureDetail +0x54, MaxTextureDetail +0x58, ShadowDetail +0x5C, MaxShadowDetail +0x60, MeshDetail +0x64,
  MaxMeshDetail +0x68, EffectsDetail +0x6C, MaxEffectsDetail +0x70. Only header: engine/_quarantine/CConfigOptionsDefaultsDef.h;
  GetMaxEffectsDetail_004084b0.cpp restates it as `: public CParentDefClassBase` -- copy that.
- CSystemManager.h (FABLE_RETAIL_CSYSTEMMANAGER): PDisplayManager CCountedPointer +0x60, inline GetDisplayManager().
- CDisplayManager.h (FABLE_RETAIL_CDISPLAYMANAGER): +0x1A8 = BackBufferFormat (CPixelFormat) -- draft calls it "PixelFormat";
  +0x1C8 = RefreshRate (long). FableWin GetRefreshRate is the D3D query (0x009BE340), NOT this member read.
- CDisplayModeDesc.h: Dimensions(C2DExtentsI X,Y) +0, BitsPerPixel +8, RefreshRate +0xC.
Locally modelled in draft though shared headers exist: CUserProfileManager, CConfigOptionsDefaultsDef (quarantine),
CDefPointer, CDisplayManager, CPixelFormat, CSystemManager, C2DExtentsI, CDisplayModeDesc.
Globals (established spellings): 0x0137507C g_MaxDetailA (effects cap), 0x01375074 g_MaxDetailD (texture cap),
0x01375078 g_MaxDetailB (shadow cap), 0x01375070 g_MaxDetailC (mesh cap) [5 rows each]; 0x01375098 g_DetailA, 0x01375090
g_DetailD, 0x01375094 g_DetailB, 0x0137508C g_DetailC, 0x0137509C g_MemoryA, 0x013750A0 g_MemoryB (GFConfigDetection
0x004017B0 only); 0x01375080 g_MaxAntialiasing / g_ConfigLevel, 0x01375084 g_MaxResolutionWidth / g_FableConfigWidth,
0x01375088 g_MaxRefreshRate / g_maxDisplayRefreshRate (conflicting spellings -- pick GFConfigDetection's); 0x0122DE30
g_refreshRateOptions[4] (GetMaxRefreshRate 0x00406890). Draft names g_CmdLine* are made up and inconsistent; draft also
SWAPS texture/mesh caps in names (0x01375070 is the MESH cap, 0x01375074 the TEXTURE cap).
Arg mapping (verified in disasm): Effects(+70): value g_DetailA, limit g_MaxDetailA, def EffectsDetail/MaxEffectsDetail;
Texture(+3C): g_DetailD / g_MaxDetailD / def +54/+58; Mesh(+44): g_DetailC / g_MaxDetailC / +64/+68;
Shadow(+40): g_DetailB / g_MaxDetailB / +5C/+60. width=g_MemoryA, height=g_MemoryB (if >0).

## 5. Callee-body visibility (GOTCHAS Q22 / eb / wf11)
- /O2 => /Ob2: ClampResolution (84 B, 1 out-of-line caller 0x40C3B5) and ClampRefreshRate (52 B, zero callers) are
  AUTO-INLINED here -> define their real bodies (non-inline, landed ClampResolution recipe: `*(begin()+n)` and the
  __forceinline early-return IsModeWiderThan helper giving the unrotated loop) in the TU BEFORE SetDefaultValuesVideo.
- ClampVideoOption (202 B) and ClampAntialiasing (126 B) stay calls; putting their real bodies in the TU under
  `#pragma auto_inline(off)` (or __declspec(noinline)) may tell c2 they don't capture `&member` (Q32 lesson).
- GetConfigOptionsDefaultsDef: GetMaxEffectsDetail_004084b0.cpp's documented lever -- define its real body (and the
  GetDef<T> template) in the TU; with only a declaration VC gave the def an extra frame word (84v67).
- CDefPointer members are __forceinline (engine/CDefPointer.h; default Release gives `dec [ecx+4]; jne`, matches).
- No STLport helper calls; vector reads are inline (begin/end/size).

## 6. Siblings
- 00/40/CUserProfileManager_GetMaxEffectsDetail_004084b0.cpp (4 rows in one file): defines FABLE_RETAIL_CCHARSTRING,
  DEF_CLASS_BASE, CGAMECOMPONENT, CMAINGAMECOMPONENT, SCOPED_PTR, CRESOURCELIST, CDEFINITIONMANAGER, CUSERPROFILEMANAGER,
  CSTARTUPDEFINITIONMANAGER; includes fable_string.h, fable_game.h, fable_user_profile.h, engine/CParentDefClassBase.h,
  engine/CDefPointer.h, CGameDefinitionManager.h, CStartupDefinitionManager.h; GetConfigOptionsDefaultsDef body in TU.
- 00/40/CUserProfileManager_ClampResolution_00406d90.cpp: fable_compiler.h, local CRetailVector, FABLE_FORCEINLINE IsModeWiderThan.
- 00/40/CUserProfileManager_ClampAntialiasing_00409140.cpp: same unrotated-loop trick (IsAbove), engine/CCountedPointer.h.
- 00/40/CUserProfileManager_ClampVideoOption_004068d0.cpp: FABLE_FORCEINLINE ClampToLimit.
- 00/40/CUserProfileManager_SetDefaultValuesAudio_004083b0.cpp (engine/CUserProfileManager.h AUDIODEFAULTS block).
- Flags in this class: mostly none; /GS only on string rows (LoadFileList etc.). This row: no /GS cookie, no EH.

## 7. Old draft model errors
1. Hand-written resolution/refresh clamp loops instead of the same-TU ClampResolution/ClampRefreshRate bodies (retail
   inlines them, incl. the unrotated loop shape) -> "clamp loops keep index in register" residue.
2. Refresh selection loop peeled in ours; retail keeps one rotated loop with i=1..3, RefreshRate in memory.
3. GetRenderTargetDimensions modeled as ref-out; it returns `const C2DExtentsI` by value (one shared temp slot).
4. Reference params spelled as pointers (ClampVideoOption/ClampAntialiasing/ClampResolution/ClampRefreshRate are `&`).
5. Local non-template CDefPointer/CDefBase models; GetConfigOptionsDefaultsDef only declared (frame-slot lever lost).
6. CDisplayManager member misnamed (PixelFormat -> BackBufferFormat); GetColourDepth return should be unsigned long;
   depth local likely `unsigned long`/long - retail keeps it in a stack slot (frame+0).
7. Block placement (def arm out of line, `fild` of the first limit hoisted) still unexplained; width/height should be
   ebx/ebp initialised -1 just before the GetColourDepth call (draft order already matches; residue likely from 1-5).

