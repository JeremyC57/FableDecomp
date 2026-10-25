# Hand chunk 483 (std::vector<unsigned char> .. CMatrix3x4): 27 of 40 byte-exact

All 40 rows are `_global` catalog rows. 27 grade RELOCATION_MATCH (gall.sh -> grades.txt) and pass their behaviour test under
g++ (tall.sh) and VC7.1 (tall.py). 13 are parked in differ/: 3 are LTCG-style lane gaps, and 10 are register, x87 or frame
differences that were still open after the 4-grade budget. Call targets were checked with calls.py; every import and
callee identity matches retail.

## What matched (27)
- **STLport vector helpers (12):**
  - `_M_range_insert`: 009b3630 (unsigned char), 009b5310, 009b54e0, 009e73f0.
  - `_M_insert_overflow`: 009b8dd0, 009e14b0, 009e77e0, 00a00c80, 00a01120.
  - `_M_fill_insert`: 00a00f50, 00a01290.
  - `__partial_sort`: 009b9030 (/Gr).
  - Shape: STLport 4.6 verbatim in namespace `std`. Every out-of-line helper is a same-TU noinline body under `auto_inline(off)` that never touches its tag, so tag temporaries are homed on the live argument slot.
  - `max()` takes the address of `fill_len` / `n`.
  - `__uninitialized_copy` needs `__forceinline`.
- **STLport deque<C2DVector> (3):**
  - 00a0ee50 `_M_reallocate_map`, 00a0f040 `erase`, 00a0faa0 `_M_fill_insert`.
  - Shape: `_M_map` / `_M_map_size` must be `_STLP_alloc_proxy` members, which fixes the `lea [eax+ecx+2]` operand order.
  - `erase` needs same-TU noinline `copy` / `copy_backward` so their results share the dead `last` slot.
- **STLport list / find_if (2):**
  - 009c71a0 `_List_base::clear`: node frees have no null test; sentinel frees do.
  - 009e6a70 `__find_if` over `pair<CCharString, CSymbolMap::CSymbolInfo>` with a group-name predicate: Compare's real body (0x00411570) under `auto_inline(off)` keeps the predicate's string block in esi across the calls.
- **CDisplayManager::EnumerateDisplayModes (009c02d0):**
  - `push_back` and `_M_insert_overflow` must be `__forceinline`.
  - `pair<iterator,bool>` with user ctors comes back through a hidden pointer.
  - A declared C2DExtentsI copy ctor fixes a register swap.
- **CFileInstaller::PostFileReadCompletion (009c4120):** TransferSize is read once into a local, because the stores could alias it.
- **CConsole::~CConsole (009ecf30):** the implicit CConsoleCommandLine dtor stores no vtable; the list is cleared twice.
- **CVsyncCallbackTimer ctor (009f70e0):** an unoptimised TU (`optimize("", off)`); 0.0 comes from the double pool.
- **CProfileManager ctor / dtor (009f9160, 009f9020):**
  - An inline accessor on a GFGetSystemManager() result leaves a dead `add eax,N`.
  - VC7.1 keeps the empty tree walk.
- **CMovieSubtitles::Play (009bd910, PDB name, 4 grades):**
  - GFGetSystemManager() is a named local.
  - The screen C2DBoxF is built from two C2DVector temporaries, which reproduces retail's x87 spill.
  - The font reference is a named local read before GetText().
  - The inline CSoundInit ctor assigns in the 0x0041C930 order (PlaybackType last).
- **TransferIn(CPersistContext*, CStringParser*, long*) (009e2590, 4 grades):**
  - An inline `GetSymbolMap()` accessor loads `this` before the pushes.
  - The operator literals are IMPLICIT CCharString conversions, which are re-addressed with `lea`.
  - `long rhs` is block-scoped per branch, so it shares the dead `value` slot.
- **InitializeMemorySystem (009f1d20, a boot.ini scanner whose results are dropped):** the '=' search must be an inline helper, or VC7.1 jump-threads past `p < equals`.
- **WordWrap_TokenizeA (00a10800):** the do/while counter is a second local copied inside the `> 0` test, so its store lands after the `jle`.

## Parked for Fable 5.1 (13)
- 009c05c0 CDisplayManager::EnumerateAntiAliasingModes DIFFER(699v699): CheckDeviceMultiSampleType argument setup: retail loads the MultisampleTypes / BackBufferFormats bases into eax/edx, reads i after `push 0` and fetches DeviceType into ecx before the format-table lookup; ours keeps i in edx (every later register name shifts by one) | tried: the indexed double loop as written (2 grades) | next: read DeviceType and the two array bases into named locals before the inner loop
- 009c33a0 GetMarkerFileName DIFFER(314v315): retail takes the hidden CWideString result pointer in esi (caller `lea esi,[esp+14h] ; call`) and never saves esi; ours passes it on the stack | tried: plain hidden-result return (1 grade) | next: lane gap: LTCG custom register convention
- 009fa620 CTexture::GetNextValidDimensions DIFFER(336v351): caps test `test byte ptr [eax+98h],2` (ours) v `mov ecx,[eax+98h] ; shr ecx,1 ; test cl,1`; ours also drops the store of 0 into a side that is already known 0, where retail goes `xor eax,eax ; mov edi,eax` | tried: `(caps >> 1) & 1`, a bitfield, `& 2`, an inline IsBitSet(value, 1), `__forceinline` HighestSetBit / GetNextPowerOfTwo, size = requested (4 grades) | next: read the caps word into a named unsigned local through a noinline same-TU accessor
- 00a04200 (anonymous)::TransformControllerInputFromRadiallyToRectangularlyLimited DIFFER(386v382): retail keeps the sqrt length on the x87 stack (`fstp st(2) ; fstp st(0) ; fst [esp+10h]`, 0xc frame); ours stores and reloads it (8-byte frame) | tried: GetLength through an inline sqrtf wrapper, direct sqrt, float and double length locals, a keep-alive function pointer (4 grades) | next: length as an inline-returned C2DVector member or `__forceinline` GetLength over a by-value copy
- 00a0c450 CCamera::GetAngleXZ DIFFER(288v288): cos(pitch) products: retail `fld st(2) ; fmulp st(1)` / `fmulp st(1)`, ours `fmul st(2)` / `fmul st(1)` + `fstp st(0)` | tried: operand order, named float and double locals, CSE of cos(pitch), `__forceinline` GetAngleXY/YZ, the QFE-4035 compiler (4 grades) | next: build the up vector through a C3DVector constructor taking (heading, pitch) polar arguments
- 00a0c570 CCamera::SetAngleXY DIFFER(380v373): ours CSEs &RHSet.Forward into edi (`push edi ; lea edi,[esi+18h]`) and multiplies the spilled sin/cos(xy) doubles late; retail forms each product as soon as its factor exists | tried: the SetAngleYZ v3 body shape (1 grade, same family) | next: share the fix found for 00a0c6f0
- 00a0c6f0 CCamera::SetAngleYZ DIFFER(318v315): &RHSet.Forward CSE'd into edi (retail recomputes `lea eax/ecx,[esi+0Ch]`) plus the `fld st(2) ; fmulp` v `fmul st(2)` product shuffle | tried: a named angleXY local (fixed the memory-form wrap), double locals for yz / cos(yz) / xy, a named forward temporary, QFE-4035 (4 grades) | next: an inline SetForward(const C3DVector&) member that takes &RHSet.Forward inside
- 00a0db60 GFInitMaths DIFFER(288v290): frame slot only: retail keeps tempi and tempf in separate slots (`sub esp,8`); ours packs them into one (`push ecx`) | tried: pointer walk `*entry++`, a union, pointer casts, a reassigned temp (4 grades) | next: tempi and tempf as two named locals of a small struct, or an address-taken temp passed to a noinline same-TU helper
- 00a0dcd0 CJoystickPositionTracker::HasPerformed360ClockwiseMove DIFFER(256v256): the step dot product: retail forms X first (`fld prev.X ; fmul [esi]`); ours always forms Y first | tried: X-first and Y-first source order, an inline Dot member, front() + operator[] copies (4 grades) | next: hold previous as two float locals or as a pointer into the deque
- 00a0fe80 IsNonBeginningChar DIFFER(814v737): retail takes ch in ax (`cmp ax,imm16` chain; callers `mov eax,ecx ; call` / `mov ax,[esi-2] ; call`); ours compares cx | tried: __fastcall wchar_t parameter (1 grade) | next: lane gap: LTCG custom register convention (/GL)
- 00a10390 WordWrap_FindNextLineW DIFFER(396v458): the callees IsHangulChar 0x00A10240, IsNonBeginningChar 0x00A0FE80 and IsNonEndingChar 0x00A10170 take their character in ax and return in al; the reference body also expands TrimSpaceBackward once | tried: full reference body with __fastcall callees (1 grade) | next: lane gap: LTCG custom register convention in the callees
- 00a10560 WordWrap_FindNextLineA DIFFER(433v433): one instruction: retail `cmp eax,esi` (esi = the zero register) on the MultiByteToWideChar result, ours `test eax,eax` | tried: `== 0`, `!result`, a late `truncated` declaration, an indexed default-char loop (4 grades) | next: compare the result against a register-held zero variable (e.g. the flags argument as a named `unsigned long flags = 0` passed to the call)
- 00a112f0 CMatrix3x4::TransformPlane DIFFER(316v316): retail spills the copied normal (x to the dead argument slot, y to [esp+8], z through eax) and reuses the rotated temporary (0x1c frame); ours keeps x/y/z on the FPU (0x24 frame) | tried: implicit-copy normal, `point = normal * D`, Rotate(normal).Normalise(), a user copy ctor on C3DVector, a z,y,x DotProduct (4 grades) | next: plane.Initialise through a noinline same-TU helper taking the normal by value

## Skipped
None.

## Label corrections
- 009bd910 "CEngineInternalPrimitiveBase_UpdateSubtitleDisplay" -> CMovieSubtitles::Play(float, bool) (PDB `?Play@CMovieSubtitles@@QAEXM_N@Z`; single caller 0x00628C56 passes the subtitles at +0x30).
- 009e6a70 "ParticleMorphs_FindEntryByKey" (and the t5 "std_binary_search_pair_by_string") -> std::__find_if over CSymbolMap's pair<CCharString, CSymbolInfo> with a GroupName-equality predicate. This is a linear find_if; the predicate name CSymbolGroupEquals is descriptive.
- 009e2590 "TransferIn" kept: the free `__fastcall` TransferIn(CPersistContext*, CStringParser*, long*), most likely CPersistTraits<long>::TransferIn (the class is not recoverable from the body).
- 009f1d20 "InitializeMemorySystem" names the caller's purpose (the 0x009F20B0 constructor). The body is a boot.ini name=value scanner whose results are compiled out (descriptive name).
- 0x009E5C60 (catalog / BSim "CTCVillage::OnInitialActivate") is CSymbolMap::UpdateDirty() const (sort-if-dirty of the symbol vector map; ICF-shared body).
- 0x009BE100 (BSim "list<CSubTitle>::clear") is list<CSubTitle>::pop_front; landed STLlist_popfront_009be100 agrees.
- 0x009917E0 "CSymbolMap::FindFirstSymbolWithVal" is also called on CMovieSubtitles::PSoundManager as the sound-bank-by-name lookup (an ICF-shared body).
- 00a112f0 -> CMatrix3x4::TransformPlane(CPlane&) const (its callee labelled "GetScaled" is C3DVector::Normalise 0x00A14440).
- 00a04200 -> (anonymous)::TransformControllerInputFromRadiallyToRectangularlyLimited(const C2DVector&).
- Joystick-input helpers:
  - 0x00A043A0 "GetDefaultVal<CEngineGraphic>" -> CInputEvent::GetJoystickPosition.
  - 0x00A04380 "CMessageEventFilter_IsOriginatedByThing" -> likely CInputEvent::GetMousePos.
  - 0x00A043D0 "GetFacingVector" -> likely CInputEvent::GetMouseMovement.
- CCamera:
  - 0x00A0C380 -> GetAngleXY; 0x00A0C3E0 -> GetAngleYZ.
  - 00a0c450 -> GetAngleXZ; 00a0c570 -> SetAngleXY; 00a0c6f0 -> SetAngleYZ.
  - 0x00A0C830 -> SetAngleXZ.
- 009fa620 -> static CTexture::GetNextValidDimensions(const C2DExtentsI&, const CPixelFormat&).
- 00a0db60 -> GFInitMaths.
- 00a0dcd0 -> CJoystickPositionTracker::HasPerformed360ClockwiseMove() const.
- Word-wrap helpers (descriptive names):
  - 0x00A10310 SkipSpaceForward and 0x00A10350 TrimSpaceBackward.
  - 0x00A10240 IsHangulChar and 0x00A10170 IsNonEndingChar.
  - 0x00A102B0 CanBreakAt(p, start).
- Catalog names of the matched STL rows: see STATUS.tsv (e.g. "Vector_PushBack" is `_M_insert_overflow`, "RenderList_*" are deque<C2DVector> members, "ParticleMorphs_Destructor" is CConsole::~CConsole).
