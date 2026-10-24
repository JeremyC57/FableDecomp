# Hand chunk 430 (_global::__allrem .. _global::lzo1x_999_compress): 13 of 40 byte-exact

The chunk is the statically linked third-party code at 0xBFEC30..0xC08170: three MSVC CRT 64-bit helpers,
**zlib 1.2.1** (string "1.2.1" at 0x0129CFE0, built with Z_PREFIX so the public names are `z_*`) and
**LZO 1.08** (lzo1x_9x.c + lzo_swd.ch). The Ego_r PDB name sets list every row by name, and the Ego_r PDB has
the layouts (`z_stream_s` 0x38, `internal_state.conflict` 0x16B8 = deflate_state, `inflate_state` 0x1BA8,
`lzo1x_999_t` 0x74, `lzo1x_999_swd_t` 0x6F168), so the models are the library structs with PDB names. Retail
was linked with whole-program optimisation: the public `z_*` entry points kept `__cdecl`, but the `local`
(static) helpers got custom register conventions (arguments in eax/ebx/esi/edi). Those cannot come from one
VC7.1 TU and are parked with faithful reference bodies.

## What matched (13)
- zlib public entry points (plain C bodies, `extern "C" __cdecl`, exact `_z_*` leaves):
  - 00c00ae0 `z_deflateEnd`: status check against INIT/BUSY/FINISH, TRY_FREE in reverse allocation order; the
    `status == BUSY_STATE ? Z_DATA_ERROR : Z_OK` select is `setne ; dec ; and -3`.
  - 00c021c0 `z_deflateInit_`, 00c02310 `z_inflateInit_` (catalog "ConstructString"): one-line forwarders to the
    `*Init2_` functions with the default constants.
  - 00c021f0 `z_inflateReset`: the 1.2.1 body (no adler/dmax/head/write resets, which arrived in 1.2.2+);
    the chained `total_in = total_out = state->total = 0` stores right to left.
  - 00c02240 `z_inflateInit2_`: version byte compare with '1', sizeof(z_stream) 0x38, ZALLOC of 0x1BA8, the
    GUNZIP windowBits decode.
  - 00c038d0 `z_inflateEnd`: the zfree pointer tested in the guard stays in ecx for the first `call ecx`.
  - 00c03de0 `z_compress2`: the 0x38-byte z_stream is the whole frame; the avail_out round-trip check folds away.
  - 00c06030 `zcalloc`: `malloc(items * size)` through the dllimport slot 0x01440158 (the "make compiler happy"
    no-op and the dead `calloc` branch fold away).
- zlib/LZO static helpers whose LTCG convention maps onto `__fastcall` (declared `extern "C" __fastcall` so the
  TU emits them; leaves `@name@N`):
  - 00c04f90 `set_data_type` (s in ecx): 1.2.1 form (bin 0..6 + 128..255 vs ascii 7..127 >> 2); VC fully unrolls
    the n < 7 loop.
  - 00c07140 `swd_remove_node` (s in ecx, node in edx): HEAD3 hash + unaligned 16-bit HEAD2.
  - 00c03ee0 `init_block`, 00c00dd0 `lm_init` (s in EDX only, ecx unused): modelled as
    `__fastcall(void* unused_ecx, deflate_state* s)` -- REVIEW AT LANDING (dummy parameter, see LESSONS.md).
- LZO public entry point:
  - 00c08130 `lzo1x_999_compress`: `lzo1x_999_compress_level(..., 8)` was inlined/folded in retail, so the body is
    the direct `lzo1x_999_compress_internal` call with table row c[7] = {2, 32, 128, 0, 2048, 1} as immediates.

## Parked for Fable 5.1 (27)
- 00bfec30 __allrem DIFFER(0v178): not authored -- CRT hand-written asm (Vc7/crt/src/intel/llrem.asm) | tried: none (any C++ `%` on __int64 calls this helper) | next: lane gap: CRT asm, not ours
- 00bfecf0 __aullshr DIFFER(0v31): not authored -- CRT hand-written asm (ullshr.asm, shrd with edx:eax/cl register convention) | tried: none | next: lane gap: CRT asm, not ours
- 00bfee00 __aulldiv DIFFER(0v104): not authored -- CRT hand-written asm (ulldiv.asm) | tried: none | next: lane gap: CRT asm, not ours
- 00c00600 putShortMSB DIFFER(47v39): retail reads s from eax and b from ecx, ours loads both from the stack | tried: zlib 1.2.1 body as __cdecl | next: lane gap: LTCG custom convention (eax arg)
- 00c00630 flush_pending DIFFER(104v100): retail reads strm from eax, ours `mov eax,[esp+4]`; memcpy/update sequence identical | tried: zlib 1.2.1 body as __cdecl | next: lane gap: LTCG custom convention (eax arg)
- 00c00d50 read_buf DIFFER(130v119): retail reads strm from ebx and size from ecx (buf on the stack) | tried: zlib 1.2.1 body as __cdecl | next: lane gap: LTCG custom convention (ebx/ecx args)
- 00c00fd0 longest_match_fast DIFFER(203v192): retail reads s from esi (cur_match on the stack); unrolled compare identical | tried: zlib 1.2.1 body as __cdecl | next: lane gap: LTCG custom convention (esi arg)
- 00c01b60 z_deflateReset DIFFER(144v145): only the lm_init call -- retail `add esp,4 ; mov edx,esi ; call lm_init`, ours `push esi ; call ; add esp,8` | tried: zlib 1.2.1 body with cdecl lm_init | next: lane gap: LTCG callee convention (s in edx without an ecx load; a dummy-ecx __fastcall lm_init would add `mov ecx,..`)
- 00c03f50 pqdownheap DIFFER(203v196): retail reads s from eax and tree from edi (k on the stack) | tried: zlib 1.2.1 body as __cdecl | next: lane gap: LTCG custom convention (eax/edi args)
- 00c04260 scan_tree DIFFER(224v216): retail reads tree from eax and max_code from ecx (s on the stack) | tried: zlib 1.2.1 body as __cdecl | next: lane gap: LTCG custom convention (eax arg)
- 00c05040 bi_flush DIFFER(131v127): retail reads s from eax | tried: zlib 1.2.1 body as __cdecl | next: lane gap: LTCG custom convention (eax arg)
- 00c050c0 bi_windup DIFFER(113v109): retail reads s from eax | tried: zlib 1.2.1 body as __cdecl | next: lane gap: LTCG custom convention (eax arg)
- 00c05130 copy_block DIFFER(138v130): retail reads s/len/buf from eax/ecx/edx (header on the stack) and calls bi_windup with s in eax | tried: zlib 1.2.1 body as __cdecl | next: lane gap: LTCG custom convention (eax arg)
- 00c05230 gen_codes DIFFER(156v138): retail reads bl_count/tree/max_code from edx/edi/ebx; /GS cookie frame and inlined bi_reverse match | tried: zlib 1.2.1 body as __cdecl under /GS | next: lane gap: LTCG custom convention (edi/ebx args)
- 00c054c0 build_bl_tree DIFFER(124v108): retail reads s from eax and calls scan_tree/build_tree with register args | tried: zlib 1.2.1 body as __cdecl | next: lane gap: LTCG custom convention (eax arg + LTCG callees)
- 00c05530 _tr_stored_block DIFFER(160v160): only the copy_block call -- retail `mov ecx,len ; mov edx,buf ; push 1 ; call ; pop ecx ; ret` (s in eax), ours tail-jumps with four stack args | tried: zlib 1.2.1 body with cdecl copy_block | next: lane gap: LTCG callee convention (eax arg)
- 00c05fd0 z_crc32 DIFFER(18v24): retail `mov edx,len ; mov eax,crc ; jmp crc32_little` (buf in ecx), ours reuses the stack frame for a cdecl jmp | tried: zlib 1.2.1 body with cdecl crc32_little | next: lane gap: LTCG callee convention (eax arg)
- 00c06de0 swd_initdict DIFFER(108v98): retail reads s/dict/dict_len from eax/ecx/edx | tried: LZO 1.08 body as __cdecl | next: lane gap: LTCG custom convention (eax arg)
- 00c06e50 swd_insertdict DIFFER(208v211): retail reads s/node from ecx/edx with len on the stack and a caller-cleaned `ret` (a __fastcall would `ret 4`) | tried: LZO 1.08 body as __cdecl | next: lane gap: LTCG custom convention (caller-cleaned register args)
- 00c07090 swd_getbyte DIFFER(173v169): retail reads s from eax | tried: LZO 1.08 body as __cdecl | next: lane gap: LTCG custom convention (eax arg)
- 00c072d0 swd_search DIFFER(231v232): retail reads s from eax and node from ecx (cnt on the stack) | tried: LZO 1.08 body as __cdecl | next: lane gap: LTCG custom convention (eax arg)
- 00c073c0 swd_search2 DIFFER(85v81): retail reads s from eax | tried: LZO 1.08 body as __cdecl | next: lane gap: LTCG custom convention (eax arg)
- 00c075c0 init_match DIFFER(89v79): retail reads c from eax and flags from ecx (s on the stack) and calls swd_init with register args + one push (constant NULL dict folded) | tried: LZO 1.08 body as __cdecl | next: lane gap: LTCG custom convention (eax arg)
- 00c07610 find_match DIFFER(198v166): retail reads c/s/this_len from ebx/ecx/eax and calls swd_accept/swd_findbest/swd_getbyte with register args | tried: LZO 1.08 body as __cdecl | next: lane gap: LTCG custom convention (ebx/eax args)
- 00c07890 STORE_RUN DIFFER(179v191): retail reads op from ecx, c/ii/t from the stack, caller-cleaned `ret` | tried: LZO 1.08 body as __cdecl | next: lane gap: LTCG custom convention (caller-cleaned register arg)
- 00c07980 len_of_coded_match DIFFER(160v139): retail reads m_len/m_off/lit from ecx/edx/esi | tried: LZO 1.08 body as __cdecl | next: lane gap: LTCG custom convention (esi arg)
- 00c07a60 better_match DIFFER(160v140): retail reads swd/m_len/m_off from ecx/ebx/edi | tried: LZO 1.08 body as __cdecl | next: lane gap: LTCG custom convention (ebx/edi args)

## Skipped
None (no row spans two functions; every row starts at its PDB-named entry).

## Label corrections
- 0x00C02310 "ConstructString" (__fastcall) -> `z_inflateInit_` (zlib 1.2.1, __cdecl): forwards (strm, 15, version, size)
  to z_inflateInit2_ 0x00C02240; callers CCompressorZlib 0x00A3A840/0x00A3AB00 already call it as inflateInit_;
  PDB name sets list `_z_inflateInit_`.
- 0x00C06050 "operator_new" / catalog "CEngineSubPrimitiveBase::operator_new" (landed as global_operatornew_00c06050.cpp)
  -> zlib `zcfree(voidpf opaque, voidpf ptr)` = `free(ptr)` through the free import 0x01440154; z_inflateInit2_ and
  deflateInit2_ store it as the default strm->zfree (abs refs at 0x00C0228A / 0x00C01FE3).
- 0x00C011A0 "sub_00c011a0" -> `deflate_stored`, 0x00C013C0 "sub_00c013c0" -> `deflate_fast`: the func column of
  configuration_table 0x012AE260 (level 0 -> 0xC011A0, levels 1..3 -> 0xC013C0, 4..9 -> deflate_slow 0xC01750),
  same order as the PDB name sets (deflate_stored, deflate_fast, deflate_slow).
- Static helper names in this chunk (putShortMSB, lm_init, init_block, swd_*, ...) are real (Ego_r/FableWin PDB name
  sets); the catalog's calling-convention column ("unknown") reflects the LTCG register conventions.
