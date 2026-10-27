# Chunk 497 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- VC7.1 /O2 unrolls a plain counted float loop by four by itself (`lea ecx,[n-4] ; shr ecx,2 ; inc ecx`, then a remainder
  loop); retail's bumped-pointer operands (`add eax,10h` with `[eax-10h]`..`[eax-4]`) come from `*p++ op= x`, while `p[i]`
  gives a base+index walk (libpk CDspOps kernels 0x00C73FC0..0x00C741C0).
- An unsigned `for (i = 0; i < n; ++i)` that VC rewrites as a down-counter keeps retail's `test ; jbe` entry; writing the
  down-counter yourself (`n != 0`) gives `je` (0x00C73F10).
- `sin(f)` on a float in a C++ TU binds math.h's `float sin(float)` inline; with `#pragma intrinsic(sin)` it is a bare
  `fsin`, and `* 32767.0f` stays a float pool operand (0x00C73F10).
- A polymorphic class with an `__int64` member pads its vfptr to 8: the first data member sits at +8 (0x00C72FE0).
- A libvorbis static that LTCG cloned out of line with every argument on the stack and a callee pop is `__stdcall` at the
  call site (res1_class -> _01class `ret 10h`, 0x00CA1250), as decode_packed_entry_number 0x00C96320.
- Xiph rows whose static callee takes LTCG registers (eax/ebx/esi) or whose caller keeps ecx/edx live across a call park
  after one grade (0x00C8A8B0, 0x00C8CB20, 0x00C8D9C0, 0x00C8DB20).
- strmbase pin tables: the IQualityControl table precedes the IPin table; count primary slots in CBasePin declaration
  order (CheckMediaType 8, SetMediaType 9, CheckConnect 10, BreakConnect 11, CompleteConnect 12) to name renderer-pin rows.
- 0x00C9EA70 is window.c `_vorbis_window_get` (vwin[] pointer table 0x013ABD90); its tail-jumping caller 0x00C90C80 is
  block.c `vorbis_window`.

Review at landing:
- 0x00CA7D60 holds four functions (CBasePin::NonDelegatingAddRef 0x00CA7D60, NonDelegatingRelease 0x00CA7D80, `ret 4`
  0x00CA7DA0, `ret 8` 0x00CA7DB0): re-split the catalog row.
- Landed 0x00C90C80 (CPoolEntryTable_00c90c80::GetEntryAllocatorSize) is libvorbis `vorbis_window`; its binding
  `?LookupAllocatorSize@@YIKK@Z` for 0x00C9EA70 is invented: 0x00C9EA70 lands here as `@_vorbis_window_get@4`.
- Landed 0x00C8DB10 (std_DestvalCFlasha) is libogg `ogg_stream_packetout`; landed 0x00CA8090 (C_00b38810::M /
  AddChildPrimitive) is `CBasePin::CompleteConnect`, which 0x00CA5190 now binds as `?CompleteConnect@CBasePin@@UAEJPAUIPin@@@Z`.
- Landed CBaseVideoRenderer::OnStopStreaming 0x00CA5380 is declared non-virtual (mangles QAE; retail slot 17 is a virtual,
  UAE as 0x00CA5360 here).
- Landed CBasePin::QueryPinInfo 0x00CA8420 models `enum PIN_DIRECTION`; strmif.h's tag is `_PinDirection` (used by
  QueryDirection 0x00CA8490 here: `PAW4_PinDirection@@`).
- Landed COggVorbisStream ctor 0x00C88780 binds ov_info 0x00C8BAB0 with C++ linkage (`?ov_info@@YI...`) beside the row's own
  `@ov_info@8`; the rows here call it extern "C".
- Landed CVtable_012bf014 ctor 0x00C73EF0 types +8 as `long m_position`; Read 0x00C73F10 uses it as the float phase.
- The IAudioStream Read slot takes a {short* data; channels} descriptor (0x00C886B0 and 0x00C73F10 both read it); the
  landed ctor model spells it `Read(void*, unsigned long)`, the tone row here `Read(SAudioBuffer*, unsigned long)`.
- reloc_bindings.tsv keeps a stale `??1CKrakenElement_00c75400@@UAE@XZ` for 0x00C75400 next to `??1CDelayLine@@UAE@XZ`.
