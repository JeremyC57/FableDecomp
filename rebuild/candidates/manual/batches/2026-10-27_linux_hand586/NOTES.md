# Hand chunk 586 (fill_window .. CBowerstoneTeleportPointMarker): 7 of 10 byte-exact

Ten engine-range rows (catalog class `global`, all from the released pass 9 reservation). 7 matched (all
RELOCATION_MATCH), 3 parked in differ/ (one of them a retail_asm-grade row), 0 skipped. All 7 src/ tests PASS under
g++ (tall.sh) and VC7.1 (tall.py); the 3 parked rows carry `differ/<addr>.test.cpp`, which PASS under g++ and VC7.1
(verify_and_land.behaviour_of). Every row's first and last instruction was checked against the oracle (no int3 inside,
no second body). vland.py (read-only): 7 WINS, no RELOC_IDENTITY, no DIFFER; the warnings are listed under "Review".

## What matched (7)
- **zlib 1.2.1 fill_window 0x00C01090** (deflate.c; "deflate 1.2.1" in .rdata, w_size at +0x24 = no gzhead): the
  1.2.x body with the `sizeof(int) <= 2` block folded away. read_buf 0x00C00D50 is a TU-local static with a private
  register convention (strm in ebx, size in ecx, buf on the stack): its verbatim body sits in the TU as
  `extern "C" { static __declspec(noinline) ... }` and fill_window is `extern "C" __cdecl` (retail's has s on the stack).
  Callees z_adler32 0x00C03CB0 (landed), z_crc32 0x00C05FD0. Model: the landed z_deflateEnd 0x00C00AE0 internal_state.
- **libvorbis decode_packed_entry_number 0x00C96320** (codebook.c, libVorbis I 20030909): verbatim; the bound
  `_decode_packed_entry_number@8` (`extern "C" __stdcall`), oggpack_look / oggpack_adv `__fastcall` (landed), and the
  bitreverse static (lok in eax) as a same-TU `extern "C" { static __declspec(noinline) }` body. MATCH on the first
  grade; the second grade only gave the static C linkage (`_bitreverse`, not `?bitreverse@@YAII@Z`).
- **CCodaPlayback::GetPlayProgress 0x00C27890** (/EHsc, slot 2 of vtable 0x012B4034): under the PKCProtectionInstance
  lock, a stopped source or a one-shot that sat at position 0 longer than the panel's +0x888 timeout reports
  {1.0f, 0}; else position / length (position == length -> length - 1). The playing test must be the `if` with the
  stopped case in the `else` (the first grade, `if (!playing) { ...; return; }`, duplicated an epilogue up front).
- **CDSoundAccelerationQuery::FindDeviceInterface 0x00C5DB40** (/GS): LoadLibraryA("dsound.dll"), DllGetClassObject
  (CLSID_DirectSoundPrivate, IID_IClassFactory), CreateInstance(IID_IKsPropertySet), Get(DSPROPSETID_DirectSoundDevice,
  DSPROPERTY_DIRECTSOUNDDEVICE_ENUMERATE_A = 7, {callback 0x00C5DAA0, &{guid, this}}); the failure arms only empty the
  path at +0 and carry on (retail has no early returns). GUID-to-push pairing checked by hand against the retail bytes.
- **CAudioSource::Initialise 0x00C72930** (slot 1 of the three OpenAL source vtables 0x012B8CC8 / 0x012B8D48 /
  0x012BEE40): base slot 1 0x00C596E0 called directly; then on g_AudioContext 0x0143BDB0 the inline GetError (result
  dropped), Suspend (bool kept in bl), GenSources(1, &SourceId +0x58), Process unless it was suspended, CheckError.
  The landed CAudioContext model of CheckError 0x00C4DBE0 plus inline copies of the landed Suspend 0x00C4D790 /
  Process 0x00C4D7D0 bodies. First grade.
- **CSoundEvent::SetPriority 0x00C20CE0** (the bound spelling): out of the state queue (3: channel Stop + manager
  +0x164, 4: +0x194, 6: channel Stop at priority * 2), out of / back into the manager's +0x1C4 list, then the mirror.
  The landed ResetLevel 0x00C20DF0 model (same-TU CheckState stand-in). First grade.
- **NScript::CPersonalScript_GlobalThingsScript::CBowerstoneTeleportPointMarker::Main 0x00CDF800** (PDB, private
  virtual): wait for the hero within (float)pGlobalScriptDef->TeleporterActivationDist, DisplayTutorial
  (TUTORIAL_CATEGORY_TELEPORTING 31), wait for MsgIsTutorialClickedPast, DeactivateQuestLater
  ("Hook_BowerstoneTeleportTutorial", 0). Shared headers (CScriptThing / CGameScriptInterfaceBase / CEntityScriptBase).
  The quest name must be a named CCharString local (a temporary is built after `push 0`, 267v268).

## Parked for Fable 5.1 (3)
- 00c73660 DetectWindowsVersion DIFFER(260v262): retail copies the major version before the NT switch (`mov eax,ecx ; sub eax,3 ; dec eax ; dec eax`), ours switches on ecx in place (`sub ecx,3 ; dec ecx ; dec ecx`) | tried: switch on the local, on the stored global, on `(unsigned char)version`, an if/else-if chain (cmp ladder) | next: keep major live after the switch (e.g. a later read of the local in the 9x arm order, or the classic MSDN `dwWindowsMajorVersion` locals with the globals stored at the end); else lane gap: register copy
- 00c62790 CPKDecompressedStream::Read DIFFER(269v264): retail `add dword ptr [esi+8],eax` for `m_position += read` (0x00C62824), ours `mov edx,[esi+8] ; add edx,eax ; mov [esi+8],edx` | tried: merged `||` early return (block placement), split early returns + `total` first, total/frames updates before Advance, if/else loop tail (those fixed everything else) | next: `m_position = m_position + read`, `read` unsigned or Decode returning int, the Seek result through a local before the store; else lane gap: RMW selection
- 00c6b9d0 CStateVariableFilter::FilterWithGain DIFFER(269v269): one x87 operand order in the high-pass product, retail `fld [esi+4] ; fmul [esi+18h]` (damping first), ours `fld [esi+18h] ; fmul [esi+4]` (as the landed Filter 0x00C6BAE0 retail does) | tried: `damping * band` and `band * damping` (canonicalised, same bytes), a damping float local, a band float local (worse, 272) | next: the out-of-line ramped twin 0x00C6B7C0 in the same TU above it (retail TU order), or `high` computed in the band update and re-derived; else lane gap: x87 operand order. retail_asm-grade row (Retail-asm line cites the fistp at 0x00C6BAA7): when it matches it goes in retail_asm/, not src/.

## Skipped
None.

## Names and evidence
- 0x00C01090 `_fill_window`, **proven**: zlib 1.2.1 deflate.c body and version string; Ego_r / FableWin PDB name sets
  hold `fill_window` and `read_buf`; callers deflate_stored / fast / slow (0x00C011CB / 0x00C013DE / 0x00C0176B).
- 0x00C96320 `_decode_packed_entry_number@8`, **proven**: libvorbis 1.0.1 codebook.c body, the bound spelling (landed
  callers vorbis_book_decode 0x00C96430, vorbis_book_decodev_set 0x00C96700).
- 0x00CDF800 `?Main@CBowerstoneTeleportPointMarker@CPersonalScript_GlobalThingsScript@NScript@@EAEXXZ`, **proven**:
  Ego_r / FableWin PDB name sets + slot 1 of vtable 0x012C33A4, which the landed Alloc 0x00CDF750 stores; the row is
  referenced only from that slot; the "Hook_BowerstoneTeleportTutorial" literal.
- 0x00C20CE0 `?SetPriority@CSoundEvent@@QAEXPAUCSoundEventManager@@J@Z`, **bound descriptive** (libpk, no PDB): the
  spelling the landed ResetLevel 0x00C20DF0 binds (it calls it with priority 1); the body moves the event between
  priority-ordered queues.
- 0x00C27890 `CCodaPlayback::GetPlayProgress(CPanel*, SPlayProgress*)`, **descriptive**: class CCodaPlayback from the
  landed slots 0/1/6/7 and Update of vtable 0x012B4034; method and result struct names are mine.
- 0x00C5DB40 `CDSoundAccelerationQuery::FindDeviceInterface(const _GUID*)`, **descriptive**: GameCODA has no PDB; the
  class does the DirectSound acceleration probe (0x00C5DD70 reads "DirectSound\Mixer Defaults" "Acceleration" and
  "DirectSound\Device Presence" "VxD"/"WDM" for the device interface found here). The COM / dsound GUIDs and
  DSPROPERTY_DIRECTSOUNDDEVICE_ENUMERATE_A are the SDK names (GUID bytes checked).
- 0x00C72930 `CAudioSource::Initialise(CAudioSound*)`, **descriptive**: CAudioSource is the landed name of these
  vtables (slot 8 Update 0x00C72FE0); base CVtable_012B5C10 keeps its landed placeholder name (its destructor
  0x00C3F3D0 is landed under it) -- the naming pass should name the libpk voice base once.
- Parked: 0x00C73660 DetectWindowsVersion, 0x00C62790 CPKDecompressedStream::Read, 0x00C6B9D0
  CStateVariableFilter::FilterWithGain -- all **descriptive** (libpk); C16BitSamplePtr (cursor class) is **proven** by
  its constructor's ".\C16BitSamplePtr.cpp" trace string.

## Callee and global names
- Bound and reused exactly: z_adler32, oggpack_look / oggpack_adv, CSoundEvent::CheckState, CSoundChannel::Stop / Start,
  CODA_GetSourcePosition, PKCProtectionInstance ctor/dtor, CAudioContext::GetProcAddress / CheckError, g_AudioContext,
  IsDistanceBetweenThingsUnder, CEntityScriptBase::IsActiveThreadTerminating (shared header), CCharString(const char*,
  long) / ~CCharString, pGlobalScriptDef, the CGameScriptInterfaceBase slots.
- New descriptive spellings (no correct binding existed; NAME_FIXES.tsv lists the wrong landed ones):
  `_CODA_IsSourcePlaying@4` 0x00C35CA0, `?g_soundLock@@3VPKCCriticalSection@@A` 0x0143BAFC,
  `?Initialise@CVtable_012B5C10@@UAEJPAUCAudioSound@@@Z` 0x00C596E0, `_z_crc32` 0x00C05FD0 (zlib name, Z_PREFIX like
  z_adler32), `_bitreverse` 0x00C962B0 and `_read_buf` 0x00C00D50 (static library names),
  `?g_anyDeviceGuid@@3U_GUID@@B` 0x012B8FF4, `?FindDeviceInterfaceCallback@CDSoundAccelerationQuery@@SGHPAU...@PAX@Z`
  0x00C5DAA0 (descriptive). The parked rows add `?DetectWindowsVersion@@YAXXZ`, the 0x0143CCC1..CCD4 flag/word names
  (g_isWin32s, g_isWindowsNT3/NT4/2000/XP, g_windowsBuildNumber/MajorVersion/MinorVersion; the 95/98/98SE/Me ones are
  the bound spellings), `??1C16BitSamplePtr@@QAE@XZ` 0x00C62A20, `?Advance@C16BitSamplePtr@@QAEXJ@Z` 0x00C62A30,
  `?FilterWithGainRamped@CStateVariableFilter@@QAEXPAFK@Z` 0x00C6B7C0.
- 0x012B8FF4 is NOT uuid.lib's GUID_NULL (that is 0x012C0694, inside the IID_IUnknown / IID_IClassFactory block, bound
  `_GUID_NULL` by the landed 0x00CB1920): it is a second all-zero GUID in the GameCODA .rdata (after the waveout vtable),
  also read by the enumeration callback. vland raised RELOC_CONFLICT for `_GUID_NULL` until it got its own name.

## Label corrections
- 0x00C72930 sub_00c72930 -> CAudioSource::Initialise
- 0x00C20CE0 EquipmentState_UpdateInventory -> CSoundEvent::SetPriority
- 0x00C5DB40 Audio_InitializeDirectSound -> CDSoundAccelerationQuery::FindDeviceInterface (it opens no DirectSound device)
- 0x00C73660 Sys_DetectOSVersion -> DetectWindowsVersion (right idea, descriptive)
- 0x00C62790 CStreamBuffer_WriteData -> CPKDecompressedStream::Read (it decodes into a sample buffer; nothing is written to a stream)
- 0x00C96320 PropertyDef_LookupByClass -> libvorbis decode_packed_entry_number
- 0x00CDF800 sub_00cdf800 -> NScript::CPersonalScript_GlobalThingsScript::CBowerstoneTeleportPointMarker::Main
- 0x00C01090 fill_window (correct)
- 0x00C6B9D0 Audio_FilterSamples_2ndOrder -> CStateVariableFilter::FilterWithGain
- 0x00C27890 sub_00c27890 -> CCodaPlayback::GetPlayProgress
- Neighbours: 0x00C35CA0 CEngine::Is3DSceneRenderingEnabled -> CODA_IsSourcePlaying; 0x00C596E0
  CEnvironmentSpellEffectInterface::ClearScreenFilter -> the voice base's slot 1 (Initialise); 0x00C62A20
  Nop_ret0_00c62a20 -> ~C16BitSamplePtr; 0x00C62A30 StreamBuffer_AdvancePosition -> C16BitSamplePtr::Advance;
  0x00C5DB30 CFourierAnalysis::CFourierAnalysis -> the CDSoundAccelerationQuery constructor; 0x00C962B0
  Util_BitReversalPermutation -> libvorbis bitreverse; 0x00C26480 / 0x00C26410 catalog CStreamBuffer::RemoveUsers /
  AddUsers are the bound CSoundChannel::Stop / Start; 0x00C73770 CDefString::SetDefaultString is the platform-info
  constructor (`if (!probed) DetectWindowsVersion(); return this;`).

## Header debt
- CScriptDef has no engine header: TeleporterActivationDist is a long at retail +0xFB4, Ego_r +0x1070 (0xBC high from
  the Dinkumware 0x10 vectors before it; consistent with the landed GUI_MeleeBeetles +0xF10).
- CSoundEvent (vtable 0x012B3674: slot 0 0x00C21D50 returns this+0xC, slot 1 0x00C20FD0 thunks to ResetLevel) is modelled
  in every landed row with a flattened `void* __vftable`; 0x00C20CE0 keeps that landed model (it touches no vptr).
  CAudioContext (vtable 0x012B8C18) likewise: the landed CheckError model has `void* __vftable`; 0x00C72930 uses
  `virtual void UnusedSlot0()` as the landed SourcePause does. Both classes need a shared header with real slot names.
- No engine headers exist for the libpk / GameCODA classes used here (CCodaPlayback, CPanel, PKCProtectionInstance,
  PKCCriticalSection, CAudioContext, CAudioSource, CVtable_012B5C10, CSoundEvent, CSoundEventManager, CSoundChannel,
  CPKDecompressedStream, CDecompressorInterface, C16BitSamplePtr, CStateVariableFilter); every row reuses the landed
  local models. zlib / libvorbis structs are local C models (as the landed siblings).
- vland SHARED_HEADER_AVAILABLE for CScriptThing / CEntityScriptBase in 0x00CDF800 was fixed: the row now includes
  engine/CScriptThing.h, CGameScriptInterfaceBase.h and CEntityScriptBase.h (ETutorialCategory declared before them).
