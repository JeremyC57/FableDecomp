# Chunk 774 notes

Toolchain: VC7.1 RTM cl 13.10.3077 under Wine, full mirror (repo headers plus the `hdr/` overlay). All 13 tests (7 `test/`, 6 `differ/`) build and pass under g++ and under VC7.1 /Od. No `__asm`, `naked`, `_emit` or `volatile` appears in any row source or test.

Read-only vland (`vland_result.txt`, run with `--recheck-landed` so the landed bake 0x0042DF9E is graded too): **7 wins, 0 rejects.** The first run without that flag (`vland_result_norecheck.txt`) silently skips 0x0042DF9E, because a bake is already landed at that address.

## Results

| address | name | grade | attempts | flags | note |
|---|---|---|---|---|---|
| 0x00F21590 | NScript::CQ_ArenaScript::GetEndRoundQuestion | RELOCATION_MATCH | 2 | none | `TRet<CCharString>` `_Fail_Val` shape (new engine/TRet.h) |
| 0x00E80060 | NScript::CV_FishingCompetitionScript::GetHeroTitleTextTag | RELOCATION_MATCH | 1 | none | one local tag plus `operator=` per case |
| 0x007A4520 | CTCWillEnflame::PlaySoundW | RELOCATION_MATCH | 1 | none | GetTC<CTCSoundPlayer>, then CThingCreatureBase::MakeNoise |
| 0x007A5580 | CTCForcePushPower::PlayCastSound | RELOCATION_MATCH | 1 | none | same shape, no noise |
| 0x00C4B890 | PKCBufferedReader::ReadBlock | RELOCATION_MATCH | 2 | /EHsc | the float ratio, i.e. the unsigned fix-up uses float 2^32 |
| 0x00C4B300 | PKCBufferedReader::PKCBufferedReader | RELOCATION_MATCH | 1 | /EHsc | member-order init; PKCStaticQueue/PKCMailbox<SReadRequest> |
| 0x0042DF9E | CNewFrontendGameComponent::Draw | RELOCATION_MATCH (retail_asm through the shared helper) | 2 (+44 FPU probes) | /O1 /Oy- | new helper engine/GFFPUSetRoundingNearest.h |
| 0x00E7FD50 | NScript::CV_FishingCompetitionScript::GenerateSignText | DIFFER(802v784) | 10 (+10 non-grading builds) | /Gr | parked |
| 0x00C40D10 | CEffectOwner_00c40750::CommitProperties (descriptive) | DIFFER(252v272) | 12 | none | parked |
| 0x00551340 | NUISystem::CTable::GetDataFromDef | DIFFER(431v428) | 4 | none | **the oracle is cut 3 bytes short; all 428 bytes match** |
| 0x00597BF2 | NUISystem::CFrontEndManager::GotoPreviousScreen | DIFFER(517v517) dl=14 | 40 | /O1 /Oy- | parked |
| 0x009BEF80 | CDisplayManager::SetViewport | DIFFER(479v479) | 30 (8 in asm round 2b + 22 here) | none | parked |
| 0x0053C332 | NUISystem::CList::DoRecomputeOffsets | DIFFER(1111v1111) dl=2 | 57 | /O1 /Oy- | parked |

Matched 7, parked 6. Of the 5 bakes, 0x0042DF9E lands as C++ plus a shared helper. 0x00551340 is byte-complete against the catalog row but blocked by a bad oracle cut.

### 0x00551340: the oracle cut (evidence)
- The catalog row is 428 bytes (0x00551340..0x005514EB), so it holds only the first byte of the epilogue `add esp,10h` at 0x005514EB.
- Retail's `jne` at 0x005514E4 targets 0x005514EB. The function ends with `ret` at 0x005514EE, and the int3 padding at 0x005514EF comes before the next function at 0x005514F0. That gives a true length of 431.
- Our object is 431 bytes and is identical to the catalog in the first 428.
- The row needs a re-cut to 431 bytes. The source and test are in `differ/00551340.*`, ready to land against the corrected span.

## Header overlay (`hdr/`, mirror rebuilt with mkmirror.sh)
hdrcheck2 on the overlay directory prints `default=same` for every changed header. NEW: engine/TRet.h, engine/GFFPUSetRoundingNearest.h.

| header | macro | addition |
|---|---|---|
| engine/CIEngine.h | `FABLE_RETAIL_CIENGINE_RENDER` (inside FABLE_RETAIL_CIENGINE) | slot 8 `virtual void Render(const CEngineInterpolationInfo&, unsigned long) = 0;` (the `#else` keeps Virtual08_00b27d90) |
| engine/CDisplayManager.h | `FABLE_RETAIL_CDISPLAYMANAGER` | `C2DBoxF GetVVisibleTelevisionScreenArea() const; // 0x009BECE0` |
| engine/CDisplayManager.h | `FABLE_RETAIL_CDISPLAYMANAGER_SETVIEWPORT` (from asm round 2b) | `void SetViewport(const C2DBoxI&); // 0x009BEF80` |
| engine/CRenderManager2D.h | `FABLE_RETAIL_CRENDERMANAGER2D` | `void ClearRenderTarget(const CRGBColour&, unsigned long); // 0x009D8CF0` |
| engine/CRenderManager.h | `FABLE_RETAIL_CRENDERMANAGER_ONVIEWPORTCHANGED` (round 2b) | `void OnViewportChanged(); // 0x00A0AAC0` |
| engine/NUISystem_CManager.h | `FABLE_RETAIL_CMANAGER` | inline `HasErrorMessage()`; `RenderErrorMessage(CIEngine&); // 0x0041D03C` |
| engine/CFrontEndManager.h | `FABLE_RETAIL_CFRONTENDMANAGER` | `static CFrontEndManager* __fastcall GetInstance(); // 0x00595582`; `Draw(CIEngine&, CEnginePrimitiveHandle*); // 0x00595222` |
| engine/CThingCreatureBase.h | `FABLE_RETAIL_CTHINGCREATUREBASE` | `MakeNoise(const CCharString&)` declaration (0x00662110) |
| boost/scoped_ptr.h | `FABLE_RETAIL_SCOPED_PTR_BOOL` (round 2b) | `operator unspecified_bool_type` |
| engine/TRet.h | new | `TRet<T>` with `_Fail_Val` (CQ_ArenaScript and the script getters) |
| engine/GFFPUSetRoundingNearest.h | new shared asm helper | see below |

**Siblings:** 62 landed rows whose preprocessed text differs between base and overlay were rechecked with vland `--recheck-landed` (`SIBLINGS.tsv`). Results: 60 RELOCATION_MATCH and 2 MATCH, all PASS, **no regressions**.

### New shared asm helper: engine/GFFPUSetRoundingNearest.h
- **Retail evidence.**
  - 0x0042DF9E+0x40 holds `fnstcw word [ebp-..] ; and byte [ebp-..+1],0F3h ; fldcw word [ebp-..]`. This is the engine's inline-asm `GFFPUSetRoundingNearest` (FableWin.pdb `?GFFPUSetRoundingNearest@@YIXXZ`).
  - Its sibling `GFFPUSetDoublePrecision` has the same fnstcw/fldcw frame at 0x004177AE and 0x004182B2.
- **Plain C++ was tried first.** 32 C++ spellings and 12 intrinsic spellings (`ATTEMPTS_0042df9e_fpu.tsv`) emit `_controlfp`/`_control87`/`__control87_2` calls, SSE `stmxcsr`/`ldmxcsr`, or nothing. None emits inline fnstcw/fldcw.
- **The helper.** It is a `__forceinline` `__fastcall` with `// Retail-asm-helper:` and `// Retail-asm:` lines.
  - The `FABLE_PORTABLE` twin uses `_controlfp(_RC_NEAR,_MCW_RC)` under MSVC and `fesetround(FE_TONEAREST)` otherwise.
- **Dual-path test.** `tests_portable/test_GFFPUSetRoundingNearest.cpp` goes to `rebuild/tests/portable/`. It passes in VC7.1 asm, dual and portable modes, and under g++ `-DFABLE_PORTABLE`.
  - On MSVC it checks behaviour through GFFloatToLongNear (2.7/-2.7/2.5/-2.5 → 3/-3/2/-2), because a `_controlfp(0,0)` read-back merges MXCSR.
- `engine/GFFloatToLongFloor.h`, `Ceil.h` and `Near.h` were checked first. None contains an fnstcw/fldcw block, so there was nothing to reuse.

## Header debt
- `CNewFrontendGameComponent`, `NUISystem::CIME` (Get 0x00404A80 / Render 0x00404C00) and `CInterpolationInfo` (ctor 0x00415A60, 0x34 bytes) are local declarations in src/0042df9e.cpp. No shared engine header exists for them yet. They are candidates for `gen_class_headers.py`.
- differ/00597bf2 locally models STLport deque/stack (`_Deque_iterator_base::_M_subtract`, `back`, `pop_back`), FRONTEND_SCREEN enumerators 7/20/23/26/27, CUserProfileManager (Load 0x0040D350) and CVirtualKeyboard. A shared `stlport/fable_deque.h` would retire these.
- `PRenderManager` (0x013B8384) is still a naming HOLD (other spellings: g_pRenderManager2D, g_pDisplayCaps). Draw declares it `extern CRenderManager*`.
- libpk `CString` stream operators: vland warns SHARED_HEADER_AVAILABLE CString for 0x00C4B890 and 0x00C4B300. The libpk `CString` is the vendor string, not the engine `CString` in engine/CString.h, so the local declaration is deliberate.
- RELOC_NAME warnings: CopyOutOfBuffers 0x00C4B640 is labelled `NScript_CV_MurderTwistScript_InitializeRead`, and the PKCMailbox ctor 0x00C4BC20 is labelled `_unattributed::CScriptEntry_00c4bc20`. Both are corrected in NAME_FIXES.tsv and LABELS.tsv.

## Callee names (bound or proven this chunk)
- **CNewFrontendGameComponent::Draw callees.**
  - GFGetTime; CRenderManagerCore SetFogEnable (DeviceState.FogEnable at +0x2944) and SetTextureMipMapMode (+0x2E44, table 0x01396E64) as inline setters; SetVWindow 0x00A0B560.
  - CDisplayManager::GetVVisibleTelevisionScreenArea 0x009BECE0, CRenderManager2D::ClearRenderTarget 0x009D8CF0, CDisplayManager::BeginRender 0x009BEF20.
  - CFrontEndManager::GetInstance 0x00595582 and Draw 0x00595222; CManager::GetInstance 0x0041E5F2 and RenderErrorMessage 0x0041D03C.
  - CIEngine slot 8 Render; CRenderManager2D::Render2DDrawList 0x009DA9F0; CIME::Get 0x00404A80 and Render 0x00404C00.
  - Globals: VIRTUAL_SCREEN_WIDTH/HEIGHT are doubles at 0x013961E8/F0.
- **Vtable slots proven.**
  - CGameScriptInterface 72, 205, 713 (SetReadableObjectText), 715, 716.
  - NUISystem::CComponent 108 LoadAndReturnDef, 82 GetStates, 48 ChangeState, 43 Initialise, 38 ChangeColour, 31 ChangePosition.
  - CIEngine 8 Render. CManager 40 PlaySoundW, 42 GetMiscThingsDef. CObservable 5 RemoveObserver. CList 156 InitialiseOffsets. IDirect3DDevice9 47 SetViewport.
- **Templates.**
  - map<ETableSprites,CCountedPointer<CComponent>>::operator[] 0x00554030 (PDB spelling, no binding).
  - deque<CComponent*>::back 0x0059B039.
  - map<FRONTEND_SCREEN,CComponent*>::operator[] 0x0059B5D7.
  - vector<map<unsigned long,CUIState>> push_back 0x0053CFC2 and range erase 0x0053CD69.
- Full names with their wrong spellings are in NAME_FIXES.tsv. The new labels are in LABELS.tsv.

## Parked rows: residue and next idea
- **0x00E7FD50 GenerateSignText, DIFFER(802v784).**
  - Retail calls 4 STLport vector<CWideString> helpers out of line inside an otherwise expanded vector: the allocator ctor 0x00430004, `_Construct` 0x0040DD00 (×3), the element `??_G(p,0)` 0x0040EC50 and `~_Vector_base` 0x0040F2C0.
  - VC7.1 /O2 expands them all. Explicit-specialisation declarations (try/e7fd50/x2) reach 782v784 with dl=19.
  - Next idea: find why c2 leaves those sites out of line. Candidates are bottom-up compile order or a TU-level STLport config; also try the per-call-site inline split (INLINE_DEPTH_REVISIT 2026-11-03).
- **0x00C40D10 CommitProperties, DIFFER(252v272).**
  - Retail runs a 0x18 frame (the y of each pan sits in the second 12-byte slot) and pre-stores the second pan's x/y/z into the result temporary before the length test.
  - Scalar locals give a 0x10 frame. Struct copies give memory loads (290 bytes).
  - Next idea: an inline by-value helper that returns the clamped vector, or a libpk float vector class with an inline ctor in a header.
- **0x00551340 GetDataFromDef.** The oracle is cut 3 bytes short (see above). Next step: re-cut the row to 431 bytes and land differ/00551340 as is.
- **0x00597BF2 GotoPreviousScreen, DIFFER(517v517), dl=14.**
  - Retail reads `m_stackScreens.top()` before `m_mapScreens[20]` and keeps that value at [ebp-8]. It spills later map references to the dead param slot [ebp+8].
  - The back-sound tail pushes the hidden def pointer before saving the second GetInstance.
  - Next idea: a non-inline `top()`, or a by-reference local of the top taken before the map lookup inside the `if` (retail order). Also an explicit `CDefPointer<const CUIMiscThingsDef>` local for the tail.
- **0x009BEF80 SetViewport, DIFFER(479v479).** This is a register-allocation residue.
  - Retail keeps 0 in ebp (later the top), width/height in edi/ebx and the viewport pointer in ecx, and spills right to the dead argument slot.
  - GFClamp (round 2b), explicit if/else and ternary clamps were all tried.
  - Next idea: clamp into a C2DBoxI temp built by its 4-argument ctor and compared through a by-reference local, or reorder the zero/width/height declarations to change VC7.1's register-pressure order.
- **0x0053C332 DoRecomputeOffsets, DIFFER(1111v1111), dl=2.**
  - One store pair is swapped. Retail stores PositionOffsetToUse.Y (0x0053C36E) before the fstp of the negated X (0x0053C374).
  - `fchs` spellings give the opposite order. `*-1` gives retail's order, but as an fmul.
  - Next idea: a C2DVector unary `operator-` returning by value, assigned whole (`to_use = -to_use` with an inline member operator in the shared header), so the Y copy is emitted before the X store.

## Unaligned bakes (coordinator hint)
0x0042DF9E, 0x0053C332 and 0x00597BF2 have neighbours that end in `leave ; ret` with no int3/nop padding, and retail entry points that are not 16-aligned. Their bodies use `push imm/pop`, `pop ecx` and `and [mem],0` idioms. These point to /O1-style size TUs.
- Every grade of those three rows used `/O1 /Oy-` (the landed siblings use `#pragma optimize("s",on)` + `("y",off)`).
- 0x0053C332 was also graded under `/O1`, `/O1 /Oy- /G5`, `/O1 /Oy- /G6`, `/O1 /Oy- /Op`, `/Og /Os /Oy-`, `/Os /Og- /Oy-`, `/Os /Oy-` and `/Os /Oy- /Ob1`.
- 0x00597BF2 was also graded under `/O1 /Oy- /Oi`, `/O2 /Os /Oy-`, `/Og /Os /Oy- /Ob1` and `/Os /Oy-`.
- Only `/O1 /Oy-` reproduces the prologue and the frame. The per-attempt flags are in the tables below.

## GOTCHAS one-liners
- **Unaligned retail entry** (no padding before it, a `leave;ret` neighbour): grade under `/O1 /Oy-` first.
- `fnstcw ; and byte [cw+1],0F3h ; fldcw` is `GFFPUSetRoundingNearest` (FableWin.pdb). VC7.1 cannot emit it from C++ or intrinsics, so use engine/GFFPUSetRoundingNearest.h.
- `_controlfp(0,0)` on VC7.1 merges MXCSR into the value it returns. Test rounding behaviourally (GFFloatToLongNear), not through the control-word read-back.
- **TRet<T> failure path:** the `_Fail_Val` static gives retail's `TRet` failure shape.
- A float 2^32 unsigned fix-up (`fadd dword [__real@4f800000]`) means the source cast to `(float)`, not to double.
- When retail calls `??_G(p,0)` out of line where VC7.1 inlines the delete, c2 did not inline that site. A local stand-in does not reproduce it.
- **Bad cut:** the catalog oracle for 0x00551340 is 3 bytes short (the jne target 0x005514EB lies outside the row).
- **`lea` addressing:** a reference local (`C2DVector& v = member;`) reproduces retail's `lea eax,[esi+off]`, where a plain member access gives `mov eax,[esi+off]`.
- **fadd operand order:** a float temporary (`float y = a + b; a = y;`) keeps retail's order.
- `!(a >= c)` on floats compiles to retail's `fnstsw ax ; test ah,1` form.
- **Hoisting key locals:** an explicit guard plus a `do{}while` loop makes VC7.1 initialise them in the preheader, as retail does.
- **Wine hang:** a fatal cl error on a missing header hangs Wine until the timeout, about 110 s. Check includes before grading.
- **vland skip:** vland silently skips a payload row whose address is already landed (bake rewrites). Pass `--recheck-landed` to grade it.
- 0x0041E5F2 is CManager::GetInstance (0xD0 object, ctor 0x0041E3F6). CFrontEndManager::GetInstance is 0x00595582 (0xE0 object, ctor 0x005953E2).

## ATTEMPTS
- FPU site of 0x0042DF9E: `ATTEMPTS_0042df9e_fpu.tsv` has 44 probes with what VC7.1 emitted. It is reproduced below.
- Per-row grades: `ATTEMPTS_rows.tsv`, generated from LOG.tsv, the work/*.d diffs and the try/ sources. It is reproduced below.
  - "first difference" is the first masked line where retail and our object differ.
  - "spelling change" is the first source lines added relative to the previous attempt of the same row.
- 0x009BEF80 asm round 2b attempts a01..a09: `$S/plan/asm2b/ATTEMPTS.tsv`. All are DIFFER at 444..462v479 under `/O2 /Oy /W3`, with frame 0x14/0x10 against retail's 0x18.
- SRC_FAIL, PPFAIL and MISSING_INCLUDE lines are builds that did not grade. They are not counted in the attempt totals.


### 0x0042DF9E FPU site: 32 C++ + 12 intrinsic probes

| probe | spelling | inline fnstcw+fldcw | VC7.1 (RTM 3077) /O2 emitted |
|---|---|---|---|
| c01 | _controlfp(_RC_NEAR, _MCW_RC); | no | push 0x300; push 0x0; call c; DISP32 __controlfp; add esp,0x8; ret |
| c02 | _control87(_RC_NEAR, _MCW_RC); | no | push 0x300; push 0x0; call c; DISP32 __control87; add esp,0x8; ret |
| c03 | unsigned int cw = _controlfp(0, 0); _controlfp(cw & ~_MCW_RC, _MCW_RC); | no | push 0x0; push 0x0; call 9; DISP32 __controlfp; and eax,0xfffffcff; push 0x300; push eax; call 19; DISP32 __controlfp; add esp,0x10; ret |
| c04 | unsigned int cw = _control87(0, 0); _control87((cw & ~_MCW_RC) \| _RC_NEAR, 0xfffff); | no | push 0x0; push 0x0; call 9; DISP32 __control87; and eax,0xfffffcff; push 0xfffff; push eax; call 19; DISP32 __control87; add esp,0x10; ret |
| c05 | _controlfp(_RC_NEAR, kMask); | no | push 0x300; push 0x0; call c; DISP32 __controlfp; add esp,0x8; ret |
| c06 | SetNear(); | no | push 0x300; push 0x0; call c; DISP32 __controlfp; add esp,0x8; ret |
| c07 | _control87(_RC_NEAR, g_RoundMask); | no | mov eax,ds:0x0; dir32 ?g_RoundMask@@3IB; push eax; push 0x0; call d; DISP32 __control87; add esp,0x8; ret |
| c08 | _clearfp(); _controlfp(_RC_NEAR, _MCW_RC); | no | call 5; DISP32 __clearfp; push 0x300; push 0x0; call 11; DISP32 __controlfp; add esp,0x8; ret |
| c09 | _fpreset(); | no | jmp 5; DISP32 __fpreset |
| c10 | _controlfp(_CW_DEFAULT, 0xfffff); | no | push 0xfffff; push 0x9001f; call f; DISP32 __controlfp; add esp,0x8; ret |
| c11 | _control87(_CW_DEFAULT, 0xfffff); | no | push 0xfffff; push 0x9001f; call f; DISP32 __control87; add esp,0x8; ret |
| c12 | _control87(_RC_NEAR \| _PC_53, _MCW_RC \| _MCW_PC); | no | push 0x30300; push 0x10000; call f; DISP32 __control87; add esp,0x8; ret |
| c13 | _controlfp(_RC_NEAR, _MCW_RC); _controlfp(_PC_53, _MCW_PC); | no | push 0x300; push 0x0; call c; DISP32 __controlfp; push 0x30000; push 0x10000; call 1b; DISP32 __controlfp; add esp,0x10; ret |
| c14 | g_set(_RC_NEAR, _MCW_RC); | no | push 0x300; push 0x0; call DWORD PTR ds:0x0; dir32 _g_set; add esp,0x8; ret |
| c15 | unsigned int s = _statusfp(); if (s) _controlfp(_RC_NEAR, _MCW_RC); | no | call 5; DISP32 __statusfp; test eax,eax; je 18; push 0x300; push 0x0; call 15; DISP32 __controlfp; add esp,0x8; ret |
| c16 | fesetround(FE_TONEAREST); | no | COMPILE_ERROR: Cannot open include file: 'fenv.h': No such file or directory |
| c17 | std::fesetround(FE_TONEAREST); | no | COMPILE_ERROR: Cannot open include file: 'cfenv': No such file or directory |
| c18 | _set_controlfp(_RC_NEAR, _MCW_RC); | no | COMPILE_ERROR: '_set_controlfp': identifier not found, even with argument-dependent lookup |
| c19 | unsigned int cw; _controlfp_s(&cw, _RC_NEAR, _MCW_RC); | no | COMPILE_ERROR: '_controlfp_s': identifier not found, even with argument-dependent lookup |
| c20 | g_sink = (long)g_value; | no | fld DWORD PTR ds:0x0; dir32 ?g_value@@3MA; call b; DISP32 __ftol2; mov ds:0x0,eax; dir32 ?g_sink@@3JA; ret |
| c21 | g_sink = (long)g_value; | no | fld DWORD PTR ds:0x0; dir32 ?g_value@@3MA; call b; DISP32 __ftol2; mov ds:0x0,eax; dir32 ?g_sink@@3JA; ret |
| c22 | g_sink = (long)g_value; | no | fld DWORD PTR ds:0x0; dir32 ?g_value@@3MA; call b; DISP32 __ftol2; mov ds:0x0,eax; dir32 ?g_sink@@3JA; ret |
| c23 | g_sink = (long)floor(g_value + 0.5); | no | fld QWORD PTR ds:0x0; dir32 ?g_value@@3NA; sub esp,0x8; fadd QWORD PTR ds:0x0; dir32 __real@3fe0000000000000; fstp QWORD PTR [esp]; call 17; DISP32 _floor; add esp,0x8; call 1f; DISP32 __ftol2; mov ds:0x0,eax; dir32 ?g_s |
| c24 | g_sink = (long)(g_value + 0.5f); | no | fld DWORD PTR ds:0x0; dir32 ?g_value@@3MA; fadd DWORD PTR ds:0x0; dir32 __real@3f000000; call 11; DISP32 __ftol2; mov ds:0x0,eax; dir32 ?g_sink@@3JA; ret |
| c25 | g_dsink = floor(g_value); | no | fld QWORD PTR ds:0x0; dir32 ?g_value@@3NA; sub esp,0x8; fstp QWORD PTR [esp]; call 11; DISP32 _floor; fstp QWORD PTR ds:0x0; dir32 ?g_dsink@@3NA; add esp,0x8; ret |
| c26 | unsigned int cw = _control87(0, 0); cw &= ~_MCW_RC; _control87(cw, _MCW_RC); | no | push 0x0; push 0x0; call 9; DISP32 __control87; and eax,0xfffffcff; push 0x300; push eax; call 19; DISP32 __control87; add esp,0x10; ret |
| c27 | unsigned long cw = _controlfp(0, 0); cw = (cw & ~0x300UL) \| 0x0UL; _controlfp((unsigned int)cw, 0x30 | no | push 0x0; push 0x0; call 9; DISP32 __controlfp; and eax,0xfffffcff; push 0x300; push eax; call 19; DISP32 __controlfp; add esp,0x10; ret |
| c28 | if ((_controlfp(0, 0) & _MCW_RC) != _RC_NEAR) _controlfp(_RC_NEAR, _MCW_RC); | no | push 0x0; push 0x0; call 9; DISP32 __controlfp; add esp,0x8; test ah,0x3; je 20; push 0x300; push 0x0; call 1d; DISP32 __controlfp; add esp,0x8; ret |
| c29 | CFPUState state; (void)state; | no | push 0x0; push 0x0; call 9; DISP32 __controlfp; push 0x300; push 0x0; call 15; DISP32 __controlfp; add esp,0x10; ret |
| c30 | TRound<_RC_NEAR>::Set(); | no | push 0x300; push 0x0; call c; DISP32 __controlfp; add esp,0x8; ret |
| c31 | _controlfp(_RC_NEAR, _MCW_RC \| _MCW_PC & 0); | no | push 0x300; push 0x0; call c; DISP32 __controlfp; add esp,0x8; ret |
| c32 | unsigned short cw = (unsigned short)_control87(0,0); cw &= 0xf3ff; _control87(cw, 0xffff); | no | push 0x0; push 0x0; call 9; DISP32 __control87; and eax,0xf3ff; movzx eax,ax; push 0xffff; push eax; call 1c; DISP32 __control87; add esp,0x10; ret |
| i01 | _mm_setcsr((_mm_getcsr() & ~_MM_ROUND_MASK) \| _MM_ROUND_NEAREST); | no | push ecx; stmxcsr DWORD PTR [esp]; mov eax,DWORD PTR [esp]; and eax,0xffff9fff; mov DWORD PTR [esp],eax; ldmxcsr DWORD PTR [esp]; pop ecx; ret |
| i02 | _MM_SET_ROUNDING_MODE(_MM_ROUND_NEAREST); | no | push ecx; stmxcsr DWORD PTR [esp]; mov eax,DWORD PTR [esp]; and eax,0xffff9fff; mov DWORD PTR [esp],eax; ldmxcsr DWORD PTR [esp]; pop ecx; ret |
| i03 | g_sink = (long)_MM_GET_ROUNDING_MODE(); | no | push ecx; stmxcsr DWORD PTR [esp]; mov eax,DWORD PTR [esp]; and eax,0x6000; mov ds:0x0,eax; dir32 ?g_sink@@3JA; pop ecx; ret |
| i04 | g_sink = (long)_mm_getcsr(); | no | push ecx; stmxcsr DWORD PTR [esp]; mov eax,DWORD PTR [esp]; mov ds:0x0,eax; dir32 ?g_sink@@3JA; pop ecx; ret |
| i05 | _mm_setcsr(0x1f80); | no | push ecx; mov DWORD PTR [esp],0x1f80; ldmxcsr DWORD PTR [esp]; pop ecx; ret |
| i06 | g_sink = _mm_cvtss_si32(_mm_set_ss(g_value)); | no | movss xmm0,DWORD PTR ds:0x0; dir32 ?g_value@@3MA; cvtss2si eax,xmm0; mov ds:0x0,eax; dir32 ?g_sink@@3HA; ret |
| i07 | g_sink = _mm_cvt_ss2si(_mm_load_ss(&g_value)); | no | mov eax,0x0; dir32 ?g_value@@3MA; movss xmm0,DWORD PTR [eax]; cvtss2si ecx,xmm0; mov DWORD PTR ds:0x0,ecx; dir32 ?g_sink@@3HA; ret |
| i08 | g_sink = _mm_cvtsd_si32(_mm_set_sd(g_value)); | no | movsd xmm0,QWORD PTR ds:0x0; dir32 ?g_value@@3NA; cvtsd2si eax,xmm0; mov ds:0x0,eax; dir32 ?g_sink@@3HA; ret |
| i09 | _m_empty(); | no | emms; ret |
| i10 | _control87(_RC_NEAR, _MCW_RC); | no | push 0x300; push 0x0; call c; DISP32 __control87; add esp,0x8; ret |
| i11 | _controlfp(_RC_NEAR, _MCW_RC); | no | push 0x300; push 0x0; call c; DISP32 __controlfp; add esp,0x8; ret |
| i12 | g_sink = _mm_cvtt_ss2si(_mm_load_ss(&g_value)); | no | mov eax,0x0; dir32 ?g_value@@3MA; movss xmm0,DWORD PTR [eax]; cvttss2si ecx,xmm0; mov DWORD PTR ds:0x0,ecx; dir32 ?g_sink@@3HA; ret |

### 0x00F21590 grades

| tag | grade | dl | flags | first_difference(retail / VC7.1 emitted) | spelling_change_vs_previous |
|---|---|---|---|---|---|
| a1 | PPFAIL |  |  |  |  |
| a1 | DIFFER(176v204) | dl=27 |  | retail `sub esp, 8` / ours `push ecx` |  |
| a2 | RELOCATION_MATCH | dl= |  |  |  |
| final | RELOCATION_MATCH | dl= |  |  |  |


### 0x00E80060 grades

| tag | grade | dl | flags | first_difference(retail / VC7.1 emitted) | spelling_change_vs_previous |
|---|---|---|---|---|---|
| a1 | RELOCATION_MATCH | dl= |  |  |  |
| final | RELOCATION_MATCH | dl= |  |  |  |


### 0x00E7FD50 grades

| tag | grade | dl | flags | first_difference(retail / VC7.1 emitted) | spelling_change_vs_previous |
|---|---|---|---|---|---|
| a1 | DIFFER(802v784) | dl=190 |  | retail `lea ecx, [esp + 0x20]` / ours `push ebx` |  |
| a2 | DIFFER(462v784) | dl=178 |  | retail `sub esp, 0x38` / ours `mov ebp, esp` |  |
| a3 | DIFFER(462v784) | dl=178 |  | retail `sub esp, 0x38` / ours `mov ebp, esp` |  |
| diag10 | DIFFER(521v784) | dl=220 |  | retail `sub esp, 0x38` / ours `sub esp, 0x34` |  |
| diag20 | DIFFER(538v784) | dl=219 |  | retail `sub esp, 0x38` / ours `sub esp, 0x34` |  |
| diag30 | DIFFER(554v784) | dl=273 |  | retail `sub esp, 0x38` / ours `sub esp, 0x34` |  |
| diag40 | DIFFER(580v784) | dl=147 |  | retail `push ebp` / ours `push ebx` |  |
| diag50 | DIFFER(677v784) | dl=104 |  | retail `lea eax, [esp + 0xf]` / ours `mov dword ptr [esp + 0x2c], 0` |  |
| diag60 | DIFFER(623v784) | dl=112 |  | retail `mov esi, dword ptr [ebp + 0x40]` / ours `mov ecx, dword ptr [ebp + 0x40]` |  |
| diagcr3 | DIFFER(802v784) | dl=190 |  | retail `lea ecx, [esp + 0x20]` / ours `push ebx` |  |
| diagcr5 | DIFFER(802v784) | dl=190 |  | retail `lea ecx, [esp + 0x20]` / ours `push ebx` |  |
| diagcr8 | DIFFER(802v784) | dl=190 |  | retail `lea ecx, [esp + 0x20]` / ours `push ebx` |  |
| a4 | DIFFER(782v784) | dl=195 |  | retail `lea ecx, [esp + 0x20]` / ours `push ebx` |  |
| x1 | SRC_FAIL | dl= | /Gr |  | first spelling |
| x2 | DIFFER(782v784) | dl=19 | /Gr | retail `mov esi, dword ptr [ebp + 0x40]` / ours `mov ecx, dword ptr [ebp + 0x40]` | changed: #include "stlport/fable_containers.h" \| namespace std { |
| x3 | DIFFER(782v784) | dl=19 | /Gr | retail `mov esi, dword ptr [ebp + 0x40]` / ours `mov ecx, dword ptr [ebp + 0x40]` | changed: enum EHeroTitle { TITLE_NONE=0, TITLE_REAPER=1, TITLE_SHADOWHUNTER=2, TITLE_MALEFICUS=3, TITLE_DEATHBRINGER=4, TITLE_ASSASSIN=5, TITLE_NECROMANCER=6, T |
| x4 | DIFFER(782v784) | dl=24 | /Gr | retail `mov esi, dword ptr [ebp + 0x40]` / ours `lea ecx, [esp + 0x18]` | changed: if (Winner == "HERO") { \| CCharString title = GetHeroTitleTextTag(); |
| x5 | DIFFER(782v784) | dl=27 | /Gr | retail `mov esi, dword ptr [ebp + 0x40]` / ours `mov ecx, dword ptr [ebp + 0x40]` | changed: if (Winner == "HERO") \| winnerName = Game->GetTextString(GetHeroTitleTextTag()); |
| x6 | DIFFER(782v784) | dl=19 | /Gr | retail `mov esi, dword ptr [ebp + 0x40]` / ours `mov ecx, dword ptr [ebp + 0x40]` | changed: winnerName = Game->GetTextString(CCharString(GetHeroTitleTextTag())); \| float nextWeight = GetNextPrizeCategoryWeight(); |
| x7 | DIFFER(782v784) | dl=19 | /Gr | retail `mov esi, dword ptr [ebp + 0x40]` / ours `mov ecx, dword ptr [ebp + 0x40]` | changed: CWideString winnerName(Game->GetTextString(Winner)); \| winnerName = Game->GetTextString(GetHeroTitleTextTag()); |


### 0x007A4520 grades

| tag | grade | dl | flags | first_difference(retail / VC7.1 emitted) | spelling_change_vs_previous |
|---|---|---|---|---|---|
| a1 | RELOCATION_MATCH | dl= |  |  |  |
| final | RELOCATION_MATCH | dl= |  |  |  |


### 0x007A5580 grades

| tag | grade | dl | flags | first_difference(retail / VC7.1 emitted) | spelling_change_vs_previous |
|---|---|---|---|---|---|
| a1 | RELOCATION_MATCH | dl= |  |  |  |
| final | RELOCATION_MATCH | dl= |  |  |  |


### 0x00C40D10 grades

| tag | grade | dl | flags | first_difference(retail / VC7.1 emitted) | spelling_change_vs_previous |
|---|---|---|---|---|---|
| a1 | DIFFER(290v272) | dl=67 |  | retail `fld dword ptr [esi + 0xc]` / ours `lea ecx, [esi + 0xc]` |  |
| a2 | DIFFER(252v272) | dl=37 |  | retail `sub esp, 0x18` / ours `sub esp, 0x10` |  |
| a3 | DIFFER(290v272) | dl=68 |  | retail `fld dword ptr [esi + 0xc]` / ours `lea ecx, [esi + 0xc]` |  |
| a4 | DIFFER(290v272) | dl=68 |  | retail `fld dword ptr [esi + 0xc]` / ours `lea ecx, [esi + 0xc]` |  |
| a5 | DIFFER(44v272) | dl=81 |  | retail `sub esp, 0x18` / ours `lea ecx, [esi + 0xc]` |  |
| a6 | DIFFER(252v272) | dl=37 |  | retail `sub esp, 0x18` / ours `sub esp, 0x1c` |  |
| a7 | DIFFER(290v272) | dl=56 |  | retail `sub esp, 0x18` / ours `sub esp, 0xc` |  |
| a8 | DIFFER(290v272) | dl=56 |  | retail `sub esp, 0x18` / ours `sub esp, 0xc` |  |
| a9 | DIFFER(290v272) | dl=67 |  | retail `fld dword ptr [esi + 0xc]` / ours `lea ecx, [esi + 0xc]` |  |
| a10 | DIFFER(290v272) | dl=67 |  | retail `fld dword ptr [esi + 0xc]` / ours `lea ecx, [esi + 0xc]` |  |
| a11 | DIFFER(240v272) | dl=68 |  | retail `fld dword ptr [esi + 0xc]` / ours `mov eax, dword ptr [esi + 0xc]` |  |
| a12 | DIFFER(240v272) | dl=69 |  | retail `sub esp, 0x18` / ours `sub esp, 0xc` |  |


### 0x00C4B890 grades

| tag | grade | dl | flags | first_difference(retail / VC7.1 emitted) | spelling_change_vs_previous |
|---|---|---|---|---|---|
| a1 | DIFFER(538v538) | dl=2 |  | retail `fadd dword ptr [0x122dcb4]` / ours `fadd qword ptr [0]` |  |
| a2 | RELOCATION_MATCH | dl= |  |  |  |
| final | RELOCATION_MATCH | dl= |  |  |  |


### 0x00C4B300 grades

| tag | grade | dl | flags | first_difference(retail / VC7.1 emitted) | spelling_change_vs_previous |
|---|---|---|---|---|---|
| a1 | RELOCATION_MATCH | dl= |  |  |  |
| final | RELOCATION_MATCH | dl= |  |  |  |


### 0x00551340 grades

| tag | grade | dl | flags | first_difference(retail / VC7.1 emitted) | spelling_change_vs_previous |
|---|---|---|---|---|---|
| a | DIFFER(431v428) | dl=3 | none | retail `sub esp, 0x10` / ours `sub esp, 0xc` | first spelling |
| b | DIFFER(431v428) | dl=3 | none | retail `sub esp, 0x10` / ours `sub esp, 0xc` | changed: { \| CCountedPointer<CComponent> component(CManager::GetInstance()->CreateComponent((*it).second)); |
| c | DIFFER(431v428) | dl=2 | none |  / ours `add esp, 0x10` | changed: #define FABLE_RETAIL_CCOUNTEDPOINTER \| Sprites[(*it).first] = CCountedPointer<CComponent>(CManager::GetInstance()->CreateComponent((*it).second)); |
| d | DIFFER(431v428) | dl=3 | none | retail `sub esp, 0x10` / ours `sub esp, 0xc` | changed: CComponent* component = CManager::GetInstance()->CreateComponent((*it).second); \| Sprites[(*it).first] = CCountedPointer<CComponent>(component); |
| final | DIFFER(431v428) | dl=2 |  |  / ours `add esp, 0x10` |  |


### 0x00597BF2 grades

| tag | grade | dl | flags | first_difference(retail / VC7.1 emitted) | spelling_change_vs_previous |
|---|---|---|---|---|---|
| a | DIFFER(501v517) | dl=43 | /O1 /Oy- | retail `mov edi, ecx` / ours `mov esi, ecx` | first spelling |
| b | DIFFER(517v517) | dl=34 | /O1 /Oy- | retail `mov edi, ecx` / ours `mov esi, ecx` | changed: #define FABLE_RETAIL_DEF_CLASS_BASE \| #include "engine/CDefPointer.h" |
| c | DIFFER(517v517) | dl=15 | /O1 /Oy- | retail `mov eax, dword ptr [eax]` / ours `mov dword ptr [ebp - 4], 0x14` | changed: if (m_mapScreens[FRONTEND_SCREEN_20] == m_stackScreens.top()) { |
| d | DIFFER(517v517) | dl=33 | /O1 /Oy- | retail `mov edi, ecx` / ours `mov esi, ecx` | changed: #define FABLE_RETAIL_CCHARSTRING \| #define FABLE_RETAIL_CDEFSTRINGTABLE |
| g | DIFFER(517v517) | dl=20 | /O1 /Oy- | retail `lea ebx, [edi + 0x20]` / ours `lea ebx, [edi + 0x20]` | changed: if (m_stackScreens.top() == m_mapScreens[FRONTEND_SCREEN_20]) { |
| h | DIFFER(517v517) | dl=35 | /O1 /Oy- | retail `mov edi, ecx` / ours `mov esi, ecx` | changed: CComponent* GetCurrentScreen() { return m_stackScreens.top(); } \| if (GetCurrentScreen() == m_mapScreens[FRONTEND_SCREEN_20]) { |
| i | DIFFER(517v517) | dl=33 | /O1 /Oy- | retail `mov edi, ecx` / ours `mov esi, ecx` | changed: CComponent* current = m_stackScreens.top(); \| bool atRoot = current == m_mapScreens[FRONTEND_SCREEN_20]; |
| j | DIFFER(517v517) | dl=36 | /O1 /Oy- | retail `mov edi, ecx` / ours `mov esi, ecx` | changed: CComponent* GetCurrentScreen() { return m_stackScreens.top(); } \| if (m_mapScreens[FRONTEND_SCREEN_20] == GetCurrentScreen()) { |
| k | DIFFER(517v517) | dl=20 | /O1 /Oy- | retail `lea ebx, [edi + 0x20]` / ours `lea ebx, [edi + 0x20]` | changed: if ((CComponent*)m_stackScreens.top() == m_mapScreens[FRONTEND_SCREEN_20]) { |
| df1 | DIFFER(517v517) | dl=33 | /Os /Oy- | retail `mov edi, ecx` / ours `mov esi, ecx` | changed: CComponent* current = m_stackScreens.top(); \| if (current == m_mapScreens[FRONTEND_SCREEN_20]) { |
| df2 | DIFFER(517v517) | dl=33 | /O2 /Os /Oy- | retail `mov edi, ecx` / ours `mov esi, ecx` | same body, other flags |
| df3 | DIFFER(517v517) | dl=33 | /Og /Os /Oy- /Ob1 | retail `mov edi, ecx` / ours `mov esi, ecx` | same body, other flags |
| df4 | DIFFER(517v517) | dl=33 | /O1 /Oy- /Oi | retail `mov edi, ecx` / ours `mov esi, ecx` | same body, other flags |
| gf5 | DIFFER(517v517) | dl=20 | /Os /Oy- | retail `lea ebx, [edi + 0x20]` / ours `lea ebx, [edi + 0x20]` | changed: if (m_stackScreens.top() == m_mapScreens[FRONTEND_SCREEN_20]) { |
| gf6 | DIFFER(517v517) | dl=20 | /O2 /Os /Oy- | retail `lea ebx, [edi + 0x20]` / ours `lea ebx, [edi + 0x20]` | same body, other flags |
| gf7 | DIFFER(517v517) | dl=20 | /Og /Os /Oy- /Ob1 | retail `lea ebx, [edi + 0x20]` / ours `lea ebx, [edi + 0x20]` | same body, other flags |
| gf8 | DIFFER(517v517) | dl=20 | /O1 /Oy- /Oi | retail `lea ebx, [edi + 0x20]` / ours `lea ebx, [edi + 0x20]` | same body, other flags |
| t1 | DIFFER(517v517) | dl=20 | /O1 /Oy- | retail `lea ebx, [edi + 0x20]` / ours `lea ebx, [edi + 0x20]` | changed: CManager::GetInstance()->PlaySoundW((*CManager::GetInstance()->GetMiscThingsDef()).SoundBack, 1); |
| t2 | DIFFER(517v517) | dl=20 | /O1 /Oy- | retail `lea ebx, [edi + 0x20]` / ours `lea ebx, [edi + 0x20]` | changed: CManager::GetInstance()->PlaySoundW(CManager::GetInstance()->GetMiscThingsDef().Get()->SoundBack, 1); |
| t3 | DIFFER(517v517) | dl=20 | /O1 /Oy- | retail `lea ebx, [edi + 0x20]` / ours `lea ebx, [edi + 0x20]` | changed: CManager::GetInstance()->PlaySoundW(CManager::GetInstance()->GetMiscThingsDef()->SoundBack, true); |
| t5 | DIFFER(515v517) | dl=30 | /O1 /Oy- | retail `lea ebx, [edi + 0x20]` / ours `lea ebx, [edi + 0x20]` | changed: CManager* manager = CManager::GetInstance(); \| manager->PlaySoundW(CManager::GetInstance()->GetMiscThingsDef()->SoundBack, 1); |
| m1 | DIFFER(514v517) | dl=26 | /O1 /Oy- | retail `lea ebx, [edi + 0x20]` / ours `lea ebx, [edi + 0x20]` | changed: if (m_stackScreens.top() != m_mapScreens[FRONTEND_SCREEN_20]) { \| } else { |
| m2 | DIFFER(514v517) | dl=39 | /O1 /Oy- | retail `mov edi, ecx` / ours `mov esi, ecx` | changed: CComponent* current = m_stackScreens.top(); \| if (current != m_mapScreens[FRONTEND_SCREEN_20]) { |
| m3 | DIFFER(534v517) | dl=46 | /O1 /Oy- | retail `push ecx` / ours `mov esi, ecx` | changed: if (current == m_mapScreens[FRONTEND_SCREEN_20] && !force) \| return; |
| m4 | DIFFER(517v517) | dl=33 | /O1 /Oy- | retail `mov edi, ecx` / ours `mov esi, ecx` | changed: { \| if (current == m_mapScreens[FRONTEND_SCREEN_20]) { |
| m5 | DIFFER(517v517) | dl=14 | /O1 /Oy- | retail `mov eax, dword ptr [eax]` / ours `mov dword ptr [ebp - 4], 0x14` | changed: if (!(m_mapScreens[FRONTEND_SCREEN_20] != m_stackScreens.top())) { \| } else { |
| n1 | DIFFER(517v517) | dl=33 | /O1 /Oy- | retail `mov edi, ecx` / ours `mov esi, ecx` | changed: CComponent* current = m_stackScreens.top(); \| CComponent* root = m_mapScreens[FRONTEND_SCREEN_20]; |
| n2 | DIFFER(517v517) | dl=34 | /O1 /Oy- | retail `mov edi, ecx` / ours `mov esi, ecx` | changed: if (m_mapScreens[FRONTEND_SCREEN_20] == current) { |
| n5 | DIFFER(517v517) | dl=33 | /O1 /Oy- | retail `mov edi, ecx` / ours `mov esi, ecx` | changed: if (current == m_mapScreens[FRONTEND_SCREEN_20]) { \| current = m_stackScreens.top(); |
| n6 | DIFFER(517v517) | dl=35 | /O1 /Oy- | retail `mov edi, ecx` / ours `mov esi, ecx` | changed: CComponent* current; \| if ((current = m_stackScreens.top()) == m_mapScreens[FRONTEND_SCREEN_20]) { |
| m6 | DIFFER(512v517) | dl=19 | /O1 /Oy- | retail `push ecx` / ours `mov esi, dword ptr [eax]` | changed: CComponent* current = m_stackScreens.top(); \| if (current == m_mapScreens[FRONTEND_SCREEN_20]) { |
| m7 | DIFFER(517v517) | dl=16 | /O1 /Oy- | retail `mov eax, dword ptr [eax]` / ours `mov dword ptr [ebp - 4], 0x14` | changed: if (!(m_mapScreens[FRONTEND_SCREEN_20] != m_stackScreens.top())) { |
| p1 | DIFFER(452v517) | dl=35 | /O1 /Oy- | retail `mov eax, dword ptr [eax]` / ours `mov dword ptr [ebp - 4], 0x14` | changed: void PlayBackSound() { CManager::GetInstance()->PlaySoundW(CManager::GetInstance()->GetMiscThingsDef()->SoundBack, 1); } \| GotoNextScreen(m_mapScreens[ |
| p2 | DIFFER(515v517) | dl=20 | /O1 /Oy- | retail `mov eax, dword ptr [eax]` / ours `mov dword ptr [ebp - 4], 0x14` | changed: { \| CDefPointer<CUIMiscThingsDef> misc = CManager::GetInstance()->GetMiscThingsDef(); |
| qd | DIFFER(517v517) | dl=33 |  | retail `mov edi, ecx` / ours `mov esi, ecx` |  |
| qm5 | DIFFER(517v517) | dl=14 |  | retail `mov eax, dword ptr [eax]` / ours `mov dword ptr [ebp - 4], 0x14` |  |
| q1 | DIFFER(468v517) | dl=87 | /O1 /Oy- | retail `push ecx` / ours `push 0x14` | changed: bool IsCurrentScreen(FRONTEND_SCREEN screen) { return m_stackScreens.top() == m_mapScreens[screen]; } \| if (IsCurrentScreen(FRONTEND_SCREEN_20)) { |
| q2 | DIFFER(460v517) | dl=54 | /O1 /Oy- | retail `push ecx` / ours `mov esi, ecx` | changed: CComponent* current = m_stackScreens.top(); \| if (current == m_mapScreens[FRONTEND_SCREEN_20]) { |
| q3 | DIFFER(521v517) | dl=41 | /O1 /Oy- | retail `push ecx` / ours `push 0x14` | changed: bool IsCurrentScreen(FRONTEND_SCREEN screen) { CComponent* current = m_stackScreens.top(); return current == m_mapScreens[screen]; } \| if (IsCurrentScr |
| q4 | DIFFER(468v517) | dl=87 | /O1 /Oy- | retail `push ecx` / ours `push 0x14` | changed: if (IsCurrentScreen(FRONTEND_SCREEN_27)) { \| if (IsCurrentScreen(FRONTEND_SCREEN_23)) { |
| final | DIFFER(517v517) | dl=14 |  | retail `mov eax, dword ptr [eax]` / ours `mov dword ptr [ebp - 4], 0x14` |  |


### 0x0042DF9E grades

| tag | grade | dl | flags | first_difference(retail / VC7.1 emitted) | spelling_change_vs_previous |
|---|---|---|---|---|---|
| a | MISSING_INCLUDE |  | /O1 /Oy- |  | first spelling |
| a | RELOCATION_MATCH | dl= | /O1 /Oy- |  | same body, other flags |
| final | RELOCATION_MATCH | dl= |  |  |  |
| final | RELOCATION_MATCH | dl= |  |  |  |


### 0x009BEF80 grades

| tag | grade | dl | flags | first_difference(retail / VC7.1 emitted) | spelling_change_vs_previous |
|---|---|---|---|---|---|
| v01 | DIFFER(474v479) | dl=160 | none | retail `sub esp, 0x18` / ours `sub esp, 0x14` | first spelling |
| v02 | SRC_FAIL | dl= | none |  | changed: C2DBoxI box(left, top, right, bottom); |
| v03 | DIFFER(474v479) | dl=160 | none | retail `sub esp, 0x18` / ours `sub esp, 0x14` | changed: C2DBoxI box; \| box.Initialise(left, top, right, bottom); |
| v04 | DIFFER(477v479) | dl=136 | none | retail `sub esp, 0x18` / ours `sub esp, 0x14` | changed: long left = viewport.TLX < 0 ? 0 : (viewport.TLX > RenderTargetDimensions.X ? RenderTargetDimensions.X : viewport.TLX); \| long right = viewport.BRX + 1 |
| v05 | DIFFER(474v479) | dl=160 | none | retail `sub esp, 0x18` / ours `sub esp, 0x14` | changed: long width = RenderTargetDimensions.X; \| long height = RenderTargetDimensions.Y; |
| v06 | DIFFER(477v479) | dl=136 | none | retail `sub esp, 0x18` / ours `sub esp, 0x14` | changed: if (viewport.TLX < 0) left = 0; else if (viewport.TLX > RenderTargetDimensions.X) left = RenderTargetDimensions.X; else left = viewport.TLX; \| if ((vie |
| v07 | DIFFER(534v479) | dl=155 | none | retail `sub esp, 0x18` / ours `sub esp, 0x1c` | changed: #define FABLE_MIN(a, b) ((a) < (b) ? (a) : (b)) \| #define FABLE_MAX(a, b) ((a) > (b) ? (a) : (b)) |
| v08 | SRC_FAIL | dl= | none |  | changed: long left = viewport.TLX < 0 ? 0 : (viewport.TLX > RenderTargetDimensions.X ? RenderTargetDimensions.X : viewport.TLX); \| long right = viewport.BRX + 1 |
| v09 | DIFFER(474v479) | dl=160 | none | retail `sub esp, 0x18` / ours `sub esp, 0x14` | changed: long height = RenderTargetDimensions.Y; \| long width = RenderTargetDimensions.X; |
| v10 | DIFFER(476v479) | dl=159 | none | retail `sub esp, 0x18` / ours `sub esp, 0x14` | changed: long top = 0; \| if (viewport.TLY >= 0) top = viewport.TLY > height ? height : viewport.TLY; |
| v11 | DIFFER(476v479) | dl=159 | none | retail `sub esp, 0x18` / ours `sub esp, 0x14` | changed: long width = RenderTargetDimensions.X; |
| v12 | DIFFER(466v479) | dl=167 | none | retail `sub esp, 0x18` / ours `sub esp, 0x14` | changed: long height = RenderTargetDimensions.Y; \| long top = viewport.TLY < 0 ? 0 : (viewport.TLY > height ? height : viewport.TLY); |
| v13 | DIFFER(474v479) | dl=160 | none | retail `sub esp, 0x18` / ours `sub esp, 0x14` | changed: long left, right, top, bottom; \| left = viewport.TLX < 0 ? 0 : (viewport.TLX > width ? width : viewport.TLX); |
| v14 | DIFFER(479v479) | dl=138 | none | retail `mov ecx, dword ptr [esp + 0x28]` / ours `mov ebx, dword ptr [esp + 0x20]` | changed: long left = 0, right = 0, top = 0, bottom = 0; \| if (viewport.TLX >= 0) left = viewport.TLX > width ? width : viewport.TLX; |
| v15 | DIFFER(476v479) | dl=159 | none | retail `sub esp, 0x18` / ours `sub esp, 0x14` | changed: long top = 0; \| long left; |
| v16 | DIFFER(469v479) | dl=154 | none | retail `sub esp, 0x18` / ours `sub esp, 0x14` | changed: long left = 0, right = 0, top = 0, bottom = 0; \| if (viewport.TLX >= 0) left = viewport.TLX > width ? width : viewport.TLX; |
| v17 | DIFFER(474v479) | dl=160 | none | retail `sub esp, 0x18` / ours `sub esp, 0x14` | changed: inline long ClampViewport(long value, long minimum, long maximum) \| { |
| v18 | DIFFER(474v479) | dl=160 | none | retail `sub esp, 0x18` / ours `sub esp, 0x14` | changed: long left = ClampViewport(viewport.TLX, 0, width); \| long right = ClampViewport(viewport.BRX + 1, 0, width); |
| v19 | DIFFER(442v479) | dl=139 | none | retail `sub esp, 0x18` / ours `sub esp, 0x10` | changed: long left = viewport.TLX; \| if (left < 0) left = 0; |
| v20 | DIFFER(446v479) | dl=140 | none | retail `sub esp, 0x18` / ours `sub esp, 0x10` | changed: if (left < 0) left = 0; else if (left > width) left = width; \| if (right < 0) right = 0; else if (right > width) right = width; |
| v21 | DIFFER(441v479) | dl=116 | none | retail `sub esp, 0x18` / ours `sub esp, 0x10` | changed: box.TLX = viewport.TLX < 0 ? 0 : (viewport.TLX > width ? width : viewport.TLX); \| box.BRX = viewport.BRX + 1 < 0 ? 0 : (viewport.BRX + 1 > width ? widt |
| v22 | DIFFER(488v479) | dl=123 | none | retail `sub esp, 0x18` / ours `sub esp, 0x14` | changed: long left = viewport.TLX < 0 ? 0 : (viewport.TLX > width ? width : viewport.TLX); \| long right = viewport.BRX + 1 < 0 ? 0 : (viewport.BRX + 1 > width ? |
| final | DIFFER(479v479) | dl=138 |  | retail `mov ecx, dword ptr [esp + 0x28]` / ours `mov ebx, dword ptr [esp + 0x20]` |  |


### 0x0053C332 grades

| tag | grade | dl | flags | first_difference(retail / VC7.1 emitted) | spelling_change_vs_previous |
|---|---|---|---|---|---|
| a | DIFFER(1128v1111) | dl=95 | /O1 /Oy- | retail `sub esp, 0x28` / ours `sub esp, 0xc` | first spelling |
| b | DIFFER(1128v1111) | dl=96 | /O1 /Oy- | retail `sub esp, 0x28` / ours `sub esp, 0xc` | changed: const C2DVector& offset = PDefPositionOffset; \| PositionOffsetToUse = offset; |
| c | DIFFER(1123v1111) | dl=37 | /O1 /Oy- | retail `lea eax, [esi + 0x1f0]` / ours `mov eax, dword ptr [esi + 0x1f0]` | changed: unsigned long state1 = 1, state4 = 4, state5 = 5, state0 = 0, state6 = 6; \| child->GetStates()[state1].Position = NextScrollingChildPosition; |
| d | DIFFER(960v1111) | dl=121 | /O1 /Oy- | retail `sub esp, 0x28` / ours `sub esp, 0xc` | changed: namespace { \| inline void SetStatePosition(NUISystem::CComponent* child, unsigned long state, const C2DVector& position) |
| e | DIFFER(1128v1111) | dl=92 | /O1 /Oy- | retail `sub esp, 0x28` / ours `sub esp, 0xc` | changed: const unsigned long state1 = 1, state4 = 4, state5 = 5, state0 = 0, state6 = 6; \| child->GetStates()[state1].Position = NextScrollingChildPosition; |
| f | DIFFER(1152v1111) | dl=97 | /O1 /Oy- | retail `sub esp, 0x28` / ours `sub esp, 0x10` | changed: PositionOffsetToUse = C2DVector(PDefPositionOffset); \| PositionOffset = C2DVector(PDefPositionOffset); |
| g | DIFFER(1119v1111) | dl=29 | /O1 /Oy- | retail `lea eax, [esi + 0x1f0]` / ours `mov eax, dword ptr [esi + 0x1f0]` | changed: const C2DVector& offset = PDefPositionOffset; \| PositionOffsetToUse = offset; |
| h | DIFFER(1119v1111) | dl=29 | /O1 /Oy- | retail `lea eax, [esi + 0x1f0]` / ours `mov eax, dword ptr [esi + 0x1f0]` | changed: NextScrollingChildPosition.Y = PositionOffsetToUse.Y + NextScrollingChildPosition.Y; |
| i | DIFFER(1128v1111) | dl=40 | /O1 /Oy- | retail `lea eax, [esi + 0x1f0]` / ours `mov eax, dword ptr [esi + 0x1f0]` | changed: unsigned long state1 = 1; \| unsigned long state4 = 4; |
| j | DIFFER(1121v1111) | dl=13 | /O1 /Oy- | retail `lea eax, [esi + 0x1f0]` / ours `mov eax, dword ptr [esi + 0x1f0]` | changed: unsigned long i = 0; \| if (ScrollingChildren.size() != 0) { |
| k | DIFFER(1121v1111) | dl=11 | /O1 /Oy- | retail `lea eax, [esi + 0x1f0]` / ours `mov eax, dword ptr [esi + 0x1f0]` | changed: float y = NextScrollingChildPosition.Y + PositionOffsetToUse.Y; \| NextScrollingChildPosition.Y = y; |
| l | DIFFER(1119v1111) | dl=14 | /O1 /Oy- | retail `lea eax, [esi + 0x1f0]` / ours `mov eax, dword ptr [esi + 0x1f0]` | changed: C2DVector& next = NextScrollingChildPosition; \| next.Y += PositionOffsetToUse.Y; |
| m | DIFFER(1119v1111) | dl=12 | /O1 /Oy- | retail `lea eax, [esi + 0x1f0]` / ours `mov eax, dword ptr [esi + 0x1f0]` | changed: if (i < ScrollingChildren.size()) { \| NextScrollingChildPosition.Y = PositionOffsetToUse.Y + NextScrollingChildPosition.Y; |
| n | DIFFER(1121v1111) | dl=13 | /O1 /Oy- | retail `lea eax, [esi + 0x1f0]` / ours `mov eax, dword ptr [esi + 0x1f0]` | changed: const C2DVector* offset = &PDefPositionOffset; \| PositionOffsetToUse = *offset; |
| km | DIFFER(1119v1111) | dl=10 | /O1 /Oy- | retail `lea eax, [esi + 0x1f0]` / ours `mov eax, dword ptr [esi + 0x1f0]` | changed: const C2DVector& offset = PDefPositionOffset; \| PositionOffsetToUse = offset; |
| km1 | DIFFER(1119v1111) | dl=10 | /Os /Oy- | retail `lea eax, [esi + 0x1f0]` / ours `mov eax, dword ptr [esi + 0x1f0]` | same body, other flags |
| km2 | DIFFER(1119v1111) | dl=10 | /Og /Os /Oy- | retail `lea eax, [esi + 0x1f0]` / ours `mov eax, dword ptr [esi + 0x1f0]` | same body, other flags |
| km3 | DIFFER(1119v1111) | dl=10 | /O1 | retail `lea eax, [esi + 0x1f0]` / ours `mov eax, dword ptr [esi + 0x1f0]` | same body, other flags |
| km4 | DIFFER(1119v1111) | dl=10 | /Os /Oy- /Ob1 | retail `lea eax, [esi + 0x1f0]` / ours `mov eax, dword ptr [esi + 0x1f0]` | same body, other flags |
| o1 | DIFFER(1470v1111) | dl=291 | /Os /Og- /Oy- | retail `sub esp, 0x28` / ours `sub esp, 0x80` | same body, other flags |
| o2 | DIFFER(1084v1111) | dl=15 | /O1 /Oy- | retail `lea eax, [esi + 0x1f0]` / ours `lea ecx, [esi + 0x1f0]` | changed: namespace { \| inline void SetOffsets(C2DVector& offset, C2DVector& offset_to_use, const C2DVector& def_offset) |
| o3 | DIFFER(1123v1111) | dl=11 | /O1 /Oy- | retail `lea eax, [esi + 0x1f0]` / ours `mov eax, dword ptr [esi + 0x1f0]` | changed: PositionOffsetToUse = PDefPositionOffset; \| PositionOffsetToUse.X *= -1.0f; |
| o4 | DIFFER(1119v1111) | dl=9 | /O1 /Oy- | retail `lea eax, [esi + 0x1f0]` / ours `mov eax, dword ptr [esi + 0x1f0]` | changed: PositionOffsetToUse.X = -PositionOffsetToUse.X; |
| p1 | DIFFER(1119v1111) | dl=10 | /O1 /Oy- | retail `lea eax, [esi + 0x1f0]` / ours `mov eax, dword ptr [esi + 0x1f0]` | changed: namespace { \| __forceinline void SetOffsets(C2DVector& offset, C2DVector& offset_to_use, const C2DVector& def_offset) |
| p2 | DIFFER(1131v1111) | dl=13 | /O1 /Oy- | retail `sub esp, 0x28` / ours `sub esp, 0x2c` | changed: PositionOffsetToUse = C2DVector(PDefPositionOffset); \| PositionOffsetToUse.X = -PositionOffsetToUse.X; |
| p3 | DIFFER(1111v1111) | dl=2 | /O1 /Oy- | retail `fstp dword ptr [esi + 0x190]` / ours `fstp dword ptr [esi + 0x190]` | changed: C2DVector& to_use = PositionOffsetToUse; \| to_use = PDefPositionOffset; |
| q1 | DIFFER(1119v1111) | dl=9 | /O1 /Oy- | retail `lea eax, [esi + 0x1f0]` / ours `mov eax, dword ptr [esi + 0x1f0]` | changed: PositionOffsetToUse.X = -PositionOffsetToUse.X; |
| q2 | DIFFER(1111v1111) | dl=4 | /O1 /Oy- | retail `fstp dword ptr [esi + 0x190]` / ours `fstp dword ptr [esi + 0x190]` | changed: to_use.X = -to_use.X; \| C2DVector& offset = PositionOffset; |
| q3 | DIFFER(1119v1111) | dl=10 | /O1 /Oy- | retail `lea eax, [esi + 0x1f0]` / ours `mov eax, dword ptr [esi + 0x1f0]` | changed: const C2DVector& def_offset = PDefPositionOffset; \| to_use = def_offset; |
| q4 | DIFFER(1111v1111) | dl=4 | /O1 /Oy- | retail `mov ecx, dword ptr [eax + 4]` / ours `mov ecx, dword ptr [eax + 4]` | changed: to_use = PDefPositionOffset; \| to_use.X = -PositionOffsetToUse.X; |
| q5 | DIFFER(1107v1111) | dl=4 | /O1 /Oy- | retail `fld dword ptr [esi + 0x190]` / ours `mov dword ptr [esi + 0x194], ecx` | changed: to_use.X = -PDefPositionOffset.X; |
| q6 | DIFFER(1115v1111) | dl=1 | /O1 /Oy- | retail `fchs ` / ours `fmul dword ptr [0]` | changed: to_use.X = to_use.X * -1.0f; |
| r3 | DIFFER(1111v1111) | dl=2 | /O1 /Oy- | retail `fstp dword ptr [esi + 0x190]` / ours `fstp dword ptr [esi + 0x190]` | changed: float x = to_use.X; \| to_use.X = -x; |
| r4 | DIFFER(1114v1111) | dl=5 | /O1 /Oy- | retail `mov ecx, dword ptr [eax + 4]` / ours `mov ecx, dword ptr [esi + 0x1f4]` | changed: to_use.X = PDefPositionOffset.X; \| to_use.Y = PDefPositionOffset.Y; |
| r5 | DIFFER(1119v1111) | dl=11 | /O1 /Oy- | retail `lea eax, [esi + 0x1f0]` / ours `mov eax, dword ptr [esi + 0x1f0]` | changed: PositionOffsetToUse = PDefPositionOffset; |
| r6 | DIFFER(1113v1111) | dl=11 | /O1 /Oy- | retail `fstp dword ptr [esi + 0x190]` / ours `fstp dword ptr [esi + 0x190]` | changed: to_use = PDefPositionOffset; \| C2DVector& offset = PositionOffset; |
| r7 | DIFFER(1111v1111) | dl=2 | /O1 /Oy- | retail `fstp dword ptr [esi + 0x190]` / ours `fstp dword ptr [esi + 0x190]` | changed: C2DVector* to_use = &PositionOffsetToUse; \| *to_use = PDefPositionOffset; |
| s1 | DIFFER(1102v1111) | dl=9 | /O1 /Oy- | retail `mov ecx, dword ptr [eax]` / ours `fld dword ptr [eax]` | changed: C2DVector& to_use = PositionOffsetToUse; \| to_use.X = PDefPositionOffset.X; |
| s2 | DIFFER(1111v1111) | dl=2 | /O1 /Oy- | retail `fstp dword ptr [esi + 0x190]` / ours `fstp dword ptr [esi + 0x190]` | changed: to_use = PDefPositionOffset; \| to_use.X = -(to_use.X); |
| s3 | DIFFER(1111v1111) | dl=2 | /O1 /Oy- | retail `fstp dword ptr [esi + 0x190]` / ours `fstp dword ptr [esi + 0x190]` | changed: to_use.X = (-to_use).X; |
| s4 | DIFFER(1115v1111) | dl=1 | /O1 /Oy- | retail `fchs ` / ours `fmul dword ptr [0]` | changed: to_use.X *= -1; |
| t1 | DIFFER(1111v1111) | dl=2 | /O1 /Oy- | retail `fstp dword ptr [esi + 0x190]` / ours `fstp dword ptr [esi + 0x190]` | changed: C2DVector& to_use = PositionOffsetToUse; \| to_use.X = -to_use.X; |
| t2 | DIFFER(1119v1111) | dl=10 | /O1 /Oy- | retail `lea eax, [esi + 0x1f0]` / ours `mov eax, dword ptr [esi + 0x1f0]` | changed: const C2DVector& def_offset = PDefPositionOffset; \| to_use = def_offset; |
| t5 | DIFFER(1119v1111) | dl=11 | /O1 /Oy- | retail `lea eax, [esi + 0x1f0]` / ours `mov eax, dword ptr [esi + 0x1f0]` | changed: C2DVector& to_use = PositionOffsetToUse = PDefPositionOffset; \| PositionOffset = PDefPositionOffset; |
| t6 | DIFFER(1111v1111) | dl=4 | /O1 /Oy- | retail `fstp dword ptr [esi + 0x190]` / ours `fstp dword ptr [esi + 0x190]` | changed: C2DVector& to_use = PositionOffsetToUse; \| to_use = PDefPositionOffset; |
| u1 | DIFFER(1131v1111) | dl=16 | /O1 /Oy- | retail `sub esp, 0x28` / ours `sub esp, 0x2c` | changed: to_use = C2DVector(PDefPositionOffset.X, PDefPositionOffset.Y); \| PositionOffset = PDefPositionOffset; |
| u2 | DIFFER(1102v1111) | dl=9 | /O1 /Oy- | retail `mov ecx, dword ptr [eax]` / ours `mov eax, dword ptr [esi + 0x1f4]` | changed: to_use.Y = PDefPositionOffset.Y; \| to_use.X = PDefPositionOffset.X; |
| u3 | DIFFER(1114v1111) | dl=9 | /O1 /Oy- | retail `fstp dword ptr [esi + 0x190]` / ours `fstp dword ptr [esi + 0x190]` | changed: to_use = PDefPositionOffset; \| PositionOffset.X = PDefPositionOffset.X; |
| u4 | DIFFER(1122v1111) | dl=10 | /O1 /Oy- | retail `sub esp, 0x28` / ours `sub esp, 0x2c` | changed: PositionOffset = C2DVector(PDefPositionOffset); |
| v1 | DIFFER(1111v1111) | dl=10 | /O1 /Oy- | retail `mov ecx, dword ptr [eax + 4]` / ours `mov dword ptr [esi + 0x188], ecx` | changed: PositionOffset = PDefPositionOffset; |
| v2 | DIFFER(1111v1111) | dl=2 | /O1 /Oy- | retail `fstp dword ptr [esi + 0x190]` / ours `fstp dword ptr [esi + 0x190]` | changed: PositionOffset = PDefPositionOffset; \| PositionOffset = PDefPositionOffset; |
| v3 | DIFFER(1114v1111) | dl=10 | /O1 /Oy- | retail `fstp dword ptr [esi + 0x190]` / ours `fstp dword ptr [esi + 0x190]` | changed: PositionOffset.Y = PDefPositionOffset.Y; \| PositionOffset.X = PDefPositionOffset.X; |
| w3 | DIFFER(1147v1111) | dl=71 | /O1 /Oy- /Op | retail `fstp dword ptr [esi + 0x190]` / ours `fstp dword ptr [esi + 0x190]` | changed: PositionOffset = PDefPositionOffset; |
| w4 | DIFFER(1111v1111) | dl=2 | /O1 /Oy- /G6 | retail `fstp dword ptr [esi + 0x190]` / ours `fstp dword ptr [esi + 0x190]` | same body, other flags |
| w5 | DIFFER(1111v1111) | dl=2 | /O1 /Oy- /G5 | retail `fstp dword ptr [esi + 0x190]` / ours `fstp dword ptr [esi + 0x190]` | same body, other flags |
| qp3 | DIFFER(1111v1111) | dl=2 |  | retail `fstp dword ptr [esi + 0x190]` / ours `fstp dword ptr [esi + 0x190]` |  |
| qs4 | DIFFER(1115v1111) | dl=1 |  | retail `fchs ` / ours `fmul dword ptr [0]` |  |
| final | DIFFER(1111v1111) | dl=2 |  | retail `fstp dword ptr [esi + 0x190]` / ours `fstp dword ptr [esi + 0x190]` |  |
