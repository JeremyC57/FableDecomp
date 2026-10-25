# Hand chunk 446 (vorbisfile ov_time_total .. libvorbis vorbis_synthesis_init): 22 of 40 byte-exact

The rows 0x00C8AAF0..0x00C8FF00 are not libpk's own code. They are the Xiph Ogg Vorbis decoder that libpk links
statically: vorbisfile (0x00C89xxx..0x00C8CAxx), libogg framing.c (0x00C8CCxx..0x00C8D5xx), libvorbis info.c
(0x00C8DBxx..0x00C8EAxx), synthesis.c (0x00C8EEF0, 0x00C8EFC0) and block.c (0x00C8F0D0..0x00C8FF00).

How the library was identified:
- 0x00C8E790 copies the 30-byte vendor string "Xiph.Org libVorbis I 20030909" (@ 0x012BF240) and "vorbis" (@ 0x012BF238).
- 0x00C8CF60 is the CRC-32 page checksum over the static `crc_lookup` table @ 0x0139D9B0, at header bytes 22..25.
- The structure sizes match the Xiph ones: ogg_stream_state 0x168, vorbis_block 0x70, codec_setup_info 0xE80,
  and OggVorbis_File 0x2C0. The libpk fork drops the ov_callbacks member and calls the libpk reader directly
  (0x00C4AFD0 seek, 0x00C4AA20 tell).
- The tree is a post-1.0.1 CVS snapshot. It has `codec_setup_info::halfrate_flag` (+0xE78), the crosslap
  `_ov_initprime`, and `vorbis_synthesis_trackonly` storing `sequence = packetno` with no `- 3`.

There is no PDB or RTTI. Every name is the Xiph API or static-function name, which is a real source name
(not a descriptive one). The LTCG link changed the calling conventions:
- API functions take ecx/edx, which is `__fastcall`. Sources declare them `extern "C" ... __fastcall`, so the
  symbols are `@name@N`.
- Static helpers take eax/ebx/esi/edi, which is a custom convention (the lane gap below).
- `_vorbis_pack_comment` takes ecx plus the stack, callee-clean. That is the thiscall shape, modelled as a member
  of `oggpack_buffer`.

Every manifest label in the chunk was a propagated game-side name (see Label corrections). Every src/ file grades
MATCH/RELOCATION_MATCH through gall.sh (grades.txt). Every test prints PASS under tall.sh (g++) and tall.py
(VC7.1/Wine). All IAT slots were checked against the PE import table: malloc 0x01440158, calloc 0x01440058, realloc
0x01440054, free 0x01440154, memmove 0x01440144, toupper 0x014400DC (all MSVCR71).

## What matched (22)

- **vorbisfile (4).**
  - 0x00C8AAF0 ov_time_total: the 1.0.x body verbatim. It recurses through `__fastcall`, and the double
    accumulator gets an `and esp,-8` frame.
  - 0x00C8B9D0 ov_time_tell. SHAPE: give it the real ov_time_total body in the same TU, under
    `auto_inline(off)` + `__declspec(noinline)`. VC7.1 then knows the callee writes no memory and hoists
    `&vf->pcmlengths[link*2+1]` into ebx across the loop calls. With an extern callee it reloads the base every
    pass (209v212).
  - 0x00C8BAB0 ov_info and 0x00C8BAE0 ov_comment: the nested if/else verbatim (MATCH).
- **libogg framing.c (7).**
  - 0x00C8CD30 ogg_page_granulepos and 0x00C8CDE0 ogg_page_pageno: the libogg expressions verbatim (MATCH).
    VC7.1 reassociates pageno into the `mov ch ; mov cl` shape.
  - 0x00C8CE20 ogg_stream_init: verbatim. The third malloc re-reads lacing_storage after the second call.
  - 0x00C8CE80 ogg_stream_clear: verbatim.
  - 0x00C8CF60 ogg_page_checksum_set: verbatim. Each header byte store re-reads `og->header`.
  - 0x00C8D420 ogg_sync_clear: calls ogg_sync_init 0x00C8D400 (`__fastcall`).
  - 0x00C8D470 ogg_sync_buffer: memmove must be the dllimport IAT call. The `<string.h>` memmove is a direct call.
- **libvorbis info.c (4).**
  - 0x00C8DBF0 vorbis_comment_add: verbatim. strlen/strcpy are the /O2 intrinsics.
  - 0x00C8DC90 vorbis_comment_add_tag: `_alloca` (`__alloca_probe` = retail 0x00BFEA30 `__chkstk`). The "=" strcat
    is a word copy.
  - 0x00C8DEF0 vorbis_comment_clear: the memset sits outside `if (vc)`.
  - 0x00C8EA70 vorbis_commentheader_out: `__fastcall`. `_vorbis_pack_comment` is called as a member of the local
    packer, which gives the thiscall shape, and oggpack_bytes is called three times.
- **libvorbis synthesis.c (2).**
  - 0x00C8EEF0 vorbis_synthesis_trackonly: `sequence = packetno` (this tree has no `- 3`). The bit reader is
    `__fastcall` (oggpack_readinit takes its third argument on the stack).
  - 0x00C8EFC0 vorbis_packet_blocksize: verbatim, with modebits as the `sar ; inc` loop.
- **libvorbis block.c (5).**
  - 0x00C8F0D0 _vorbis_block_alloc and 0x00C8F140 _vorbis_block_ripcord: verbatim.
  - 0x00C8F1B0 vorbis_block_clear: frees localstore and internal. The memset's eax = 0 doubles as `return 0`.
  - 0x00C8F880 vorbis_analysis_buffer: verbatim.
  - 0x00C8FEA0 vorbis_synthesis_restart: the halfrate version (MATCH).

## Parked for Fable 5.1 (18)
- 00c8c1d0 _ov_initset DIFFER(48v37): retail takes vf in esi (unsaved, no prologue) and passes it to _fetch_and_process_packet 0x00C8A540 in eax | tried: the vorbisfile body as __fastcall (1 grade) | next: lane gap: LTCG custom register arguments
- 00c8c200 _ov_initprime DIFFER(65v54): vf in esi (unsaved), `lea ebx,[esi+1E0h]`, vf passed in eax to 0x00C8A540 | tried: vorbisfile body as __fastcall (1 grade) | next: lane gap: LTCG custom register arguments
- 00c8c8c0 _open_seekable2 DIFFER(224v217): vf in esi (unsaved); _get_prev_page 0x00C89D70 and _prefetch_all_headers 0x00C8A1D0 take vf in eax, _bisect_forward_serialno 0x00C89E80 takes the serial number in edx | tried: vorbisfile body over the libpk reader (1 grade) | next: lane gap: LTCG custom register arguments
- 00c8c9a0 _ov_open1 DIFFER(241v233): initial in eax, vf in ebx (unsaved), f/ibytes on the stack; _fetch_headers 0x00C8A0A0 takes og_ptr in eax | tried: vorbisfile 1.0.x body with the libpk reader (1 grade) | next: lane gap: LTCG custom register arguments
- 00c8ca90 _ov_open2 DIFFER(73v71): vf in eax (`mov esi,eax`), passed to _open_seekable2 in esi | tried: vorbisfile body (1 grade) | next: lane gap: LTCG custom register arguments
- 00c8cae0 ov_open_callbacks DIFFER(35v33): the row is __fastcall, but _ov_open1 wants initial in eax and vf in ebx and _ov_open2 wants vf in eax | tried: vorbisfile body (1 grade) | next: lane gap: LTCG custom register arguments (callee)
- 00c8cef0 _os_body_expand DIFFER(42v38): os in esi and needed in ecx, nothing saved | tried: libogg body as __fastcall (1 grade) | next: lane gap: LTCG custom register arguments
- 00c8cf20 _os_lacing_expand DIFFER(68v60): os in esi, needed in ecx | tried: libogg body as __fastcall (1 grade) | next: lane gap: LTCG custom register arguments
- 00c8db80 _v_writestring DIFFER(38v35): o in ebx (unsaved), s in ecx, bytes in eax | tried: libvorbis loop as __fastcall (1 grade) | next: lane gap: LTCG custom register arguments
- 00c8dbb0 _v_readstring DIFFER(40v37): o in ebx (unsaved), buf in ecx, bytes in eax | tried: libvorbis loop as __fastcall (1 grade) | next: lane gap: LTCG custom register arguments
- 00c8dd30 tagcompare DIFFER(88v88): s2 in eax (`mov esi,eax`), s1 in ecx, n on the stack | tried: libvorbis body as __fastcall (1 grade) | next: lane gap: LTCG custom register arguments
- 00c8dd90 vorbis_comment_query DIFFER(185v185): everything matches except the tagcompare call, where retail passes s2 in eax (`mov eax,[ebp-0Ch]`) and ours in edx | tried: libvorbis body with alloca (1 grade) | next: lane gap: LTCG custom register arguments (callee 0x00C8DD30)
- 00c8de50 vorbis_comment_query_count DIFFER(156v156): only the tagcompare call differs, `mov eax,ebx` (s2 in eax) against ours in edx | tried: libvorbis body with alloca (1 grade) | next: lane gap: LTCG custom register arguments (callee 0x00C8DD30)
- 00c8e120 _vorbis_unpack_info DIFFER(234v220): vi in edi and opb in esi, both unsaved | tried: libvorbis body as __fastcall (1 grade) | next: lane gap: LTCG custom register arguments
- 00c8e200 _vorbis_unpack_comment DIFFER(218v221): vc in esi (unsaved), opb on the stack, _v_readstring takes o in ebx / bytes in eax | tried: libvorbis body (1 grade) | next: lane gap: LTCG custom register arguments
- 00c8e790 oggpack_buffer::_vorbis_pack_comment DIFFER(239v230): the /GS frame and thiscall shape line up, but every _v_writestring 0x00C8DB80 call passes o in ebx and bytes in eax | tried: libvorbis body as a packer member under /GS (1 grade) | next: lane gap: LTCG custom register arguments (callee)
- 00c8f5f0 vorbis_analysis_init DIFFER(80v83): the row is __fastcall, but _vds_shared_init 0x00C8F200 takes v in ebx (`mov ebx,ecx` before the pushes) | tried: libvorbis body (1 grade) | next: lane gap: LTCG custom register arguments (callee)
- 00c8ff00 vorbis_synthesis_init DIFFER(32v33): the row is __fastcall, but _vds_shared_init 0x00C8F200 takes v in ebx | tried: libvorbis body (1 grade) | next: lane gap: LTCG custom register arguments (callee)

## Skipped
None. No row holds two functions.

## Label corrections
Every manifest label in the chunk is a propagated game-side name. The real names are the Xiph ones:
- Matched rows:
  - 0x00C8AAF0 GetNormalizedSample -> ov_time_total
  - 0x00C8B9D0 Audio_GetCurrentSamplePosition -> ov_time_tell
  - 0x00C8BAB0 Indexed_GetElementOffsetSafe -> ov_info
  - 0x00C8BAE0 Script_GetEntryPointer -> ov_comment
  - 0x00C8CD30 Timecode_UnpackBytes -> ogg_page_granulepos
  - 0x00C8CDE0 SetDebugBoxColour1 -> ogg_page_pageno
  - 0x00C8CE20 CreatureHitNotification_Buffer_Initialize -> ogg_stream_init
  - 0x00C8CE80 EntityData_FreeResources -> ogg_stream_clear
  - 0x00C8CF60 CRC32_ComputeBuffer -> ogg_page_checksum_set
  - 0x00C8D420 StdVector_Destroy -> ogg_sync_clear
  - 0x00C8D470 MemoryStream_CompactAndRealloc -> ogg_sync_buffer
  - 0x00C8DBF0 RTC_AllocateDebugBuffer -> vorbis_comment_add
  - 0x00C8DC90 _RTC_StackFailure -> vorbis_comment_add_tag
  - 0x00C8DEF0 Array_FreeAllElements -> vorbis_comment_clear
  - 0x00C8EA70 Audio_CreateAlignedBuffer -> vorbis_commentheader_out
  - 0x00C8EEF0 WorldMap_LoadNotifications -> vorbis_synthesis_trackonly
  - 0x00C8EFC0 Bitstream_ReadTableValue -> vorbis_packet_blocksize
  - 0x00C8F0D0 MemAllocator_Allocate -> _vorbis_block_alloc
  - 0x00C8F140 MemoryPool_FreeNodes -> _vorbis_block_ripcord
  - 0x00C8F1B0 Pair_DestroyElements -> vorbis_block_clear
  - 0x00C8F880 Buffer_Reallocate -> vorbis_analysis_buffer
  - 0x00C8FEA0 AudioFormat_GetChannelMask -> vorbis_synthesis_restart
- Parked rows:
  - 0x00C8C1D0 CreatureHitNotification_WaitForCompletion -> _ov_initset
  - 0x00C8C200 VertexBuffer_WaitAndAllocate -> _ov_initprime
  - 0x00C8C8C0 AsyncBankFile_Search -> _open_seekable2
  - 0x00C8C9A0 CreatureHitNotification_Load -> _ov_open1
  - 0x00C8CA90 CreatureHitNotification_Shutdown -> _ov_open2
  - 0x00C8CAE0 lua_dobuffer -> ov_open_callbacks (the libpk fork has no callbacks argument)
  - 0x00C8CEF0 WeatherRenderer_AllocateBuffer -> _os_body_expand
  - 0x00C8CF20 WeatherRenderer_ReallocateBuffer -> _os_lacing_expand
  - 0x00C8DB80 _Fill_n -> _v_writestring
  - 0x00C8DBB0 _Fill_n -> _v_readstring
  - 0x00C8DD30 CCharString_CompareNoCase -> tagcompare
  - 0x00C8DD90 CCharString_FindOccurrence -> vorbis_comment_query
  - 0x00C8DE50 CCharString_CountOccurrences -> vorbis_comment_query_count
  - 0x00C8E120 C3DGenerator_ReadGeometry -> _vorbis_unpack_info
  - 0x00C8E200 C3DGenerator_ReadAnimationData -> _vorbis_unpack_comment
  - 0x00C8E790 CreatureHitNotification_Serialize -> _vorbis_pack_comment
  - 0x00C8F5F0 AudioSystem_InitializeAudio -> vorbis_analysis_init
  - 0x00C8FF00 Audio_InitializeIfAttached -> vorbis_synthesis_init
- Callees outside the chunk (each checked from its body and its call sites):
  - vorbisfile:
    - 0x00C89D70 AsyncBankFile_Process -> _get_prev_page
    - 0x00C89E80 BankFile_BinarySearch -> _bisect_forward_serialno
    - 0x00C8A0A0 CreatureHitNotification_Initialize -> _fetch_headers
    - 0x00C8A1D0 Subsystem_ProcessAllNotifications -> _prefetch_all_headers
    - 0x00C8A540 CreatureHitNotification_StateProcess -> _fetch_and_process_packet
    - 0x00C8A7E0 CAInputProcess_Shutdown -> ov_clear
    - 0x00C8AA80 GetSampleRecursive -> ov_pcm_total
    - 0x00C8AB70 CreatureHitNotification_Process -> ov_raw_seek
  - libogg:
    - 0x00C8D400 construct<...> -> ogg_sync_init
    - 0x00C90F10 CVsyncCallbackTimerInfo -> oggpack_writeinit
    - 0x00C90FC0 CBitstream::WriteBits -> oggpack_write
    - 0x00C91320 _Dest_val<...> -> oggpack_writeclear
    - 0x00C91360 OpenMemorySource -> oggpack_readinit
    - 0x00C915A0 CBitstream::ReadBits -> oggpack_read
    - 0x00C917B0 AlignValue_8Byte -> oggpack_bytes
  - libvorbis:
    - 0x00C8DFB0 C3DGenerator_Destructor -> vorbis_info_clear
    - 0x00C8F200 AudioSystem_Initialize -> _vds_shared_init
    - 0x00C908E0 VertexBuffer_GetRange -> vorbis_synthesis_pcmout
    - 0x00C91820 Memory_AllocateSmall -> _vp_global_look
    - 0x00C9CD20 CVsyncCallbackTimerInfo::Initialize -> vorbis_bitrate_init
    - 0x00C9DB40 FFT_InitializeTwiddleTable -> _ve_envelope_init
- Already-landed neighbours whose names are wrong (see LESSONS.md, Review at landing):
  - 0x00C8A920 -> ov_halfrate_p (landed through engine/CWorldMap.h, which is the wrong class)
  - 0x00C8A950 -> ov_seekable
  - 0x00C8B9B0 -> ov_pcm_tell
  - 0x00C8BB10 -> host_is_big_endian (likely)
  - 0x00C8CD00 -> ogg_page_continued
  - 0x00C8CD10 -> ogg_page_bos
  - 0x00C8CD20 -> ogg_page_eos
  - 0x00C8CDC0 -> ogg_page_serialno
  - 0x00C8CED0 -> ogg_stream_destroy
  - 0x00C8D450 -> ogg_sync_destroy
  - 0x00C8D4F0 -> ogg_sync_wrote
  - 0x00C8D950 -> ogg_sync_reset
  - 0x00C8D970 -> ogg_stream_reset
  - 0x00C8DB10 -> ogg_stream_packetout (landed as an ASM_BAKE std::_Dest_val<CFlash> thunk)
  - 0x00C8DB30 -> ogg_packet_clear
  - 0x00C8DBE0 -> vorbis_comment_init
  - 0x00C8DF60 -> vorbis_info_blocksize
  - 0x00C8DF70 -> vorbis_info_init
  - 0x00C8F050 -> vorbis_synthesis_halfrate_p
  - 0x00C8F080 -> vorbis_block_init
  - 0x00C8C730 -> _ov_64_seek_lap (landed as a naked ASM_BAKE "Batch_InvokeWithStackArgs")
- Unlanded neighbours named on the way:
  - 0x00C8D9C0 -> ogg_stream_reset_serialno
  - 0x00C8D9D0 -> _packetout (LTCG custom: os in eax, op in ecx, adv on the stack)
  - 0x00C8DB20 -> ogg_stream_packetpeek
