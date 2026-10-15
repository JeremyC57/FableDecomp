# Hand chunk 106 (CTargetPoint .. CThemeChannel): 39 of 40 byte-exact

Linux VC7.1 lane (Wine + DDK cl.exe 13.10.4035). 39 rows graded MATCH / RELOCATION_MATCH (`grades.txt`), each with a
behaviour test that prints PASS under g++ (`tall.sh`) and under the lane's VC7.1 cl.exe (`tall.py`). One row parked in
`differ/`, none skipped. Pragmas/flags: `#pragma optimize("s", on)` in 0x004E1537 / 0x0057FC71 / 0x0057FC9C (size-optimised
modules); the parked 0x00C202F0 needs `/EHsc`. Most catalog labels in this chunk were propagated and are corrected below;
line 1 of each source carries the real class/member name and a `// Leaf:` line selects the symbol.

## What matched (39)
- **CTextureManager** (0x00A696D0 Uninitialise, 0x00A698B0 FrameEnd): retail layout is 0x40 bytes shorter than the PDB
  (CPreallocTexturePool stride 0x54, FailedAllocations +0x58c, TextureBuffer +0x5d0) -- local model with real names.
  Uninitialise is `Initialised = false; if (TextureBuffer) TextureBuffer = 0;`. FrameEnd's `rep stosd` fill of
  FailedAllocations needs BOTH the count and the array pointer in named locals (member count: a real loop re-reading
  PoolCount; count local only: `lea edi` scheduled after the `jbe`).
- **CTexture** (0x009F9D00 copy ctor, 0x009F9E00 GetSurfaceLevel, 0x009F9E80 GetLevelByteLength): the PDB bitfield word
  (`ByteLength:28, AllocationSource:4`) gives the retail `((src ^ dst) & 0x0fffffff) ^ dst` merges; GetSurfaceLevel returns
  a CSurface by value from a local (inline ctor zeroes two words before the vtable store; no NRVO, so copy ctor + inline
  destructor); GetLevelByteLength is the CVolumeTexture::GetByteLength sibling (`bits * Width * Height / 8`).
- **CEngineResourceManager** (0x00B3B1F0 AllocateTexture, 0x00B3B5B0 FreeTexture): `new CTexture` (inline zeroing ctor) +
  Initialise(size, levels, format, 0, 1, false, false); `delete texture` with an inline `~CTexture() { Uninitialise(); }`.
- **CScopedObjectPointer** (0x00C098F0): destructor `if (m_pObject) { delete m_pObject; m_pObject = 0; }` (virtual dtor in slot 6).
- **CTattooDef** (0x0057FC71 GetTypeBalancedScariness, 0x0057FC9C GetTypeBalancedGoodstrength): /Os; `size() == 5` over a
  vector of 0x48-byte CAppearanceModifierScalingDef, `Field * scalings[4].Field`, engine/CTattooDef.h used.
- **CTCIdleScheduler** (0x004E1537): /Os; CTCBase(parent), two out-of-line map ctors, `Reset()` in the body.
- **CTCRegionFollower / CTCWound / CTCHeroGuide / CGraphicBankManager** (0x006AED80 / 0x00757640 / 0x00780070 /
  0x009F8690): base ctor, vtable, then the STLport list member's inline ctor (head nulled, malloc'd sentinel self-linked,
  stored); CTCWound's LastFrameChanged shares the zero register.
- **NUISystem::CTextContainer** (0x00546D30 ctor, 0x00546CD0 GetDataFromDef) and **NUISystem::CZoomingComponent**
  (0x00546AA0 GetDataFromDef): the landed CZoomingComponent / CScrollingViewport models (CChangingStateComponent with three
  vptrs, CDefPointer<const CUIDef> temporary released through DecRef/GetRefCount accessors).
- **NUISystem::CText** (0x0054F580 ResetToDefaultValueWithPrefix): `GetDataFromDef(); SetText(prefix + GetText(), false, true);`
  -- the trailing constants are pushed before the GetText call (right-to-left argument evaluation), fastcall operator+.
- **CChildListLoader** (0x00C2E270 AddChildren): `for (i < src->GetCount()) { AddChild(src->GetItem(i)); if (!status->IsOk()) return; }`.
- **CVectorMap instantiations** (0x004D1EB0 / 0x0081D7D0 Add, 0x0081B2A0 AddEntry): the landed 0x0077F270 family with a POD
  entry (two alternating copy registers); AddEntry = inline Add(entry.key, entry) + inline Sort() (SGI /Gr sort with the
  1-byte comparator pushed, Dirty=true kept before the call, Dirty=false after).
- **DirectShow BaseClasses** (CTextureRenderer rows): 0x00A3B590 CTextureRenderer::CheckMediaType (SDK sample without
  CheckPointer; GUID compares = intrinsic memcmp `repe cmpsd`), 0x00CA49F0 CBaseRenderer::GetSampleTimes, 0x00CA4B20
  CBaseRenderer::Render, 0x00CA5CF0 CBaseVideoRenderer::ScheduleSample, 0x00CA6020 / 0x00CA6050
  CBaseVideoRenderer::get_DevSyncOffset / get_Jitter (`__stdcall` on the IQualProp sub-object at +0xe0, `lea ecx,[eax-0E0h]`),
  0x00CA6580 CBaseRenderer::SourceThreadCanWait, 0x00CA7620 CBaseFilter::GetClassID (`this` = IBaseFilter sub-object),
  0x00CA77A0 CBaseFilter::StreamTime, 0x00CACD20 CUnknown::NonDelegatingRelease -- all the BaseClasses source verbatim.
- **CTestQuest** (0x004A89D0 copy ctor, 0x004A8F50 operator=): memberwise over six CCharStrings + ColourIndex, written by hand.
- **CTargetPointFilter_FurtherFromPosThan** (0x00805920 operator()): `(pos - *Pos).MagnitudeSquared() > Range * Range` with
  MagnitudeSquared grouped `X*X + (Y*Y + Z*Z)` (VC7.1 re-associates the plain left-to-right sum differently).
- **CThemeChannel** (0x006B5FB0 ctor): three CIDrawEnvironmentTextureSets of two STLport vectors each; matched only with the
  real `_Vector_base` + `_STLP_alloc_proxy` structure (a plain three-pointer ctor schedules the second element differently).
- **CTemporaryBuffer** (0x00A98910 dtor): three-case switch on AllocateType (pool Unlock+Free / scratch Unlock / delete[]),
  CBase dtor tail-jumped from each case.
- **CTeamSpawn** (0x00DCD160 ctor): the landed CPermanentBarrier entity-script shape plus an inline default CScriptThing
  (CBase() + vtable + PImp zeroing) and two CCharString default ctors.

## Parked for Fable 5.1 (1)
- 00c202f0 CVtable_012b3420_Derived_00c202f0::~CVtable_012b3420_Derived_00c202f0 DIFFER(74v74): the closing vtable stores come out `mov [esi],0x12B3420 ; mov [esi+8],0x12B3444` where retail stores +8 first, then +0 | tried: second base of a novtable derived class (null-checked this-adjust + 2 EH states, 90 bytes); one two-vptr base with a user empty inline dtor (74v74, order only); intermediate base with implicit dtor over two bases with user dtors, and a novtable user-dtor intermediate (right order, but null-checked this-adjust, 88 bytes) | next: a two-vptr base whose own vtables are stored +8 then +0 (e.g. `__forceinline` second-base destructor without the null check), else lane gap: vptr store order in an inlined MI base destructor

## Skipped
- none (no row holds two functions).

## Label corrections
- 0x009F9E00 "CTexture::GetNoLevels" is **CTexture::GetSurfaceLevel(unsigned long)** returning a CSurface (COM slot 18 GetSurfaceLevel).
- 0x00B3B1F0 / 0x00B3B5B0 "CTexture::Create / Destroy" (autonames) are the thiscall targets of CEngine::Allocate/FreeTextureFromEngineResourcePools
  (0x00B23F40 / 0x00B23F50 forward through the engine-resource record at 0x01436E98): named **CEngineResourceManager::AllocateTexture / FreeTexture**.
- 0x00C098F0 "CTexture::Uninitialise" is a scoped owner's **destructor** (only reference: a `jmp` from unwind funclet 0x01210AB3; ctor 0x00C098E0).
- 0x004E1537 "CTattooDef::CTattooDef" is **CTCIdleScheduler::CTCIdleScheduler(CThing*)** (vtable 0x012426C4); its callees 0x004DFAA7 / 0x004DFA89
  ("CDisplayManager::CopyBackBufferToTexture") are the two map default ctors and 0x008018B0 ("LTextBinTree_RebuildWalkerArray") is CTCIdleScheduler::Reset.
- 0x0057FC71 / 0x0057FC9C "CTattooDef::GetTypeBalancedAttractiveness" are **GetTypeBalancedScariness / GetTypeBalancedGoodstrength** (only 0x0057FC46 reads Attractiveness).
- 0x006AED80 / 0x00757640 / 0x00780070 / 0x009F8690 "CTextContainer::CTextContainer" are the constructors of **CTCRegionFollower / CTCWound /
  CTCHeroGuide / CGraphicBankManager** (vtables 0x0125D234 / 0x012698B4 / 0x0126B0F4 / 0x0129C7E4).
- 0x00546AA0 "CTextContainer::GetDataFromDef" is **NUISystem::CZoomingComponent::GetDataFromDef** (called by the landed CZoomingComponent ctor 0x00546B00).
- 0x00C2E270 "CTextContainer::AddChild" is a loader step (class unknown) iterating a source list; 0x00C2E190 is its AddChild, 0x00C307C0
  ("CTCInventoryBase::GetCanItemBeQuickAssigned") is a status `IsOk()` byte getter (+0x30).
- 0x004D1EB0 / 0x0081B2A0 / 0x0081D7D0 "CTextureMorphs::Add" are CVectorMap<K,V>::Add instantiations (24- and 20-byte entries) and an
  Add-then-sort member; 0x004D18E0 / 0x0081D620 ("OnInitLeaf") are the pair vectors' push_back, 0x0081E030 ("std::make_heap_Sift") is SGI sort.
- CTextureRenderer "vfunc_N" rows are DirectShow BaseClasses members: vfunc_2 CUnknown::NonDelegatingRelease, vfunc_3 CBaseFilter::GetClassID
  (IBaseFilter vtable), vfunc_4 CBaseFilter::StreamTime, vfunc_6 / vfunc_8 CBaseVideoRenderer::get_Jitter / get_DevSyncOffset (IQualProp
  vtable), vfunc_10 CBaseRenderer::SourceThreadCanWait, vfunc_21 CBaseVideoRenderer::ScheduleSample, vfunc_22 CBaseRenderer::GetSampleTimes,
  vfunc_42 CBaseRenderer::Render. Callees: 0x00CA4AA0 ("Audio_ProcessEventWithCallback") = CBaseRenderer::ScheduleSample,
  0x00CA5F50 ("AudioDevice_ComputeLatencyDelta") = CBaseVideoRenderer::GetStdDev.
- 0x004A8F50 "CTestQuest::CTestQuest" is **CTestQuest::operator=** (calls CCharString::operator= 0x0099EFB0).
- 0x00C202F0 "CTargetPoint::CTargetPoint" is a **destructor** of a no-RTTI class (deleting dtor 0x00C202D0); PDB CTargetPoint has no vtable.
- 0x009F49A0 ("CXMVPlaybackInfo") is CScratchBuffer::Unlock (`Locked = false`).
