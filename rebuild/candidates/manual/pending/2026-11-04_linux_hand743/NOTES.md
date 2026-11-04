# Chunk 743 -- Xiph Ogg Vorbis rows inside libpk (11/11 RELOCATION_MATCH)

Sources: libvorbis 1.0.1 (gitlab.xiph.org xiph/vorbis tag v1.0.1; GitHub was blocked, gitlab worked) and libogg 1.1.1
(gitlab xiph/ogg v1.1.1; no v1.1 tag there, framing.c pagein identical). Downloads in `xiph_dl/` (data only, nothing run).

## Results
| address | name | file | grade | grades | flags |
|---|---|---|---|---|---|
| 0x00C8AB70 | ov_raw_seek | vorbisfile.c | RELOCATION_MATCH | 1 | /GS |
| 0x00C8AE90 | ov_pcm_seek_page | vorbisfile.c | RELOCATION_MATCH | 1 | /GS (also matches without) |
| 0x00C8B490 | ov_pcm_seek | vorbisfile.c | RELOCATION_MATCH | 1 | /GS |
| 0x00C8D690 | ogg_stream_pagein | framing.c | RELOCATION_MATCH | 2 (+1 SRC_FAIL) | none |
| 0x00C8FF30 | vorbis_synthesis_blockin | block.c | RELOCATION_MATCH | 1 (+1 SRC_FAIL) | /TC /Gr |
| 0x00C94AE0 | _vp_couple | psy.c | RELOCATION_MATCH | 2 | /TC /Gr |
| 0x00C9C620 | mdct_backward | mdct.c | RELOCATION_MATCH | 1 | none |
| 0x00C9C870 | mdct_forward | mdct.c | RELOCATION_MATCH | 1 | none |
| 0x00C9DD60 | _ve_amp (static) | envelope.c | RELOCATION_MATCH | 1 (+1 SRC_FAIL) | /TC /Gr |
| 0x00C9E280 | _ve_envelope_search | envelope.c | RELOCATION_MATCH | 1 | /TC /Gr |
| 0x00C9EF90 | mapping0_forward (static, address-taken) | mapping0.c | RELOCATION_MATCH | 1 (+1 SRC_FAIL) | /TC /Gr |

Every body is the shipped Xiph text verbatim. Identity per row (file order between landed neighbours + callers/callees +
constants) is in each source's header. Five of them (ov_raw_seek, ov_pcm_seek_page, ov_pcm_seek, ogg_stream_pagein,
vorbis_synthesis_blockin, mdct_backward, _ve_envelope_search) were already bound under their Xiph spellings by landed callers.

## Shapes (all C/C++, no asm, no helper headers)
- vorbisfile rows: the landed `_fetch_and_process_packet` 0x00C8A540 TU (minus its two scaffolds) plus the three seek functions;
  the statics there already have retail's private conventions.
- ogg_stream_pagein: the seven ogg_page_* accessors as same-TU bodies under `#pragma auto_inline(off)` (retail calls them and
  relies on them not clobbering ecx); _os_body_expand/_os_lacing_expand as `static __declspec(noinline)`.
- C TUs (/TC /Gr) include the shared `engine/Vorbis.h`; psy.h's `vorbis_info_psy` and envelope.h's types are defined locally
  (not in the shared header; envelope_lookup completes Vorbis.h's forward `struct envelope_lookup`).
- _vp_couple: unitnorm / couple_lossless / precomputed_couple_point (`__stdcall`) as same-TU noinline statics; tables are
  psy.c's statics (stereo_threshholds 0x013A3900).
- mdct: the landed mdct.c statics (butterflies 0x00C9C400 file, bitreverse 0x00C9C500 file) then backward/forward `__fastcall`.
- envelope: todB (pre-union scales.h form, as landed 0x00C9DD40) and _ve_amp as noinline statics.
- mapping0_forward: todB 0x00C9EF70 as noinline static; everything else external /Gr fastcall.

## Header overlay / header debt
- No header changes (hdr/ empty). Vorbis.h lacks `vorbis_info_psy` and the envelope.h structures (defined locally).
- Lane note: incres.py flags `stdint.h` inside Vorbis.h's non-MSC branch as MISSING; I graded with g2.sh/cc2.sh copies that
  ignore exactly that line (no file change).

## Callee names (from reloc_bindings / landed leaves)
All callees use their landed spellings except the wrong bindings listed in NAME_FIXES.tsv. Unbound but used here with Xiph
names: ogg_page_eos/continued/bos/version/pageno, ogg_stream_packetpeek, vorbis_synthesis_trackonly, _os_body_expand,
_os_lacing_expand, couple_lossless (@couple_lossless@16, landed leaf), precomputed_couple_point (_precomputed_couple_point@20),
mdct_butterflies/_bitreverse, todB copies, floor1_encode 0x00CA2BB0 (@floor1_encode@16), _vp_* (landed leaves).

## Name fixes found (NAME_FIXES.tsv)
0x00C9CFD0 vorbis_bitrate_managed, 0x00C90CB0 vorbis_analysis, 0x00C91800 oggpack_get_buffer, 0x00C912F0 oggpack_reset,
0x00C917B0 oggpack_bytes, 0x013A3DDC _mapping_P (all invented SVideoPlayer/SSurface spellings from the landed 0x00C90CB0 row),
0x00C9EA70 _vorbis_window_get (?LookupAllocatorSize from the 0x00C90C80 row), 0x00C90FC0 oggpack_write (WriteBits_ spellings).
mapping0_forward's call to 0x00C9CFD0 will show RELOC_IDENTITY until the bitrate_managed fix lands.

## GOTCHAS one-liners
- libogg framing.c: the ogg_page_* accessors called from ogg_stream_pagein need same-TU bodies under auto_inline(off) (else they inline; retail keeps og in ecx across them).
- Vorbis C rows: `// Flags: /TC /Gr` + `#include "engine/Vorbis.h"` grade the Xiph text verbatim; incres.py falsely reports Vorbis.h's guarded `<stdint.h>`.
- A /GS TU flag is harmless for array-free siblings (ov_pcm_seek_page matched with and without it).
- 0x00C90CB0 is analysis.c vorbis_analysis (not a video player); 0x00C9CFD0 is vorbis_bitrate_managed.

## vland (read-only, after the container restart)
11/11 RELOCATION_MATCH + behaviour PASS (`vland_result.txt`). Warnings only: RELOC_NAME `__alloca_probe` vs label `__chkstk`
(same CRT address, the compiler's own symbol), and RELOC_CONFLICT `@todB@4` for the per-TU scales.h copies 0x00C9DD40 /
0x00C9EF70 vs the landed 0x00C90D40 -- the known per-TU duplicate-static spelling question (GOTCHAS 2026-10-30 retry wave),
queued for the owner; the bodies are three real static copies.
First vland run: TCC_FAIL/TIMEOUT because vland also compiles the tests with VC7.1 cl, which hangs on `<stdint.h>` /
`<alloca.h>` (absent in VC7.1). Tests now take `__int64` / `<malloc.h>` under `_MSC_VER`; all 11 compile under cl and g++.
GOTCHA: lane behaviour tests must also compile under VC7.1 cl (no <stdint.h>, no <alloca.h>) or vland's TCC step hangs.
