# Chunk 506 -- names, evidence, label corrections, header debt

## Proven
00ca7210 CRendererInputPin::Receive -- IMemInputPin slot 6 of 0x012BF388; body is renbase.cpp verbatim (EC_ERRORABORT, m_bRunTimeError, two locks)
00ca8370 CBasePin::ConnectionMediaType -- IPin slot 7 of 0x012BF3C0; amfilter.cpp body (E_POINTER, VFW_E_NOT_CONNECTED 0x80040209)
00ca89f0 CBaseInputPin::GetAllocator -- IMemInputPin slot 3 of 0x012BF388; amfilter.cpp body (CLSID_MemoryAllocator create)
00ca8ac0 CBaseInputPin::NotifyAllocator -- IMemInputPin slot 4 of 0x012BF388; amfilter.cpp body
00ca9450 CMediaSample::GetProperties -- IMediaSample2 slot 19 of the CMediaSample table (0x012BF72C); amfilter.cpp body
00ca9f20 CBaseAllocator::GetBuffer -- IMemAllocator slot 7 of 0x012BF810; amfilter.cpp body (VFW_E_NOT_COMMITTED, AM_GBF_NOWAIT, VFW_E_TIMEOUT)
00caa0a0 CBaseAllocator::Commit -- IMemAllocator slot 5 of 0x012BF810; amfilter.cpp body
00cab580 CBasePin::Disconnect -- IPin slot 5 of 0x012BF3C0; amfilter.cpp body with DisconnectInternal inlined (VFW_E_NOT_STOPPED)
00cab770 CDynamicOutputPin::CDynamicOutputPin -- stores the CDynamicOutputPin vtables 0x012BF7A8/0x012BF760/0x012BF748/0x012BF738 exactly as the landed ANSI twin 0x00CAC300; base call is the wide CBasePin ctor 0x00CAB370
00ca3600 libvorbis::floor0_unpack -- floor0.c: 0x60-byte vorbis_info_floor0, order/rate/barkmap/ampbits/ampdB/numbooks field widths
00c9fea0 libvorbis::res0_pack (parked) -- res0.c: 24/24/24/6/8 widths, ilog(secondstages)>3 split, icount accumulation
00c8e6e0 libvorbis::_vorbis_pack_info (parked) -- info.c: "vorbis" literal 0x012BF238, 8/32/8/32x4/4/4/1 widths, OV_EFAULT
00c911f0 libogg::oggpack_writecopy_helper (parked) -- bitwise.c: bits/8 split, endbit test, BUFFER_INCREMENT 256 realloc, w() callback
00c95910 libvorbis::_dist (parked) -- sharedbook.c: squared distance over b[i*step]; caller is _best
00c8f960 libvorbis::_preextrapolate_helper (parked) -- block.c: order 32 (alloca 0x80), pcm_current-centerW>64 guard, lpc_from_data/vorbis_lpc_predict

## Probable
00c74130 CDspOps::ScaleRampCopy -- slot of the x87 CDspOps table 0x012BF02C built by landed CreateSharedOps 0x00C74360 (slot names from the landed model; libpk has no PDB/RTTI)
00c74230 CDspOps::Mix -- same table, landed model slot name
00c742c0 CDspOps::MixRamp -- same table, landed model slot name
00c886b0 COggVorbisStream::Read -- slot 3 of vtable 0x012BF1C4, the Read slot of the landed COggVorbisStream model; calls ov_read
00c89750 CReverbFilterTap::Process -- the landed caller CReverbTapBank::Process 0x00C75E90 already binds this address with this exact symbol

## Descriptive
00c62920 CAudioStreamOwner_00c62920::CAudioStreamOwner_00c62920 -- only caller 0x00C4812E builds it in a 0x14-byte block
00c69b60 CReverbProcessor::Reset -- slot 3 of the reverb vtable 0x012B9594 (class name from the landed CReverbProcessor model)
00c774f0 CVtable_012bf0c8::CommitAndProcess_00c774f0 -- slot 2 of 0x012BF0C8
00c6b5d0 ConvertInt128BlockToFloatInPlace_00c6b5d0 -- retail_asm row; body converts 128 int32 to float in place
00c72800 CAudioSource::Rewind (parked) -- slot 14 of the OpenAL voice vtables 0x012BEE40 / 0x012B8CC8 / 0x012B8D48
00c72d00 CAudioSource::BindSound (parked) -- slot 3 of the same vtables
00c6b7c0 CStateVariableFilter::FilterRampedWithGain (parked) -- beside the landed CStateVariableFilter slots 0x00C6B8C0 / 0x00C6BAE0 / 0x00C6BD00
00c74ab0 CBiquadCoefficients::ComputeLowPass_00c74ab0 (parked) -- callers 0x00C777C7 and 0x00C7847D

## Label corrections
- Catalog labels wrong for 00c62920 (C3DGroup2_Create), 00c69b60 (CTCNpcInteractable_Reset), 00c6b5d0 (Math_ConvertFloatsToIntegers: direction backwards), 00c6b7c0 (Audio_FilterSamples_2ndOrderDirect), 00c74ab0 (CAudioChannel_ComputeShelveFilterCoefficients: it is a low-pass), 00c89750 (Graphics_RenderWaveform), 00c8f960 (Audio_ConvolutionStep), 00c95910 (Audio_ComputeL2Norm), 00cab770 (CParticleMorphs_Initialize); all `_gapscan::sub_*` rows get the names above.
- 0x00C6B5D0 is bound by the landed caller CDecodedStream::PumpBlock 0x00C546E0 as `?ResetDecoder@@YAXPAX@Z` (invented). Evidence (the body converts the decoder's 128-sample block at +0x21C in place) gives ConvertInt128BlockToFloatInPlace_00c6b5d0; the caller's declaration should be renamed at landing (one address = one name).
- Landed 0x00CAB370 is named "CParticleMorphs::CEntry_Constructor"; it is the wide-string CBasePin constructor (CBasePin_CEntryConstructor_00cab370.cpp). That file and the 0x00CAC300 source also contain `volatile`.
- Landed accessors 0x00C628A0 / 0x00C628C0 / 0x00C628D0 / 0x00C628F0 carry propagated names; they are accessors of the 0x00C62920 stream owner.
- Landed CVtable_012bf014 constructor is declared with a `long` argument; retail 0x00C62920 pushes the float 800.0f, so it takes `float`.
- Landed CDspGain::Apply declares `(int, unsigned int)`; the binding mangles `_N K` (bool, unsigned long).
- Landed 0x00C73EF0 parameter should be `float`.
- 0x00C73AF0 (callee of 00c62920) is catalogued 'NHeroInformationScreens_CreateScreen' (vland RELOC_NAME warning); it is the libpk decoder-stream factory (source, name, segment) -> IAudioStream*, declared descriptively as CreateDecoderStream_00c73af0. No binding existed for it.
- vland RELOC_NAME on 00cab770 (0x00CAB370 'CParticleMorphs::CEntry_Constructor') is the wrong landed label above, not a wrong spelling here.

## Header debt
- vland warns SHARED_HEADER_AVAILABLE CBaseObject on every strmbase row: false positive. engine/CBaseObject.h is Lionhead's game CBaseObject (vptr + IntelligentPointer, 8 bytes); the strmbase CBaseObject is DirectShow's (release build, no members). Do not include the game header there.
- No rebuild/include/engine header exists for any class in this chunk (strmbase CBasePin/CBaseInputPin/CBaseAllocator/CMediaSample/CDynamicOutputPin/CRendererInputPin; libpk CReverbProcessor, CDspOps, CVtable_012bf0c8, COggVorbisStream, CReverbFilterTap, CAudioSource, CStateVariableFilter). Local models are pasted verbatim from the landed sources (strmbase from CBaseAllocator_Decommit_00caa160.cpp / CBaseInputPin_ReceiveMultiple_00ca8cf0.cpp / 0x00CAC300); a shared strmbase header would remove ~400 duplicated lines per row.
- The strmbase models declare CBaseFilter / CCritSec as `struct` (mangled PAU), as the landed 0x00CAC300 does; no PDB class key exists to settle it.
