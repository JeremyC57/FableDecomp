# b1 (hand3c2, Account B, 2026-11-19): 0x00AAD340 CWaveFile::ConvertSamplingRate -- READY

- Grade: RELOCATION_MATCH (g.py + strict vland `RELOCATION_MATCH PASS`, LANDVERIFY_STRICT_HEADERS=1). Purity: `retail_asm`
  (shared helpers GFFloatToLongFloor x3, GFFloatToLongNear x1; the row's `// Retail-asm:` line cites 0x00AAD381 / 0x00AAD4E1).
  ATTEMPTS.log: 3 grades this session (one SRC_FAIL for a missing fable_string.h, DIFFER(583v584), the match), after one hand3c baseline. Flags: none. Overlay headers: none (repo headers only; no header diff).
- Leaf ?ConvertSamplingRate@CWaveFile@@QAEXK@Z, 584 bytes, `ret 4`.

## Identity
Ego_r.pdb ?ConvertSamplingRate@CWaveFile@@QAEXK@Z (lib_wave_loader.obj index 8) and FableWin.pdb 0x033FE970 (same spelling).
Catalog name unchanged. Locals and scopes exactly as Ego_d.pdb lists them (rate, new_data, pos_int, data_dest, step_int, pos_frac,
FRACTION_BITS, step, new_sample_length, step_frac; c_sample; pos_int_plus_1, weight1, weight2; c_chan; sample1, sample2).

## What closed it
- Ported the parked local-model draft onto the shared headers with the TU-mate LoadWaveFile 0x00AAD650's opt-in set
  (FABLE_RETAIL_CARRAY, FABLE_RETAIL_STLP_VECTOR_N_OUTOFLINE, FABLE_RETAIL_STLP_VECTOR_C; fable_string.h first) and Ego_d's locals:
  DIFFER(583v584) at the first grade (frame slots of pos_int / weight1 swapped).
- In the wrap block `pos_int++;` before `pos_frac -= 1 << FRACTION_BITS;`: RELOCATION_MATCH (grade 2). Plain statement order, no
  Inferred / FITTED construct, no bracket.
- data_dest is `new_data.GetAsCArray()` (the shared CArray accessor: retail's `size>0 ? start : &new_data` select at 0x00AAD456).

## Test (test/00aad340.cpp)
Links the REAL row object over raw native-layout CWaveFile storage (offsets cited to the reading instructions); provides the
out-of-line vector<short>(n) 0x00AADD80. Independent reference model, including GFFloatToLongFloor's exact semantics
(fistp(v - 0.5) + 1 when float(r + 1) has v's bits -- it returns n + 1 for some floats >= 2^24). 11 cases: frac exactly 65536 (no
wrap) and 65537 (wrap), next clamped / not clamped with sentinels behind SampleLength, integer / non-integer steps, floor vs round
lengths, negative samples (arithmetic >> 16), 1/2/3 channels, empty result. Case 10 (SL = 3 * 2^22, ~67 MB) is the only kind of
input where `pos_frac > 65536` and `>= 65536` differ: the exact-65536 iteration must coincide with pos = SL - 1, which needs one
extra iteration past SL/step -- here GFFloatToLongFloor(2^24) = 2^24 + 1 (no small input does it; brute-force search to SL 40).
Mutation check (mutants.tsv / mutants.log): 33 one-decision mutants, 32 KILLED. Survivor: `pos_int_plus_1 >= SampleLength - 1` --
EQUIVALENT (it only re-clamps SL - 1 to SL - 1); the non-equivalent boundary moves (`> SampleLength`, `> SampleLength - 3`) are killed.

## Portability
The row's only non-C++ parts are the shared helpers, whose FABLE_PORTABLE twins portable_twins_check.py covers. A whole-row g++
build was tried once: 64-bit stops in rebuild_abi.h (`fable_require_ptr32`) and fable_string.h (`__cdecl`), -m32 has no 32-bit
libstdc++ headers on this machine -- repo-wide header state, not this row.

## NAME_FIXES
None.
