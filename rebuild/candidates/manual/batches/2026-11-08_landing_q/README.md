# Landing 2026-11-08q: chunk 866, ten Ogg Vorbis rows (10 of 10)

Every row is a verbatim libvorbis 1.0.1 / libogg 1.1 body under its real Xiph name. All pass their behaviour tests. Hand-back:
`pending/2026-11-08_linux_hand866/` (no headers changed; the Xiph sources stayed in scratch).

| Row | Xiph name | Grade | Flags |
|---|---|---|---|
| 0x00C969D0 | dradf2 | MATCH | /Gr |
| 0x00C98E80 | drftf1 | RELOCATION_MATCH | /Gr |
| 0x00C99060 | dradb2 | MATCH | /Gr |
| 0x00C99400 | dradb3 | RELOCATION_MATCH | /Gr |
| 0x00C9B9F0 | drftb1 | RELOCATION_MATCH | /Gr |
| 0x00C8BF90 | _ov_splice | RELOCATION_MATCH | - |
| 0x00C8D9D0 | _packetout (libogg) | MATCH | /Gr |
| 0x00C9C000 | mdct_butterfly_32 | RELOCATION_MATCH | - |
| 0x00C8E880 | _vorbis_pack_books | RELOCATION_MATCH | /TC /Gr |
| 0x00C8CB70 | ov_bitrate | RELOCATION_MATCH | - |

- **Names:** seven catalog names were descriptive guesses (FFT_Radix4Transform = dradf2, MDCT_Analysis = dradb3,
  Audio_GetSamplePosition = ov_bitrate, ...). `ghidra_out/labels_landing_2026-11-08q.tsv` also names the unlanded smallft.c neighbours
  from the call bindings: dradf4 0x00C96D20, dradfg 0x00C97600, dradb4 0x00C998B0, dradbg 0x00C9A230, drft_backward 0x00C9BC30.
  0x00C9BF30 is mdct_butterfly_16 (the manifest's C3DBoundingBox::InitialiseFromSphere was propagated).
- **Correction to a landed TU:** libvorbis_mdctbutterflies_00c9c400.cpp held a `mdct_butterfly_32` helper whose body was not mdct.c's
  (cPI2_8 products in the first pair). It is respelled from the matched 0x00C9C000 body; 0x00C9C400 still grades RELOCATION_MATCH
  and its test passes.
- **ov_bitrate:** libpk compiles vorbisfile.c as C++. The era `<math.h>` `inline float floor(float)` overload (declared over the
  `__imp__floor` dllimport) keeps retail's two separate rint() tails. One named local (`double seconds`) fixes a lea operand order
  and does not change behaviour. `_ov_splice` keeps Xiph's double `1.-wd`.
- **_vorbis_pack_books:** a C TU (`/TC`): engine/Vorbis.h is C-only (it has a member named `class`).
- vland's RELOC_NAME warnings were stale manifest labels of the smallft neighbours, fixed by the labels file. The catalog sync was
  restricted to these addresses. `bindings_multi_check.py` prints 1: the NullCoord entry that was already there.
