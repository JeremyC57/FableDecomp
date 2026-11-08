# Landing 2026-11-08z: chunk 878 (libpk / CODA audio, Opus high), 2 of 3

Both RELOCATION_MATCH + PASS (VC7.1 and g++), `/GS`. Hand-back: `pending/2026-11-08_linux_hand878/`.

| Row | Device |
|---|---|
| 0x00C7BAF0 ResampleMonoFIR (10,544, incl. its 8-entry case table) | plain C++ on the dvec.h classes (Is32vec4 / Is16vec8) + SSE2 intrinsics; explicit `Is32vec4(...)` wraps place VC7.1's inline-budget split (the first out-of-line `+=` at 0x00C7C053) |
| 0x00C7E420 ResampleMonoRampFIR (11,980) | the same FIR plus a per-lane gain ramp; `short` gains (long swaps the first-lane `lea` operands); wrap placement found by enumeration |

- Parked: 0x00C4F600 CEAXManager::SetEAXProperty DIFFER(9204v9204), 10 differing lines: scratch-register rotation in the
  ROOM / ROOMHF / ROOMLF listener cases only. Retail behaviour found: a non-deferred buffer property change also re-applies the
  listener properties (one shared trailing apply).
- Extents: 0x00C7BAF0 = catalog; 0x00C7E420 cut to 11,980 (4 bytes int3 padding), BOUNDS_FIX row.
- Names: the catalog's descriptive guesses ("Graphics_RenderTextConditional_ProcessRows", "Audio_ConvolveFilter_LongVariant")
  replaced by the names the landed MixFilteredVoice* callers declare; labels `ghidra_out/labels_landing_2026-11-08z.tsv`;
  caller respelling (short gains) queued as land0811z.
