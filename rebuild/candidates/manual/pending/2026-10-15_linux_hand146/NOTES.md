# Hand chunk 146 (CMainGameComponent .. CMemoryAllocatorVariableSize): 31 of 40 byte-exact

All 31 matched sources grade MATCH/RELOCATION_MATCH from src/ (grades.txt) and their tests print PASS under g++ (tall.sh)
and the lane's VC7.1 cl.exe (tall.py, tall_vc.txt). Shared probe models: probe/mapinfo_model.h, probe/cmap_model.h,
probe/fontmgr.h (inlined into each src file; `inl.py` does the inlining for probes).

## What matched (31)
- CMainGameComponent (0x004174F1 InitParticleEngine, 0x004179EE ChangeMaxTextureSize): frame-pointer size TUs
  (`optimize("y",off)` + `("s",on)`). The particle bank name must be a NAMED CCharString local (`lea eax,[ebp-4] ; push eax`).
  The texture clamp is `Min<unsigned long>(size, caps->X)` over SIGNED operands: both become converted temporaries (one in the
  frame word, one in the dead argument slot) and the result is picked by address.
- CManagedTextureData::CreateTexture (0x00A697A0): request and result are two C2DExtentsI locals (user ctor -> hidden result);
  result fields stored to Width/Height, then copied back into the request local which is passed to CTexture::Initialise.
- NUISystem::CManager: GetUIScale (0x0041CF47) returns a C2DVector LOCAL through its user copy ctor (X at [ebp-8]);
  IsKeyHeld (0x0041E10D) keeps the input manager in a named local loaded before the state store; ~CManager (0x0041E641) is a
  size TU with a `__declspec(novtable)` 0x10-byte base and empty counted-pointer arguments default-built in their slots.
- CManager-labelled rows that are other classes: 0x00556980 is CRedefinerList::NotifyOfRedefinition (PDB signature fits:
  control, source redefiner); 0x009E2BA0 / 0x009E2C10 are CFontManager::GetStaticFont / GetStreamingFont (no-NRVO
  `CCountedPointer p = ...; return p;`); 0x004B14F0 (label RemoveThingActionParam) appends a param when its thing UID is absent:
  the search is an inline Find RETURNING the stop node, so `== end` is compared after both loop exits.
- CManagerShadowedSpotLight (0x00B493C0, real class; 0x0075F180 is CKeyCamera): C3DVector members copy through an inline
  memberwise copy ctor (direct [reg+N] stores); a plain POD colour copies through `lea ecx,[eax+28h]`.
- CMap: InitHeightMap (0x0081ED90, scoped_ptr reset: store new then delete old); SetEngineThemeAtIncludingExtraCells (0x0081EF30:
  `theme > 0` on unsigned, palette index taken into a LONG local); UpdateScriptedMapBrushes (0x00820AB0: STLport vector::erase
  inlined, the empty iterator tag needs a user ctor so its frame-byte temp is not zeroed); CreateScriptedMapBrush (0x00820F80:
  STLport push_back with `prev` read into a local, the new entry kept as its NODE pointer, map ID a named local);
  0x00C3F490 is ~CVtable_012b5c60 under /EHsc (IsRunning accessor, member with inline virtual dtor, base dtor call).
- CMapInfo (copy ctor 0x005110B0, Clear 0x004FDDE0, SaveBinary 0x004FB3E0): C2DBoxI needs an inline memberwise copy ctor;
  the CCountedPointer Reset clears Info before Data; the UID goes out through the 8-byte writer 0x00993EC0.
- CMaterialDef::GetRandomBerserkedSwordHitParticleEffect (0x008EE720): the random generator is a named local fetched BEFORE size();
  inlined `seed = GFROR13(seed*0x24A1+0x24DF)` and `n ? r % n : 0`.
- CMatrix3x4::InitialiseSkewedSymmetric (0x00A55D80): inline identity whose zeroes/ones are CHAINED assignments (store right to left)
  and the translation three separate statements.
- CMemoryAllocator (dtor 0x009F2310, GetAllocationSize 0x009F2390, InternalDeallocate 0x009F2920): the region offset is an UNSIGNED
  long before `>> 24` (`shr`); the debug-info accessor calls GetAllocationSize twice (first result dropped); 0xCD fill is memset.
- CMemoryAllocatorBase ctor (0x00A65AD0): the per-ID clear is a plain zeroing LOOP (`lea edi` before `mov ecx,100h`; memset loads ecx first).
- CMemoryAllocatorFixedSize (InternalAlloc 0x00A65F40, DeleteChunk 0x00A66160, CreateChunk 0x00A661D0): HeapAlloc/HeapSize/HeapFree
  through dllimport with GetProcessHeap; the 64-bit overflow test is an `unsigned __int64` sum; nested success path.
- CMemoryAllocatorVariableSize (GetFragmentation 0x00A64590, GetTotalMemoryAllocated 0x00A64DB0): the alignment globals
  0x013D28D8/0x013D28D4 are written first and a STATIC align helper reads them back (VC forwards the stored registers);
  the float quotient is a named local so `fdivp` precedes the pops.

## Parked for Fable 5.1 (9)
- 009f2a40 CMemoryAllocator::InternalAllocate DIFFER(120v122): retail `mov eax,[esp+4] ; cmp eax,1 ; jge` before the pushes then `mov ebp,eax ; add eax,14h`; ours loads size into ebp after the pushes and `lea eax,[ebp+14h]` | tried: if-clamp, ternary clamp, Max() inline, `total = Max+0x14; size = total-0x14` | next: `long n = Max(size,1L)` with Max written `a < b ? b : a`, total from n
- 008ee790 CMaterialDef::GetBlendedSwordHitEffect DIFFER(152v152): frame `sub esp,0Ch` v 8 (retail shares the find iterator's word with the double spill) and `fld 1.83 ; fdiv` before `fld speed ; fabs` | tried: float tanh expression, named double, scoped iterator, swapped operands, iterator temporary | next: lane gap: frame-slot sharing between an iterator and a double spill
- 0082a3e0 CMarkerInfo::IsReversedOnMap DIFFER(118v118): `lea ecx,[esp+4] ; lea esi,[eax+44h] ; push ecx` v retail `lea esi,[eax+44h] ; lea eax,[esp+4] ; push eax` | tried: Find-pointer read, GetTCOut out-param (fixed the read), const long& key | next: lane gap: scheduling
- 0081fa20 CMap::GetThemeMinCameraSizeZOffsetAt DIFFER(121v103): retail homes blend, def pointer and the float result all in the dead `pos` argument slot (no frame); ours opens `sub esp,8` and re-reads the def after `dec [eax+4]` | tried: Release() member, hand `--RefCount`, copied def local, CDefPointer object | next: lane gap: multi-local homing into a dead argument slot
- 0081fa90 CMap::GetThemeMinCameraSizeZOffsetAt DIFFER(121v103): twin of 0081fa20 (float at +0x84), same arg-slot homing | tried: the 0081fa20 best form | next: lane gap: multi-local homing into a dead argument slot
- 006f9790 CManager::GetEventTimeInSeconds DIFFER(141v141): `fst [esp+18h]` before the three pops in retail, after two pops (`[esp+10h]`, same slot) in ours | tried: named float result, direct return | next: lane gap: scheduling
- 006f9430 CManager::EventExists DIFFER(146v146): ours `mov eax,ecx ; lea edi,[eax+edx*8]` v retail `lea edi,[ecx+edx*8]` (alignment filler then differs) | tried: end() in the condition, hoisted end after/before begin, by-value key + unsigned conversion (fixed the key) | next: index loop over m_count or `&m_pData[m_count]`
- 006f95a0 CManager::GetExtraEventsBetweenTimes DIFFER(138v151): retail keeps the collector's two time words in a frame local reused as the for_each hidden result and tests the list pointer in place (`add eax,4 ; je`); ours forwards the arguments into the argument area and uses `lea ecx,[eax+4]` | tried: temporary collector, named collector, time-range sub-object + ternary accessor, `collector = for_each(...)` | next: time range as a by-value struct parameter, or a reference-returning list accessor
- 0041d03c CManager::RenderErrorMessage DIFFER(108v108): `fld [ebp-4] ; push ecx` v retail `push ecx ; fld [ebp-4]` for UpdateErrorMessage's float argument | tried: direct time store, double local | next: lane gap: scheduling

## Skipped
- none (no row contains a second function or a catch-funclet tail).

## Label corrections
- 0x0075F180 "CManagerShadowedSpotLight::CManagerShadowedSpotLight" -> CKeyCamera::CKeyCamera (no vtable, PDB CKeyCamera layout incl. EventName CCharString at +0x30).
- 0x00556980 "CManager::DeregisterAllScreens" -> NUISystem::CRedefinerList::NotifyOfRedefinition(const CActionInputControl&, const CKeyRedefiner*).
- 0x009E2BA0 / 0x009E2C10 "CManager::GetEmbeddedEventNames" -> CFontManager::GetStaticFont / GetStreamingFont (PDB symbols, CFontBank::PManager).
- 0x00C3F490 "CMap::CMap::Destructor" -> ~CVtable_012b5c60 (vtable store; matches the landed 0x00C3F3D0 header).
- 0x004B14F0 "CManager::RemoveThingActionParam": the body only appends (kept the catalog name; the real name is likely an Add).
- 0x0081F910 "CMap::CopyPos" is GetDominantEngineThemeAt(pos, &blend); 0x0063F3A0 GetDef<CThingPhysicalSwitchDef> is the engine-theme GetDef.
- 0x00993EC0 "CDataOutputStream::WriteCBYTE" writes 8 bytes (a 64-bit writer).
- Callee twins: 0x0049D800 (DoSizeof) is a random-generator accessor; 0x00A635E0/0x00A5F090 (GetAbilityLevel) are font-bank GetFontInfo;
  0x00AEAAE0 (GFGetPreMainMemoryUsed) is the emitter-database getter; 0x00A04410 (CDefaultEngineMemoryInit) is CInputEvent's ctor;
  0x0082E780 (~CPatchTesselationEdgeStrip) is ~CScriptedMapBrush; 0x0082E600 is CScriptedMapBrush(const C2DBoxI&).
- 0x0081FA20 / 0x0081FA90 share one label; they read different theme-def floats (+0x80 / +0x84, probably PDB Height / MinCameraHeight).
