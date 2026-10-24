# Hand chunk 366 (_Uninit_fill_n .. _global): 40 of 40 byte-exact

All 40 rows grade MATCH (5) or RELOCATION_MATCH (35) under `gall.sh`. Every behaviour test prints `<addr>_TEST PASS`
under both g++ (`tall.sh`) and VC7.1/Wine (`tall.py`).

## What matched (40)

**Uninitialized fill family (13 rows; all carry the propagated `_Uninit_fill_n<pair<unsigned __int64,
CCountedPointer<CActiveEntityScriptBase>>>` label).** Each one is `__fastcall`: first in ecx, count in edx, then the
value reference and a by-value empty allocator, which gives `ret 8`. The body is
`for (; 0 < count; --count, ++first) new (first) T(value); return first;`. The element type comes from the inlined
copy constructor (vtable stores, CCharString copy 0x0099EC30, counted-pointer add-ref):
- 00a2aed0: CPhysicsMeshInfo. Base CMeshDataBankEntry vtable 0x0129CD28 then own vtable 0x0129CD50, plus an intrusive
  CIVCountedPointer MeshData. Needs a local virtual model because the engine header is flattened.
- 00a32fb0 and 00a6fe70 (byte twins): {unsigned long, {CCharString, CCountedPointer}}.
- 00a49c10: {CCharString, unsigned long, CCountedPointer}.
- 00a6b340: CIVCountedPointer. No allocator argument, so `ret 4`. Uses `engine/CIVCountedPointeeBase.h`.
- 00a6ff40: {CCharString, {CCharString, CCountedPointer}}.
- 00ae98a0: {unsigned long, CCountedPointer}.
- 00af6980: pair<NParticleEngine::EPSCType, CComponentInstance> (vtable 0x0129F478).
- 00b6b700 (`ret 8`) and 00b6bb10 (`ret 4`, no allocator): CVertexShader. The copy bumps Data->ReferenceCount.
- 00bedbb0: {unsigned long, CCompositeSurfaceInfo} (vtable 0x012A8154, Cache counted pointer).
- 00cba420: {unsigned __int64, CCountedPointer}. This is the only row that really is the instantiation the label names.
- 00cba4e0: {CCharString, CCountedPointer}.

**STLport containers and algorithms**
- 00a0e980, deque iterator `operator+(int) const` (8-byte element): `tmp = *this; tmp += n; return tmp;`, with
  operator+= out of line at 0x00A0E2A0.
- 00405bc0, `__copy_trivial_backward` (memmove wrapper). Byte twin of the landed 0x00410D60.
- 0040e350, `__copy<CActionInputControl*>(first, last, result, const random_access_iterator_tag&, int*)`. A counted
  loop; the tag and `int*` arguments give `ret 0xc`.
- 0040f6b0, `_STL::vector<char>::_M_insert_overflow(pos, x, const __true_type&, fill_len, atend)`. STLport vector.c
  verbatim: `(max)` picks by reference, allocate skips malloc when the length is 0, `__copy_trivial` is inlined as
  `memmove + n`, and fill_n is a memset intrinsic. Matched on the first grade. Unlike the unsigned-char twin
  0x00485B6C, this is a speed TU.
- 00412450, `vector<CWideString>::erase(first, last)`. The copy and destroy helpers must be `__forceinline` or VC7.1
  emits calls.
- 00412b70, `vector<CWideString>::resize(n)` → `resize(n, CWideString())` → erase 0x00412450 or insert 0x004126E0.
  The leaf must be the exact 1-argument symbol `?resize@?$CRetailVector@VCWideString@@@@QAEXI@Z`, because the
  2-argument inline is emitted as well.
- 004127e0, `deque<ETutorialCategory>::_M_push_back_aux_v`. Inline `_M_reserve_map_at_back` (calls
  `_M_reallocate_map` 0x00411630), malloc(0x80) node, placement new, `_M_set_node`.

**CRT and start-up**
- 00401312, `_RTC_Terminate`. A `__try { if (*f) (**f)(); } __except (1) {}` loop under `#pragma optimize("s", on)`
  reproduces the `__SEH_prolog` frame.
- 00401400, `vector destructor iterator` (__stdcall: ptr, size, count, dtor). Destroys from last to first.
- 004014a0, `CCRC::Calc`. Table CRC folded into the seed.
- 00401b20, GFFreeConfigDetection: GetProcAddress "Config_Release", FreeLibrary, clear the handle, return false.
- 00401b80, GFUninitialise. Fixed shutdown order, ending in a tail `jmp`.
- 00403480, WinMain (`/GS`). Named-mutex guard; the micro-thread stack is carved from a 0x32001-byte frame buffer.
  Needs `typedef unsigned short WCHAR` because VC7.1 has no native wchar_t.
- 00404440 CFileInstallerSingleton::Get and 00414ce0 CCodeSectionManagerSingleton::Get:
  `if (!g.m_pObject) g = new T; return g.m_pObject;`, with Reset reached through an inline operator=.
- 00413120, CGameInitialiser::SetupProgressDisplay. A temporary counted pointer is published with SetProgressDisplay;
  its release (`__fastcall` DeleteFunc) is inlined.
- 00415260, GetFontBankName. A switch on the Windows primary language id, in retail case order.

**CUserProfileManager (retail offsets: the STLport containers are smaller than the Ego_r.pdb Dinkumware ones)**
- 004082d0 GetProfileDir: save ProfileName (+8), swap in `name`, call GetPathNameFromFileName(g_ProfileFileName,
  create), restore. There is no NRVO, so the named path is copied out.
- 00406780 GetDebugSaveDir: `L"Data\\" + NResourceDirectoryNames::GetSaveDir()`.
- 00409090 GetMaxAntialiasing: needed 2 grades. The shape that matched is a `g_ConfigLevel != -1` then-block, a named
  CSystemManager local (a chained call gives `add eax,0x60`), and `if (index != -1) return index; return 0;`.
- 0040bb30 ProcessPendingTutorials: std::queue over the STLport deque at +0x98, pop_front 0x00410ED0,
  IsTutorialCategoryGiven 0x00407330, GiveTutorialForCategory(cat, false) 0x0040B790.

**Serialisation**
- 00405d60, CActionInputControl::TransferBinaryOut: five WriteSLONG (0x00993EB0) calls, then Write2DVector.
- 0040fca0, GFSerialiseVectorBinaryOut<CActionInputControl>: count, then each element's inline TransferBinaryOut.
- 00412d20 and 00412c70, GFSerialiseVectorBinaryIn<unsigned long / CActionInputControl>. An inline checked count read
  (`__int64` position test, memcpy fast path or ReadWithSrcChunkOverflow 0x00993CA0), then resize(n) → resize(n, T()),
  then a CPersistTraits loop. The traits object must sit in its own block scope so it shares the frame word.
- 0040e1e0, GetDefaultVal<CCharString>: `T value; return value;`. VC7.1 has no NRVO, so the named local is
  copy-constructed into the hidden result.

**IME**
- 00404e10, NUISystem::CIME::Callback_DrawFans: needed 3 grades. The mangled name equals FableWin.pdb's
  `?Callback_DrawFans@CIME@NUISystem@@SIXPBUIMEUI_VERTEX@@I@Z`. What made it match:
  - The three points are one `C2DVector points[3]` array; separate locals reverse the frame order.
  - The array is filled member by member; a float-pair constructor sends one Y through the FPU.
  - The colour bytes are cut with successive `color >>= 8`, which gives retail's shr/shr chain. A direct `>> 16`
    differs.
  - The display manager is a named local, so `+0x60` loads before the pushes and `+8` loads between them.

## Parked for Fable 5.1 (0)

## Skipped
None. No row holds two functions. The scanner extents fit every body. 00415260's extent includes its embedded jump
table.

## Label corrections
- 00a2aed0, 00a32fb0, 00a49c10, 00a6b340, 00a6fe70, 00a6ff40, 00ae98a0, 00af6980, 00b6b700, 00b6bb10, 00bedbb0,
  00cba4e0: the label `_Uninit_fill_n<pair<unsigned __int64,CCountedPointer<CActiveEntityScriptBase>>>` was
  propagated over byte-similar bodies. Each row's real element type is listed above. Only 00cba420 is correct.
- 00a0e980: `_Vb_iterator<allocator<bool>>::operator+` → STLport deque iterator operator+ (8-byte element).
- 00401312: `Init_CallFunctionPointers` → CRT `_RTC_Terminate`. Evidence: `push 0x401312 ; call atexit` at
  0x00401147. The twin at 0x004012CE is `_RTC_Initialize`.
- 00401400: `vector constructor iterator` → `vector destructor iterator` (it walks backwards calling the dtor).
- 00405bc0: `make_heap<CThing**,CThingCompare_BestForTargeting2>` → STLport `__copy_trivial_backward`
  (CopyTrivialBackward).
- 00405d60: `WriteToFile` → `CActionInputControl::TransferBinaryOut`.
- 00406780: `ConvertPathToFullPath` → `CUserProfileManager::GetDebugSaveDir`. Inferred from the PDB name pair
  NResourceDirectoryNames::GetDebugSaveDir / CUserProfileManager::GetDebugSaveDir and the body
  `"Data\\" + GetSaveDir()`.
- 004082d0: `GetDefaultVal<CSpecialEffectsStringMap>` → `CUserProfileManager::GetProfileDir`.
- 00409090: `Engine_FindPrimitiveInArray` → `CUserProfileManager::GetMaxAntialiasing`.
- 0040bb30: `Tutorial_CheckDivineWrathStrike` → `CUserProfileManager::ProcessPendingTutorials`.
- 0040f6b0: `CString_FillRange` → `_STL::vector<char>::_M_insert_overflow`. This agrees with
  labels_external966_corrections_2026-10-23; the only caller is 0x004110D0, vector<char>::_M_fill_insert.
- 00412c70: `GFSerialiseVectorBinaryIn<CBrainUpdateZone>` → `<CActionInputControl>`. The element is 28 bytes, its
  element read is 0x00410120, and the only caller is the TransferVector<CActionInputControl> body 0x00412DB0.
- 00412d20: `GFSerialiseVectorBinaryIn<CRGBColour>` → `<unsigned long>`. Any 4-byte T is ICF-identical, so the name
  is ambiguous.
- 00413120: `GFInitialise_SetupProgressDisplay` is descriptive; the real name is CGameInitialiser::SetupProgressDisplay
  on the 0x013B83D0 init object.
- 00404e10: catalog `_global::Callback_DrawFans` → `NUISystem::CIME::Callback_DrawFans` (static `__fastcall`,
  FableWin.pdb).
- Callee labels seen along the way:
  - 0x0099A350 `CXMVPlaybackInfo` → CInitBaseClass::Init.
  - 0x0099EAE0 `_Cons_val` → ~CCharString.
  - 0x00404A80 `AddChildPrimitive` → static-instance accessor (engine primitive manager).
  - 0x00407330 `CHitParameters::IsDivineWrathStrike` → CUserProfileManager::IsTutorialCategoryGiven.
  - 0x00993EB0 `WriteCBYTE` → CDataOutputStream::WriteSLONG.
  - 0x004129F0 `vector<CBrainBehaviour>::resize` → vector<CActionInputControl>::resize.
  - 0x0040FEF0 `CPersistTraits<float>` is ICF-ambiguous.
  - 0x00404CF0 (uncovered) is the IME dword→CRGBColour helper; 0x00404D20 is CIMEFont::SetColor (inferred: a
    thiscall with `ret 4` that writes Colour +4 through the same shr chain).
