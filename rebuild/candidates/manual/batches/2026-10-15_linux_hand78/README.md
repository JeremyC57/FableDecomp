# Hand chunk 78 (CSubtitledMessage .. CSystemManager): 38 of 40 byte-exact

## What matched (38)

- **CSubtitledMessage** (004cdb70, ctor): PDB layout. Retail is 0x44 bytes because each `CVectorMap` is a 12-byte vector plus the Compare and Dirty bytes. Each map's inline constructor zeroes three words and Dirty (+0xd) and leaves the empty Compare byte (+0xc) alone. The init-list scalars and the `"UNKNOWN"` narrator come first. `PSpeechBank = registry->GetBank(1)` is a body assignment, so it comes after every member. Leaf `??0CSubtitledMessage`.
- **CSurface** (engine/CSurface.h through a `CSurface_Methods` struct, as in the landed CTexture::GetPixelFormat):
  - GetPixelFormat 009f2f60: GetDesc (slot 12), then the CPixelFormat built in the hidden result.
  - GetByteLength 009f2fc0: `GetPixelFormat().GetColourDepth() * desc.Width * desc.Height / 8`, with GetPixelFormat defined in the TU and auto-inlined.
  - GetPitch 009f3390: retail loads the width into esi *before* the depth call. That only reproduces when the width comes from an inline `GetWidth()` (own GetDesc). A direct `desc.Width` read multiplies from memory after the call.
  - Swizzle 009f4330/009f4360 (twins): `CSurfaceLockedRect rect = Lock(flags); PD3DSurface->UnlockRect();`.
  - LoadFromBitmapFileToPreallocatedSurface 009f2dd0: the `ConvertPathToFullPath` CWideString temp lives in the dead slot of the by-reference filename argument. The destructor is duplicated per return path.
  - operator= 009f2e70, InitialiseFromPreallocatedSurface 009f2ec0, InitialiseFromTextureMipmap 009f2f10: `Uninitialise()` (0x009F2E20) is defined in the TU and inlined by /Ob2. `AllocationSource = surface != 0` gives `xor edx,edx ; setne dl`. AddRef goes through the argument with no null test. The mipmap variant keeps the dead zero stores.
  - Copy ctor 009f2d60: needs a local polymorphic `class CSurface` (virtual dtor, vtable 0x0122F84C), because the header's `__vftable` data word cannot express the vptr store. Leaf `??0CSurface`.
- **CVolume / CVolumeTexture** (labelled CSurface):
  - 00a67480: CVolume::GetPixelFormat, GetDesc slot 8.
  - 00a67770: CVolume Initialise. The inline `Sizeof()` is `size = sizeof(CVolume); if (PD3DVolume) size += GetByteLength() + 4;`, which gives the hoisted `mov eax,8` and `add eax,0Ch`. A ternary gives `jmp`/`mov eax,8` arms instead.
  - 00a681f0: CVolumeTexture::GetByteLength(level). The CPixelFormat temp sits in the dead `level` slot.
- **DirectShow base classes linked for CTextureRenderer** (labelled CSurface; vtable 0x0129D04C slot 12):
  - 00ca77f0: `CBaseFilter::QueryFilterInfo`, a `__stdcall` COM method whose `this` is the IBaseFilter sub-object (m_pName/m_pGraph at +0x30/+0x34). It is modelled with the SDK MI layout CUnknown + IBaseFilter + IAMovieSetup.
  - 00ca7930 `CBaseFilter::NotifyEvent` and 00ca8b50 `CBaseInputPin::BreakConnect`: the SDK source as written matched on the first try.
- **DirectSound owner** (00c53310, labelled CSurface::Uninitialise): `if (p) { p->Release(); CoUninitialize(); p = 0; status = 0x80000000; }` with a dllimport CoUninitialize.
- **Map lookup** (006e9360/006e9390, labelled CSurface::GetHeight): `unsigned __int64 key = thing->m_uid; return &map.FindNode(key)->value;`. The key copy in a local is what gives the 8-byte frame.
- **CSurfaceCompositor**:
  - EndComposition 00f38930: two GFGetSystemManager fetches go into locals (display in edi, core in ebx).
  - CompositeQuad 00f39a50: `CTexture* textures[1]` is the `push ecx` frame slot, filled after the core fetch. `++m_count` gives load/inc/store.
- **CODA playback setters** (00c275e0, 00c27670, 00c49420, labelled CSurveyDialog::GetSoundsCurrentLayer): `CProfileLock lock(section); Helper(m_pSource, value);`. The float argument is passed through as a dword (`mov eax ; push eax`). The `__stdcall` helpers' targets (0x00C42FE0 / 0x00C445C0 / 0x00C42F50) all take a float, so the landed 0x00C35F20's `bool` parameter is really a float.
- **CSuspendableProcessMicrothread** (00a44620 dtor, 00a44840 SuspendSuspendableProcess): `#pragma optimize("", off)`, as in the landed scalar deleting destructor. CASuspendableProcess is restated with a virtual dtor (the header models the vptr as data). The leaf for the virtual is the full mangled name.
- **CSwappingStateComponent::Initialise** (00547360): base Initialise, then two stores.
- **CSymbolMap**:
  - 009e5090/009e50c0/009e5120/009e51c0 use the PDB CSymbolInfo {GroupName, EntryName, Val} in `pair<CCharString, CSymbolInfo>` (16 bytes); SLONGMap's retail 12-byte vector is at +4.
  - Lookup/FindSymbol: the inline map Find must read `end()` ONCE into a local, kept as the default result: `(it == last || key < it->first) ? last : it`. The caller re-reads end(). FindSymbol returns `long` (`xor eax,eax ; setne al`).
  - 009e50c0: retail hoists the key's data block across the Compare call, which looks like whole-program alias knowledge. It reproduces with `end` and `key = name.m_pStringData` locals read inside the guard of a `do/while`. A for-loop puts the key load above the guard.
  - 00991690/009916f0/00991720: plain for-loops over 0x24-byte records at +0x30. The owner is not the PDB CSymbolMap.
  - 009917e0: a `_stricmp` loop with the null-case-first text accessor; end re-read per iteration.
- **CSystemManager**:
  - DisplayCriticalMessage 009a4ed0: `CCharString text(message); MessageBoxW(0, text.ToWideString(), L"Application Error", 0);`. The string lives in the dead `message` slot.
  - SetAsActive 009a5270: the restore test must be its own `if` nested inside `if (active && !ApplicationActive)`. With one `&&` chain VC hoists the ApplicationActive store above the second branch.

## Parked for Fable 5.1 (2)

- 009f31c0 CSurface::SetPixel DIFFER(84v82): retail `imul eax,[x] ; mov ecx,[rect] ; mov edx,[ecx+8] ; imul edx,[y] ; add edx,eax ; add edx,[ecx+0Ch]` vs ours `imul eax,[x] ; mov ecx,[rect] ; mov edx,[ecx+8] ; imul edx,[y] ; mov ecx,[ecx+0Ch] ; add eax,edx ; add ecx,eax` (register choice of the pixel-address sum) | tried: `PMem + y*Pitch + x*depth`, `&PMem[y*Pitch + x*depth]`, `PMem + Pitch*y + depth*x` (all 86v82: depth copied to ecx, rect in eax), a named `long offset` local (84v82) | next: an inline `CSurfaceLockedRect::GetPixelAddress(x, y, bytesPerPixel)` helper, or a `const CRGBColour&` colour parameter; else lane gap: register allocation.
- 004e06c3 CShopDef::CShopDef DIFFER(62v62): retail `mov [esi+4Ch],eax ; mov [esi+50h],eax ; lea ecx,[esi+64h] ; mov [esi+54h],eax` vs ours `lea ecx,[esi+64h]` right after the `[esi+48h]` store | tried: vectors as inline-constructed members, the same under optimize("s"), six plain init-list words (lea above all six stores) | next: lane gap: member-constructor `lea` scheduled after a run of register-zero stores (GOTCHAS wave 24i).

## Skipped

None. Every row is a single function (no row spans int3 padding into a second body).

## Label corrections

- 004e06c3 "CSystemManager::CSystemManager": this is **CShopDef::CShopDef** (vtable 0x01241F9C = CShopDef; retail size 0xd0). The base constructor 0x0044C0C0 is labelled "CShoveHaPennyRegionDef" (propagated); it is a definition base with vtable 0x01231D54.
- 006e9360 / 006e9390 "CSurface::GetHeight": a 64-bit-id STLport map lookup on an unidentified owner (+0x10) that returns the mapped value's address. It touches no surface.
- 00a67480 "CSurface::GetPixelFormat": **CVolume::GetPixelFormat**.
- 00a67770 "CSurface::InitialiseFromPreallocatedSurface": **CVolume**'s initialiser. Its neighbours are ReleaseVolume 0x00A67420 and GetByteLength 0x00A674E0.
- 00a681f0 "CSurface::GetByteLength": **CVolumeTexture::GetByteLength(level)**.
- 00c53310 "CSurface::Uninitialise": a DirectSound owner's Uninitialise (Release, CoUninitialize). It follows the DirectSound initialiser 0x00C53260.
- 00ca77f0 "CSurface::CSurface": **CBaseFilter::QueryFilterInfo** (DirectShow strmbase, CTextureRenderer vtable 0x0129D04C slot 12).
- 00ca7930 "CSurface::GetRefCount": **CBaseFilter::NotifyEvent**.
- 00ca8b50 "CSurface::GetRefCount": **CBaseInputPin::BreakConnect**.
- 00c275e0 / 00c27670 / 00c49420 "CSurveyDialog::GetSoundsCurrentLayer": CODA playback float setters under the 0x0143BAFC profile lock.
- 0x009E3F60 (callee) "Graphics_CopyPixelsToBuffer": a CPixelFormat member that encodes one colour at a destination (`this` = the format).
- 0x00991840 (callee) "MessageMap::FindMessageByID": a vector-map find (+0x30) on the global 0x013B8394 that returns the entry's +4 word. CSubtitledMessage uses it to get its speech bank.
- 0x00C35F20 (landed as `Enable2DSceneRenderingOnRejectingObject(CRenderTarget*, bool)`): its setter 0x00C445C0 takes a float, so the second parameter is a float, not a bool. The bytes are identical either way.
- 0x00A44610 (callee) "CBase::CBase": this is `~CASuspendableProcess` (`jmp ~CBase` 0x0099A300). 0x00A44690 "_Cons_val<...>" is an unoptimised free wrapper around the microthread scheduler switch 0x009D8650.
