# 00B8DCD0 attempts

Whole function639bytes (RET4 at00B8DF4C), actual original private SelectFlareTestVertexBuffer(unsigned long). Both PDB publics and module973 original locals/1037-byte debug body prove it. All six supplied neighbors were read; their old local stand-ins are not copied. Shared typed manager members, CCountedPointer, STLport vector/pair, vertex/buffer classes and generic vertex setters are used.

- sol2_01_original_module_cpp_rounding:824bytes/255instructions versus639/198. First difference is an aligned8byte frame caused by the plain nearest-even helper's double floor arithmetic; all circle code is scalar x87, with floor/__ftol2 calls instead of native bare dword FISTP. The original caller structure/locals come directly from module973. No shared asm helper has yet been adopted.

Two distinct native near-conversion sites00B8DE0E..00B8DE12 (sine) and00B8DE62..00B8DE66 (cosine) need the mandatory per-site plain30/intrinsic10 ladder. Each spelling of the one real GFFloatToLongNear member is tried at BOTH original GFFastSin/GFFastCos expansions. These are site spellings; they are not claimed as40 independent whole-function strategies. Compiler flags/inline diagnostics are counted separately.

- sol2_plain02_trunc_float_remainder: DIFFER,673bytes; target bare dword FISTP=False.

- sol2_plain03_trunc_double_remainder: DIFFER,673bytes; target bare dword FISTP=False.

- sol2_plain04_trunc_integer_result: DIFFER,673bytes; target bare dword FISTP=False.

- sol2_plain05_trunc_nested_sign: DIFFER,673bytes; target bare dword FISTP=False.

- sol2_plain06_trunc_abs_remainder: DIFFER,673bytes; target bare dword FISTP=False.

- sol2_plain07_trunc_result_ternary: DIFFER,673bytes; target bare dword FISTP=False.

- sol2_plain08_trunc_correction_sum: DIFFER,673bytes; target bare dword FISTP=False.

- sol2_plain09_floor_float_fraction: DIFFER,811bytes; target bare dword FISTP=False.

- sol2_plain10_floor_shifted_half: DIFFER,537bytes; target bare dword FISTP=False.

- sol2_plain11_ceil_double_fraction: DIFFER,812bytes; target bare dword FISTP=False.

- sol2_plain12_ceil_shifted_half: DIFFER,537bytes; target bare dword FISTP=False.

- sol2_plain13_modf_fraction: DIFFER,673bytes; target bare dword FISTP=False.

- sol2_plain14_modf_abs_fraction: DIFFER,673bytes; target bare dword FISTP=False.

- sol2_plain15_fmod_fraction: DIFFER,673bytes; target bare dword FISTP=False.

- sol2_plain16_positive_unsigned_correction: DIFFER,785bytes; target bare dword FISTP=False.

- sol2_plain17_short_correction: DIFFER,764bytes; target bare dword FISTP=False.

- sol2_plain18_ushort_correction: DIFFER,764bytes; target bare dword FISTP=False.

- sol2_plain19_integer_distance_comparison: DIFFER,805bytes; target bare dword FISTP=False.

- sol2_plain20_floor_even_remainder: DIFFER,829bytes; target bare dword FISTP=False.

- sol2_plain21_ceil_even_remainder: DIFFER,829bytes; target bare dword FISTP=False.

- sol2_plain22_biased_float_bits_4b200000: DIFFER,660bytes; target bare dword FISTP=False.

- sol2_plain23_biased_float_bits_4b400000: DIFFER,660bytes; target bare dword FISTP=False.

- sol2_plain24_biased_float_bits_4b600000: DIFFER,660bytes; target bare dword FISTP=False.

- sol2_plain25_biased_subtract_float: DIFFER,672bytes; target bare dword FISTP=False.

- sol2_plain26_fabs_round_then_sign: DIFFER,792bytes; target bare dword FISTP=False.

- sol2_plain27_floor_modulo_parity: DIFFER,673bytes; target bare dword FISTP=False.

- sol2_plain28_trunc_even_mask: DIFFER,673bytes; target bare dword FISTP=False.

- sol2_plain29_floor_half_sign_test: DIFFER,673bytes; target bare dword FISTP=False.

- sol2_plain30_double_cast_then_correction: DIFFER,673bytes; target bare dword FISTP=False.

- sol2_intrinsic01_cvtss_set_ss: DIFFER,659bytes; target bare dword FISTP=False.

- sol2_intrinsic02_cvtss_load_ss: DIFFER,663bytes; target bare dword FISTP=False.

- sol2_intrinsic03_cvtss_broadcast: DIFFER,667bytes; target bare dword FISTP=False.

- sol2_intrinsic04_cvtss_setr: DIFFER,537bytes; target bare dword FISTP=False.

- sol2_intrinsic05_cvtps_epi32_extract: DIFFER,671bytes; target bare dword FISTP=False.

- sol2_intrinsic06_cvtsd_double: DIFFER,659bytes; target bare dword FISTP=False.

- sol2_intrinsic07_cvtsd_float_promoted: DIFFER,679bytes; target bare dword FISTP=False.

- sol2_intrinsic08_cvtpd_epi32_extract: DIFFER,667bytes; target bare dword FISTP=False.

- sol2_intrinsic09_cvtss_shuffle: DIFFER,681bytes; target bare dword FISTP=False.

- sol2_intrinsic10_cvttss_half_even_fixup: DIFFER,537bytes; target bare dword FISTP=False.

## Original helper adoption, visibility, and closure validation

- sol2_02_existing_original_near_helper:662bytes. Only after both40-spelling site ladders, unchanged shared GFFloatToLongNear supplies the two native bare dword stores. Original GFFastSin/GFFastCos remain ordinary inline; no row flags or new inline controls.
- sol2_03_visible_real_stlport_lower_bound:653bytes. Actual vendor lower_bound body and distance/advance dispatch visible; native cached vector end restored. This is the second whole-function source strategy (callee visibility), not a register lever. Its own75-byte body independently matches.
- sol2_04_real_reset_assignment_insert_visible: target639exact. Real existing Reset, counted assignment and vector insert bodies visible together restore the native buffer/count locals and pair-temp cleanup. Third whole-function source strategy. Independent audit immediately found insert's helper body had the wrong backward-copy dispatch; target-only match was not treated as ready.
- sol2_05_real_random_access_copy_backward_abi: target639exact; vector insert293, backward copy149 and pair assignment78 exact. Restored real random-access STLport dispatch and native fastcall convention. This is an actual helper API/ABI repair, not a new caller strategy. Recursive audit then found overflow's free helpers were still cdecl.
- sol2_06_original_stlport_free_function_abi: target639exact and all eleven real helpers1206bytes exact. The new narrow fastcall source profile reuses the prior proven STLport calling convention while preserving every previous profile. No inline attributes or budget flags were used. All42 recursive relocations pass, including original typed deleter and exact float literals.
- sol2_07_shared_trig_and_pruned_probe_header: final selected source, same639/198 exact target and complete1206-byte helper closure. Existing GFFastCos reused; original GFFastSin moved to a new shared header. The attempt-only counted assignment declaration opt-in/header was removed. Packaging cleanup is not a new strategy.

Total46 worker compiler invocations:30 plain per-site spellings (initial source included),10 intrinsic per-site spellings, and6 subsequent helper/visibility/packaging validations. Three whole-function source strategies; helper ABI corrections are logged separately. Some expensive scalar/intrinsic forms outlined the trig calls; all emitted source bodies still lacked native bare dword FISTP. Float/short/unsigned/magic-bias candidate forms were considered only on this caller's bounded valid trig-conversion domain; they are rejected candidates, not final general conversion implementations. No failed scalar or intrinsic candidate was returned as the implementation.

## Behavior and old-path proofs

962 cache/build scenarios call the complete copied target1924times. All eleven actual helper bodies are compiled from retained shared source, with real valid typed backing and object lifetimes.57 protected overflow cases exercise all meaningful allocation/copy/fill/clear arms. The original inline SelectVertexBuffer device-forwarding body is also copied exactly and all four device arguments checked. Center normal/UV bytes are observed unwritten; no T() or whole-buffer zeroing replaces them. The helper twin test compares signed32 results on20014 float patterns, including exceptional input. VC71 /Od /W3 /GR- without /EHsc and g++64 C++03 output exactly00b8dcd0_TEST PASS.

Test diagnostics (protected fixture backing access, GCC duplicate standard tags/MS-only unused traits) were resolved through an access-only fixture subclass and explicit portable declaration adaptation. They are not source strategies. All used helper behavior bodies remain verbatim, recorded by TEST_HELPER_PROVENANCE.json. Helper/tag state is not read from uninitialized bytes. Empty test vectors have actual nonnull storage and vertices are placement-constructed before writes.

276 old/default header profiles preprocess identically. All22 moved original manager assertions are pinned. The new features are guarded; old ABI condition OR new flag reduces exactly to its old condition when new flag is absent. No packing or allocator-oriented inline control is introduced. Main performs combined current-header regression.
