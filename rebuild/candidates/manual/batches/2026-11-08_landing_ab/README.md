# Landing 2026-11-08ab: chunk 881 (libpk stereo SSE2 FIR resamplers, Opus high), 2 of 2

Both RELOCATION_MATCH + PASS (VC7.1 and g++), `/GS`. Hand-back: `pending/2026-11-08_linux_hand881/` (with the wrap enumerators).

| Row | Device |
|---|---|
| 0x00C812F0 ResampleStereoFIR (12,532 incl. 8-entry case table) | ResampleMonoFIR + a stereo output stage; plain C++ on dvec.h + SSE2 intrinsics; explicit `Is32vec4(...)` wraps place VC7.1's inline-budget split (128-way enumeration, 4 matches) |
| 0x00C843F0 ResampleStereoRampFIR (15,328 incl. table) | the stereo FIR + the ramp gain stage, `short` gains; case 7 calls even the dvec constructors out of line, which pins the source spelling; two remaining split points set by byte-neutral wraps (~1,190 enumerated grades) |

- Extents: 0x00C812F0 cut from 12,544 to 12,532 (12 bytes int3 padding), BOUNDS_FIX row; 0x00C843F0 = catalog.
- Names: the catalog's descriptive guesses replaced by the names the landed MixFilteredVoice* callers declare; labels
  `ghidra_out/labels_landing_2026-11-08ab.tsv`; queued as land0811ab: the short-gain respelling of ResampleStereoRampFIR in its
  callers and the three dvec constructors 0x00C7B840 / 0x00C7B810 / 0x00C7B970 (Is32vec4 / I32vec4 / Is16vec8 from __m128i).
- The landed MixFilteredVoice* header comments that call these kernels "inline asm" are wrong: compiler-emitted dvec C++.
