# Chunk 866 -- ten Ogg Vorbis / libpk rows: 10 matched, 0 parked

All ten rows are Xiph library functions (libogg 1.1 framing.c, libvorbis 1.0.1 "libVorbis I 20030909" smallft.c / mdct.c /
vorbisfile.c / info.c). Each body was identified against the public Xiph source (downloaded into scratch tmp only,
`$S/w22/agent866/tmp/xiph/`, nothing copied under the repo), spelled with the real Xiph name and signature, and graded
MATCH / RELOCATION_MATCH under q.py. Every row has a behaviour test that PASSes under g++ and under the lane's VC7.1
behaviour check (`tall.py`, verify_and_land.behaviour_of). Catalog names for 7 of the 10 rows were descriptive guesses and are
wrong (see NAME_FIXES.tsv / LABELS.tsv). No header changes (hdr/ empty). No extent holds two functions; every extent is one
Xiph function and the grader reports the full retail length.

| Address | Real Xiph name | Xiph file | Grade | Flags | Device |
|---|---|---|---|---|---|
| 0x00C969D0 | dradf2 | smallft.c | MATCH | /Gr | smallft forward TU slice (landed drft_forward TU convention), `static __declspec(noinline) __stdcall` |
| 0x00C98E80 | drftf1 | smallft.c | RELOCATION_MATCH | /Gr | same TU; calls dradfg 0x00C97600, dradf2 0x00C969D0, dradf4 0x00C96D20 |
| 0x00C99060 | dradb2 | smallft.c | MATCH | /Gr | same TU (backward half) |
| 0x00C99400 | dradb3 | smallft.c | RELOCATION_MATCH | /Gr | same TU; taur/taui file statics become `?taur@?1??dradb3@@9@24@4MA` pool refs (relocation only) |
| 0x00C9B9F0 | drftb1 | smallft.c | RELOCATION_MATCH | /Gr | same TU; calls dradbg 0x00C9A230, dradb3 0x00C99400, dradb2 0x00C99060, dradb4 0x00C998B0 |
| 0x00C8BF90 | _ov_splice | vorbisfile.c | RELOCATION_MATCH | -- | landed ov_crosslap TU (0x00C8C3E0) text, `static __declspec(noinline) void __stdcall _ov_splice` in `extern "C"` |
| 0x00C8D9D0 | _packetout | framing.c | MATCH | /Gr | landed ogg_stream_packetout TU (0x00C8DB10) with the static wrapped in `extern "C"`; leaf `@_packetout@12` |
| 0x00C9C000 | mdct_butterfly_32 | mdct.c | RELOCATION_MATCH | -- | landed mdct_butterflies TU (0x00C9C400) with the REAL mdct.c body (see below) |
| 0x00C8E880 | _vorbis_pack_books | info.c | RELOCATION_MATCH | /TC /Gr | C TU over engine/Vorbis.h, `static __declspec(noinline) int __stdcall` beside a `_caller` |
| 0x00C8CB70 | ov_bitrate | vorbisfile.c | RELOCATION_MATCH | -- | public `extern "C" long __fastcall`; VC7.1 C++ `<math.h>` float `floor` overload + a named time local |

## Per-row notes

### smallft.c rows (0x00C969D0 dradf2, 0x00C98E80 drftf1, 0x00C99060 dradb2, 0x00C99400 dradb3, 0x00C9B9F0 drftb1)
The catalog names ("FFT_Radix4Transform", "FFT_2D_DispatchByRadix", "FFT_Radix4Stage", "MDCT_Analysis",
"Audio_TransformFFTCoefficients") are all wrong: these are FFTPACK real-FFT passes from libvorbis smallft.c. The whole
smallft.c file sits at 0x00C96800 (drfti1, landed) .. 0x00C9BC30 (drft_backward) in source order: drfti1 0x00C96800, dradf2
0x00C969D0, dradf4 0x00C96D20, dradfg 0x00C97600, drftf1 0x00C98E80, dradb2 0x00C99060, dradb3 0x00C99400, dradb4 0x00C998B0,
dradbg 0x00C9A230, drftb1 0x00C9B9F0, drft_forward 0x00C9BC10 (landed), drft_backward 0x00C9BC30. All five sources are the
same TU slice (`tmp/smallft_tu.inc`, smallft.c 1.0.1 verbatim with the statics as `static __declspec(noinline) void __stdcall`
inside `extern "C" {}`, `#pragma intrinsic(cos,sin)`, `_ogg_calloc` -> `calloc`), the convention of the landed
`libvorbis_drftforward_00c9bc10.cpp`; only the `// Leaf:` differs. Each matched on the first grade. drftf1/drftb1 pick the
callee addresses above exactly (calls.py), so the static chain is proven end to end. The remaining unlanded smallft
functions dradf4 0x00C96D20, dradfg 0x00C97600, dradb4 0x00C998B0, dradbg 0x00C9A230 and drft_backward 0x00C9BC30 should
land from the same TU text (not in this chunk; not graded here).

### 0x00C8BF90 _ov_splice (vorbisfile.c)
Same TU text as the landed `libvorbis_ovcrosslap_00c8c3e0.cpp` (which already holds `_ov_splice` as a same-TU static with the
`__stdcall` lever); the row is that static's body. `1.-wd` is a double subtraction in Xiph's source and the retail bytes keep the
double round trip (fld1 / fsub / fstp dword) -- do not "fix" it to `1.f`.

### 0x00C8D9D0 _packetout (framing.c, libogg 1.1)
Same TU as the landed `ogg_stream_packetout_00c8db10.cpp`; the static needs `extern "C"` around it (else the leaf is a C++
mangled name) and under /Gr a C-linkage static is spelled `@_packetout@12` (objdump -t), which is the `// Leaf:` line. The
first grade with leaf `__packetout` fell back to the largest symbol (the wrapper) -> DIFFER; the leaf fix matched.

### 0x00C9C000 mdct_butterfly_32 (mdct.c) -- CORRECTION to a landed TU's helper body
The landed `libvorbis_mdctbutterflies_00c9c400.cpp` carries a `mdct_butterfly_32` body that is NOT mdct.c's: its first pair
writes `x[14] = (r0 - r1) * cPI2_8; x[15] = (r0 + r1) * cPI2_8` (a copy of the 16-stage pattern) where mdct.c 1.0.1 has a plain
`x[14] = r0; x[15] = r1`. Grading the landed text at 0x00C9C000 gives DIFFER(497 v 475); the real mdct.c body gives
RELOCATION_MATCH on the next grade. The landed row 0x00C9C400 (`mdct_butterflies`) is unaffected byte-wise (it only CALLS
butterfly_32), but its TU text now behaves differently from retail for anyone who reuses it; the coordinator should respell
that helper body from `src/00c9c000.cpp` when landing this row (same TU). The test checks the stage against a double-precision
reference built from the same rotations, so the plain-copy pair is covered.

### 0x00C8E880 _vorbis_pack_books (info.c)
Catalog name "OggVorbis_EncodeSetupPacket" is a guess; the body is info.c `_vorbis_pack_books` verbatim (type 5 + "vorbis",
books, the 6/16-bit time placeholders, floors / residues / maps / modes, framing bit). The shared header `engine/Vorbis.h` is
C-only (a member named `class` at line 390), so the TU compiles as C: `// Flags: /TC /Gr`. The static is a same-TU
`static __declspec(noinline) int __stdcall` beside a `_caller` (the landed vorbispackinfo/vorbispackcomment lever);
`_v_writestring` is the landed 0x00C8DB80 body as a same-TU `static __declspec(noinline)` (fastcall under /Gr, which is what
the landed caller TUs bind to). Backend tables `_floor_P` / `_residue_P` / `_mapping_P` bind to 0x013A3DC8 / 0x013A3DD0 /
0x013A3DDC as in the landed `global_vorbisinfoclear_00c8dfb0.cpp`; `vorbis_staticbook_pack` -> 0x00C95CA0, `oggpack_write` ->
0x00C90FC0.

### 0x00C8CB70 ov_bitrate (vorbisfile.c) -- 23 grades + 6 micro-probe TUs
Catalog name "Audio_GetSamplePosition" is a guess; the body is vorbisfile.c `ov_bitrate` (OggVorbis_File model from the landed
`libvorbis_ovbitrateinstant_00c8a960.cpp`). Residue history:
1. Plain 1.0.1 text DIFFER(354 v 382): VC7.1 cross-jumped the two `rint()` tails (`fadd 0.5 ; call floor ; call __ftol2`) into
   one; retail keeps both, each with its own `sub esp,8`. Flags (/Os /Ot /Op /Oi /Oa /Ow /Ob1 /GS), ltcg_grade.py, the C
   front end (/TC) and the QFE-4035 c2 all still merged them -> not a flag/inline residue.
2. mt.py micro-probes: the tails stop merging when their final conversion tuples differ. libpk compiles vorbisfile.c as C++
   (the landed neighbours are C++ TUs), and VC7.1's C++ `<math.h>` declares `inline float floorf(float)` and
   `inline float __cdecl floor(float)` overloads, so Xiph's `#define rint(x) (floor((x)+0.5f))` applied to the `float br` arm
   picks the float overload (float result, extra CVT) while the `double` arm picks the CRT `floor`. Spelling those two
   overloads in the source (declared exactly as VC7.1's math.h does, over the `__imp__floor` dllimport) split the tails:
   DIFFER down to a single `lea ecx,[esi+eax]` vs `lea ecx,[eax+esi]` operand-order residue.
3. The lea order follows the function's tuple count (c2prio trace): naming the time in the i<0 arm
   (`double seconds=ov_time_total(vf,-1); br=bits/seconds;`) flipped it with no other byte change -> RELOCATION_MATCH.
   Other perturbations (casts, br scope, locals in the nominal branch) did not move it. Whether Xiph's 20030909 CVS or
   Lionhead's copy actually named that local is unknowable from the bytes; it is the only one-token spelling found that
   reproduces retail, and it is behaviour-neutral (the test checks the i<0 rounding path).
Callees bind correctly: `@ov_time_total@8` 0x00C8AAF0, `__allmul` 0x00BFEB50, `__ftol2` 0x00BFEA70, `__imp__floor` 0x01440100.

## Labels found along the way (LABELS.tsv)
- 0x00C9BF30 is mdct.c `mdct_butterfly_16` (manifest: `C3DBoundingBox::InitialiseFromSphere`, a propagated label that is
  wrong): the two tail calls of mdct_butterfly_32 bind to it, and the landed `libvorbis_mdctbutterfly8_00c9beb0.cpp` names it.
- smallft.c statics still carrying guessed catalog names: 0x00C96D20 dradf4 ("FFT_Radix2Stage"), 0x00C97600 dradfg
  ("FFT_2D_Radix2"), 0x00C998B0 dradb4 ("FFT_MixedRadixDecomposition"), 0x00C9A230 dradbg ("Audio_FFT_MixedRadixTransform"),
  0x00C9BC30 drft_backward.

## Tests
`test/<addr>.cpp`: smallft rows share one harness (n=24 round trip through drft_forward/drft_backward with the expected factor
list [24,3,2,4,3], a pure cosine landing in bin 3, DC); _ov_splice blends two lap lengths/window choices and extra channels;
_packetout walks a 255-chained bos/eos packet, a 0x400 hole (-1), peek vs advance and the cheap probe; mdct_butterfly_32 vs a
double reference; _vorbis_pack_books with libogg's oggpack_write and a bit reader checking every field, OV_EFAULT and the
missing-floor-pack -1; ov_bitrate over a two-link model (OV_EINVAL, per-link, whole-file rounding, unseekable hints, OV_FALSE).
All ten print `<addr>_TEST PASS` under g++ and VC7.1 cl.
