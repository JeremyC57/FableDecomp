# Landing 2026-11-08s: chunk 869, nineteen Ogg Vorbis functions (15 of 17 rows + 4 functions inside split extents)

Every row is a verbatim libvorbis 1.0.1 / libogg body under its Xiph name (MATCH or RELOCATION_MATCH + PASS). Hand-back:
`pending/2026-11-08_linux_hand869/`.
- **First pass:** 12 landed (`payload.json`).
- **Second pass:** 7 landed (`second/`) after three fixes:
  1. **Two rows outside the manifest:** five function starts lay inside oversized catalogued extents. ov_read_float 0x00C8BEE0,
     vorbis_book_codeword 0x00C95C60, vorbis_book_codelen 0x00C95C80, vorbis_bitrate_flushpacket 0x00C9DA80 and the unlisted
     drft_backward 0x00C9BC30 were added to rebuild/manifest/functions.tsv (agent_source `boundary`). The three split extents are in
     BOUNDS_FIX.tsv: ov_read 950 + 172; _best 588 + 18 + 18; vorbis_bitrate_addblock 2700 + 184.
  2. **ov_read / ov_read_float graded NOT_GENUINE:** their vorbisfile.c TU defines os.h's vorbis_ftoi through the shared ASM_RULES
     header libvorbis/vorbis_ftoi.h, which c2 needs in order to see the callee's eax-only clobber set. The rows had no `// Retail-asm:`
     evidence line; it was added (0x00C89BE1 `fld` + 0x00C89BE5 `fistp dword`), and both now grade `retail_asm` (genuine). The row
     sources hold no asm.
  3. **setup_tone_curves RELOC_IDENTITY:** VC7.1 emits `__chkstk` for the large frame and `__alloca_probe` for `_alloca`. Both are
     labels of one CRT routine (retail binds both to 0x00BFEA30). `reloc_identity.py` now treats them as one identity (`_CRT_ALIASES`).

| Xiph file | Rows |
|---|---|
| smallft.c | dradf4 0x00C96D20, dradfg 0x00C97600, dradb4 0x00C998B0, dradbg 0x00C9A230, drft_backward 0x00C9BC30 |
| mdct.c | mdct_butterfly_16 0x00C9BF30 |
| info.c | _vorbis_unpack_books 0x00C8E2E0 |
| psy.c | setup_tone_curves 0x00C91AB0, _vp_psy_init 0x00C92660, bark_noise_hybridmp 0x00C931D0 |
| codebook.c / sharedbook.c (C, /TC) | _best 0x00C95A10, vorbis_book_codeword 0x00C95C60, vorbis_book_codelen 0x00C95C80, vorbis_book_decodev_add 0x00C965C0 |
| bitrate.c (C, /TC) | limit_sum 0x00C9CC60, vorbis_bitrate_addblock 0x00C9CFF0, vorbis_bitrate_flushpacket 0x00C9DA80 |
| vorbisfile.c | ov_read 0x00C8BB20, ov_read_float 0x00C8BEE0 |

- **ov_read** declares `int val = 0;` where Xiph has `int val;`. The variable is always assigned before use, so behaviour is the same;
  the change flips an 8-bit `add` register choice through variable numbering. It is documented in the row header.
- **Parked:** 0x00C89850 CReverbFilterTap::ProcessRamp (607v588, the history-tap loop's induction form) and 0x00C95580
  vorbis_book_init_decode (901v910, two frame slots swapped plus `hi` kept in a register).
- **Names:** `ghidra_out/labels_landing_2026-11-08s.tsv`: nine catalog guesses replaced (e.g. "BoundingBox_ComputeTransform" =
  setup_tone_curves, "CTCWound::CWound::Initialize" = vorbis_book_init_decode). Queue `land0811m`: the landed FtoL 0x00C89BE0 is
  vorbis_ftoi and Math_ComputeByteChunkCount 0x00C9CB60 is LACING_ADJUST.
