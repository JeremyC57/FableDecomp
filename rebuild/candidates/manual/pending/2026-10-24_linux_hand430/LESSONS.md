# Chunk 430 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- The 0xBFEC30..0xC08170 block is statically linked third-party C: MSVC CRT asm helpers (`__allrem` llrem.asm,
  `__aullshr` ullshr.asm, `__aulldiv` ulldiv.asm), **zlib 1.2.1** (not 1.1.4: string "1.2.1" at 0x0129CFE0, 1.2.x
  inflate_state 0x1BA8 with ENOUGH 1440, deflateEnd's three-state check, Z_PREFIX `z_*` names) and **LZO 1.08**
  (lzo1x_9x.c + lzo_swd.ch, swd SWD_N 0xBFFF / SWD_F 2048). The Ego_r PDB carries all their struct layouts
  (`z_stream_s`, `internal_state.conflict`, `inflate_state`, `lzo1x_999_t`, `lzo1x_999_swd_t`, `config_s`,
  `tree_desc_s`, `ct_data_s`) and the name sets carry every function: author from the library source text.
- zlib/LZO public entry points compile as plain C bodies in `extern "C" __cdecl` and match first try
  (0x00C00AE0, 0x00C021C0, 0x00C021F0, 0x00C02240, 0x00C02310, 0x00C038D0, 0x00C03DE0, 0x00C06030, 0x00C08130);
  leaves are the decorated C names (`_z_deflateEnd`, `_zcalloc`).
- Retail was a whole-program (LTCG) build: zlib/LZO `local` (static) helpers receive arguments in eax/ebx/esi/edi
  (lane gap, park on sight). When the convention is ecx[, edx] with no stack arguments, `extern "C" __fastcall`
  reproduces it exactly (0x00C04F90 set_data_type, 0x00C07140 swd_remove_node; leaf `@name@N`). A static helper
  with no caller in the TU is not emitted, so declare it extern rather than `static`.
- An LTCG helper that takes its only argument in EDX (ecx untouched) matches as
  `__fastcall(void* unused_ecx, T* arg)` (0x00C03EE0 init_block, 0x00C00DD0 lm_init): the body never reads ecx.
  This is a convention stand-in, not the library signature -- see review item below.
- Caller-cleaned register arguments (`ret` with stack args after ecx/edx, 0x00C06E50, 0x00C07890) are NOT
  __fastcall (that would be `ret N`): park.
- A function that is itself `__cdecl` but calls an LTCG helper (0x00C01B60 -> lm_init in edx, 0x00C05530 ->
  copy_block in eax/ecx/edx, 0x00C05FD0 -> crc32_little in eax/ecx/edx) differs only at that call site: park.
- A library entry that retail reaches with a folded table row (lzo1x_999_compress -> compress_internal with level-8
  immediates) is written as the direct folded call; defining the `_level` wrapper in the TU would also make the
  `_lzo1x_999_compress` leaf ambiguous (substring of `_lzo1x_999_compress_level`, the larger symbol wins).
- zlib's gen_codes (0x00C05230) carries a /GS cookie: the zlib objects were built with /GS.

Review at landing:
- 0x00C03EE0 init_block and 0x00C00DD0 lm_init are in src/ with a DUMMY leading `void* unused_ecx` parameter on a
  `__fastcall` declaration to model the LTCG edx-only convention (zlib's real signature is `(deflate_state *s)`).
  Bytes match and the tests pass; decide whether this convention stand-in is acceptable (compare the 0xC20440
  "dummy edx" finding in GOTCHAS, which was wrong because that row was really a thiscall member -- here there is
  no C/C++ form at all, retail is LTCG). If rejected, move both to differ/ as "LTCG edx-only convention".
- 0x00C04F90 set_data_type and 0x00C07140 swd_remove_node are zlib/LZO `static` functions declared
  `extern "C" __fastcall` to reproduce the LTCG register convention (precedent: chunk 230 "declaring a callee
  __fastcall reproduces the LTCG custom convention exactly").
- No row needed more than 4 grades (max 3: lm_init, same body re-graded). No local base models next to an engine
  header (zlib/LZO have no engine headers; the models follow the PDB layouts with PDB member names).
- Errors in ALREADY-LANDED sources:
  - `rebuild/src/compiled/00/c0/global_operatornew_00c06050.cpp` (catalog "CEngineSubPrimitiveBase::operator_new") is
    zlib 1.2.1 `zcfree(opaque, ptr)` -> `free(ptr)` through the free import 0x01440154; it is an old Fwd_/Imp_
    forwarder model and should be renamed/re-authored as `_global::zcfree`.
  - The CCompressorZlib sources (0x009C7060, 0x00A3A7F0, 0x00A3A840, 0x00A3A8C0, 0x00A3AB00, ...) say "zlib 1.1.4"
    and `#define ZLIB_VERSION "1.1.4"`; the retail string at 0x0129CFE0 is "1.2.1" (bytes match only because the
    literal relocation is masked). They also declare the unprefixed `inflateInit_`/`deflateInit_`; the build used
    Z_PREFIX (`_z_inflateInit_`, `_z_deflateInit_` in the PDB name sets).
- Catalog rows: none hold several functions. Manifest labels to fix: 0x00C02310 ConstructString -> z_inflateInit_,
  0x00C06050 operator_new -> zcfree, 0x00C011A0 -> deflate_stored, 0x00C013C0 -> deflate_fast.
