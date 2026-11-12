# DOSSIER 0x00A04630 -- CRenderStateManager::SetupRenderStates (4609 bytes)

## 1. Retail shape (xdis 00a04630 0x1211 / $W/dis_all.txt)
- `sub esp,134h ; push ebx ; push ebp ; push esi ; push edi ; mov esi,ecx` ... `pop edi ; pop esi ; pop ebp ; pop ebx ; add esp,134h ; ret`.
- __thiscall, `ret` (no args), NO frame pointer (ebp is a scratch register => /Oy, the default BASE_FLAGS /O2 /Oy). No SEH
  prologue, no /GS cookie, no locals with dtors. Speed-optimised code (/O2), not optimize("s").
- Frame 0x134 = two locals: [esp+10h] = loop counter `stage` (long, 4 bytes; spilled each iteration, reloaded at loop top
  0xA052AF) and [esp+14h] = `D3DCAPS9 caps` (0x130 bytes = sizeof(D3DCAPS9)). caps.RasterCaps (+0x24) = [esp+38h].
- Only ONE call: `PD3DDevice->GetDeviceCaps(&caps)` (vtable +0x1C = IDirect3DDevice9 slot 7; [esi+4] = PD3DDevice).
  Everything else is the inline expansion of three helpers (section 5). No jump tables.
- Constants held in registers across the whole body: eax = 0 (`xor eax,eax` at 0xA04654), edx = 1 (0xA0466E), esi = this.

## 2. Name / signature
- `?SetupRenderStates@CRenderStateManager@@AAEXXZ` = `private: void __thiscall CRenderStateManager::SetupRenderStates(void)`.
  Sources: FableWin.exe signatures (ghidra_out/naming_stragglers/FableWin.exe.signatures.tsv 0x030EF020, 2710b/618i debug body;
  _fablewin_api.tsv same), Ego_r.pdb (ghidra_out/egor_pdb_names.tsv 0x00B6F1B0, same mangled name), and the only caller
  CRenderStateManager::Init @ 0x00A07900 (call at 0x00A0794D; landed rebuild/src/compiled/00/a0/CRenderStateManager_Init_00a07900.cpp
  comments "SetupRenderStates @ 0x00A04630"). Manifest label is CORRECT. Private (AAE), already declared in
  rebuild/include/engine/CRenderStateManager.h line ~365 (`void SetupRenderStates();` under `private:`). Not virtual.

## 3. Callees
| site | target | name | landed row |
|---|---|---|---|
| 0x00A04647 | [[esi+4]]+0x1C | IDirect3DDevice9::GetDeviceCaps(_D3DCAPS9*) slot 7 | n/a (COM); declared in rebuild/include/engine/IDirect3DDevice9.h:52 `virtual long __stdcall GetDeviceCaps(_D3DCAPS9* pCaps) = 0;` |
No direct calls. Inlined (no call emitted): SetupRenderState / SetupTextureStageState / SetupSamplerStageState (section 5).

## 4. Members touched (engine/CRenderStateManager.h, FABLE_RETAIL_CRENDERSTATEMANAGER branch; Ego_r.pdb layout)
- +0x04 `IDirect3DDevice9* PD3DDevice` (read once).
- +0x2408 `CStateInfo* AllStatesList[256]` (write `AllStatesList[AllStatesListSize] = &info`).
- +0x2810 `long AllStatesListSize` (re-read + increment per helper; never cached: the stores alias).
- +0x2818 `CDeviceState DeviceState` (0x120C) -- every CStateInfo member except AntialiasingOn, SoftwareVertexProcessing,
  ShadowFunc, SolidOffsetEnable, StippleEnable, BlendColour, TextureCoordWrapMode is set up in order below; names in the header.
- +0x3A38 `unsigned long InitFlags` (`test byte ptr [esi+3A38h],2` at 0xA056BD).
- CStateInfo (0x14, nested, PDB): +0 CurrentState, +4 DesiredState, +8 BookmarkMask (NOT touched here), +0xC union
  {TextureStageStateParam / RenderStateParam / SamplerStageStateParam}, +0x10 bool DirtyListFlag, +0x11 uchar StateType
  (1 render, 2 texture-stage, 3 sampler -- matches landed ForceResendRenderStatesToHardware 0x00A044E0 switch), +0x12 uchar TextureStage.
- Static data: 0x01396DA8 = `CRenderStateManager::D3DTextureCoordWrapStageTable` (header: `static _D3DRENDERSTATETYPE
  D3DTextureCoordWrapStageTable[8];`, private). Evidence: retail .data at 0x01396DA8 = {0x80..0x87} = D3DRS_WRAP0..WRAP7, and it
  ends exactly where the next private table D3DTextureUVModeTable 0x01396DC8 (labels_codex_pass13) begins. Read as
  `mov edi,[1396DA8h+4k]` for k = 0..7.
- Opt-in macros: only `FABLE_RETAIL_CRENDERSTATEMANAGER` is needed (gives CStateInfo, CDeviceState, the lists, InitFlags, the static
  table and the private Setup* declarations). None of the sub-opt-ins (EARLY_RETURN, OUTLINE_INLINE, STENCIL_INLINE, COLOURWRITE4,
  DEFAULT_TRANSFORM) is relevant -- this function calls no Set* setter.
- Locally-modelled classes in the landed SIBLINGS that HAVE a shared header (do not copy): CStateInfo / CRenderStateManager_Methods
  (Init 0x00A07900, ForceResend 0x00A044E0), local IDirect3DDevice9 vtable (ForceResend), CRenderStateManagerRealiseView
  (fable_render_state.h, RealiseRenderState 0x00A058C0). Use engine/CRenderStateManager.h + engine/IDirect3DDevice9.h +
  engine/Direct3D9Types.h (D3DCAPS9 with RasterCaps; it has no _D3DRENDERSTATETYPE / TSS / SAMP enumerators and no
  D3DPRASTERCAPS_* macros -> spell values as SDK-named local constants or casts; precedent: 0x00B8D420 declares
  `enum _D3DSAMPLERSTATETYPE{};` locally).
- D3DCAPS9 RasterCaps bits tested: 0x04000000 D3DPRASTERCAPS_DEPTHBIAS (0xA05237), 0x02000000 D3DPRASTERCAPS_SLOPESCALEDEPTHBIAS
  (0xA05270), 0x00002000 D3DPRASTERCAPS_MIPMAPLODBIAS (0xA05426, inside the stage loop).

## 5. Callee-body visibility -- the key fact
Retail has OUT-OF-LINE copies of the three helpers immediately before this function, in source order (same TU
CRenderStateManager.cpp), 16-byte aligned, zero callers (xref.py), not in the function manifest (manifest gap after
ForceResend 0x00A044E0, rebuild/manifest/manifest-gaps.tsv row `00a044e0 ... 00a04580 166`):
- 0x00A04580 `?SetupRenderState@CRenderStateManager@@AAEXAAUCStateInfo@1@W4_D3DRENDERSTATETYPE@@@Z` (ret 8, 45 bytes)
- 0x00A045B0 `?SetupTextureStageState@CRenderStateManager@@AAEXAAUCStateInfo@1@W4_D3DTEXTURESTAGESTATETYPE@@J@Z` (ret 0Ch)
- 0x00A045F0 `?SetupSamplerStageState@CRenderStateManager@@AAEXAAUCStateInfo@1@W4_D3DSAMPLERSTATETYPE@@J@Z` (ret 0Ch)
Names: FableWin.exe.signatures.tsv 0x030EE580 / 0x030EE900 / 0x030EEC90 (debug bodies 712/721/721 b; the +9 b / +3 i of the stage
variants = one `TextureStage = stage` store) and the header's private declarations. Ego_r.pdb lists none (egor_pdb_names grep).
So: they are ordinary (non-inline) member functions DEFINED IN THE ROW TU BEFORE SetupRenderStates and auto-inlined by /O2 (/Ob2).
Out-of-line bodies (exact store order to reproduce):
```
SetupRenderState(info,state):  mov eax,[esp+4]; xor edx,edx; [eax]=edx; [eax+4]=edx; byte[eax+10h]=dl; edx=state;
                               byte[eax+11h]=1; [eax+0Ch]=edx; edx=[ecx+2810h]; [ecx+edx*4+2408h]=eax; inc [ecx+2810h]; ret 8
SetupTextureStageState(info,type,stage): ... [eax+0Ch]=type; dl=stage; byte[eax+11h]=2; byte[eax+12h]=dl; list; ret 0Ch
SetupSamplerStageState: same with StateType 3.
```
Suggested text (verify by compiling and comparing the out-of-line copies to 0x00A04580/5B0/5F0 -- a free oracle):
```
void CRenderStateManager::SetupRenderState(CStateInfo& info, _D3DRENDERSTATETYPE state)
{ info.CurrentState = 0; info.DesiredState = 0; info.DirtyListFlag = false; info.StateType = 1;
  info.RenderStateParam = state; AllStatesList[AllStatesListSize++] = &info; }
// Texture/Sampler: same, StateType 2/3, TextureStageStateParam/SamplerStageStateParam = type, TextureStage = (unsigned char)stage.
```
No STLport, no CIntelligentPointer, no strings, no vector maths.

## 6. Siblings (same TU CRenderStateManager.cpp, retail 0x00A044B0..0x00A079B0)
- 0x00A044B0 ~CRenderStateManager, 0x00A044E0 ForceResendRenderStatesToHardware, 0x00A05840 RestoreCaptureBlock, 0x00A058C0
  RealiseRenderState, 0x00A05970 ctor, 0x00A07900 Init, 0x00A07980 scalar deleting dtor -- all in rebuild/src/compiled/00/a0/,
  all landed with default flags (no `// Flags:` override, no pragmas), but with LOCAL models. 0x00A059E0 SetDefaultRenderStates and
  0x00A079B0 CDeviceState ctor not landed.
- Shared-header idiom to copy: rebuild/src/compiled/00/9d/CRenderManager2D_Render2DDrawList_009da9f0.cpp
  (`#define FABLE_RETAIL_CRENDERSTATEMANAGER` before includes; includes engine/IDirect3DDevice9.h, engine/Direct3D9Types.h).
- Out-of-class member definitions in a row: Init 0x00A07900 defines a same-TU stand-in under `#pragma auto_inline(off)`; here the
  opposite is needed (auto-inline ON, helpers defined before the row).

## 7. Risks for the author
- Order of the 88 helper calls must follow retail exactly (OUTLINE.md); it is NOT DeviceState declaration order.
- Per-block register/addressing variation (esi+disp vs lea ecx first; `inc [mem]` vs load/inc/store into ebp/ebx/edi) is
  allocator scheduling, expected to fall out of the same source. Block 1 stores the state id (+0xC) before `xor eax,eax`.
- Stage loop: `for (long stage = 0; stage < 8; ++stage)`; pointer strength-reduced (ecx = &DeviceState.TextureMipMapMode[0] + 0x10,
  += 0x14), first entry through `jmp 0xA052B3` (skip reload of ebx).
- Two InitFlags/caps conditions are inside the loop body (MIPMAPLODBIAS caps test re-reads [esp+38h] every iteration; InitFlags & 2).
