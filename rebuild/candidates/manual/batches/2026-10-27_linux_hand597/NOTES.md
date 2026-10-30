# Hand chunk 597 (CParticleProgram .. CAudioContext): 8 of 10 byte-exact

7 rows in src/ (1 MATCH, 6 RELOCATION_MATCH), 1 row in retail_asm/ (RELOCATION_MATCH, purity `retail_asm`), 2 rows parked
in differ/. Every test prints `<addr>_TEST PASS` under g++ (tall.sh) and VC7.1/Wine (tall.py / behaviour_of), the
retail_asm and differ tests included. vland: 7/7 wins, no RELOC_IDENTITY (log tmp/vland.log).

## What matched (8)
- **zlib 1.2.1 deflate.c** -- 0x00C00E60 `longest_match` (MATCH, 359). Verbatim 1.2.1 C (no ASMV/UNALIGNED_OK). Retail's
  callers pass s in esi and cur_match in eax: a `static __declspec(noinline)` with a same-TU caller gets that private
  convention without /GL. The caller is a REDUCED deflate_fast match step (stand-in, never graded or landed).
- **DXUT ImeUi.cpp** -- 0x00CB3570 `ImeUi_ToggleLanguageBar` (RELOCATION_MATCH, 348). The DX SDK body; `bool bCheck` (not
  BOOL) keeps the check flag out of eax; prevRestore / iShowStatusWindow are function-local statics; ImmGetDefaultIMEWnd
  goes through the non-dllimport thunk (vland RELOC_ALIAS warning only, as the landed ImeUi rows).
- **Engine data markers** -- 0x00CD5170 `LoadDataFromMarker` (RELOCATION_MATCH, 352). Parses `<type><2-char name><value up
  to a space>` entries into CDataMarkerOutput::CElement (VariableName / DataAsString / DataAsULONG, default 10; a '#'
  entry with a numeric value stores GFCharStringToInt). Shape: CCharString's inline GetLength / GetAsCharArray written
  null-arm-first (`PStringData == 0 ? 0 / "" : ...`) give retail's jne order and the preloaded "" in later reads; the
  STLport push_back's `__false_type()` tag needs a user ctor (no zero store into the tag temporary).
- **libpk ambient sound** -- 0x00C62530 `CAmbientTrigger::Play` (RELOCATION_MATCH, 354; descriptive). Drops the old voice,
  returns when the source volume is <= 0, else picks a random offset inside the sound's minimum-distance sphere, builds
  the 0x50-byte play-parameter block (the inline ctor of the landed CSoundLoop_00c37840 model), sets position / volume /
  owner id / priority 1 / the distance range (min and max minus the offset length) and plays it. Shape: 0x00C62370 (the
  random point) is a same-TU `static __declspec(noinline) __stdcall` -- VC7.1 then gives it retail's private convention
  (hidden result pointer in esi, radius on the stack, `ret 4`) and its body byte-matches retail too; 0x00C62350 (random
  0..1) is defined earlier in the TU, which is what lets the helper keep theta on the x87 stack across that call.
- **Engine render** -- 0x00B9E750 `CEnginePrimitiveManagerStaticMeshesBase::RenderMeshPrimitiveBlocksDegenerateTriangles`
  (RELOCATION_MATCH, 361). Two inline `AttachTextureToStage(stage, 0)` unbinds, then every DegenerateTriangles static block
  through the inline six-argument `DrawBuffersIndexed`. Shape: the block count read once into a local (retail's
  count-down loop); the draw arguments as named locals in the order primitiveCount, startIndex, vertexCount (puts
  PrimitiveCount in ebx as retail). Uses engine/CIndexBuffer.h and engine/CTexture.h.
- **STLport 4.6** -- 0x00B85660 `std::__rotate<CEngineSubPrimitiveRenderer::CSortElement*, int, CSortElement>`
  (RELOCATION_MATCH, 356). _algo.c random-access __rotate verbatim, `__gcd` inlined, swap_ranges 0x00B85390 called
  __fastcall; explicit instantiation emits the template.
- **Cloth particles** -- 0x00AE3360 `CParticleProgram::Clear` (RELOCATION_MATCH, 360). Each `clear()` is STLport's
  `erase(begin(), end())` through `__copy_ptrs` (`__true_type` -> __copy_trivial / memmove for the long arrays,
  `__false_type` -> the __copy loop); members modelled at their Ego_r offsets as the landed ~CParticleProgram.
- **libpk OpenAL wrapper (retail_asm/)** -- 0x00C55DA0 `CAudioContext::GetPreferredDeviceName` (RELOCATION_MATCH, 348,
  /GS; descriptive). Asks winmm for the preferred waveOut device (waveOutMessage DRVM_MAPPER_PREFERRED_GET +
  waveOutGetDevCapsA) between retail's `pushad` / `popad` (0x00C55DB4 / 0x00C55DDA: VC7.1 never emits pusha/popa, so the
  original had `__asm pushad` / `__asm popad`), then finds the OpenAL device whose name starts with it (ALC enumeration
  list walk, or the single device string) and copies it out (strncpy 0x103 + NUL). GetDeviceString 0x00C55A60 and
  IsDeviceExtensionPresent 0x00C55A10 inline here (their landed bodies). `purity.classify` prints retail_asm for source
  and test.

## Parked for Fable 5.1 (2)
- 0x00C298D0 std::vector<CHandleBasedMemoryPool::CHandleInfo>::_Insert_n DIFFER(352v542): no instruction differs -- the catalog row ends at the first `ret 0Ch` (0x00C29A2D) but the function runs to 0x00C29AEE (two catch funclets Catch@00c29a30 / Catch@00c29a3c, then the spill / hole branches); graded over the true 542 bytes it is RELOCATION_MATCH (qx.py, /EHsc /Gr) | tried: full Dinkumware model, `_Xlen` with a throwing body (noreturn), `operator delete(void*) throw()` | next: re-cut the catalog row to 542 bytes and merge the two Catch rows; then it lands as is.
- 0x00CBF13C GetBestThingFromMarkerList DIFFER(355v355): one callee-saved register tie -- retail keeps `target` in edi (reused for the loop's origin position) and `origin` in ebx, this build the reverse (16 instructions differ by register name only; frame, slots, x87 code identical) | tried: 3 grades + ~40 compile-only probes (early-return vs positive alive test, named bool, inline AreBothAlive, aliases of origin, named marker reference, const_iterator typedef, hoisted iterator, named GetPos references, /O1 /Oy-, /Os /Oy-, /G6) | next: a use-count change on the target web (GOTCHAS "callee-saved ties follow use weight"), e.g. an inline CScriptThing accessor layer on target only.

## Skipped
None. 0x00CBF13C is a real function start (0x00CBF139 is the previous function's `ret 0Ch`; `push ebp ; mov ebp,esp`),
not a mid-function label: this script-library TU is a frame-pointer /Os TU without function alignment.

## Names
Proven (PDB name set + a second source):
- 0x00C00E60 `longest_match` -- zlib 1.2.1 source + deflate_state offsets + callers deflate_fast / deflate_slow.
- 0x00CB3570 `?ImeUi_ToggleLanguageBar@@YIXH@Z` -- Ego_r + FableWin name sets + DXUT ImeUi.cpp body.
- 0x00CD5170 `?LoadDataFromMarker@@YIXABVCCharString@@AAVCDataMarkerOutput@@@Z` -- both name sets; Ego_r donor order
  GetStringFromDataMarker, GetNumberFromDataMarker, LoadDataFromMarker = retail 0x00CD4FF0, 0x00CD5090, 0x00CD5170.
- 0x00CBF13C `?GetBestThingFromMarkerList@@YI?AVCScriptThing@@ABV?$vector@VCScriptThing@@V?$allocator@VCScriptThing@@@std@@@std@@ABV1@1_N@Z`
  -- both name sets (Ego_r donor 0x00CA0632, FableWin 0x035A0FDB); the only script global with retail's argument shape
  (hidden CScriptThing result, vector<CScriptThing>, two CScriptThing refs, bool: `ret 0Ch`). Parameter names descriptive.
- 0x00B9E750 `?RenderMeshPrimitiveBlocksDegenerateTriangles@CEnginePrimitiveManagerStaticMeshesBase@@CIXAAVC3DPrimitive2@@ABVC3DTextureStages@@@Z`
  -- both name sets + the landed caller RenderMeshPrimitiveBlocks 0x00BA1110 binding.
- 0x00B85660 `std::__rotate<CSortElement*,int,CSortElement>` -- the landed caller __merge_without_buffer 0x00B86140 binding +
  STLport 4.6 source + the CSortElement stride.
- 0x00AE3360 `?Clear@CParticleProgram@@QAEXXZ` -- both name sets + the landed ~CParticleProgram 0x00AE3670 binding.
- 0x00C298D0 `vector<CHandleBasedMemoryPool::CHandleInfo>::_Insert_n` (protected, IAE) -- callers insert 0x00C29B80 and
  the landed push_back 0x00C29C30; element from Ego_r.
- Callees proven this chunk: CCharString ctor/copy/dtor/operator=/GetSubString/Append, GFIsCharStringNumber,
  GFCharStringToInt (both name sets); CScriptThing ctor/copy/dtor/operator=/IsAlive/GetPos (engine/CScriptThing.h);
  C3DVector::AccurateNormalise; CMemoryDataOutputStream::Clear 0x00996B60 (both name sets; no Reset exists);
  CMovableResourceMemoryPool::FreeResource 0x00A62ED0 (Ego_r donor order + `jmp` to CHandleBasedMemoryPool::Free).
Descriptive (no PDB covers libpk):
- 0x00C55DA0 `CAudioContext::GetPreferredDeviceName` (class = the landed descriptive CAudioContext family; callers
  0x00C56792 / 0x00C56BED sit between OpenDevice(0)/CloseDevice and OpenDevice(name)).
- 0x00C62530 `CAmbientTrigger::Play` (class from the landed CAmbientTrigger::UpdateVolume 0x00C624C0); the same-TU helper
  0x00C62370 `GetRandomPointInSphere` (static, not this chunk's row).
- Globals: 0x0129BA3C epsilon spelled `g_epsilon_0129ba3c` (const float; no PDB name, ~49 landed spellings);
  0x0129AAF4 `g_emptyLiteral_0129aaf4` (most-used landed spelling).

## Label corrections
- 0x00B85660 manifest "RotateElements_CNavigationPosition" -> std::__rotate over CEngineSubPrimitiveRenderer::CSortElement.
- 0x00AE3360 manifest "construct" -> CParticleProgram::Clear.
- 0x00CBF13C autoname "FindNearestEntity_ByDirection" -> GetBestThingFromMarkerList.
- 0x00C55DA0 "Audio_FindWaveOutDevice" -> CAudioContext::GetPreferredDeviceName (descriptive).
- 0x00C62530 "CItem_UpdateWeaponGeometry" -> CAmbientTrigger::Play (descriptive).
- Callee labels proven wrong (all in NAME_FIXES.tsv): 0x00C62440 "CRegion::SetMiniMapRegionExitTextOffsetY",
  0x00C20480 / 0x00C204A0 / 0x00C20420 "CBrainOutput::SetAsPumpCombatActions" (one name on three addresses, long vs
  float), 0x00C13140 "CTCHeroExperience::GetBlockParryReward", 0x00C46F40 "TurnToPointer", 0x00C1BA60/0x00C1BA70
  weapon labels, 0x00C62370 "Quaternion_FromAxisAngle" (it is a random point in a sphere), 0x00CDBDC0
  "CCombatWheel_ReinitializeUintVector" (it is vector<CDataMarkerOutput::CElement>::_M_insert_overflow).

## Header debt
- No engine header: C3DPrimitive2, C3DTextureStages, N3DPrimitive::CStaticBlock/CBaseBlock, CRenderManagerCore,
  CShaderRenderManager (0x00B9E750); CCharString, CDataMarkerOutput (0x00CD5170); DXUT ImeUi globals; libpk classes
  (CAudioContext, CAmbientTrigger, CAmbientSource, CSoundPlayParams_00c37840, ...); zlib deflate_state.
- vland SHARED_HEADER_AVAILABLE warnings kept on purpose: CParticleProgram.h flattens the vptr and members, and the
  CMovableResourceMemoryPool / CMemoryDataOutputStream / CMovableResource headers are layout-only (no methods), so
  0x00AE3360 models them locally to declare Clear / FreeResource; CEngineSubPrimitiveRenderer.h has no nested
  CSortElement (0x00B85660); CRenderStateManager.h is the 0x3A3C Ego_r base only, CRenderManagerCore needs retail
  offsets past it (0x00B9E750); CEnginePrimitiveManagerStaticMeshesBase.h is only needed for `this` (the row is static).
