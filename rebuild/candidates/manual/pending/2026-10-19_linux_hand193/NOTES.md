# Hand chunk 193 (CEngineTexturePalette .. CGameScriptInterface): 18 of 40 byte-exact

Every src/ row grades MATCH/RELOCATION_MATCH (gall.sh), its test prints PASS under g++ (tall.sh) and under the
lane's VC7.1 cl.exe (tall.py), and the relocation-identity check (`reloc_identity.check`, scratch
bin/relocchk.py) reports no HARD findings and no cross-row conflicts for the 18 rows. The remaining WARN lines are
propagated manifest labels. Two cross-row conflicts with landed rows were avoided by using the PDB signatures:
`CPlayer& CPlayerManager::GetMainPlayer()` (0x00449970) and a non-virtual `~CBaseClass` (0x0099A2E0). Both
landed bindings are wrong; see the label corrections below.

## What matched (18)

- **CEngineTexturePalette** 0x00BF9150 GetTexturePaletteIndex: an unsigned `for` loop that re-reads `size()` each
  pass. The new palette entry is a default-constructed local: its name is assigned from GetEntrySymbol's temporary
  (built in the dead `index` slot), then its index. push_back is the inline STLport form, with the overflow tag in
  the dead slot.
- **CWeaponDef ctor** 0x004E2612 (the catalog says CEngineVSConstantLayoutLights): a size TU
  (`optimize("s")`). Every member object has an inline ctor (particle set PeriodSecs `fldz`, two 12-byte
  CArrays, the trail set `or -1`, CFloatRange zero), with out-of-line map and CCharString ctors.
- **CMatrix3x4::operator*(float)** 0x0075B6C0 (the catalog says SetRadialBlurPositionParams): `CMatrix3x4
  r(*this); r.PostScale(s); return r;`. PostScale needs `__forceinline` and the column order E11,E21,E31,E41,E12,...
- **NCameraModes::CFirstPerson::OnInit** 0x008735D0: C3DVector has a user copy ctor and the implicit operator=
  (`lea` copies). The physics comes through an inline `GetPhysics()`, so it loads straight into ecx. The stick
  vector is assigned after the two constants. The camera cut is an inline manager member `CutCamera()`, which
  lets `this` live in esi.
- **CFishermanWasp::Main** 0x00E7BB30: the entity-script prelude with a block-scoped filter. The unused
  scripted-thing handle is a function-scope local after `if (IsDone()) return;`. Block-scoped, it shared the
  filter's frame words, where retail keeps a 0x20-byte frame. The call is `Game->GiveThingBestEnemyTarget(Me,
  Game->GetHero())`.
- **CGraphicInfoFrame::CFrameData::GetDebugMemoryInUse** 0x009FDCB0: the frame's own size comes from an inline
  `Sizeof()`, copied into the result local. The level is two Dinkumware `min` statements over long temporaries. The
  CSurface temporary is destroyed by its inline dtor, which calls Uninitialise.
- **CGameCameraManager::PushBuildingCameraMode** 0x0069AD10: nested ifs, a CDefPointer local whose dtor is the
  null-tested inline ReleaseTemporary, and `ObservedThingInBuilding = true` as the last statement.
- **CGamePlayerInterface** 0x00445800 GetButtonPressureForEvent: two `||` chains, with the ranges written as
  `>= && <=` terms. 0x00446A30 GetGameEvent: the body is wrapped in `if (!Suspended) {...} return false;` so all
  failures share one tail.
- **CGameScriptInterface**:
  - 0x006E7740 ctor: the init list is in declaration order, the CScriptThing ctor is inline, and the STLport map is
    the landed CRetailMap model.
  - 0x006E7920 CheckForCameraMessage: the typed intelligent-pointer getter (IsThingOfType<CThing> with the zero mask
    copied into a local) reproduces retail's `mov [slot],eax ; xor al,al ; jne`. The world's message manager is
    reached through its CCountedPointer member's `operator->`; a plain pointer member rotated the registers.
  - 0x0088E620 EntitySetFacingAngleTowardsThing: nested validity tests and the `atan2`/`fmod` intrinsics, wrapping
    the angle into [0, 1).
  - 0x00891170 PlaySoundAtPos: the CSoundInit ctor assigns PlaybackType and GainFaderBits in its body. The sample
    bank comes through an inline GetSampleBank().
  - 0x00891940 SetQuestInfoName and 0x00891A00 SetQuestInfoText (twins): the bank is a local, the symbol is an
    unnamed CCharString temporary whose inline GetCRC evaluates length then chars, and the group entry lands in the
    dead argument slot.
  - 0x00891CA0 GetHero: an if/else of two whole-object assignments. The inline CScriptThing dtor is expanded per arm
    and merged at the shared `operator delete`.
  - 0x00897560 GetPreviouslyWieldedMeleeWeaponName: the out-parameter GetInterface keeps the TC in eax, and
    CDefString::GetString is inline.
  - 0x00898410 SetHeroWeaponsAsUsable: `World->...` is read twice in one expression (VC keeps it in esi), and the
    region def is held by a CCountedPointer built from the raw `.Data` with an inline AddRef/Release on the def.

## Parked for Fable 5.1 (22)

- 008852e0 CEnvironmentSpellEffectInterface::CEnvironmentSpellEffectInterface DIFFER(183v183): store scheduling only (prev-frame screen-filter zeros after the tint, `push esi` placement, track-mode zeros after the second `lea`) | tried: member-ctor init, body assignments, chained `a = b = 0` (fixed the SF/RB order), inline CRGBFloatColour::Set | next: lane gap: store scheduling
- 00811760 NEntityEvents::CEventHitBase::CEventHitBase DIFFER(208v201): retail cross-jumps the outer failure exits into the inner epilogue (`pop edi`); ours emits a second `pop edi ; mov eax,ebp ; pop ebp ; ret 4` | tried: nested ifs, early returns | next: lane gap: tail cross-jumping
- 009c3590 CFileInstaller::StartReadingFile DIFFER(194v194): the dead read of the volatile ExitThread lands in cl (retail al), and GetFileSize's out pointer is formed in edx (retail ecx) | tried: `(void)` read, unused bool local, `|= true` | next: an inline getter whose bool return is discarded
- 00a636c0 CFontDataBank::NotifyEntryAdded DIFFER(208v203): retail stores the push_back zero once into the dead `index` slot and passes that slot as both the value and the `__true_type` tag; ours opens `sub esp,8` and stores the zero inside the loop | tried: rvalue `(T*)0`, named zero local | next: lane gap: tag/temporary over an argument slot
- 009fe460 CGraphicInfoFrame::CFrameData::FreeTextureMemory DIFFER(231v231): eax/ecx/edx rotation in the threshold chain and the ReduceMipmapLevel call (texture manager loaded after the pushes) | tried: threshold local, unused-count local (fixed the call order), manager local, bank local | next: inline GetBank() accessor or a reduce helper member
- 00f370c0 CGPUScratchBufferTextureManager::AddScratchPool DIFFER(171v167): retail carries the pool size in ecx out of both arms and stores the pool words once after the join; ours stores per arm and reloads | tried: per-arm member stores, split locals, ctor-built pool, declaration order | next: a ternary-built pool or an inline allocate helper returning the buffer
- 00f372b0 CGPUScratchBufferTextureManager::AllocResourceEntry DIFFER(229v224): retail keeps the address-taken index in esi as well as its frame home; ours only in the frame | tried: plain local, `long(index)` temporary | next: lane gap: enregistration of an address-taken local
- 00f37660 CGPUScratchBufferTextureManager::AllocteMemoryPage DIFFER(234v206): retail keeps the pool buffer/size in ebp/ebx across `vector<long>::operator=` and drops the local page's list dtor on the reuse path (callee knowledge); ours reloads and keeps `free` | tried: inline memberwise CResourcePage::operator=, same-TU auto_inline(off) operator= body (malloc/memmove) | next: the full STLport assignment chain (`_M_allocate_and_copy`, `__copy`) above the caller
- 006fd5e0 CGameCameraBase::GetRenderCamera DIFFER(212v212): retail reads the current position's Y/Z as `[esi+8]/[esi+0Ch]` (X through edi), ours `[edi+4]/[edi+8]`, plus one store order | tried: vector Lerp returning by value, scalar Lerp, `+=` Lerp, shared C3DVector local with SetInterpolated | next: pass `InternalCam.WorldPosition` through PeekWorldPos() or by value
- 00698b50 CGameCameraManager::PushUpManualCameraList DIFFER(221v218): ours fetches the observed thing (GetPItem) before pushing the sound string, and cross-jumps at the call | tried: temporary string argument, named string per arm, inline PlayCameraSound member (plain and forceinline) | next: the 0x00699A50 fix (a `const CCameraManagerDef* def = PDef;` local after the string) in each arm
- 00699130 CGameCameraManager::ResetFlags DIFFER(183v183): the inline list `clear()` tail only (the header reload goes into ebp, retail eax) | tried: STLport clear with header re-reads | next: lane gap: register choice
- 00699a50 CGameCameraManager::PushLookAround DIFFER(177v175): callee-saved colouring swapped (&PObservedThing in ebx, def in edi), and the shared constant 1 lands in al after copying the item to ecx | tried: direct call, def local (fixed the call order), observed-pointer local in both declaration orders | next: lane gap: register colouring
- 00699b00 CGameCameraManager::PreloadScriptedCamera DIFFER(243v238): only the by-value module parameter's release (`dec [eax] ; cmp [esi],0` vs `dec [esi] ; jne`) | tried: member dtor, local-copy dtor, nested ifs, SCountBlock::Release member | next: lane gap: parameter dtor block caching
- 0069adc0 CGameCameraManager::SetUpMainCameraManagerSetDef DIFFER(193v177): this/region-def register swap, and the set-def dtor's inline Release() shape | tried: hand add-ref, AddRef/Release members, copy-from-member, raw-pointer CDefPointer | next: combine the raw-pointer region holder with attempt 1's `--Object->m_refCount == 0` set-def dtor
- 0069e2e0 CGameCameraManager::SetupCameraMode DIFFER(231v237): retail widens the bool ctor argument through the dead key frame word (`mov [esp+0Ch],dl ; mov edx,[esp+0Ch]`), ours with `xor ecx,ecx ; mov cl` | tried: direct args, named angle/flag locals, `*begin()` list front (fixed the front access) | next: lane gap: byte argument widening
- 009f1ac0 CGameEventPackageSet::InitFromCompressedBuffer DIFFER(223v221): under /GS ours loads the buffer parameter at entry, and the loop padding moves with it | tried: long counts, unsigned char counts (fixed movzx), parameter copy local | next: lane gap: /GS parameter caching
- 004b1670 CManager::AddQuestCardEntry DIFFER(168v168): the `__find` result iterator gets a frame word (`sub esp,14h`) where retail homes it in the dead `name` slot | tried: (first,last,val,tag) and (first,last,tag,val) fastcall orders (the latter fixed the tag in the live bool slot), named iterator, forceinline Contains helper | next: lane gap: hidden-result temporary in a dead argument slot
- 008930f0 CGameScriptInterface::GetThingWithScriptName DIFFER(186v186): register rotation in the filter setup, `push edi` before the zero store, and the two result blocks in opposite order | tried: direct search call, inline search helper that fetches the tools first (fixed the order), ternary return | next: `if (notFound || !thing || dead) return CScriptThing(); return thing->GetScriptThing();` with the helper
- 008941a0 CGameScriptInterface::MsgOnTavernGameFinished DIFFER(168v176): the target's liveness test misses retail's `mov [slot],eax ; xor cl,cl ; jne` | tried: IS_THING_DEAD(CBaseObject* const&) returning false, bool local, IS_THING_ALIVE over the intelligent pointer, `return a && b && c` (fixed the int exits) | next: the 0x006E7920 typed getter (`GetPItem() && IsThingOfType<CThing>((T*)GetPItem()) ? (T*)GetPItem() : 0` with the mask copied into a local)
- 008967f0 CGameScriptInterface::SetThingAsUsable DIFFER(203v205): the hook arms keep separate tails; retail has one hook local in esi | tried: early-return form, blocks, HasTC+GetTC, else-if chain (fixed the key slots) | next: attempt 1's form (`if (!GetInterface(&hook, 0xc2)) {name; AddTC} hook->Usable = v;`) with both out pointers declared up front
- 008973a0 CGameScriptInterface::IsPlayerCarryingItemOfType DIFFER(214v211): the inline `CCharString::operator==("")` result is materialised with `sete` and re-tested | tried: if form, forceinline operator==, ternary | next: lane gap: the inline string compare materialised (GOTCHAS chunk 188)
- 00898340 CGameScriptInterface::SetHeroWillAsUsable DIFFER(195v193): the region def is held in eax and moved to ecx for the release (retail loads it straight into ecx) | tried: copy-from-member, raw-Data ctor with AddRef/Release, declaration order, World read twice (fixed the registers) | next: the twin 0x00898410 matched because its release follows a call. Try a small holder whose dtor releases (GOTCHAS chunk 169/171)

## Skipped

None. No row in this chunk spans two functions.

## Label corrections

- 0x004E2612 is the **CWeaponDef** default ctor (vtable 0x0124291C), not CEngineVSConstantLayoutLights. Its base
  ctor 0x0044C0C0 stores vtable 0x01231D54 = **CSubDefClassBase**; the landed rows call it CShoveHaPennyRegionDef.
  The map ctors 0x004DF8CB/0x004DF8E9 are labelled CDisplayManager::CopyBackBufferToTexture.
- 0x0075B6C0 is **CMatrix3x4::operator*(float)** (PDB `??DCMatrix3x4@@QAE?AV0@M@Z`), not
  CEnvironmentSpellEffectInterface::SetRadialBlurPositionParams.
- 0x008735D0 is **NCameraModes::CFirstPerson::OnInit**. 0x0084F1B0 is **NCameraModes::CBase::GetObservedThing**
  (labelled _Cons_val). 0x0084E420 is the **NCameraModes::CBase ctor** (labelled CActionParam_Constructor).
- 0x00811760 is the **NEntityEvents::CEventHitBase(const CHitParameters&)** ctor. 0x00812280 is
  CCountedPointer<CHitParameters>::Reset. 0x008688C0 is a CHitParameters damage-map test for attribute 4 (labelled
  CVertexGroupBlock::HasGroup). 0x007BEFF0 is TC 0x77's weapon getter (labelled _Cons_val).
- 0x009FDCB0/0x009FE460 are **CGraphicInfoFrame::CFrameData** members. 0x009F9E00 is a CTexture mip-surface getter
  (labelled GetNoLevels, which is really 0x009F9DF0). 0x00A00680 is CGraphicInfoFrame::DiscardDeviceIndependentResources
  (labelled CAnimatedBlock).
- 0x00699B00's mode ctor 0x0069DFE0 is **NCameraModes::CScriptedControl**(manager, def, CCountedPointer<
  CScriptModuleBase>&), labelled CActionParam...; 0x0069E3D0 (landed) builds the same mode. 0x0069DE30 is
  NCameraModes::CStringAndCage's ctor.
- 0x004B1670 "CGameScriptInterface::AddQuestCardEntry" is a member of the class of the landed 0x004B14F0 (list at
  +0x58, flag at +0x8F), not CGameScriptInterface. 0x004B91F0 is STLport `__find` over that list by name.
- 0x00449970 is the **non-const** `CPlayerManager::GetMainPlayer` (it calls GetPlayer 0x004498C0); 0x00449960 is
  the const one (it calls 0x00449910). reloc_bindings.tsv has them crossed (`QAE...` -> 0x00449960 in landed
  00435000, `QBE...` -> 0x00449970). `??1CBaseClass@@UAE@XZ` -> 0x0099A300 (landed 00418B7A) is
  ~CBaseClassNonCopyable; ~CBaseClass is 0x0099A2E0.
- 0x00404440 is `CTBaseSingleton<CFileInstaller>::Get` (labelled Get/CCodeSectionManager). Imports: 0x0143FE0C
  EnterCriticalSection, 0x0143FE10 LeaveCriticalSection, 0x0143FE2C CreateFileW, 0x0143FDF0 GetFileSize.
- CGameScriptInterface vtable slots (cgsi_slots_aligned.tsv, confirmed by the bodies): 7 NewScriptFrame, 70 GetHero
  (= 0x00891CA0), 451 GiveThingBestEnemyTarget, 474 EntitySetFacingAngle.

## Graphics / engine layer facts established here (for the rendering-layer work)

- **CEngineTexturePalette** (Ego_r.pdb; retail uses a 12-byte STLport vector): Bank +0 (CGraphicDataBank*), Palette
  +4 (8-byte CPaletteEntry {Index, CCharString Name}), DefaultIndex +0x10, RetailVersion +0x14.
- **CGraphicInfoFrame::CFrameData** (0x44, retail = PDB): CResource base, Frame +0x1C (CGraphicFrame{FrameData};
  CGraphicFrameData texture +8), ManagedTexture +0x20, LoadingManagedTexture +0x28 (data +0x2C, a CResource),
  FileData +0x30, Current/Loading/TouchedMipmapLevel +0x38/+0x39/+0x3A (0x7F = never touched), TouchedFrame +0x3C,
  GraphicInfoFrame +0x40. CGraphicInfoFrame: SmallMipmapLevel +6, GraphicInfo +0xC. CGraphicInfo: flags byte +0x12,
  bank +0x20. CGraphicDataBank: unused-frame thresholds +0x26C (free) and +0x270 (reduce), texture manager +0x280
  (CTextureManager::ReduceMipmapLevel 0x00A69F70). CSurface vtable **0x0122F84C** (0x10 bytes), with GetByteLength
  0x009F2FC0 and Uninitialise 0x009F2E20.
- **CGPUScratchBufferTextureManager** (retail: PDB CArrays 16 bytes become 12-byte STLport vectors): Resources +4
  (0x2C CResourceEntry), MemoryPages +0x10 (0x1C CResourcePage: ScratchPool, ResourceList vector +4,
  NextFreeBufferAddress +0x10, FreeSpaceSize +0x14, EnableAutoRestore +0x18), ScratchPools +0x1C (8-byte {Buffer,
  BufferSize}), FreeTexturePageSlots +0x28, FreeTextureSlots +0x34, ShutdownFlag +0x40. g_HeadlessRendering byte
  0x0143E968.
- **CFontDataBank**: retail entry table (STLport vector of CFontDataBankEntry*) at +0x120. **CFontDataBankEntry**
  vtable **0x0129DA70**, 0x18 bytes: CBaseClassNonCopyable vptr, Font +4 (CCountedPointer<CFontBank>), Bank +0xC,
  Index +0x10, DataType +0x14.
- **CMatrix3x4**: 0x30 bytes, E11..E43 row-major (4 rows of 3).
- **CGameCameraBase** (header agrees): InternalCam +4, LastInternalCam +0x3C (0x38-byte CCamera: WorldPosition +0,
  RHSet +0xC, locks +0x24/+0x25, FOVFlags +0x28, FOVs +0x2C/+0x30, Zoom +0x34), LastPointingAt +0x74, PointingAt
  +0x80. CCamera copy ctor 0x0049D380, PointAt 0x00A0C2B0.
- **CGameCameraManager** (retail; engine/CGameCameraManager.h has the PDB offsets): Camera +4, OldCamera +0x3C,
  World +0x74, CurrentMode +0x98 (CCountedPointer<NCameraModes::CBase>), ObservedThingInBuilding +0xAA,
  AvailableCamPoints +0xAC (STLport list header pointer), SingleCut +0xB0, PCurrentScriptModule +0xD4,
  PPendingScriptModule +0xDC, PObservedThing +0x100 (CIntelligentPointer), CameraList +0x108 (12-byte vector),
  CameraListIndex +0x114, UsingCameraManually +0x12C, BlockModeChange +0x12D, PDef +0x134.
- **NCameraModes::CBase** retail is 0xA4 bytes (PDB 0xC8): CameraManager +0x40, LeftStickVector +0x98. Vtable
  0x0124DD74. **CFirstPerson**: OffsetHeight +0xA4, CylinderRadius +0xA8, Base +0xAC, PrincipleAxis +0xB8, Theta
  +0xC4, Phi +0xC8, RSVector +0xCC. **COrbitGame** vtable **0x0125C6DC**, 0xBC bytes. CStringAndCage 0xD0 bytes,
  CScriptedControl 0xE8 bytes.
- **CEnvironmentSpellEffectInterface** (0x104, retail = PDB): Environment +0, RadialBlurParams +4, PositionParams
  +0x34, DisplacementParams +0x54, ScreenFilterParams +0x64, and PrevFrame copies at +0x84/+0xB4/+0xD4/+0xE4.
- **CWeaponDef** retail 0xE4 (PDB 0xF4): FlourishParticles +0x50 (0x2C), FlourishSpecialWeaponTrail +0x7C,
  WeaponTrails map +0x84, MissileLoadCarrySlot +0x94, ArrowTrails +0xB0, ReloadSpeedRange +0xBC, the sheathe anims
  +0xD4..+0xE0. **CRegionDef** retail: CameraManagerSetIndex +0x48, PreventHeroWeaponUse +0x70, PreventHeroWillUse
  +0x71. **CTCHero** (TC 0x29): WeaponsUsable +0xC, WillUsable +0xD, WeaponUse/WillUseOverriddenByScripts +0xE/+0xF.
