# Hand chunk 445 (CMixerStream .. ov_pcm_total): 33 of 40 byte-exact

All 40 rows belong to libpk, the audio middleware library (no PDB, no RTTI), or to the Ogg Vorbis code linked statically into it. Every catalog label in the chunk was either a guess or propagated from an unrelated body. Each row is therefore named from its callers, vtables, constants, the ivec/dvec/fvec class libraries and the libvorbis 1.0.1 source. Library names (ivec.h, dvec.h, fvec.h, vorbisfile.c) are real. The libpk class names are descriptive, and they follow agent444's reverb model where the two chunks overlap.

33 rows grade through gall.sh: 6 MATCH and 27 RELOCATION_MATCH. Every test passes under both g++ (tall.sh) and VC7.1 (tall.py). 28 rows matched on their first formal grade, and 5 needed a second (00c78040, 00c78970, 00c7ba00, 00c89110, 00c896c0). 7 rows are parked:

- 3 are retail_asm candidates.
- 3 are LTCG custom-convention vorbisfile statics.
- 1 is a loop induction-variable shape.

**Where libpk ends.** The libpk proper code (mixer, reverb, SIMD helpers, OpenAL glue) runs up to the reverb tap methods at about 0x00C89BE0.

- 0x00C89C10..~0x00C8CB40 is libpk's private, modified copy of vorbisfile.c. It is built /Gr, so every ov_* function is `__fastcall`. Its statics are LTCG-private, with vf in esi or eax. It has no `ov_callbacks`, so `sizeof(OggVorbis_File)` is 0x2C0, and reads go through the owner's CStreamReader.
- libogg framing follows: ogg_stream_clear 0x00C8CE80, and ogg_sync_clear/buffer/wrote/pageseek/reset at 0x00C8D420 / 0x00C8D470 / 0x00C8D4F0 / 0x00C8D510 / 0x00C8D950.
- libvorbis 1.0.1 (20030909) comes next: vorbis_comment_clear 0x00C8DEF0, vorbis_info_clear 0x00C8DFB0, vorbis_block_clear 0x00C8F1B0, vorbis_dsp_clear 0x00C8F650.

**Import identity.** I checked every relocation of the matched rows against retail's call and data targets, in order:

- the free import [0x01440154] through esi in ov_clear;
- `_ftol2` 0x00BFEA70 in ov_bitrate_instant;
- `__security_cookie` 0x0139C8A8 and `__security_check_cookie` 0x00BFE9F9 in the /GS rows;
- MixerAlloc 0x00C3C360 and MixerFree 0x00C3C370;
- CDspGain::Reset 0x00C744E0.

No row in the chunk uses new/delete.

## What matched (33)
- **ivec.h MMX class library** (12 rows, all with `// Flags: /GS`, RELOCATION_MATCH on the first grade):
  - 00c78590 M64::M64(__m64)
  - 00c78630 Is32vec2::operator+=
  - 00c78670 Is32vec2::operator>>
  - 00c786b0 Is32vec2::operator>>=
  - 00c786e0 unpack_low
  - 00c78720 unpack_high
  - 00c78760 I16vec4::I16vec4(__m64)
  - 00c787d0 sat_add
  - 00c78810 mul_high
  - 00c78850 mul_add
  - 00c78890 pack_sat
  - 00c788d0 operator+(Is32vec2)

  The real VC7.1 `ivec.h` class bodies reproduce retail exactly under /GS: an 8-byte aligned cookie frame, and VC itself emitting `emms` after the epilogue when the function receives a `__m64` in mm0. Retail never ran inline asm here.
- **dvec.h SSE2**: 00c7b870 Is32vec4::operator+= (/GS, 16-byte aligned frame).
- **fvec.h SSE mixing kernels**, CMixKernelsSSE (vtable 0x012BF1F4; 6 rows):
  - 00c88a50 Scale
  - 00c88ab0 ScaleCopy
  - 00c88b20 ScaleRamped
  - 00c88bf0 ScaleCopyRamped
  - 00c88d10 Mix
  - 00c88d80 MixRamped

  The loop forms are `*p++ *= g;` (Scale), `_mm_store_ps((float*)d++, *s++ * g)` (ScaleCopy) and `*d = *d + *s * g; d++; s++;` (Mix). The ramps are a 4-float `F32vec4(start + 3*step, start + 2*step, start + step, start)` plus a 4-float increment. The 1-float constructor adds a `shufps` that retail lacks.
- **Reverb parameter updates** on CReverbProcessor:
  - 00c77ae0 UpdateLineGains and 00c77c00 UpdateTapLevels.
  - 00c78040 UpdateDelayLength (2nd grade). The parameter product is a named local before the rate's `fild`, and the limit is converted twice through if/else.
- **00c775e0 CMixerStream::AllocatePlanes**: free the old block, then allocate 3 * planeSize bytes through MixerAlloc and memset them.
- **00c78970 / 00c7ba00 BuildKernelRows4 / BuildKernelRows8** (2nd grade). These need the pointer walk `*out++ = (short)((*in-- * gain) / 32768)`; the indexed form loses. Each row is followed by memcpy row copies at fixed offsets.
- **Reverb DSP parts** (agent444's model: CDspObject base, CDspGain, CRingBuffer):
  - 00c89110 CReverbDiffuserStage::Allocate and 00c896c0 CReverbFilterTap::Allocate (2nd grade each). The ring {data, size, pos} is a CRingBuffer sub-object whose inline `Clear()` does the memset of `m_size` floats. Only that spelling gives retail's `mov ecx,esi ; shl ecx,2` count after the m_size/m_pos stores. A memset of `size * 4` in the body, or through any helper on `this`, gives `lea ecx,[esi*4]` scheduled above the stores.
  - 00c89aa0 CReverbFilterTap::Feedback (MATCH). An inner `for (j < 4)` accumulates onto `sum = feedback[i]`. VC fully unrolls the inner loop in source order and leaves the outer loop rolled; the one-expression sum is reassociated and unrolled four times.
  - 00c89b60 CReverbFilterTap::MixHistory. An inline `CRingBuffer::Back(pos, n)` is used for both offsets, then MixFromRing twice and virtual slot 7 Mix.
- **00c88780 COggVorbisStream::COggVorbisStream** (/EHsc): reader member, ov_open, ov_seekable, the comment walk, and the all-links-stereo check. Two spelling points:
  - The vorbisfile entry points must be declared with C++ linkage. Under /EHsc an `extern "C"` callee is nothrow, which drops retail's EH state-1 store before ov_open.
  - Named `vc` / `vi` locals plus an empty `for` comment walk give retail's ebp/edi assignment. The one-expression spelling swaps the two registers.
- **libvorbis 1.0.1 vorbisfile, verbatim**, as `extern "C" __fastcall` functions:
  - 00c8a7e0 ov_clear, minus the close_func call; its memset is 0x2C0 bytes.
  - 00c8a960 ov_bitrate_instant.
  - 00c8aa80 ov_pcm_total.

## Parked for Fable 5.1 (7)
- 00c77700 MillibelsToAmplitude DIFFER(38v109): retail has a hand-written x87 2^x block after the C prologue (`frndint ; fst ; fwait ; fld1 ; fscale ... f2xm1 ; fadd ; fmulp ; fstp ; fwait`); the C++ reference body calls pow | tried: the agent438 reference body (byte twin of 0x00C40EE0), 1 grade | next: lane gap: retail_asm candidate (minimal `__asm` 2^x block under the 2026-10-24 exception)
- 00c77b30 CReverbProcessor::UpdateLineLengths DIFFER(165v193): both inlined round helpers are bare `fld [slot] ; fistp dword [local]` after `fsub 0.5 ; fstp [slot]`; ours calls _ftol2 twice | tried: C++ cast reference body, 1 grade | next: lane gap: retail_asm candidate (`fistp dword` round helper)
- 00c77fc0 CReverbProcessor::UpdateChorusLength DIFFER(63v72): inline `fld [slot] ; fistp dword [esp]` round helper; ours calls _ftol2 | tried: C++ cast reference body, 1 grade | next: lane gap: retail_asm candidate (`fistp dword` round helper)
- 00c780c0 CReverbProcessor::UpdateTapPans DIFFER(116v133): retail keeps the 4-trip loop rolled with four induction variables (edi = sign pointer, eax = byte offset and counter `cmp eax,24h ; jb`, edx / esi = the two tap-field pointers); ours shares one IV per pair | tried: plain `for (i < 4)` (fully unrolled), step-2 `for (i = 1; i < 9; i += 2)` (graded; rolled, 2 IVs), `do { } while (++i < 4)` over m_taps[i] with a [4][2] table (rolled with retail's counter, still 2 IVs), pointer walks (`dec esi` counter), signed/unsigned and three product orders (cc.py probes) | next: likely LTCG (/GL) loop codegen. Try separate A/B index expressions (`g_signs[i][0]` vs `g_signs[0][2*i+1]`) once, else lane gap
- 00c89c10 _get_data DIFFER(133v112): retail is an LTCG-private static taking vf in esi (no ecx or stack argument), and its `errno = 0` is `call [__imp___errno]`; ours takes vf in ecx (__fastcall), and the lane's non-/MD errno is a direct store | tried: libvorbis 1.0.1 body with libpk's reader (Tell / GetSize / ReadBlock), 1 grade | next: lane gap: LTCG custom convention
- 00c89c80 _seek_helper DIFFER(45v41): retail takes vf in esi, the int64 offset on the stack, `ret 8`; ours takes vf in ecx (`push esi ; mov esi,ecx` prologue) | tried: libvorbis 1.0.1 body with CStreamReader::Seek(offset, 2 = from start), 1 grade | next: lane gap: LTCG custom convention
- 00c89cb0 _get_next_page DIFFER(187v187): retail takes vf in eax, with og and boundary on the stack (`ret 0Ch`), and calls _get_data with vf already in esi; ours takes vf in ecx and og in edx. The body is otherwise instruction for instruction | tried: libvorbis 1.0.1 body verbatim, 1 grade | next: lane gap: LTCG custom convention

## Skipped
None. Every row has a src/ or differ/ entry.

## Label corrections
This chunk's rows. Catalog label, then the corrected name:

- 00c775e0 CTCHeroStats_NotifyObservers → CMixerStream::AllocatePlanes
- 00c77700 CAudioChannel_GetOutputGain → MillibelsToAmplitude
- 00c77ae0 CAudioChannel_ApplyCompressorEffect → CReverbProcessor::UpdateLineGains
- 00c77b30 CAudioChannel_ApplyDistortionEffect → CReverbProcessor::UpdateLineLengths
- 00c77c00 CAudioChannel_ApplyReverbEffect → CReverbProcessor::UpdateTapLevels
- 00c77fc0 CAudioChannel_ApplyChorusEffect → CReverbProcessor::UpdateChorusLength
- 00c78040 CAudioChannel_ApplyDelayEffect → CReverbProcessor::UpdateDelayLength
- 00c780c0 Audio_Channel_ComputeEnvelope_Amplitude → CReverbProcessor::UpdateTapPans
- 00c78590 `__EH_epilog3` → M64::M64(__m64)
- 00c78760 `__EH_epilog3` → I16vec4::I16vec4(__m64)
- 00c78630 / 00c78670 / 00c786b0 Math_Add_Int64 / Math_ShiftRight_Int64 / _InPlace → Is32vec2::operator+= / >> / >>=
- 00c786e0 / 00c78720 Simd_PackLowDwords / Simd_PackHighDwords → unpack_low / unpack_high
- 00c787d0 / 00c78810 / 00c78850 / 00c78890 / 00c788d0 Simd_* → sat_add / mul_high / mul_add / pack_sat / operator+
- 00c7b870 operator+ → Is32vec4::operator+=
- 00c78970 / 00c7ba00 Audio_RenderMultichannelData(_Alt) → BuildKernelRows4 / BuildKernelRows8 (descriptive)
- 00c88780 Script_LoadAndInitialize → COggVorbisStream::COggVorbisStream
- 00c88a50..00c88d80 DSP_* → CMixKernelsSSE::Scale / ScaleCopy / ScaleRamped / ScaleCopyRamped / Mix / MixRamped
- 00c89110 / 00c896c0 GuildTrainingSkillScript_AllocSkillSlots / _AllocEffectArray → CReverbDiffuserStage::Allocate / CReverbFilterTap::Allocate
- 00c89aa0 Audio_ApplyConvolution_Samples → CReverbFilterTap::Feedback
- 00c89b60 WaveformBuffer_Process → CReverbFilterTap::MixHistory
- 00c89c10 SearchValue_InvokeCallback → _get_data
- 00c89c80 DiscardAsyncData → _seek_helper
- 00c89cb0 SearchValue_WithBorrow → _get_next_page
- 00c8a7e0 CAInputProcess_Shutdown → ov_clear
- 00c8a960 Audio_GetLevel_dB → ov_bitrate_instant
- 00c8aa80 GetSampleRecursive → ov_pcm_total

Already-landed or neighbouring rows whose names or models are wrong:

- **0x00C885C0 "CAInputProcess::~CAInputProcess"** is COggVorbisStream::~COggVorbisStream. Its model needs these fixes:
  - +0x78 is the "opened" flag, not a device pointer.
  - The +0x80 "handle Release" is OggVorbis_File + ov_clear 0x00C8A7E0.
  - The +4 member is CStreamReader, which is 0x70 bytes, not 0x74. +0x74 is the valid flag.
  - Base vtable 0x012BEFFC = IAudioStream.
- 0x00C88870 "CLandscapeBackgroundPatch vector_deleting_destructor" is COggVorbisStream's scalar deleting destructor (vtable 0x012BF1C4 slot 0).
- 0x00C4B270 "CTeleportDataOwner_00c4b270" dtor is CStreamReader::~CStreamReader (libpk reader, vtable 0x012B8458). Its ctor is 0x00C4B300 (manifest "PKCBufferedReader_Initialize").
- CStreamReader methods:
  - 0x00C4AA20 "CGameMap::GetMemoryFootprint" = Tell
  - 0x00C4AA30 "CWorld::DrawGetEnvironment" = GetSize
  - 0x00C4AFD0 = Seek(offset, origin 0 cur / 1 end / 2 start)
  - 0x00C4B890 "NScript_CV_MurderTwistScript_ReadImpl" = ReadBlock
- 0x00C744E0 "C00c744e0::M" = CDspGain::Reset.
- vorbisfile and libogg/libvorbis callees, all propagated labels: 0x00C8CAE0 ov_open, 0x00C8A950 ov_seekable ("DrawGetEnvironment"), 0x00C8BAE0 ov_comment, 0x00C8BAB0 ov_info, 0x00C8D470 ogg_sync_buffer ("MemoryStream_CompactAndRealloc"), 0x00C8D4F0 ogg_sync_wrote ("CTCInventoryBase::GetSelectedCategory"), 0x00C8D510 ogg_sync_pageseek ("SearchValue_ValidateCRC"), 0x00C8D950 ogg_sync_reset ("CDataInputStream::Close"), 0x00C8D420 ogg_sync_clear ("StdVector_Destroy"), 0x00C8CE80 ogg_stream_clear, 0x00C8DFB0 vorbis_info_clear ("C3DGenerator_Destructor"), 0x00C8DEF0 vorbis_comment_clear, 0x00C8F1B0 vorbis_block_clear, 0x00C8F650 vorbis_dsp_clear. 0x00C8A920 "CWorldMap::GetNextRegionOnRouteTo" is a vorbisfile accessor on vf->vi (+0x48) returning OV_EINVAL.
- SIMD neighbours:
  - 0x00C7B7D0 / 0x00C7B810 / 0x00C7B840 "CDateAndTime" / CVectorSSE ctors = dvec.h M128 / I32vec4 / Is32vec4(__m128i).
  - 0x00C7B8B0 / 0x00C7B8F0 "CBitfield::RightShiftCopy / RightShift" = Is32vec4::operator>>(int) / operator>>=(int). The landed 0x00C7B8F0 is declared void but returns *this.
  - 0x00C88930 = F32vec4(f3, f2, f1, f0).
  - 0x00C88990 / 0x00C889F0 "CQuadric::operator+=" and 0x00C889B0 / 0x00C88A10 "CMovie::GetSize" = F32vec4 add/mul operators.
  - 0x00C88A30 = CMixKernelsSSE ctor; 0x00C88A40 = its slot 0.
  - 0x00C888A0..0x00C888E0 = stub slots of vtable 0x012BF1DC.
- agent444's parked 0x00C75F80 CReverbTap::Allocate has the same `mov ecx,esi ; shl ecx,2` memset count as 00c89110 / 00c896c0. Moving m_data/m_size/m_pos into a CRingBuffer sub-object whose inline `Clear()` does the memset should match it.
