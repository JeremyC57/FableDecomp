# Chunk 497 (agent497, 2026-10-27): 35 of 40 single functions byte-exact

Queued 40-row chunk of single functions under 128 bytes (address order, 0x00C72FE0..0x00CA8560: libpk audio DSP,
Xiph libogg/libvorbis, DirectShow strmbase). One Opus 5.5 agent at high effort.

- **Landed:** 35 rows with `vland.py --land` (verify.log: 17 MATCH, 18 RELOCATION_MATCH, every test PASS, no
  RELOC_IDENTITY, no RELOC_DUPLICATE). The one x87 transcendental (`fsin` in 0x00C73F10) is `#pragma intrinsic(sin)`,
  plain C++.
- **Parked (4, pending/2026-10-27_linux_hand497/differ/), all LTCG register conventions the lane cannot link, one grade
  each:** 0x00C8A8B0 ov_halfrate, 0x00C8CB20 ov_test_callbacks, 0x00C8D9C0 ogg_stream_reset_serialno, 0x00C8DB20
  ogg_stream_packetpeek.
- **Skipped (1):** 0x00CA7D60 packs four functions (CBasePin::NonDelegatingAddRef / NonDelegatingRelease and two
  `ret N` stubs); needs a catalog re-split.
- **Identities:** 23 proven by the library sources (19 landed, in ghidra_out/labels_chunk497_2026-10-27.tsv), 10
  probable, 6 descriptive.

Label corrections (NOTES.md): 0x00C75400 is CDelayLine::~CDelayLine (stale binding `??1CKrakenElement_00c75400`);
0x00C8CCF0 ogg_page_version; 0x00C95440 vorbis_book_clear; 0x00C9EA70 _vorbis_window_get (binding
`?LookupAllocatorSize@@YIKK@Z` is invented); 0x00CA3EF0 vorbis_invsq2explook; 0x00CA5360 CBaseVideoRenderer::OnStartStreaming;
landed 0x00C90C80 is vorbis_window, 0x00C8DB10 ogg_stream_packetout, 0x00CA8090 CBasePin::CompleteConnect; 0x00C8CAE0 is
libpk's `ov_open_callbacks` (the landed COggVorbisStream ctor calls it `ov_open`, which is the jump at 0x00C8CB10).
Review: `_01class` 0x00CA0680 is declared `__stdcall` at the call site to model LTCG's stack-argument callee-pop static
(`ret 10h`); landed CBaseVideoRenderer::OnStopStreaming 0x00CA5380 is declared non-virtual but is virtual slot 17; landed
QueryPinInfo uses tag `PIN_DIRECTION` where strmif.h has `_PinDirection`; landed 0x00C73EF0 types +8 as long where Read
uses a float phase.
