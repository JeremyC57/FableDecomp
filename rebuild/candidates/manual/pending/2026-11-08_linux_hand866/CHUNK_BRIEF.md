# Chunk 866 -- ten never-attempted Ogg Vorbis / libpk audio rows (0x00C8B000..0x00C9C000) (overrides PROMPT.md where they differ)

These rows sit in the Xiph Ogg Vorbis decoder linked inside libpk (GOTCHAS: "0xC89xxx..0xC9Exxx is the Xiph Ogg Vorbis decoder ...
libVorbis I 20030909", "0xC908E0..0xC9FDF0 is libogg bitwise.c + libvorbis 1.0.1 ... verbatim Xiph bodies match on the first grade").
Read every GOTCHAS line mentioning vorbis / libpk / LTCG first, and the landed neighbours in rebuild/src/compiled/00/c8 and 00/c9
(global_drfti1_00c96800.cpp, global_mdctinit_00c9bcd0.cpp, global_vorbisapplywindow_00c9ea80.cpp ...) for the conventions: C linkage,
`__fastcall` on public / address-taken functions, `__cdecl` qsort comparators, static helpers with LTCG register arguments.
The catalog names are DESCRIPTIVE guesses ("FFT_Radix4Transform", "Audio_GetSamplePosition", ...): identify each function in the public
Xiph sources (libogg 1.1 / libvorbis 1.0.1 or the 20030909 CVS: smallft.c dradf4/dradb4/drftf1/drftb1, mdct.c mdct_butterfly_*,
framing.c _packetout / ogg_stream_packetout, vorbisfile.c _ov_splice / ov_*, info.c, ...) -- you may read them on the web (WebFetch /
WebSearch), never copy third-party files into the repo -- and use the real Xiph name and signature. Byte-check the body against the
retail bytes; an extent that holds two functions or stops early must be reported (`chunk_pool/check_extents.py`).
LTCG eax/ebx-argument static helpers cannot be expressed in VC7.1 C++: those rows grade `retail_asm` only under docs/pipeline/ASM_RULES.md
(shared-header macro + FABLE_PORTABLE twin); prefer the public __fastcall callers. Use the `C` / `extern "C"` spelling the neighbours use.

## The main goal (owner, verbatim, 2026-11-08)
"Remember the main goal is readable clean c++ that is portable and byte matches retail code and behaves the same as retail."
Every landed row: MATCH / RELOCATION_MATCH under vland, a behaviour test that PASSes, real names in the exact PDB spelling
(ghidra_out/egor_pdb_names.tsv, fablewin_pdb_names.tsv, struct_layouts_egor.tsv, fable_types.h; two sources), shared headers
`engine/<Class>.h` / `stlport/*.h` (never a local pad struct or a local copy of the STL; add an additive `#ifdef FABLE_RETAIL_*`
opt-in in your hdr/ overlay instead, default path unchanged, diff against rebuild/include), the full mangled `// Leaf:` line (never a
prefix) and the same `leaf` in the payload, comments that say what the game does. No `__asm` / `naked` / `_emit` / `volatile`.
A header body you add must be retail's own logic (a stand-in that does something else fails "behaves the same").

## Inline-budget residues (owner decision 2026-11-08; EXTRA_RULES "Owner decision, 2026-11-08")
If the ONLY residue is an inline decision (an inline callee expanded at some sites and called at others, inline depth, the caller-size
cap): (1) plain C++; (2) a per-row flag with the source untouched -- payload `"flags": ["/d2inlS<N>"]` or `/d2inlT<N>` (bisect, smallest
N; /d2inlS often cliffs at 15, default S9; T default 74), plus a matching `// Flags:` line; (3) only if no flag reaches it, a scoped
`#pragma inline_depth(0)` ... `()` / `auto_inline` / `_MSC_VER`-guarded `__forceinline`. Line 2: `// Inline-budget: <device> -- <evidence>`.
Never for register-choice / block-layout / x87-order residues.

## Budget and conduct
Up to 150 grades per row (huge budget, owner): keep working EVERY row until it matches or you can name, with probe evidence, the
compiler quantity no allowed spelling moves. Do not stop early: a row with fewer than 50 real source attempts is not finished unless it
matched. Diagnose first (c2prio.py, the call/ret sequence, the first differing byte), then spell. Log every grade in ATTEMPTS.log and keep
STATUS.tsv current. At most 2 concurrent compile jobs; never grade one address twice at once (q.py shares one work dir per address).
Never commit, never write under /home/user/FableDecomp (scratch only, no stray redirect files), no git commands.

## Rows (bytes = retail length after trailing padding; oracle `/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/batch/chunk866_oracle.tsv`)
| Address | Catalog name | Bytes |
|---|---|---|
| 0x00C8BF90 | _global::_ov_splice | 571 |
| 0x00C8D9D0 | _global::_packetout | 311 |
| 0x00C9C000 | _global::mdct_butterfly_32 | 475 |
| 0x00C969D0 | _global::FFT_Radix4Transform | 836 |
| 0x00C98E80 | _global::FFT_2D_DispatchByRadix | 470 |
| 0x00C99060 | _global::FFT_Radix4Stage | 917 |
| 0x00C9B9F0 | _global::Audio_TransformFFTCoefficients | 536 |
| 0x00C8CB70 | _global::Audio_GetSamplePosition | 382 |
| 0x00C8E880 | _global::OggVorbis_EncodeSetupPacket | 483 |
| 0x00C99400 | _global::MDCT_Analysis | 1190 |

## Research rules (shared block; read docs/pipeline/VC71_CODEGEN_RESEARCH.md and C2_ALLOCATOR_PROBE.md in full)
## Research rules (2026-11-08; read docs/pipeline/VC71_CODEGEN_RESEARCH.md sections 2, 4-7 and docs/pipeline/C2_ALLOCATOR_PROBE.md in full)
These are proven on lane probes. Use them BEFORE spending grades on blind spellings:
- **Read the allocator, don't guess it.** For any register difference run
  `python3 tools/decomp_pipeline/linux_lane/lane_tools/c2probe/c2prio.py <src.cpp> --trace` (25-50 s; `--func` / `--flags` default from
  `// Leaf:` / `// Flags:`; set FABLE_HDR_OVERLAY as for a grade; `--rotation` for eax/ecx/edx temporaries, `--blocks` for per-block
  priority shares). A probe run counts as one of your 2 concurrent jobs.
- **Callee-saved roles (esi/edi/ebx/ebp)** = priority colouring in table order eax,ecx,edx,esi,edi,ebx,ebp: priority = sum over blocks
  of w*K*2 per reference minus w*K per live-through block (w = 1<<loop depth, K = candidates referenced in the block); ties go to the
  larger tie key (last writing tuple); a copy relation gives a -1..-3 preference for the copied value's register. Move a role by
  adding/removing ONE real reference, changing which value is defined first, or breaking / creating a copy relation (`--trace` shows it).
- **Scratch rotation (eax/ecx/edx)** comes from a per-function rotating pointer, not the allocator: add or remove exactly one expression
  temporary BEFORE the site (a named value local, an inline accessor at one site, a folded cast); `--rotation` shows the pointer.
  A named local that becomes a callee-saved candidate is not a temporary (it moves other things).
- **Address-taken locals have no candidate** (they live in memory): to get a value into a register, pass a COPY by reference
  (`const T key = v; map[key]`) so the original is never address-exposed.
- **Zero in a register:** 3 zero stores -> `xor` scratch, 5 with calls -> `xor` callee-saved; cross the threshold by writing stores
  per arm or merging them.
- **Frame-slot sharing / dead stores / reloads:** give each callee that receives a local's address a same-TU body under
  `#pragma auto_inline(off)` that only READS through the pointer (landed bodies where they exist); a writing or declared-only callee
  keeps the slot and the stores (the reverse lever reproduces a retail dead store). Store->load forwarding of a member needs the same
  base expression on both sides (store through `T* p = &m;`, read through the member, or the reverse, to block it).
- **x87 order:** with parameter/member bases the larger displacement is loaded first (textual order irrelevant), equal -> earlier
  parameter; direct operands (globals, by-value, frame) keep textual order (right operand first) and beat indirect ones; sums of
  products are re-sorted regardless of parentheses / named locals / casts and pinned only by a store or a call; `float& r = x; f(r)`
  gives push-then-lea; naming a float call result before adding it lets an unrelated integer load schedule ahead of the `fadd`.
- **Flags:** /G5 = /G6 = /GB = default and retail is not /G7 -- never spend grades on them; a flag that matches one row must not break
  the TU's landed rows (`rebuild/candidates/manual/pending/2026-11-08_research861/tools/tu_flag_check.py <lo> <hi> <flag>`).
- **Pads / block layout:** an alignment-pad difference always follows an earlier byte difference: fix the first byte first.
- c2 is function-local: declarations, path length and neighbouring functions do not change a function's bytes (only visible bodies of
  callees do).



## Output
As PROMPT.md: `payload.json` (matched rows only; `flags` per entry where used), `oracle.tsv`, `differ/<addr>.cpp` for the rest (line 2 = residue
+ probe quantity, line 3 = next idea), `NOTES.md`, `LESSONS.md` (GOTCHAS one-liners), `NAME_FIXES.tsv` (landed spellings you prove wrong),
`LABELS.tsv` (address, PDB name, two sources), `hdr/README.md` for every header change. Run finish.py last. Final report: matched / parked
counts, per-row device, header changes, names, and anything the coordinator must know.
