# Chunk 447 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- The library past libpk is Xiph libogg + libvorbis 1.0.1, not libpk. The string "Xiph.Org libVorbis I 20030909"
  is at 0x00EBF240, next to "vorbis"/"OggS".
  - It covers at least 0x00C8A000 (vorbisfile ov_read) .. 0x00CA4000 (floor0).
  - Rebuild these rows from the real v1.0.1 sources and headers, not from invented models: the shipped bodies
    graded MATCH/RELOCATION_MATCH on the first grade for all 27 non-LTCG rows (0x00C908E0..0x00C9FDF0).
  - The bitwise.c bodies are libogg's (oggpack_* with the {endbyte, endbit, buffer, ptr, storage} buffer, the mask
    table at 0x0139DDB8).
- Calling conventions in this library:
  - Public and address-taken functions are `__fastcall`. res0_free_look 0x00C9FDF0 is called through the residue
    table and takes ecx, so it is the declared /Gr convention.
  - The qsort comparators apsort 0x00C945B0 and sort32a 0x00C95560 are `__cdecl`.
  - The `static`/`STIN` helpers carry LTCG custom conventions and are lane gaps: eax (min_curve, max_curve,
    attenuate_curve, bitreverse, mdct_butterfly_8, mdct_butterflies), ebx (seed_loop, limit_sum), esi
    (floater_interpolate), edi/esi (precomputed_couple_point), or ecx/edx swapped (couple_lossless).
- An LTCG out-of-line STIN helper that takes its arguments on the stack with a callee pop is plain `__stdcall` at the
  call site. Declare it that way: decode_packed_entry_number 0x00C96320 (`push b ; push book ; call`, no cleanup),
  seen from vorbis_book_decodev_set 0x00C96700 and vorbis_book_decodevv_add 0x00C96760. The landed
  vorbis_book_decode 0x00C96430 already does the same under another name.
- The VC7.1 memset intrinsic of a struct, `memset(p,0,sizeof *p)`:
  - up to 24 bytes it becomes a zero register and dword stores (drft_clear 12, mdct_clear 20, the staticbook trees
    24/16);
  - from 0x2c it becomes `rep stosd` (res0 look 0x2c, psy look 0x30, static_codebook 0x34, bitrate state 0x68).
  - A free that follows the `rep stosd` has its `push` scheduled before the `rep stosd` (res0_free_look).
- The x87 math in this library:
  - `pow` under `#pragma intrinsic(pow)` calls `_CIpow` 0x00BFEB40.
  - floor, ldexp, qsort, memmove, realloc, calloc and free go through the MSVCR71 IAT: declare them
    `extern "C" __declspec(dllimport)`.
  - `_alloca` (`#pragma intrinsic(_alloca)`) emits a `__alloca_probe` relocation, which is retail's __chkstk
    0x00BFEA30 (the same CRT routine).
  - A C `0.` literal in `a > 0.` compares against a qword 0.0 pool constant (dipole_hypot 0x00C94310).
- A static helper that the TU never references is not emitted under /O2. Give such a helper external linkage
  (`extern "C"`) so the row's symbol exists (unitnorm 0x00C92640, sort32a, apsort). Byte-identical static twins in
  two TUs need distinct symbols (the codebook.c bitreverse copy 0x00C962B0 is in `namespace codebook_c` in differ/).
- Tests: VC7.1 at /Od folds a `-0.0f` literal to +0.0. Build a negative zero from bits (memcpy of 0x80000000).

Review at landing:
- 0x00C941E0 couple_lossless is byte-identical except that qA/qB sit in edx/ecx.
  - Declaring the pointers in swapped order `(float A, float B, float* qB, float* qA)` grades RELOCATION_MATCH
    (probe/00c941e0_swapped.cpp).
  - It is parked because that misstates the Xiph signature. The owner decides whether a parameter-order model of an
    LTCG convention is acceptable.
- All 13 parked rows are LTCG register-argument rows with the shipped source in differ/ (1 grade each, 2 for
  couple_lossless). attenuate_curve and mdct_butterfly_8 differ only in the argument register.
- The names are real Xiph names. Line 1 uses the modules `libogg` / `libvorbis` (C functions, `extern "C"`,
  symbols `@name@N`).
- Already-landed sources in this library are modelled as libpk/engine classes with wrong names (table in NOTES.md).
  - 0x00C95420 "CEngineResourceManager::WorldFrameUpdate" is vorbis_staticbook_destroy. Its source DEFINES a data
    global `void (__cdecl *g_afterUpdate)(...) = 0` to stand in for the MSVCR71 free import [0x01440154]. The bytes
    match, but the model is wrong. It should be `extern "C" __declspec(dllimport) void __cdecl free(void*)`.
  - 0x00C96430 "CDefinitionManager::GetPDefFromClassIndex" is vorbis_book_decode. Its "ValidateClassIndex" is
    decode_packed_entry_number and its m_classArray is codebook::dec_index.
  - 0x00C90FC0 / 0x00C915A0 / 0x00C911B0 / 0x00C912F0 / 0x00C90F10 / 0x00C91320 are oggpack_write / oggpack_read /
    oggpack_writealign / oggpack_reset / oggpack_writeinit / oggpack_writeclear.
  - The comments of 0x00C90FC0 and 0x00C96140 call them "libpk"; they are libogg/libvorbis.
  - 0x00C918A0 / 0x00C9EC00 / 0x00C9FDD0 (catalog `_Cons_val<std::allocator<C3DGeneratorObject>...>`) are
    _vi_psy_free / mapping0_free_info / res0_free_info.
- The catalog has no row that holds several functions. No row needed more than 2 grades.
