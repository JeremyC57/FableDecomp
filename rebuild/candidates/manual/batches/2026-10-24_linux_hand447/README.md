# Hand chunk 447 (libvorbis::vorbis_synthesis_pcmout .. libvorbis::res0_free_look): 27 of 40 byte-exact

The chunk is NOT libpk. 0x00C908E0..0x00C9FDF0 is the statically linked Xiph codec: libogg's bitwise.c and
libvorbis 1.0.1. The version string "Xiph.Org libVorbis I 20030909" sits at 0x00EBF240 next to "vorbis" / "OggS"
(0x00EBF230). The rows are block.c, bitwise.c, psy.c (+ scales.h), sharedbook.c, codebook.c, smallft.c, mdct.c,
bitrate.c, envelope.c and res0.c. The callers back this up: vorbisfile's ov_read sits at 0x00C8A5xx..0x00C8C2xx and
calls pcmout/read. vorbis_dsp_clear at 0x00C8F6xx calls drft_clear / mdct_clear / vorbis_bitrate_clear. The residue
export tables at 0x013ABDD0/F0/E10 hold res0_free_info 0x00C9FDD0 and res0_free_look 0x00C9FDF0. The mask table at
0x0139DDB8 starts 0,1,3,7,... Every name below is the real libvorbis/libogg function name (no PDB: the names come
from matching the bodies to the Xiph source).

Method: each row was rebuilt from the shipped Xiph 1.0.1 source (struct layouts from codec.h, codec_internal.h,
psy.h, envelope.h, bitrate.h, backends.h, codebook.h; function bodies as Xiph wrote them). Every non-LTCG row
graded MATCH (9) or RELOCATION_MATCH (18) on its FIRST grade. All 27 src/ files grade MATCH/RELOCATION_MATCH through
gall.sh (grades.txt), and every test prints PASS under tall.sh (g++, tall_gpp.txt) and tall.py (VC7.1/Wine,
tall_vc.txt). 13 rows are parked: all of them take LTCG custom register arguments. 0 skipped.

Conventions:
- The public and address-taken functions are `__fastcall`, with the first two integer-sized arguments in ecx/edx,
  floats on the stack and a callee pop. res0_free_look is reached through the residue function table and still
  takes ecx, so the declared convention is fastcall (/Gr), not an LTCG rewrite.
- The qsort comparators (apsort, sort32a) are `__cdecl`.
- The `static`/`STIN` helpers were given custom conventions by LTCG (args in eax/ebx/esi/edi, or ecx/edx swapped).
  That is a lane gap.
- Sources use `extern "C"` (C library, symbols `@name@N`) and explicit conventions, with no #include.
- CRT calls are explicit `__declspec(dllimport)` declarations through the MSVCR71 IAT: realloc 0x01440054,
  calloc 0x01440058, free 0x01440154, ldexp 0x01440014, floor 0x01440100, qsort 0x01440060 and memmove 0x01440144.
- `#pragma intrinsic` covers memset, sqrt, fabs, pow (→ _CIpow 0x00BFEB40) and _alloca (→ __alloca_probe, which is
  retail's __chkstk 0x00BFEA30).

## What matched (27)

- **libvorbis block.c**
  - 0x00C908E0 vorbis_synthesis_pcmout and 0x00C90930 vorbis_synthesis_read.
  - The shipped bodies over the real vorbis_dsp_state layout (pcmret +0x0c, pcm_current +0x14, pcm_returned +0x18;
    `vi` is read before the range test).
- **libogg bitwise.c**
  - The rows: 0x00C910C0 oggpackB_write (big-endian writer, realloc +256), 0x00C913A0 oggpack_look, 0x00C91530
    oggpack_adv and 0x00C917B0 oggpack_bytes.
  - They are verbatim libogg with the oggpack_buffer {endbyte, endbit, buffer, ptr, storage} layout. The bits
    parameter becomes the running total, and `bits/8` is a signed divide.
- **libvorbis psy.c**
  - 0x00C91820 _vp_global_look: VC emits the three stores in reverse after calloc(1,0x24), and -9999 is the
    immediate 0xC61C3C00. The model includes the real codec_setup_info prefix (psy_g_param at +0xb34).
  - 0x00C92640 unitnorm (the scales.h pointer-pun form, which rewrites the argument slot).
  - 0x00C92BF0 _vp_psy_clear: P_BANDS 17 x P_LEVELS 8 tone curves and 3 noise curves; memset → `rep stosd` x12.
  - 0x00C94190 _vp_ampmax_decay: `(float)n/vi->rate` → fild/fidiv; gi->ampmax_att_per_sec is ci+0xb78.
  - 0x00C94310 dipole_hypot and 0x00C943F0 round_hypot: `> 0.` compares as double, and VC evaluates both sign tests
    up front.
  - 0x00C945B0 apsort (cdecl qsort comparator).
  - 0x00C94750 _vp_noise_normalize_sort: an ebp frame for `_alloca`, and qsort with apsort as an immediate.
    vorbis_info_psy normal_start is at +0x1fc and normal_partition at +0x200.
- **libvorbis sharedbook.c**
  - 0x00C94F90 _float32_unpack (ldexp through the IAT; the double result is returned unrounded).
  - 0x00C95140 _book_maptype1_quantvals (`floor(pow((float)entries, 1.f/dim))` → _CIpow + floor import + __ftol2).
  - 0x00C95370 vorbis_staticbook_clear: the nearest/threshold trees' memsets of 24/16 bytes inline as stores, and
    the 0x34-byte one as `rep stosd`.
  - 0x00C95560 sort32a (cdecl).
- **libvorbis codebook.c**
  - 0x00C96170 vorbis_book_errorv: calls `_best(book, a, 1)` 0x00C95A10, then a 4x-unrolled copy that re-reads
    valuelist. It has no callers in retail.
  - 0x00C96700 vorbis_book_decodev_set and 0x00C96760 vorbis_book_decodevv_add.
    - The STIN decode_packed_entry_number 0x00C96320 was kept out of line by LTCG with both arguments pushed and
      `ret 8`.
    - Declaring it `__stdcall` reproduces that call shape exactly.
- **libvorbis smallft.c / mdct.c**: 0x00C9BC90 drft_clear and 0x00C9C4C0 mdct_clear. Their 12- and 20-byte memsets
  inline as stores.
- **libvorbis bitrate.c**
  - 0x00C9CF10 vorbis_bitrate_clear: bitrate_manager_state is 0x68 bytes with avgfloat (double) at +0x58.
  - It calls oggpack_writeclear 0x00C91320 on each queued buffer.
- **libvorbis envelope.c**
  - 0x00C9E4A0 _ve_envelope_mark: the envelope_lookup layout has mark +0xa0 and curmark +0xac, and the private_state
    ve is reached through v->backend_state +0x68.
  - 0x00C9E560 _ve_envelope_shift: VE_POST folds into `lea eax,[eax*4+8]` for memmove.
- **libvorbis res0.c**: 0x00C9FDF0 res0_free_look (vorbis_look_residue0 without TRAIN_RES, 0x2c bytes).

## Parked for Fable 5.1 (13)

- 00c918c0 min_curve DIFFER(180v183): retail `mov esi,ecx ; mov edi,eax ; sub edi,esi ; lea edx,[eax+0Ch]` (c2 arrives in EAX); ours takes c2 in edx | tried: shipped source as `__fastcall` (1 grade) | next: lane gap: LTCG custom calling convention (register argument)
- 00c91980 max_curve DIFFER(180v183): the same EAX c2 argument as min_curve | tried: shipped source as `__fastcall` (1 grade) | next: lane gap: LTCG register argument
- 00c91a40 attenuate_curve DIFFER(97v97): only the first instruction differs: retail `add eax,8` (c in EAX), ours `lea eax,[ecx+8]` | tried: shipped source as `__fastcall` (1 grade) | next: lane gap: LTCG register argument
- 00c92e20 seed_loop DIFFER(235v237): retail reads p from EBX without saving it (`mov eax,[ebx+4]`), curves..specmax on the stack (`ret 14h`); its callee seed_curve 0x00C92CA0 is LTCG too | tried: shipped source as `__fastcall` (1 grade) | next: lane gap: LTCG register argument
- 00c941e0 couple_lossless DIFFER(202v202): retail has qA in EDX and qB in ECX (the reverse of fastcall); every other byte matches | tried: shipped source (1 grade); pointers declared in swapped order `(A, B, qB, qA)` (1 grade) = RELOCATION_MATCH (probe/00c941e0_swapped.cpp) | next: owner call. The swapped declaration is byte-exact compiler output but misstates the Xiph signature; otherwise lane gap: LTCG register argument
- 00c942b0 precomputed_couple_point DIFFER(99v91): retail floorA in EDI, floorB in ESI (`cmp edi,esi ; setg cl`), premag/mag/ang on the stack (`ret 0Ch`); ours pushes ebx/esi/edi and takes ecx/edx | tried: shipped source as `__fastcall` (1 grade) | next: lane gap: LTCG register argument
- 00c954f0 bitreverse DIFFER(99v99): retail x in EAX (`mov ecx,eax ; shl eax,10h`); ours x in ecx with renamed registers | tried: shipped source as `__fastcall` (1 grade) | next: lane gap: LTCG register argument
- 00c962b0 bitreverse (codebook.c twin) DIFFER(99v99): byte-identical to 0x00C954F0, the same EAX argument | tried: the same source in `namespace codebook_c` (1 grade) | next: lane gap: LTCG register argument
- 00c9bc10 drft_forward DIFFER(29v30): the row's own ecx/edx arguments match, but retail calls drftf1 0x00C98E80 with c in EDI, ch in EAX, ifac in ECX and n/wa pushed (callee pop) | tried: shipped source with a `__fastcall` drftf1 declaration (1 grade) | next: lane gap: LTCG callee convention
- 00c9beb0 mdct_butterfly_8 DIFFER(125v125): retail x in EAX; our 47 instructions are identical modulo the base register (ecx) | tried: shipped source as `__fastcall` (1 grade) | next: lane gap: LTCG register argument
- 00c9c400 mdct_butterflies DIFFER(174v185): retail init in EAX (`mov edx,[eax+8]`), x/points on the stack, and three LTCG callees (0x00C9C1E0 edi/ecx, 0x00C9C2F0 ecx/edx + push, 0x00C9C000 eax) | tried: shipped source as `__fastcall` (1 grade) | next: lane gap: LTCG register arguments
- 00c9cb80 floater_interpolate DIFFER(215v211): retail bm in ESI (`fld qword [esi+58h]` first), vi/desired_rate on the stack (`ret 0Ch`); ours pushes esi/ebx and takes ecx/edx | tried: shipped source as `__fastcall` (1 grade) | next: lane gap: LTCG register argument
- 00c9cc60 limit_sum DIFFER(170v178): retail bm in EBX (`mov ecx,[ebx+3Ch]` first), limit on the stack (`ret 4`) | tried: shipped source as `__fastcall` (1 grade) | next: lane gap: LTCG register argument

## Skipped

None. Every row ends at its own `ret` plus int3 padding.

## Label corrections

All 40 manifest labels in the chunk are propagated or descriptive and wrong. The correct names are the Xiph
function names:

| Row | Manifest label | Correct name |
|---|---|---|
| 0x00C908E0 | VertexBuffer_GetRange | vorbis_synthesis_pcmout (libvorbis block.c) |
| 0x00C90930 | VertexBuffer_CommitData | vorbis_synthesis_read |
| 0x00C910C0 | Bitstream_WriteBits | oggpackB_write (libogg bitwise.c) |
| 0x00C913A0 | CBitstream_ReadBitsVariable | oggpack_look |
| 0x00C91530 | CBitstream_AdvanceBitPosition | oggpack_adv |
| 0x00C917B0 | AlignValue_8Byte | oggpack_bytes |
| 0x00C91820 | Memory_AllocateSmall | _vp_global_look (psy.c) |
| 0x00C918C0 | BoundingBox_UpdateMin | min_curve |
| 0x00C91980 | BoundingBox_AccumulateMax | max_curve |
| 0x00C91A40 | Vector8f_AddScalar | attenuate_curve |
| 0x00C92640 | Float_SignBitExtract | unitnorm |
| 0x00C92BF0 | GraphicsResource_FreeMemory | _vp_psy_clear |
| 0x00C92E20 | Graphics_ApplyConversion | seed_loop |
| 0x00C94190 | Audio_ComputeClampedPosition | _vp_ampmax_decay |
| 0x00C941E0 | Vector_NormalizeCross | couple_lossless |
| 0x00C942B0 | Audio_ComputeLog2WeightedCoeff | precomputed_couple_point |
| 0x00C94310 | Math_SignedSqrtMagnitude | dipole_hypot |
| 0x00C943F0 | Math_SignedSqrtMagnitudeAlt | round_hypot |
| 0x00C945B0 | Float_CompareAbsValue | apsort |
| 0x00C94750 | Mesh_SortIndicesByOffset | _vp_noise_normalize_sort |
| 0x00C94F90 | Float_DecompressCompactRep | _float32_unpack (sharedbook.c) |
| 0x00C95140 | Waveform_FindBestFrequencyFit | _book_maptype1_quantvals |
| 0x00C95370 | CEngineResourceManager_Cleanup | vorbis_staticbook_clear |
| 0x00C954F0 | Bit_ReverseBits | bitreverse (sharedbook.c) |
| 0x00C95560 | Uint_Compare | sort32a |
| 0x00C96170 | Matrix_CopyRectTransposed | vorbis_book_errorv (codebook.c) |
| 0x00C962B0 | Util_BitReversalPermutation | bitreverse (codebook.c copy) |
| 0x00C96700 | PropertyDef_CopyMatrix | vorbis_book_decodev_set |
| 0x00C96760 | PropertyDef_AccumulateMatrixByStride | vorbis_book_decodevv_add |
| 0x00C9BC10 | DiscardAsyncData | drft_forward (smallft.c) |
| 0x00C9BC90 | GFCheckTreeNodes | drft_clear |
| 0x00C9BEB0 | Sphere_Initialize | mdct_butterfly_8 (mdct.c) |
| 0x00C9C400 | Mesh_ProcessLOD | mdct_butterflies |
| 0x00C9C4C0 | GFCheckTreeNodes | mdct_clear |
| 0x00C9CB80 | Float_ConvertToInteger | floater_interpolate (bitrate.c) |
| 0x00C9CC60 | Array_LinearSearch | limit_sum |
| 0x00C9CF10 | AnimationEventMap_Destructor | vorbis_bitrate_clear |
| 0x00C9E4A0 | Audio_CheckBufferRegionActive | _ve_envelope_mark (envelope.c) |
| 0x00C9E560 | Memory_MoveData | _ve_envelope_shift |
| 0x00C9FDF0 | C3DGeneratorObject_Destroy | res0_free_look (res0.c) |

Callees and neighbours outside the chunk:

| Address | Manifest label | What it is |
|---|---|---|
| 0x00C95A10 | Waveform_FindClosestFrequencyBin | _best (sharedbook.c) |
| 0x00C96320 | PropertyDef_LookupByClass | decode_packed_entry_number (codebook.c, LTCG stack/callee-pop) |
| 0x00C98E80 / 0x00C9B9F0 | FFT_2D_DispatchByRadix / Audio_TransformFFTCoefficients | drftf1 / drftb1 (smallft.c, LTCG register arguments) |
| 0x00C9BC30 | (none) | drft_backward |
| 0x00C92CA0 | Graphics_ConvertFloatToFixed | seed_curve (psy.c) |
| 0x00C9C000 / 0x00C9C1E0 / 0x00C9C2F0 | C3DBoundingBox::TransformByRotation / FFT_Radix2MixedStride / FFT_Radix2VariableStride | mdct_butterfly_32 / mdct_butterfly_first / mdct_butterfly_generic (mdct.c) |
| 0x00C91320 | _Dest_val<...CSmallVector<NAnimationEvents::CEvent,32>...> (landed as FreeBufferHeader) | oggpack_writeclear |

Already-landed rows in the same library (their sources model them as libpk/engine classes; see LESSONS.md):

| Address | Landed as | Real name |
|---|---|---|
| 0x00C90F10 | SStringBuffer_00c90f10::Init | oggpack_writeinit |
| 0x00C90FC0 | CBitstream::WriteBits | oggpack_write |
| 0x00C911B0 | CBitstream_00c911b0::FlushByte | oggpack_writealign |
| 0x00C912F0 | CTextBuffer_00c912f0::Clear | oggpack_reset |
| 0x00C91320 | FreeBufferHeader | oggpack_writeclear |
| 0x00C915A0 | ReadBits_00c915a0 | oggpack_read |
| 0x00C918A0 | _Cons_val<std::allocator<C3DGeneratorObject>,...> | _vi_psy_free (memset 0x210 = sizeof vorbis_info_psy + free) |
| 0x00C94EE0 | Bitcount_00c94ee0 | _ilog (sharedbook.c) |
| 0x00C95420 | CEngineResourceManager::WorldFrameUpdate | vorbis_staticbook_destroy (allocedp test, staticbook_clear 0x00C95370, free) |
| 0x00C954A0 | InitCodecStage_00c954a0 | vorbis_book_init_encode (codebook 0x2c bytes) |
| 0x00C96140 | CCodeTable_00c96140::WriteCode | vorbis_book_encode |
| 0x00C96430 | CDefinitionManager::GetPDefFromClassIndex | vorbis_book_decode (decode_packed_entry_number, then dec_index +0x18) |
| 0x00C9CD20 | InitStreamDecoder_00c9cd20 | vorbis_bitrate_init (0x68-byte bitrate_manager_state) |
| 0x00C9DCE0 | SBufferRecord00c9dce0::FreeBuffers | _ve_envelope_clear (0xB4 envelope_lookup; calls mdct_clear 0x00C9C4C0) |
| 0x00C9EC00 | _Cons_val<std::allocator<C3DGeneratorObject>,...> | mapping0_free_info (memset 0xC88 = sizeof vorbis_info_mapping0 + free) |
| 0x00C9FDD0 | _Cons_val<std::allocator<C3DGeneratorObject>,...> | res0_free_info (memset 0x714 = sizeof vorbis_info_residue0 + free; in the residue tables) |
| 0x00C9FE70 | Util_CountHighBits | ilog (res0.c copy) |
| 0x00C8DF70 | SCodecInfo_00c8df70::Init | vorbis_info_init (zero 8 words, calloc the codec_setup_info) |
