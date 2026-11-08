# Chunk 869 -- the 17 unlanded catalogued starts 0x00C89000..0x00CA0400 (+ drft_backward): 15 matched + 4 bonus rows, 2 parked

(Chunk 866's ten rows were landed 2026-11-08q; their notes went with that hand-over and their sources sit in `landed866/`.)

Every row but one is a Xiph library function (libvorbis 1.0.1 "libVorbis I 20030909": smallft.c, mdct.c, info.c, psy.c,
codebook.c, sharedbook.c, bitrate.c, vorbisfile.c, os.h; libogg 1.1 bitwise.c for callees). The one engine row is libpk's
CReverbFilterTap::ProcessRamp (no PDB / RTTI; hand-written C++). Bodies were identified against the public Xiph source
(scratch only: `$S/w22/agent866/tmp/xiph/`, `tmp/xiph2/`; nothing copied under the repo), spelled with the real Xiph names and
signatures, and graded with q.py / qt.py. Every authored row (19 = 15 chunk rows + 4 bonus functions found inside split extents)
has a behaviour test that PASSes under g++ and under the lane's VC7.1 behaviour check (`tall.py`, verify_and_land.behaviour_of):
`tmp/tall_vc71.txt`. calls.py binds every callee / table / import to the retail address named in the row header. No header
changes (hdr/README.md). 13 of the 17 catalog names were guesses and are wrong (NAME_FIXES.tsv / LABELS.tsv).

| Address | Real name | File | Grade | Flags | Device |
|---|---|---|---|---|---|
| 0x00C96D20 | dradf4 | smallft.c | RELOCATION_MATCH | /Gr | chunk-866 smallft TU text, leaf `_dradf4@28` |
| 0x00C97600 | dradfg | smallft.c | RELOCATION_MATCH | /Gr | same, `_dradfg@40` |
| 0x00C998B0 | dradb4 | smallft.c | RELOCATION_MATCH | /Gr | same, `_dradb4@28` |
| 0x00C9A230 | dradbg | smallft.c | RELOCATION_MATCH | /Gr | same, `_dradbg@40` |
| 0x00C9BC30 | drft_backward | smallft.c | RELOCATION_MATCH | /Gr | same TU, public `@drft_backward@8` (grades DIFFER 35v29 without the leaf: the grader takes the largest symbol) |
| 0x00C9BF30 | mdct_butterfly_16 | mdct.c | RELOCATION_MATCH | -- | landed mdct_butterfly_8 TU text (0x00C9BEB0), leaf `_mdct_butterfly_16` |
| 0x00C8E2E0 | _vorbis_unpack_books | info.c | RELOCATION_MATCH | /Gr /GS | landed vorbis_synthesis_headerin TU text (0x00C8E580), leaf `__vorbis_unpack_books@8` |
| 0x00C931D0 | bark_noise_hybridmp | psy.c | RELOCATION_MATCH | /Gr | `static __declspec(noinline) __stdcall` beside the landed `_vp_noisemask` body (its two call sites give the convention), `_alloca` intrinsic x5 |
| 0x00C965C0 | vorbis_book_decodev_add | codebook.c | RELOCATION_MATCH | /Gr | landed decodevs_add TU (0x00C96450) + verbatim body; `extern "C" long __stdcall decode_packed_entry_number` |
| 0x00C95A10 | _best | codebook.c | RELOCATION_MATCH | /TC /Gr | C TU (Xiph names a local `this`); extent SPLIT 642 -> 588 + 18 + 18 |
| 0x00C95C60 | vorbis_book_codeword | codebook.c | MATCH | /TC /Gr | bonus (2nd function of the 0x00C95A10 extent) |
| 0x00C95C80 | vorbis_book_codelen | codebook.c | MATCH | /TC /Gr | bonus (3rd function of the 0x00C95A10 extent) |
| 0x00C91AB0 | setup_tone_curves | psy.c | RELOCATION_MATCH | /Gr | psy.c init slice TU (min/max/attenuate_curve statics + setup_tone_curves + _vp_psy_init), leaf `_setup_tone_curves@20` (curveatt_dB lands in ecx: `ret 10h`) |
| 0x00C92660 | _vp_psy_init | psy.c | RELOCATION_MATCH | /Gr | same TU, public `@_vp_psy_init@20` |
| 0x00C9CC60 | limit_sum | bitrate.c | MATCH | /TC /Gr | bitrate.c C TU over engine/Vorbis.h; `static __declspec(noinline) long __stdcall` beside addblock gives retail's bm-in-ebx |
| 0x00C9CFF0 | vorbis_bitrate_addblock | bitrate.c | RELOCATION_MATCH | /TC /Gr | same TU; extent SPLIT 2888 -> 2700 + 184 |
| 0x00C9DA80 | vorbis_bitrate_flushpacket | bitrate.c | MATCH | /TC /Gr | bonus (2nd function of the 0x00C9CFF0 extent) |
| 0x00C8BB20 | ov_read | vorbisfile.c | RELOCATION_MATCH | /GS | landed ov_pcm_seek slice TU (0x00C8B490) + ov_read / ov_read_float; same-TU `vorbis_ftoi` through libvorbis/vorbis_ftoi.h; `int val=0`; extent SPLIT 1132 -> 950 + 172 |
| 0x00C8BEE0 | ov_read_float | vorbisfile.c | RELOCATION_MATCH | /GS | bonus (2nd function of the 0x00C8BB20 extent), same TU |
| 0x00C95580 | vorbis_book_init_decode | sharedbook.c | DIFFER(901v910) | /TC /Gr | PARKED: allocation-only residue, differ/00c95580.cpp |
| 0x00C89850 | CReverbFilterTap::ProcessRamp | libpk engine | DIFFER(607v588) | -- | PARKED: loop-induction residue, differ/00c89850.cpp |

## Extent splits (the coordinator must split three chunk-oracle rows at landing)
xdis.py / the retail listings show a `ret` + int3 pad + a new prologue inside three catalogued extents. `trim_oracle.tsv` holds
the split bytes (each slice ends in its `ret`), `bin/qt.py` is q.py reading only that file, and finish.py now takes the bytes of
these seven rows from it:
- 0x00C8BB20 (1132) = ov_read 950 (`ret 14h` at 0x00C8BED3) + pad + ov_read_float 0x00C8BEE0 172 (`ret 8`).
- 0x00C95A10 (642) = _best 588 (`ret 4`) + vorbis_book_codeword 0x00C95C60 18 + vorbis_book_codelen 0x00C95C80 18.
- 0x00C9CFF0 (2888) = vorbis_bitrate_addblock 2700 + pad + vorbis_bitrate_flushpacket 0x00C9DA80 184 (`ret 4`).
The payload therefore carries 19 authored rows for 17 chunk rows (two of them parked): 15 chunk starts + 4 second/third starts.

## Devices worth keeping

### ov_read 0x00C8BB20: the callee clobber set is part of the shape (14 grades)
The verbatim vorbisfile.c text with `vorbis_ftoi` merely *declared* grades 1004v950: VC7.1 spills both packing-loop indices and
`off` around every `vorbis_ftoi` call, and keeps `vf` in ebx. Retail keeps `i` in edx / `off` in ebx across the call because LTCG
knew the helper (os.h's `fld f ; fistp i`, out of line at 0x00C89BE0) touches only eax. Giving the helper a body IN the TU restores
that knowledge without /GL: a stub clobbering eax only -> 936, the real asm body through the shared ASM_RULES header
`libvorbis/vorbis_ftoi.h` (`VORBIS_FTOI`, portable twin under FABLE_PORTABLE; the row source itself has no asm) -> 950v950 with
one residue, `add al,bl ; mov [esi],al` (retail) vs `mov cl,bl ; add cl,al ; mov [esi],cl` in the 8-bit arm. The `/QIfist (int)f`
body of the landed lsp.c TU gives 1000 here (its clobber set differs). The last residue is operand numbering: Xiph's `int val;`
numbers `off` before `val`, and VC7.1 then puts `off` first in the commutative add (copying it since ebx is live); `int val=0;`
(dead store folded) numbers `val` first and the add lands in al -> RELOCATION_MATCH. Spellings that do NOT move it: `off+val`,
`val+=off`, `(char)(val+off)`, `(char)val+(char)off`, `(unsigned char)`, `/J`, `int val` inside the loop, `int val` after `int off`.
The deviation from Xiph is the single `=0`, documented in the row header.

### setup_tone_curves 0x00C91AB0: a stdcall leaf counts every argument
The static is `static __declspec(noinline) float ***__stdcall setup_tone_curves(float*, float, int, float, float)`: the symbol is
`_setup_tone_curves@20` even though VC7.1 then passes `curveatt_dB` in ecx and the function does `ret 10h`. The first grade with
`@16` passed only because the grader fell back to the largest symbol, which happened to be this function; the WARN line is the
tell (`leaf ... matches no symbol; graded the largest function`). Fixed in the source; regraded RELOCATION_MATCH.

### bitrate.c: limit_sum's bm-in-ebx comes from its caller being in the TU
`static __declspec(noinline) long __stdcall limit_sum(bm, limit)` beside `vorbis_bitrate_addblock` (which calls it twice in the
hard-max / hard-min loops) is what makes VC7.1 pass `bm` in ebx; alone it would be plain stdcall. The TU compiles as C (`/TC /Gr`)
because engine/Vorbis.h is C-only and bitrate.c names a local `double new`. `LACING_ADJUST` and `BINBYTES` are
`static __declspec(noinline)` without a convention (VC7.1 picks fastcall-like register passing; the calls bind 0x00C9CB60 /
0x00C9CB40 exactly). Both are landed; the catalog's "Math_ComputeByteChunkCount" for 0x00C9CB60 is wrong (NAME_FIXES).

### codebook.c / sharedbook.c as C
`_best` has `float this=_dist(...)` and sharedbook.c keeps C idioms (`alloca` returning void* assigned without casts): both TUs
are C (`/TC /Gr`), drop `extern "C"`, and declare the retail imports with `__declspec(dllimport)` (malloc / calloc / free / qsort).
`_dist` is `static __declspec(noinline) float __stdcall` (landed 0x00C95910 convention); `bitreverse` is a plain
`static __declspec(noinline)` (ecx), `sort32a` a plain `static __cdecl` (qsort callback).

### psy.c init slice
`tmp/psy_init_tu.inc`: psy.h / scales.h macros, the three structs, `extern float ATH[88]` / `tonemasks[17][6][56]` (masking.h tables
in .data at 0x0139DE60 / 0x0139DFC0, adjacent as in the header), the landed min_curve / max_curve (plain statics) and
attenuate_curve (`__stdcall`) bodies, `setup_tone_curves` (`__stdcall`), `_vp_psy_init` (`__fastcall`). log / exp / atan / abs /
memset / memcpy / _alloca are `#pragma intrinsic`; floor / ceil / malloc are dllimports ([0x01440100], [0x014400F4], [0x01440158]).

## Parked rows (the quantity nothing moves)

### 0x00C89850 CReverbFilterTap::ProcessRamp -- DIFFER(607v588), 13 grades
Prologue, start/first computation, both CDspGain::Crossfade calls, the tail output mix and the `rep movsd` record copy all match.
Two source changes moved it 631 -> 615 -> 607: dividing by `(float)count` instead of a `float samples` local (VC7.1 hoisted
`1.0f/samples` out of the loop as a reciprocal multiply; retail divides inside the loop), and loading each record's gain into a
`float g` local so it is read once (`g*blend`, `blendStep*g/(float)count`, as retail's `fld [eax+ebp+8] ... fmul st(1) ... fmul blend`).
What is left is one quantity: the two-trip history-tap loop's induction form. Retail eliminates `i` (ebp walks `target` 4 bytes a
trip, `current`'s delay and gain are both `[eax+ebp]` / `[eax+ebp+8]` through the `current - target` distance in eax, and the trip
count is a `mov dword ptr [esp+20h],2 ; dec ; jne` down-counter); VC7.1 /O2 bases its induction pointer at `target+8` (the gain),
reaches the target delay as `[ebp-8]` and the current gain as `[eax+ebp]`, but re-indexes `current->m_historyDelay[i]` through a
reload of `current` and so keeps `i` as an up-counter in a frame slot (`inc ; cmp eax,2 ; jb`). The frame-slot numbering of
other / blendEnd / data / distance follows from that. Not moved by: `int i`, `i` declared at the top, `do { } while (++i < 2)`,
a `const SReverbTapParams* cur` alias, a `d` local for the delay (before or after `g`), the Back() result in a local (worse, 613),
four array-base pointers outside the loop (worse, 620), a pointer walk over the records (worse, 634). Next: c2prio on the loop
body to see which address expression becomes the SR base and why the delay index is left; or an LTCG-style TU with
CDspOps::MixRampFromRing's body present.

### 0x00C95580 vorbis_book_init_decode -- DIFFER(901v910), 7 grades
The instruction stream is retail's; the residue is allocation only. (1) In the `codes[i]=bitreverse(codes[i]); codep[i]=codes+i`
loop the two spilled temps (the strength-reduced codes pointer and the n down-counter) take [ebp-0Ch] / [ebp-8] in the opposite
order. (2) In the hint loop `while(hi<n && word>=(c->codelist[hi]&mask))hi++;` retail keeps `hi` in memory ([ebp-0Ch], reloaded into
ecx each pass, `mov edx,[esi+14h] ; lea eax,[edx+eax*4] ; mov ecx,[eax] ; and ecx,[ebp-1Ch]`) while VC7.1 holds hi in edx (`mov
eax,[esi+14h] ; lea eax,[eax+edx*4] ; mov ecx,[ebp-1Ch] ; and ecx,[eax]`) and stores it after the loop; the two 0x7fff clamps then
read loval / hival from the swapped registers (9 bytes fewer). Not moved by: swapping the `&` operands, `long hi=0,lo=0`, declaring
`sortindex` first; worse: swapping the two loop statements (917), same-TU `_ilog` + `vorbis_book_clear` bodies (990: book_clear
inlined). The libvorbis 1.1.0 text of this function is identical, so no version lever. Next: c2prio over the hint loop (what takes
edx in retail), a same-TU `_book_unquantize` body.

## Catalog / label corrections (two sources each: NAME_FIXES.tsv, LABELS.tsv)
Wrong catalog names: 0x00C8BB20 "Primitive_ProcessVertexBuffer" = ov_read; 0x00C91AB0 "BoundingBox_ComputeTransform" =
setup_tone_curves; 0x00C92660 "AudioSystem_ProcessSources" = _vp_psy_init; 0x00C931D0 "FFT_ComputeMagnitudes" =
bark_noise_hybridmp; 0x00C95580 "CTCWound::CWound::Initialize" = vorbis_book_init_decode; 0x00C965C0 "sub_00c965c0" =
vorbis_book_decodev_add; 0x00C9BF30 "C3DBoundingBox::InitialiseFromSphere" = mdct_butterfly_16; 0x00C9CC60 "Array_LinearSearch"
= limit_sum; 0x00C9CFF0 "CLandscapeBackgroundDynamicBuildTemplate::UpdateTerrainGrid" = vorbis_bitrate_addblock; 0x00C89850
"ProcessCrossfade" = the landed ProcessRamp. Callee labels seen through calls.py: 0x00C9CB60 LACING_ADJUST (manifest
"Math_ComputeByteChunkCount"), 0x00C9CB40 BINBYTES (coverage "Hora"), 0x00C91800 oggpack_get_buffer ("DrawGetEnvironment"),
0x00C8BB10 host_is_big_endian ("AddChildPrimitive"), 0x00C89BE0 vorbis_ftoi (landed as "FtoL"; the landed header already names it).

## Files
`src/<addr>.cpp` x19, `test/<addr>.cpp` x19 (g++ and VC7.1 PASS), `differ/00c89850.cpp`, `differ/00c95580.cpp` (line 2 residue,
line 3 next idea), `trim_oracle.tsv`, `payload.json` / `oracle.tsv` (finish.py: 19 authored, bytes of the split rows from
trim_oracle.tsv), STATUS.tsv, ATTEMPTS.log (chunk 869 section), LABELS.tsv, NAME_FIXES.tsv, LESSONS.md, hdr/README.md,
`probe/` (every variant graded), `tmp/*.q.txt` (every listing), `bin/` (qt.py, qw.py / qtw.py = q.py with a QWORK work-dir suffix so
two variants of one address never share `$S/wine/q/<addr>` -- grading one address twice at once corrupts the .obj and reports
OBJDUMP_ERR; the test generators gen_bitrate_tests.py / gen_ovread_tests.py; regrade.sh).
