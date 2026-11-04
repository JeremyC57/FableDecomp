# Chunk 761 notes

53 rows: **47 RELOCATION_MATCH** (src/ + test/, vland read-only: WINS 47, every test PASS), **6 parked** (differ/):
2 DIFFER after 27 / 47 attempts, 2 third-party (libvorbis), 1 catalog bounds error, 1 duplicate COMDAT twin.

## Results

| address | name | grade | attempts | note |
|---|---|---|---|---|
| 00514eb0 | std::vector<C2DLineF>::vector (explicit instantiation) | RELOCATION_MATCH | 1 | /Gr; vland RELOC_CONFLICT warn: `__uninitialized_copy<const C2DLineF*,C2DLineF*>` exists at 0x00513BB0 and 0x00A11210 |
| 005b432c | CTCInventoryClothing::ClearClothingMenuEntries | RELOCATION_MATCH | 2 | overlay FABLE_RETAIL_CTCINVENTORYCLOTHING_MENUENTRIES |
| 00639900 | CTCRegionDisplay::~CTCRegionDisplay | RELOCATION_MATCH | 1 | |
| 006e2500 | NParticleEngine::CParticleUpdateParameters<ulong,ulong>::~CParticleUpdateParameters | RELOCATION_MATCH | 1 | |
| 0071a790 | CTCMapwho::~CTCMapwho | RELOCATION_MATCH | 1 | overlays CTCMapwho / CWorldMap / CThingMapManager |
| 0072e6c0 | CTCActionUseBuyHouse::~CTCActionUseBuyHouse | RELOCATION_MATCH | 1 | |
| 00779cd0 | CTCDExperienceOrb::~CTCDExperienceOrb | RELOCATION_MATCH | 5 | overlay FABLE_RETAIL_CDEFPOINTER_RESET |
| 0077cd30 | CTCHighlightItem::~CTCHighlightItem | RELOCATION_MATCH | 1 | overlay FABLE_RETAIL_CTCHIGHLIGHTITEM |
| 0079a620 | CTCSummonSpell::~CTCSummonSpell | RELOCATION_MATCH | 1 | |
| 0079c200 | CTCSummonableCreature::~CTCSummonableCreature | RELOCATION_MATCH | 1 | |
| 007a4310 | CTCWillEnflame::~CTCWillEnflame | RELOCATION_MATCH | 1 | |
| 007a5460 | CTCForcePushPower::~CTCForcePushPower | RELOCATION_MATCH | 1 | |
| 007d5e80 | CTCTavernGameCardBase::~CTCTavernGameCardBase | RELOCATION_MATCH | 1 | |
| 007f8860 | CTCParticleAttacher::DetachParticles | RELOCATION_MATCH | 2 | |
| 00850330 | NCameraModes::CPhysicsCollisionPointsObject::~CPhysicsCollisionPointsObject | RELOCATION_MATCH | 1 | SHARED_HEADER_AVAILABLE warn (header debt) |
| 008bd610 | CTCWillLightning::SetPItemRemoved | RELOCATION_MATCH | 1 | |
| 0098f090 | CThreadedFile::~CThreadedFile | RELOCATION_MATCH | 1 | |
| 0098f380 | CThreadedFileManager::BeginThread | RELOCATION_MATCH | 1 | |
| 009918f0 | CASoundManager::ClearBanks | RELOCATION_MATCH | 1 | |
| 009a8fe0 | std::pair<const CCharString,CBankFileManager::CBankInfo>::~pair | RELOCATION_MATCH | 1 | |
| 009aa440 | (twin of 009a8fe0) | PARKED | 1 | duplicate COMDAT, graded RELOCATION_MATCH with the 009a8fe0 source |
| 009c5870 | CFileInstaller::BeginThread | RELOCATION_MATCH | 1 | |
| 009cc6c0 | CBankFileEntryUpdateData::~CBankFileEntryUpdateData | RELOCATION_MATCH | 4 | |
| 00a0b060 | CRenderManagerCore::~CRenderManagerCore | RELOCATION_MATCH | 1 | overlays CRenderManagerCore (+ new CSystemFont.h) |
| 00a26830 | C3DAnimationInfo::BuildSharedAnimation | RELOCATION_MATCH | 1 | overlays C3DAnimationInfo / CResource |
| 00a26aa0 | C3DAnimationInfo::Preload | RELOCATION_MATCH | 1 | as above |
| 00a5ec10 | CPixelShaderData::~CPixelShaderData | RELOCATION_MATCH | 1 | overlay FABLE_RETAIL_CPIXELSHADERDATA |
| 00a63680 | CFontDataBank::~CFontDataBank | RELOCATION_MATCH | 1 | |
| 00aa7710 | CChunkedFileRawDataChunk::~CChunkedFileRawDataChunk | RELOCATION_MATCH | 1 | overlay FABLE_RETAIL_CCHUNKEDFILERAWDATACHUNK |
| 00ab5fc0 | CKeyboardDX::~CKeyboardDX | RELOCATION_MATCH | 1 | |
| 00abf800 | C3DMeshMaterial::~C3DMeshMaterial | RELOCATION_MATCH | 1 | |
| 00af7c10 | NParticleEngine::CDecal::~CDecal | RELOCATION_MATCH | 2 | overlay FABLE_RETAIL_CDECAL |
| 00b2b000 | CEngineSurfaceCompositionManager::BeginBankUpdate | RELOCATION_MATCH | 6 | engine/CEngine.h retail branch; local manager model (header debt) |
| 00c1d500 | _global::JenkinsHashBuffer | RELOCATION_MATCH | 1 | Bob Jenkins lookup2 (public-domain hash, Fable-side copy) |
| 00c206e0 | CSoundEvent::GetPlaybackKind | RELOCATION_MATCH | 1 | |
| 00c222d0 | CSoundEventSet::CSoundEventSet | RELOCATION_MATCH | 1 | /EHsc |
| 00c295b0 | std::vector<CHandleBasedMemoryPool_012b4098::CHeader*>::_Insert_n | RELOCATION_MATCH | 2 | /Gr, noreturn _Xlen |
| 00c42a20 | _global::RandomBelow_00c42a70 | RELOCATION_MATCH | 2 | dllimport rand |
| 00c491e0 | CCodaFrame::CCodaFrame | RELOCATION_MATCH | 1 | /EHsc; `operator new[]` declared |
| 00c49820 | CCodaFrameStreamer_00c49650::CCodaFrameStreamer_00c49650 | RELOCATION_MATCH | 2 | /EHsc; extern "C" CODA helpers |
| 00c4c440 | CMasterObject_00c4cd80::Complete | RELOCATION_MATCH | 1 | /EHsc |
| 00c4cab0 | CMasterObject_00c4c8b0::LoadSlave | RELOCATION_MATCH | 2 | /EHsc |
| 00c4d240 | CRenderLayerWin32::CreatePooledEvent | RELOCATION_MATCH | 2 | single exit |
| 00c54c10 | CAudioSource::ApplyParameters | DIFFER(651v651) | 47 | see parked |
| 00c565f0 | CAudioContext::Initialise | RELOCATION_MATCH | 1 | /GS |
| 00c78150 | CReverbProcessor::UpdateTapDelays | RELOCATION_MATCH | 13 | retail_asm via libpk/FtoL.h (table below) |
| 00c89850 | CReverbFilterTap::ProcessCrossfade | DIFFER(607v588) | 27 | see parked |
| 00c8e2e0 | libvorbis::_vorbis_unpack_books | PARKED | 0 | third-party: libvorbis |
| 00c95a10 | libvorbis::_best | PARKED | 0 | third-party: libvorbis |
| 00ca7780 | CUnknown::~CUnknown | RELOCATION_MATCH | 1 | DirectShow strmbase |
| 00cb9200 | std::pair<uint64,CPersistInfo> scalar deleting dtor | RELOCATION_MATCH | 1 | `delete p` on a non-polymorphic pair emits ??_G |
| 00d6bb80 | NScript::CQ_HangingTreeEvilScript::SpawnEnemies | RELOCATION_MATCH | 2 | now on engine/C3DVector.h + engine/CRGBColour.h (no shared-header warnings) |
| 00d6bb91 | (inside 00d6bb80) | PARKED | 0 | catalog bounds error |

## Header overlay ($D/hdr/engine; every change behind a new FABLE_RETAIL_* macro, default path unchanged)

hdrcheck2 `$D/hdr`: **default=same for all 12 changed headers** (CDefPointer.h has no plain default -- it #errors without
FABLE_RETAIL_DEF_CLASS_BASE -- and is `FABLE_RETAIL_DEF_CLASS_BASE=same[94]`); CSystemFont.h is NEW. No file in rebuild/
defines any of the new macros.

| header | macro | for |
|---|---|---|
| C3DAnimationInfo.h | FABLE_RETAIL_C3DANIMATIONINFO | 00a26830, 00a26aa0 |
| CChunkedFileRawDataChunk.h | FABLE_RETAIL_CCHUNKEDFILERAWDATACHUNK | 00aa7710 |
| CDecal.h | FABLE_RETAIL_CDECAL | 00af7c10 |
| CDefPointer.h | FABLE_RETAIL_CDEFPOINTER_RESET (inline Reset(T*)) | 00779cd0 |
| CPixelShader.h | FABLE_RETAIL_CPIXELSHADERDATA | 00a5ec10 |
| CRenderManagerCore.h | FABLE_RETAIL_CRENDERMANAGERCORE_SYSTEMFONT (typed CSystemFont member) | 00a0b060 |
| CResource.h | FABLE_RETAIL_CRESOURCE_RESOURCELIST (RemoveFromResourceList decl, 0x009FC440) | 00a26830, 00a26aa0 |
| CSystemFont.h (new) | -- | 00a0b060 |
| CTCHighlightItem.h | FABLE_RETAIL_CTCHIGHLIGHTITEM | 0077cd30 |
| CTCInventoryClothing.h | FABLE_RETAIL_CTCINVENTORYCLOTHING_MENUENTRIES | 005b432c |
| CTCMapwho.h | FABLE_RETAIL_CTCMAPWHO | 0071a790 |
| CThingMapManager.h | FABLE_RETAIL_CTHINGMAPMANAGER | 0071a790 |
| CWorldMap.h | FABLE_RETAIL_CWORLDMAP_THINGMAPMANAGER (inline GetThingMapManager) | 0071a790 |

Siblings: 46 landed includers rechecked with `vland.py --recheck-landed` on the overlay mirror (SIBLINGS.tsv).

## Header debt

- engine/CString.h has no default ctor over the shared nil string 0x0139C900 (six competing data spellings: g_pDebugBuffer,
  g_pchNil_0139c900, _afxPchNil, ...), so the libpk trace rows (00c491e0, 00c49820, 00c4c440, 00c4cab0) keep a local
  CString model (vland SHARED_HEADER_AVAILABLE CString). One nil spelling + a default ctor in the header would retire them.
- CEngineSurfaceCompositionManager (00b2b000), CPhysicsCollisionPointsObject (00850330), CRuntimeScriptedEntityPersist
  (00cb9200) keep local models (warnings only).
- 00ca7780 warns SHARED_HEADER_AVAILABLE CBaseObject: a false positive -- the strmbase CBaseObject (0x00CACAF0, public
  non-virtual dtor) is a different class from the engine's CBaseObject (0x0099A430, protected virtual). Same name in two
  libraries; the vland RELOC_CONFLICT on `??1CBaseObject@@QAE@XZ` comes from the landed 00af10e0 binding the engine dtor
  with the default-header (public) spelling (NAME_FIXES.tsv).

## Callee names (vland RELOC_NAME warnings: the label db spelling differs from the bound symbol)

0x00637A10 NThingMap::CThingMapManager::RemoveThing (label DestroyMapwhoNode); 0x0098E7D0 CloseAllStreamsForFile (label
CThreadedFileManager_ProcessPendingRequests); 0x009FC440 CResource::RemoveFromResourceList (label CTCActivationTrigger::OnKill);
0x00993030 vector<pair<long,CSoundBankRecord>>::erase (label CTCSkeletalMorph_RangeAssign); 0x00513BB0 __uninitialized_copy
(label UninitCopy_SElement16_00513bb0); CSoundEventSet member tables/views 0x00C239B0 / 0x00C23C10 / 0x00C23E00 / 0x00C24050 /
0x00C252D0..0x00C254B0 / 0x00C28AE0 (labels NScript_* / CVtable_*: placeholders on both sides); CString operator<< 0x00C0DEC0 /
0x00C0DF00; 0x00C49B30 CMailboxQueue ctor (label CScriptEntry_00c49b30); 0x00C49C90 CFrameStack ctor (label
CLocalDetailPrimitiveRepeatedMesh -- wrong module); 0x00C491E0 CCodaFrame ctor (label CFrame_ctor); 0x00C35EC0 CreateSource
(label AudioSystem_SetOutputFlags, returns the source); 0x00C35DD0 CODA_StopSource (label CDrawAutoPickups_ShouldRender);
0x00C62E20 CSlaveObject ctor (label PKRASlaveObject::PKRASlaveObject); 0x00C4C330 / 0x00C4C6C0 CMasterObject Reset / Commit
(labels CDebugString::AddString / PKRACDependency_OnFinalized); 0x00C55C80 CAudioContext::ClearEntryPoints (label
CScreenEffectOwner_00c55c80::Reset). libpk has no PDB: the class spellings CMasterObject_00c4c8b0 / _00c4cd80 / _00c4c7a0
(likely one PKRACMasterObject, per the trace file name) and CCodaFrame vs "CFrame" need one coordinator pick.

## Parked residues and next ideas

- **00c89850 CReverbFilterTap::ProcessCrossfade -- DIFFER(607v588), dl 45, 27 attempts.** Found: the step must divide by
  `(float)count` at each use (a named `n` makes VC7.1 hoist a 1/n reciprocal), memset after the end gains. Residue: VC7.1
  strength-reduces the history arrays off `&target->m_historyGain[i]` (+8) and keeps `i` for `current->m_historyDelay[i]`;
  retail bases everything on `&target->m_historyDelay[i]` (+0) with `current` as an offset and an eliminated 2-count loop, so
  every frame slot shifts by the extra `i`. Tried: delay/gain load order, separate locals, int/unsigned i, `!= 2`, pointer
  copies of current, a const view, inline ternaries for Back, Gain() accessor, int delays. Next: an inline per-tap helper
  taking `(const SReverbTapParams&, int)`, a `for (const unsigned* d = target->m_historyDelay; ...)` pointer loop written in
  source, or the SIMD twin's history loop if it is ever landed (same source file in retail).
- **00c54c10 CAudioSource::ApplyParameters -- DIFFER(651v651), dl 3, 47 attempts.** Found: `(m_flags >> 6) & 1` through a
  bool local (a direct test folds to `test byte`), the muted/unmuted amplitude as two calls in an if/else (retail pushes in
  each arm and shares the call), the volume chain parenthesised left-deep. Residue: VC7.1 canonicalises the stream-gain
  product's leaves to [edi+134h],[edi+130h],[esi+94h],[ebp+4],[esi+30h] for every source order (all permutations tried, also
  non-const params, a CCodaVoice base, a nested S3DParams); retail has [esi+94h] first. Explicit parentheses give the retail
  order but move the fmuls against the integer argument set-up (dl 7-8). Next: the gain as a member/inline function of the
  voice (`GetLevel() * ...`), m_levelScale through a different declared type (double/array), or the SetParameters call split
  so the gain is a call argument computed after the bool.
- **00c8e2e0 / 00c95a10** -- third-party: libvorbis (`_vorbis_unpack_books`: 15 oggpack_read 0x00C915A0 calls + the
  vorbis_info_clear 0x00C8DFB0 error path; `_best`: calls `_dist` 0x00C95910). Not attempted per the brief.
- **00d6bb91** -- bounds: no function starts here; it is +0x11 inside SpawnEnemies 0x00D6BB80 (landed). Retire the row.
- **009aa440** -- duplicate COMDAT: the second byte-identical copy of `std::pair<const CCharString,CBankInfo>::~pair`
  (0x009A8FE0) in another TU; the same source grades RELOCATION_MATCH for both. Also twinned: CBankInfo ctors 0x009A7670 /
  0x009AA210 and pair dtors 0x009A9010 / 0x009AA470. Coordinator: alias record or twin landing.

## Retail-asm helper calls (every one unavoidable: a bare `fistp dword` from libpk inline asm)

00c78150 (3 sites) and the parked 00c54c10 (1 site) call `FtoL` from rebuild/include/libpk/FtoL.h (two-path helper; no asm
in the rows). 00c78150 wraps it in a TU-local `__forceinline RoundHalfDownToLong` exactly as the landed 0x00C3FF70 body
(`FtoL(value - 0.5f)`, `+1 if negative`).

### C++ attempts before helper -- 0x00C78150 (sites 0x00C781CF, 0x00C78281, 0x00C782E5)

Retail at each site: `fstp dword [slot] ; fld dword [slot] ; fistp dword [slot2] ; mov eax,[slot2] ; test eax,eax ; jge ; inc eax`
(slot = the dead params arg slot for site 1, frame words for sites 2-3). Same spelling substituted in the shared round-half-down
(all three sites change together; each site emitted the same):

| spelling | VC7.1 RTM emitted at 0xC781CF / 0xC78281 / 0xC782E5 |
|---|---|
| `(long)(v - 0.5f)` | call __ftol2 (x3) |
| `(int)(v - 0.5f)` | call __ftol2 (x3) |
| `(short)(v - 0.5f)` | call __ftol2 (x3) |
| `float t = v - 0.5f; (long)t` | call __ftol2 (x3) |
| `(long)(v - 0.5f + 0.5f)` | call __ftol2 (x3) |
| `(long)floor(v - 0.5f)`, /Oi + `#pragma intrinsic(floor)` | call floor + call __ftol2 (x3) |
| `(long)(v - 0.5f)` with /QIfist | fistp qword (x3, 8-byte slot) |
| `(long)(double)(v - 0.5f)` | call __ftol2 (x3) |
| same-TU `static inline long ToLongCxx(float)` | call __ftol2 (x3) |
| `_mm_cvtss_si32(_mm_set_ss(v - 0.5f))` | cvtss2si (x3) |
| `_mm_cvttss_si32(_mm_set_ss(v - 0.5f))` | cvttss2si (x3) |
| macro vs function | the brief's macro form is not in the shared header; the `__forceinline` FtoL routes the operand through the frame slot exactly as retail (RELOCATION_MATCH) |
| landed rows matching this sequence without a helper | none: 0x00C3FF70, 0x00C77FC0, 0x00C5A500, 0x00C40F50 all carry the helper / retail_asm |

### C++ attempts before helper -- 0x00C54C10 (site 0x00C54D07, parked row)

Retail: `fstp dword [esp+14h] ; fld dword [esp+14h] ; fistp dword [esp+18h] ; mov eax,[esp+18h] ; cmp ax,[esi+23Eh]`.

| spelling | emitted |
|---|---|
| `(short)(long)x`, `(short)(int)x`, `(short)x`, temporary, `(long)(x+0.5f)`, `(double)`, same-TU C++ callee | call __ftol2 |
| `floor` with /Oi + pragma | call floor + call __ftol2 |
| /QIfist | fistp qword |
| `_mm_cvtss_si32` / `_mm_cvttss_si32` | cvtss2si / cvttss2si |

## GOTCHAS one-liners

- A named float divisor (`float n = (float)count; ... / n`) inside a loop lets VC7.1 hoist a reciprocal multiply; spell
  `/ (float)count` at each use to keep retail's per-call `fdiv`.
- VC7.1 hoists `(float)(i * segment)` out of an inner loop when `offset = i * segment` is an outer-loop local; declaring the
  local inside the inner body keeps the integer multiply hoisted and the `fild` per iteration (retail shape).
- `cond ? scale : 1.0f` inline in a product keeps the selected factor on the x87 stack (`fld f ; fld x ; fmul st(1) ... fstp st(0)`);
  a named factor local folds to a memory `fmul`.
- `if (a) x = f(0); else x = f(b);` with an identical tail cross-jumps to `push 0 ; jmp` / `push reg` + one call; a `?:` on
  the argument merges into a register first.
- VC7.1 (no /Op) canonicalises the leaf order of an unparenthesised float multiply chain independent of source order;
  explicit parentheses pin the order but change the scheduling.
- `(x >> 6) & 1` tested directly folds to `test byte ptr [m],40h`; through a `bool` local it stays `mov ; shr ; test al,1`.
- Two libraries in one image can share a class name: strmbase CBaseObject (0x00CACAF0, `??1CBaseObject@@QAE@XZ`) vs the
  engine's CBaseObject (0x0099A430, `??1CBaseObject@@MAE@XZ`); the shared-header warning on the strmbase rows is a false positive.
