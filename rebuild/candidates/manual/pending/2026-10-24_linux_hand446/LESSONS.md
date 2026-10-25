# Chunk 446 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- 0x00C89xxx..0x00C9Exxx is the Xiph Ogg Vorbis decoder that libpk links statically ("Xiph.Org libVorbis I 20030909"
  @ 0x012BF240; a post-1.0.1 CVS tree with halfrate/crosslap). Name these rows from the libogg/libvorbis/vorbisfile
  sources, which are real source names, not descriptive ones. The manifest labels there are all propagated game
  names (e.g. CreatureHitNotification_*, C3DGenerator_*, CCharString_*).
- The Xiph code was LTCG-linked, so two kinds of row exist:
  - API functions with ecx/edx arguments are plain `extern "C" ... __fastcall` and match verbatim from the Xiph
    source (22 rows here).
  - Static helpers taking eax/ebx/esi/edi are the known lane gap. So is any caller whose only diff is the register
    of such a callee's argument: 0x00C8DD90 differs only in `mov eax,` against `mov edx,` (18 rows).
- An LTCG static taking ecx plus stack arguments, callee-clean, is the thiscall shape. Call it as a member function
  of its first argument's struct (0x00C8EA70 calls `opb._vorbis_pack_comment(vc)`).
- Same-TU noinline bodies also give VC7.1 mod/ref knowledge, not only register knowledge. With the real
  ov_time_total body in the TU, ov_time_tell hoists the loop-invariant `&vf->pcmlengths[link*2+1]` across the calls
  (0x00C8B9D0: 209v212 with an extern callee, RELOCATION_MATCH with the body).
- `<string.h>` memmove is a direct call. Retail calls the MSVCR71 IAT slot (0x01440144), so declare it
  `extern "C" __declspec(dllimport)` without the header (0x00C8D470).
- `_alloca(n)` gives `add eax,3 ; and eax,-4 ; call __alloca_probe`. Retail's 0x00BFEA30 is `__chkstk`, the same CRT
  label (0x00C8DC90).
- `memset(p, 0, sizeof *p)` right before `return 0` leaves eax = 0 from the `rep stosd`, and VC7.1 emits no second
  `xor eax,eax` (0x00C8F1B0, 0x00C8CE80).

Review at landing:
- 0x00C8DC90's `__alloca_probe` relocation must resolve to retail 0x00BFEA30 (manifest `__chkstk`; chkstk.obj
  defines both labels at one address). Check that vland accepts the alias.
- 0x00C8B9D0 carries a same-TU external-linkage stand-in of ov_time_total (the landed 0x00C8AAF0 body, same
  `@ov_time_total@8` symbol), as the stand-in rule asks.
- 0x00C8EA70 calls `?_vorbis_pack_comment@oggpack_buffer@@QAEHPAUvorbis_comment@@@Z` (retail 0x00C8E790, parked);
  the parked differ/00c8e790.cpp uses the same symbol.
- Already-landed rows with wrong names or classes (all Xiph code; see NOTES.md, Label corrections):
  - 0x00C8A920 is ov_halfrate_p, landed through engine/CWorldMap.h, which is the wrong class.
  - 0x00C8DB10 is ogg_stream_packetout, landed with `__asm` as an ASM_BAKE std::_Dest_val<CFlash> thunk.
    The real cause is the LTCG-custom callee `_packetout` 0x00C8D9D0 (os in eax, op in ecx).
  - 0x00C8C730 is _ov_64_seek_lap, landed as a naked ASM_BAKE "Batch_InvokeWithStackArgs" (vf in eax, ret 0xC).
  - Renames only: 0x00C8CDC0, 0x00C8CED0, 0x00C8D450, 0x00C8D4F0, 0x00C8D950, 0x00C8D970, 0x00C8DB30, 0x00C8DBE0,
    0x00C8DF60, 0x00C8DF70, 0x00C8F050, 0x00C8F080, 0x00C8B9B0, 0x00C8A950, 0x00C8BB10, 0x00C8CD00/10/20.
- No row needed more than 2 grades. No engine header applies: this is third-party C, with local Xiph struct models.
- No catalog row holds several functions.
