# Hand chunk 497 (CAudioSource .. CBasePin): 35 of 40 byte-exact

17 MATCH + 18 RELOCATION_MATCH (no retail_asm rows: the only x87 special, `fsin` in 0x00C73F10, is the /Oi intrinsic of
`sin`). Every src/ row graded with gall.sh, every test PASS under g++ and the lane's VC7.1 (tall.sh / tall.py), and the
read-only vland dry run reported WINS 35 with no RELOC_IDENTITY / RELOC_DUPLICATE (only RELOC_NAME label warnings and the
strmbase `CBaseObject` shared-header warning, see Header debt).

## Names

### Proven
Real source names (Xiph libogg/libvorbis 2003 CVS and DirectShow strmbase), from the shipped source body plus the
function-table / vtable slot or source-order evidence given in each source's Identity line.
- 0x00C8AA00 ov_raw_total (vorbisfile.c; self-recursive; precedes ov_pcm_total 0x00C8AA80)
- 0x00C8CCF0 ogg_page_version (framing.c; header[4]; precedes ogg_page_continued 0x00C8CD00)
- 0x00C8F030 vorbis_synthesis_halfrate (synthesis.c; called by ov_halfrate; precedes vorbis_synthesis_halfrate_p 0x00C8F050)
- 0x00C91570 oggpack_adv1 (bitwise.c; oggpackB_adv1 0x00C91590 tail-jumps to it)
- 0x00C917D0 oggpack_bits (bitwise.c; oggpackB_bits 0x00C917F0 tail-jumps to it)
- 0x00C95440 vorbis_book_clear (sharedbook.c; 5 frees + memset 0x2C; between vorbis_staticbook_destroy and vorbis_book_init_encode)
- 0x00C9EA70 _vorbis_window_get (window.c; vwin[] table 0x013ABD90 of vwin64..vwin8192)
- 0x00CA1250 res1_class (res0.c; slot 5 of residue1_exportbundle 0x013ABDE4)
- 0x00CA1770 floor1_free_look (floor1.c; slot 4 of floor1_exportbundle 0x013AC228; 0x148 dwords = sizeof(vorbis_look_floor1))
- 0x00CA3850 floor0_look (floor0.c 2003 CVS; slot 2 of floor0_exportbundle 0x013AC244)
- 0x00CA3EF0 vorbis_invsq2explook (lookup.c; INVSQ2EXP table 0x013AC4F0, MIN -32)
- 0x00CA5190 CRendererInputPin::CompleteConnect (renbase.cpp; primary slot 12 of 0x012BF408)
- 0x00CA5360 CBaseVideoRenderer::OnStartStreaming (renbase.cpp; slot 16 of 0x012BF5D8 / CTextureRenderer 0x0129D08C; calls slot 47 ResetStreamingTimes)
- 0x00CA80C0 CBasePin::CheckConnect (amfilter.cpp; primary slot 10 of 0x012BF408)
- 0x00CA8340 CBasePin::ConnectedTo (IPin slot 6)
- 0x00CA8490 CBasePin::QueryDirection (IPin slot 9)
- 0x00CA84B0 CBasePin::QueryId (IPin slot 10 of 0x012BF760; tail call to AMGetWideString 0x00CB0940)
- 0x00CA84C0 CBasePin::QueryAccept (IPin slot 11)
- 0x00CA8560 CBasePin::SetSink (IQualityControl slot 4 of 0x012BF3AC / 0x012BF748)
- 0x00C8A8B0 ov_halfrate (parked; vorbisfile.c; self-recursive; calls vorbis_synthesis_halfrate)
- 0x00C8CB20 ov_test_callbacks (parked; between ov_open 0x00C8CB10 and ov_test 0x00C8CB40, which jumps here)
- 0x00C8D9C0 ogg_stream_reset_serialno (parked; framing.c; ogg_stream_reset + serialno +0x150)
- 0x00C8DB20 ogg_stream_packetpeek (parked; framing.c; _packetout(os,op,0), after ogg_stream_packetout 0x00C8DB10)

### Probable
Identity proven (vtable slot / binding / landed relatives); the class names are the existing descriptive libpk names
(libpk has no PDB/RTTI), reused so one address keeps one name.
- 0x00C75400 CDelayLine::~CDelayLine (bound `??1CDelayLine@@UAE@XZ`; stores vtable 0x012BF074)
- 0x00C75470 CDelayLine::`scalar deleting destructor' (slot 0 of 0x012BF074)
- 0x00C73FC0 CDspOps::Scale (slot 4 of the x87 kernel table 0x012BF02C, landed CreateSharedOps model)
- 0x00C74030 CDspOps::ScaleCopy (slot 3)
- 0x00C740B0 CDspOps::ScaleRamp (slot 2)
- 0x00C741C0 CDspOps::Add (slot 5)
- 0x00C88620 COggVorbisStream::GetLength (slot 1 of 0x012BF1C4, documented by the landed ctor 0x00C88780)
- 0x00C88640 COggVorbisStream::Seek (slot 2)
- 0x00C88740 COggVorbisStream::GetRate (slot 4)
- 0x00C88770 COggVorbisStream::GetState (slot 5)

### Descriptive
No source name exists (libpk / OpenAL layer, no PDB/RTTI); class named by the landed vtable name, method by its role.
- 0x00C72FE0 CAudioSource::Update (slot 8 of 0x012B8CC8 / 0x012B8D48 / 0x012BEE40; caller of the landed QueueNextBuffer)
- 0x00C73EB0 CVtable_012bf014::GetLength (440 Hz test-tone IAudioStream; landed ctor 0x00C73EF0)
- 0x00C73EC0 CVtable_012bf014::Seek
- 0x00C73F10 CVtable_012bf014::Read
- 0x00C774A0 CVtable_012bf0c8::SetSettings (slot 1 of the DSP settings-block base 0x012BF0C8, inherited by CReverbProcessor 0x012B9594)
- 0x00C888F0 CVtable_012bf1dc::CVtable_012bf1dc (null IAudioStream: GetLength 0, the rest -1)

## What matched (35)
- Test-tone stream CVtable_012bf014 (3): GetLength = rate*3600, Seek returns the position, Read writes sin(phase)*32767 to
  each channel. Shape: the unsigned up-counting frame loop (VC makes it the down-counter with a `jbe` entry test); `sin`
  on a float under `#pragma intrinsic(sin)` is a bare `fsin`.
- CDspOps x87 kernels (4): Scale / ScaleCopy / ScaleRamp / Add. Shape: `*p++ op= ...` pointer walks (indexing gives a
  base+index walk); VC7.1 /O2 unrolls by four on its own; the ramp gain stays in st(0).
- CDelayLine (2): the virtual destructor clearing data/size, and its `??_G` with the destructor inlined (the destructor
  defined in the same TU).
- CVtable_012bf0c8::SetSettings: memcpy intrinsic into the current or the pending block + dirty flag, verbatim.
- COggVorbisStream (4): extern "C" __fastcall ov_* callees; GetLength's truncated 64-bit total becomes a tail jump; the
  two-constant select in GetState is neg/sbb/and/dec.
- CVtable_012bf1dc ctor: an empty body over an inline abstract IAudioStream (the base vtable store is dead and dropped).
- CAudioSource::Update: a class with an `__int64` member pads its vfptr to 8, so the first member is at +8.
- Xiph rows (11): the shipped source verbatim, /Gr extern "C" __fastcall; res1_class declares the LTCG-cloned static
  _01class `__stdcall` (four stack args, callee pop); data tables (vwin, INVSQ2EXP_LOOKUP) as extern arrays.
- strmbase rows (8): the landed QueryPinInfo model (IPin +0x0C, IQualityControl +0x10) with the amfilter.cpp/renbase.cpp
  bodies verbatim; STDMETHODIMP members are __stdcall with `this` at the interface sub-object.

## Parked for Fable 5.1 (4)
- 0x00C8A8B0 ov_halfrate DIFFER(107v105): extra `mov ecx,esi` before `call _decode_clear` (retail passes vf in esi) | tried: verbatim vorbisfile.c with a __fastcall _decode_clear | next: lane gap: LTCG register argument
- 0x00C8CB20 ov_test_callbacks DIFFER(5v22): retail `push ibytes ; mov eax,initial ; push f ; mov ebx,vf ; call _ov_open1`, ours a tail `jmp` | tried: verbatim body, __fastcall _ov_open1 | next: lane gap: LTCG register arguments (eax/ebx)
- 0x00C8D9C0 ogg_stream_reset_serialno DIFFER(22v14): ours saves os/serialno in esi/edi around ogg_stream_reset, retail keeps them in ecx/edx | tried: verbatim framing.c | next: lane gap: LTCG callee-register knowledge
- 0x00C8DB20 ogg_stream_packetpeek DIFFER(8v12): retail `mov eax,ecx ; push 0 ; mov ecx,edx ; call _packetout`, ours the __fastcall push+call | tried: verbatim framing.c | next: lane gap: LTCG register arguments (eax/ecx)

## Skipped
- 0x00CA7D60 (83 bytes): four functions separated by int3 padding: CBasePin::NonDelegatingAddRef 0x00CA7D60 and
  NonDelegatingRelease 0x00CA7D80 (`return m_pFilter->AddRef()/Release()`, primary slots 1/2 of 0x012BF408 and IQualityControl
  slots of 0x012BF7A8), then two `ret 4` / `ret 8` stubs at 0x00CA7DA0 / 0x00CA7DB0. Needs a catalog re-split.

## Label corrections
- 0x00C75400 "Ctor_Init_012bf074_b" -> CDelayLine::~CDelayLine; reloc_bindings.tsv also holds a stale
  `??1CKrakenElement_00c75400@@UAE@XZ` for this target (site 0x00C75440).
- 0x00C888F0 "CBase::CBase" -> the null audio stream's constructor (vtable 0x012BF1DC).
- 0x00C8CCF0 "ConsoleDisplayCSTimecode" -> ogg_page_version.
- 0x00C8D9C0 "CRegionMinimap_CRegionMinimap" -> ogg_stream_reset_serialno.
- 0x00C8DB20 "_Dest_val<...CFlash>" -> ogg_stream_packetpeek; the landed 0x00C8DB10 "std_DestvalCFlasha" is ogg_stream_packetout.
- 0x00C95440 "CWound::Kill" -> vorbis_book_clear.
- 0x00C9EA70 "CMemoryAllocator::IsAllocatorFixedSize" and binding `?LookupAllocatorSize@@YIKK@Z` -> _vorbis_window_get; the
  landed caller 0x00C90C80 "CPoolEntryTable_00c90c80::GetEntryAllocatorSize" is block.c's vorbis_window(v,W)
  (backend_state +0x68, window[W] +4, halfrate_flag +0xE78).
- 0x00CA3EF0 "CMemoryAllocator::IsAllocatorFixedSize" -> vorbis_invsq2explook.
- 0x00CA5360 "CTextureRenderer::vfunc_16" -> CBaseVideoRenderer::OnStartStreaming (CTextureRenderer only inherits it).
- Callees: 0x00CA8090 (catalog "ReturnZero_ret4", landed as CEngineInternalPrimitiveBase::AddChildPrimitive) is
  CBasePin::CompleteConnect; 0x00CA0680 "Audio_InterpolateScaledSamples" is res0.c _01class; 0x00C8C9A0
  "CreatureHitNotification_Load" is _ov_open1; 0x00C8D9D0 "Notification_ProcessState" is _packetout; 0x00C8A520
  "CMemoryDataOutputStream::Clear" is vorbisfile.c _decode_clear; 0x00C8AB70 ov_raw_seek, 0x00C8AE90 ov_pcm_seek_page
  ("CreatureHitNotification_StateProcess"), 0x00C8B490 ov_pcm_seek ("Subsystem_LoadWorldMapAndNotifications"), 0x00C8B770
  ov_time_seek, 0x00C8B880 ov_time_seek_page; 0x00C8CAE0 ov_open_callbacks (fork), 0x00C8CB10 ov_open, 0x00C8CB40 ov_test.

## Callee names (no PDB spelling; descriptive or Xiph statics)
- 0x00C59BC0 CAudioSource::BeginSound, 0x00C72E00 CAudioSource::ApplySound, slot 17 CAudioSource::Restart (descriptive).
- 0x00C8A520 _decode_clear, 0x00C8C9A0 _ov_open1, 0x00C8D9D0 _packetout, 0x00CA0680 _01class (Xiph statics, extern "C").
- 0x00C8AE90 ov_pcm_seek_page, 0x00C8B490 ov_pcm_seek (extern "C" __fastcall, new bindings).
- Data: 0x013ABD90 vwin, 0x013AC4F0 INVSQ2EXP_LOOKUP (the +0x80 displacement folds the -32 minimum).

## Header debt
- No engine header covers these classes (libpk, Xiph, strmbase). The strmbase rows declare their own `CBaseObject`
  (combase.h); vland warns SHARED_HEADER_AVAILABLE because the game also has a CBaseObject header: same as the landed
  strmbase rows, it is a different class and no CBaseObject member is called here.
