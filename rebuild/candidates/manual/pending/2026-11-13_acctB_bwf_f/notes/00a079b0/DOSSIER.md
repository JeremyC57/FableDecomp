# DOSSIER 0x00A079B0 (4950 bytes) -- identity scout

## 0. Verdict: the manifest label `CPSCOrbit::CPSCOrbit` is WRONG
Real function: `CRenderStateManager::CDeviceState::CDeviceState()`
Mangled leaf: `??0CDeviceState@CRenderStateManager@@QAE@XZ` (public, __thiscall, no params, returns this).
Evidence (independent sources):
- Caller: the only xref (`xref.py`) is 0x00A05984 inside CRenderStateManager::CRenderStateManager 0x00A05970
  (landed: rebuild/src/compiled/00/a0/CRenderStateManager_CRenderStateManager_00a05970.cpp), which does
  `lea ecx,[esi+0x2818] ; call 0x00A079B0`. Ego_r.pdb layout: CRenderStateManager +10264 (0x2818) = `CDeviceState DeviceState`.
  The landed caller's header comment already names the callee `CDeviceState::CDeviceState @ 0x00A079B0`.
- Size: struct_layouts_egor.tsv `CDeviceState` sizeof 4620 = 0x120C = 231 x CStateInfo(0x14). The function writes exactly
  231 contiguous 0x14-byte elements, offsets 0x0000..0x1209 (sim script $W/sim.py: 231 elements, none missing, every element
  writes +0,+4,+8 dword and +0x10,+0x11 byte, in that order). 55 scalar CStateInfo members + 22 arrays x 8 = 231.
- PDB symbols: Ego_r.pdb (egor_pdb_names.tsv / egor_signatures.tsv) `??0CDeviceState@CRenderStateManager@@QAE@XZ` @0x00B722A0
  (Ego_r build, 5701 B); FableWin.pdb (fablewin_pdb_names.tsv / naming_stragglers FableWin signatures) same leaf @0x030EC9A0
  (debug, 1280 B / 326 insns; @0x017DC6C0 is its 5-byte ILT thunk). Shared header already declares it:
  rebuild/include/engine/CRenderStateManager.h:54 `CDeviceState();  // ??0CDeviceState@CRenderStateManager@@QAE@XZ` (retail branch).
- The real CPSCOrbit ctor is `??0CPSCOrbit@NParticleEngine@@QAE@XZ` @0x00AD7F00 (Ego_r.pdb); CPSCOrbit is 372 bytes -- cannot be this.
- Not virtual, no vtable slot (CDeviceState has no RTTI/vtable; rtti-vtables.tsv has nothing for it).
- NAME_FIXES: manifest 00a079b0 CPSCOrbit/CPSCOrbit -> CRenderStateManager / CDeviceState (`CRenderStateManager::CDeviceState::CDeviceState`).
  Landed file name should be like `CRenderStateManager_CDeviceState_00a079b0.cpp` (coordinator's call).

## 1. Retail shape
- `mov eax,ecx ; xor edx,edx ; <1155 zero stores> ; ret` (c3, no imm) -- __thiscall, no args, returns `this` in eax.
- NO frame pointer, NO push of callee-saved regs, NO SEH (CStateInfo has no dtor -> no EH states), no /GS cookie, no calls.
- Zero source is edx/dl only (one zero register). eax = this throughout; ecx = walking pointer inside the arrays.
- 55 scalar members: 5 stores each through [eax+off] in member order (0x00A079B4..0x00A07FDD).
- 22 arrays of 8: VC7.1 fully unrolled the per-array element-ctor loop (trip count 8): either element 0 is peeled through
  [eax+off] and then `lea ecx,[eax+arraybase]` walks elements 1..7 with `add ecx,0x14`, or `lea ecx` comes first; stores use
  both positive (+0..+0x11 / +0x14..+0x25) and negative (-0x14..-3) displacements around the `add ecx,0x14`; the final
  induction `add ecx,0x14` of a group is often dead (followed straight by the next `lea ecx`), e.g. 0x00A08101, 0x00A081BD,
  0x00A08241 ... 0x00A08C74 (17 sites). These are loop-unroller/scheduler artefacts: the source is just the member arrays.
- Size-opt idioms: none (it is /O2-style unrolled; nothing like optimize("s")).
- Element and field order is strictly ascending (checked: $W/order.py) = declaration order of CDeviceState members and
  CStateInfo ctor init order CurrentState, DesiredState, BookmarkMask, DirtyListFlag, StateType. Never touched: +0x0C
  (union D3D state id) and +0x12 TextureStage.

## 2. Signature
`CRenderStateManager::CDeviceState::CDeviceState()` -- no params, no const, ctor. Class is nested `class CDeviceState`
(public) in CRenderStateManager (Ego_r.pdb; header line 52).

## 3. Callees
None (no call/jmp). The only "callee" is the INLINE ctor `CRenderStateManager::CStateInfo::CStateInfo()`
(`??0CStateInfo@CRenderStateManager@@QAE@XZ`): Ego_r.pdb has NO out-of-line copy (inline in release); FableWin.pdb has the debug
body @0x030EC910 (57 B / 18 insns = prologue + 5 zero stores) and an ILT thunk @0x018313AF. Its matched release expansion is in
the landed sibling 0x00A05970 (BookmarkDummyState, +0x3A24):
    CStateInfo() : CurrentState(0), DesiredState(0), BookmarkMask(0), DirtyListFlag(false), StateType(0) {}
(rebuild/src/compiled/00/a0/CRenderStateManager_CRenderStateManager_00a05970.cpp:18). Same five stores, same order, as here.

## 4. Members (all via CDeviceState, shared header rebuild/include/engine/CRenderStateManager.h, retail branch
`#define FABLE_RETAIL_CRENDERSTATEMANAGER`, lines 34-131)
- CStateInfo (0x14, Ego_r.pdb): +0 CurrentState ulong, +4 DesiredState ulong, +8 BookmarkMask ulong, +0xC union
  (TextureStageStateParam/RenderStateParam/SamplerStageStateParam; CStateInfo_u_12), +0x10 DirtyListFlag bool,
  +0x11 StateType uchar, +0x12 TextureStage uchar. Header declares `CStateInfo();` (line 35) but DEFINES NO BODY anywhere
  (grep: no `CStateInfo::CStateInfo` in rebuild/include or rebuild/src except the local model in 0x00A05970).
  => the author must provide an INLINE body visible in the TU (see 5). Without it VC7.1 calls the ctor / `vector constructor
  iterator` instead of expanding.
- CDeviceState (0x120C): 55 scalar CStateInfo members PolyShadeMode(+0)..BlendColour(+0x438), then 22 `CStateInfo X[8]`
  TextureCoordWrapMode(+0x44C)..TextureBumpEnvMat11(+0x116C). Full list with first-store retail address: $W/membermap.txt.
  Header already has all names/offsets matching struct_layouts_egor.tsv exactly.
- Header opt-ins present: FABLE_RETAIL_CRENDERSTATEMANAGER (whole retail model), _DEFAULT_TRANSFORM, _COLOURWRITE4,
  _OUTLINE_INLINE, _STENCIL_INLINE, _EARLY_RETURN -- none concern the ctors. No opt-in exists for a CStateInfo ctor body.
- Default (non-retail) branch of the header is the generated flattened layout: DeviceState is `unsigned char[0x120c]`, no
  nested types -> unusable here; use the retail branch.
- Local models of these classes that have a shared header: the sibling 0x00A05970 and 0x00A07900 rows model CStateInfo /
  CDeviceState / CRenderStateManager locally (author should NOT copy that; use the header retail branch).

## 5. Callee-body visibility / TU
- Retail TU = CRenderStateManager.cpp: neighbours 0x00A044B0 ~CRenderStateManager, 0x00A044E0 ForceResend..., 0x00A05840
  RestoreCaptureBlock, 0x00A058C0 RealiseRenderState, 0x00A05970 ctor, 0x00A059E0 SetDefaultRenderStates, 0x00A07900 Init,
  0x00A07980 scalar deleting dtor (COMDAT), then 0x00A079B0 this ctor (placed after the COMDAT => likely emitted as a COMDAT /
  compiler-generated-or-inline ctor; bytes are the same either way). Next function (CMeshBankManager) at 0x00A09D80 is another TU.
- CStateInfo::CStateInfo was visible and inline (Ego_r.pdb has no copy; 0x00A05970 expands it inline in the same TU).
- No STL, no CIntelligentPointer, no strings, no maths.
- Recommended spelling (row source): `#define FABLE_RETAIL_CRENDERSTATEMANAGER` + `#include "engine/CRenderStateManager.h"`, then
    inline CRenderStateManager::CStateInfo::CStateInfo()
        : CurrentState(0), DesiredState(0), BookmarkMask(0), DirtyListFlag(false), StateType(0) {}
    CRenderStateManager::CDeviceState::CDeviceState() {}
  (an `inline` definition after the non-inline in-class declaration is legal C++). If the coordinator prefers it in the header,
  propose an overlay opt-in e.g. `FABLE_RETAIL_CRENDERSTATEMANAGER_STATEINFO_CTOR` defining that inline body at namespace scope
  after the class. Check with fwsize: FableWin's debug CStateInfo ctor is 57 bytes / 18 insns, CDeviceState ctor 1280 B / 326 insns
  (`python3 /opt/fable_lane/tools/fwsize.py <src.cpp> 'CStateInfo|CDeviceState'`, FW_EXTRA=/EHsc).
- The retail-branch header pulls engine/CBaseClassNonCopyable.h and engine/RenderStateEnums.h and declares the static D3D tables;
  only declarations, nothing emitted. If the inline setters' FABLE_FORCEINLINE bodies cause unused-code noise they are not emitted.

## 6. Sibling landed rows (same class / TU) -- rebuild/src/compiled/00/a0/
- CRenderStateManager_CRenderStateManager_00a05970.cpp: local model, no pragmas/flags; inline CStateInfo ctor (member-init list) -- the
  proven expansion.
- CRenderStateManager_Init_00a07900.cpp: `#include "engine/CRenderStateManager.h"` (default branch) + local CStateInfo;
  `#pragma auto_inline(off/on)` around a same-TU noinline stand-in.
- CRenderStateManager_ForceResendRenderStatesToHardware_00a044e0.cpp: includes engine/CRenderStateManager.h.
- CRenderStateManager_RealiseRenderState_00a058c0.cpp (fable_render_state.h), _RestoreCaptureBlock_00a05840.cpp
  (fable_render_capture.h), _scalardeletingdestructor_00a07980.cpp (fable_compiler.h, local model).
- Retail-branch users: 00/9d/CRenderManager2D_Render2DDrawList_009da9f0.cpp and the 00/b5/CEngineStateBlock*_Apply rows
  `#define FABLE_RETAIL_CRENDERSTATEMANAGER` before including the header.
- None of the siblings uses `// Flags:` lines; no /Oy- or optimize pragmas needed (no frame in retail).

## 7. Risk notes for the author
- The only real risk is the unroll / peel / dead-`add` pattern per array. It is compiler output from the plain member arrays; do
  not hand-unroll or write a loop. If VC7.1 emits `call ??_H` (vector constructor iterator) instead, the CStateInfo ctor body is
  not visible/inline. If arrays come out rolled, check it is a plain user ctor with empty body (or implicit) and /O2 like siblings.
