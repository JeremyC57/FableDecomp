# Hand chunk 627 (CVsyncCallbackTimer .. CTCShop): 4 of 10 byte-exact

Chunk 627 is a 10-row parked-retry chunk (every row previously parked with a same-length miss). All 4 matches are
RELOCATION_MATCH, behaviour tests PASS under g++ and the lane's VC7.1 cl.exe (tall.sh / tall.py).

## What matched (4)
- **CVsyncCallbackTimer::GetInfo 0x009F7050** (138 bytes, unoptimised TU) -- snapshot of the CPU/GPU vsync timings under the
  record's interlocked busy counter. Shape: VC7.1 `/Od` lays out locals by NAME (symbol-table hash order), not declaration
  order. Retail's frame (info ebp-0x30, record pointer ebp-0x34, `sub esp,38h`) needs both locals at function scope (no inner
  block) and a pointer name that sorts below `info`: `pRecord`/`r`/`gpu` work, `record`/`rec`/`timeRecord` give the padded
  0x3C frame. Name: the PDB says non-const (`QAE`); the earlier attempt's `const` mangled `QBE`.
- **CWorld::ResetToFrontEnd 0x0049DCC0** -- build the legacy/new front-end component, Init it, queue it as the main
  component's NextComponentToRun, SetQuit. Shape: Init() written in EACH arm (VC7.1 tail-merges them and both arms form the
  init address in ecx), plus the main component read into a named local before the store/call. Ctors take `CGame&` (both PDBs).
- **NGraphicAppearance::CTCGraphicAppearance::OnCreate 0x004BFC50** (vtable 0x0123986C slot 4; the class's PDB overrides are
  OnCreate/OnKill/Copy/HandleMessage/BulletTimeFrameUpdate/GetClassTypeName/GetInterfaceType, sibling slot 4 = OnCreate).
  Shape: GetInterface into a LOCAL out pointer then `PTCAnimation = animation;` under the found test (retail's component load
  into eax), together with the base def read through the inline getter `GetBaseDef()` (lever of the landed CatchMessage
  0x004BFD10). Either lever alone leaves a register residue. GetSubDef is the PDB template
  `CDefClassBase::GetSubDef<CAppearanceDef>(CDefPointer<const CAppearanceDef>*) const` as a same-TU stand-in.
- **CTCShop::IsShopKeeperNearerThan 0x007723F0** -- returns **bool** (both PDBs; `mov eax,1/xor eax,eax` is VC7.1's code for
  `return a && b && c;`). Shape: one bool return expression fixes the whole callee-saved map (hero edi, keeper in a late
  `push ebp`); the earlier `if (...) return 1; return 0;` gives hero ebp + shrink-wrapped edi. The XY distance is a C2DVector
  temporary's inline `X*X + Y*Y` (retail `fld st(1) ; fmul st(2)` twice; named dx/dy square dy first) in a __forceinline
  helper taking the two things. IS_THING_ALIVE is the PDB's single `template<class T> bool __fastcall IS_THING_ALIVE(const T&)`
  (`thing != 0 && !thing->Dead`) whose two instantiations mangle exactly as FableWin.pdb's.

## Parked for Fable 5.1 (6)
- 00994a80 CCompressedMemoryDataInputStream::SetPosition DIFFER(142v141): position in ebp saved at entry + chunk/pointer in shrink-wrapped edi (retail position edi, chunk ebp pushed on the inflate path, pointer in the dead ebx), CurrentSourcePtr stored before StreamPos | tried: PDB inlines CanSeekToPosWithinSourceChunk + SetPositionAssumeWithinSourceChunk (fixed the chunk-end lea), explicit old-position stores, rotated while loop, skip-size local, nested if/else, local names | next: the in-chunk branch as `else if` with the base SetPosition as the single tail, or a same-TU noinline InternalReadUnbuffered stand-in; budget spent before the 0x007723F0 single-expression lesson was found
- 00978b90 std::__adjust_heap<CThing**,int,CThing*,CThingCompare_InDirectionOfAndNearestToHeuristicBitch> DIFFER(142v142): comparator copy into __push_heap's args uses `mov ecx,esp` + edx/eax temps (retail `mov eax,esp` + ecx/edx, two-deep load pipeline) | tried: POD C2DVector, user C2DVector copy ctor (eax pointer but one-deep), non-POD value-ctor C2DVector, user comparator copy ctor, QFE 4035 | next: lane gap: by-value struct-argument copy scheduling
- 0085fbc0 NCreatureMode::IsAbleToBeEngagedInCombat DIFFER(142v142): shared `xor al,al ; pop edi ; pop ecx ; ret` laid after the 0x51 compare (retail: right after the first flags test, later checks jump back) | tried: one && chain with a __forceinline mode helper (151), negated first test wrapping the body, single-exit bool result, do/while(0) break ladder (plus the earlier split ifs, compound, goto, switch, optimize("s")) | next: lane gap: return-block placement (0x0085FB30 family)
- 00beeda0 std::__pop_heap<pair<pair<long,long>,CEngineTextureDecal>*,...> DIFFER(147v147): value.first.second loaded after `sub esp,1Ch` (retail before; two-deep copy pipeline) | tried: real STLport pair with its user copy ctor, POD halves, member-wise copy ctors, QFE 4035 | next: lane gap: by-value struct-argument copy scheduling (same as 00978b90)
- 00492ed1 CSubtitleRenderer::SetText(CCharString overload) DIFFER(152v152): ToWideString temporary homed in the dead pauseGame slot [ebp+1Ch] (retail own frame word [ebp-14h], sub esp,14h) | tried: optimize("g",off) (161), caption/parameter renames, const CWideString return | next: lane gap: argument temporary in a dead parameter slot next to frame locals
- 00475dfa GFSerialiseVectorBinaryIn<C2DVector> DIFFER(152v152): loop index esi / reloaded start edi (retail index edi / start esi) | tried: real FableWin template names, named start-pointer local, operator[] as *(_M_start+n) / _M_start[n] (index edi but `lea esi,[eax+edi*8]`, 153), toy sweeps of index type x operator[] form, while/post-increment loops, inline T& helper, local names | next: lane gap: index/base register choice

## Skipped
- none

## Label corrections
- 0x00BEEDA0: catalog `_Cons_val<pair<CCharString,C3DVector>>` is propagated; the body is STLport `std::__pop_heap` over
  `pair<pair<long,long>,CEngineTextureDecal>` (reloc_bindings has the right spelling; the chunk brief's "_Cons_val" hint is wrong).
- 0x00978B90: the cutter's `_Adjust_heap<...,_anon_FFAB493A::...>` is the Dinkumware (Ego_r) spelling; retail is STLport
  `std::__adjust_heap` (per-TU anonymous-namespace hash).
- 0x0085FBC0: "global::IsAbleToBeEngagedInCombat" -> NCreatureMode (both PDBs).
- 0x004BFC50: "vfunc_4" -> OnCreate.
- 0x00991620: manifest "CTCDParticleEmitter::EditFrameUpdate" is propagated; the body is a non-virtual bool setter (+0x26,
  notify slot 2) on the object at 0x013B8394 (called by CGameScriptInterface::EnableSounds 0x0088F620 with true/false). No PDB
  name provable (CASoundManager is an empty interface in Ego_r.pdb; the +0x26 bool's class is not in the PDB), so the call
  binds the landed body's spelling `?func_00991620@CFlaggedObject_Retail@@QAEX_N@Z` (follow-up 2026-10-31; the earlier
  descriptive `CASoundManager::EnableSounds_00991620` is gone). The landed 0x0088F620 binds the same address as
  `?SetMute@CSoundMgr@@QAEXH@Z` (int parameter; the body compares a byte) -- NAME_FIXES row.
- 0x013B8394: binds the established `?DAT_013B8394@@3PAUCGameScriptInterface_SoundProvider@@A` of the landed 0x0088F610 (no
  PDB data symbol; reloc_bindings carries ~10 other spellings for this address from other landed rows).
- 0x013B83D0: bindings `g_InitBase` / `g_GFInitialiseState_013B83D0` are wrong for this use; it is passed as `CGame&` (RTTI:
  CGame : CInitBaseClass, explaining the ~CInitBaseClass atexit chain). No PDB data symbol: descriptive `g_Game`.

## Names: proven vs descriptive
- Proven (PDB + second source): every row function; CVsyncCallbackTimerInfo; CGameComponent::Init/SetQuit; both front-end
  ctors; GetSubDef<CAppearanceDef>; SetLightingChannel; PeekPlayerManager / GetMainPlayer const / PeekPControlledCreature /
  GetPItem / GetPos; IS_THING_ALIVE<T>; Read2DVector; DecompressStream; CanSeekToPosWithinSourceChunk /
  SetPositionAssumeWithinSourceChunk.
- Descriptive: the vsync globals (0x013CB4B0.. 0x013CB4F4) and their record struct; `pRecord`; g_Game 0x013B83D0;
  the landed placeholder spellings func_00991620 / DAT_013B8394 (0049dcc0); C2DVector::GetSquaredLength and
  IsWithin2DDistance (inline helpers; the PDB's GFWorld2DDistanceSmallerThan takes positions, which would evaluate the
  keeper's position first).

## Header debt
- 004bfc50 now uses the shared retail def model (FABLE_RETAIL_DEF_CLASS_BASE: engine/CSubDefClassBase.h, CDefPointer.h) and
  engine/CDefinitionManager.h via the define pattern (adds the protected GetPDefFromGlobalIndex); the GetSubDef stand-in is
  the landed 0x0042B0A2 body verbatim. Still RELOCATION_MATCH (follow-up 2026-10-31). 007723f0 uses engine/CPlayer.h, CPlayerManager.h and CTCBase.h (define pattern).
- engine/CTCBase.h, engine/CDataInputStream.h, engine/CCompressedMemoryDataInputStream.h declare the vptr as `void*`, so
  polymorphic local models with the PDB member names are used (CDataInputStream via the `#define ... Layout` pattern in
  00475dfa / 00994a80). CWorld uses the shared header (CWorld_PDB define pattern).
- CMainGameComponent NextComponentToRun is retail +0x161C4 (PDB +0x161CC). CAppearanceDef LightingChannel retail +0x58 (PDB +0x5C).
- Landed 0x009F7020 declares CPUFrameCount/GPUFrameCount `unsigned long`; the PDB says `long` (no mangling impact).
