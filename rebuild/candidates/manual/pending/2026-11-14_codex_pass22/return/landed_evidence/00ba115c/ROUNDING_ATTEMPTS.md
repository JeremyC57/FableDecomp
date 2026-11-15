# Rounding-site trials for parent00BA1140

Two sites: GFFloatToLongNear load00BA1606/store00BA160A; GFFastFloatToLong load00BA1701/store00BA1709. Every candidate replaces BOTH routines in the complete parent and is compiled by unmodified VC7.1 RTM. Primary production source remains separate.

30 distinct C++ spellings and10 intrinsic spellings completed. These are kernel diagnostics, NOT40 distinct parent-source strategies. Simple truncation/unsigned/narrow/floor variants intentionally establish their incompatible lowering/semantics; bounded bit/magic variants are not selected production code. No candidate matches the parent.

| Trial | Hypothesis | Compiler result | Conversion instructions |
|---|---|---|---|
|round01_signed_long|truncates instead of nearest|DIFFER 2384/2344|calls __ftol2|
|round02_signed_int|different integer destination type|DIFFER 2384/2344|calls __ftol2|
|round03_unsigned_long|unsigned conversion lowering|DIFFER 2384/2344|calls __ftol2|
|round04_unsigned_int|unsigned32 conversion lowering|DIFFER 2384/2344|calls __ftol2|
|round05_signed_short|narrow signed conversion lowering|DIFFER 2388/2344|calls __ftol2|
|round06_unsigned_short|narrow unsigned conversion lowering|DIFFER 2388/2344|calls __ftol2|
|round07_signed_char|narrow byte conversion lowering|DIFFER 2386/2344|calls __ftol2|
|round08_double_input|double precision conversion input|DIFFER 2384/2344|calls __ftol2|
|round09_long_double_input|long-double type lowering|DIFFER 2384/2344|calls __ftol2|
|round10_signed64_intermediate|64-bit intermediate conversion|DIFFER 2384/2344|calls __ftol2|
|round11_unsigned64_intermediate|unsigned64 intermediate conversion|DIFFER 2384/2344|calls __ftol2|
|round12_float_output_local|explicit float local lifetime|DIFFER 2384/2344|calls __ftol2|
|round13_addressed_integer_output|original pointer-out shape without special instruction|DIFFER 2382/2344|calls __ftol2|
|round14_floor|floor intrinsic/library lowering|DIFFER 2405/2344|calls __ftol2, _floor|
|round15_ceil|ceil intrinsic/library lowering|DIFFER 2405/2344|calls __ftol2, _ceil|
|round16_floor_half|positive nearest ties-up|DIFFER 2414/2344|calls __ftol2, _floor|
|round17_ceil_half|positive nearest ties-down|DIFFER 2414/2344|calls __ftol2, _ceil|
|round18_signed_half|nearest ties-away|DIFFER 2450/2344|calls __ftol2|
|round19_truncate_remainder|nearest-even via truncated remainder|DIFFER 2267/2344|calls ?GFFloatToLongNear@@YIJM@Z, __ftol2|
|round20_floor_parity|nearest-even via floor and parity|DIFFER 2457/2344|calls ?GFFloatToLongNear@@YIJM@Z, __ftol2, _floor|
|round21_ceil_parity|nearest-even via ceil and parity|DIFFER 2452/2344|calls ?GFFloatToLongNear@@YIJM@Z, __ftol2, _ceil|
|round22_modf_parity|nearest-even via split integral/fractional parts|DIFFER 2382/2344|calls ?GFFastFloatToLong@@YIJM@Z, ?GFFloatToLongNear@@YIJM@Z, __ftol2, _modf|
|round23_fmod_parity|floating parity instead of integer parity|DIFFER 2382/2344|calls ?GFFastFloatToLong@@YIJM@Z, ?GFFloatToLongNear@@YIJM@Z, __CIfmod, __ftol2, _floor|
|round24_absolute_even|round magnitude then restore sign|DIFFER 2267/2344|calls ?GFFloatToLongNear@@YIJM@Z, __ftol2, _floor|
|round25_even_cell|nearest-even in width-two cells|DIFFER 2472/2344|calls ?GFFloatToLongNear@@YIJM@Z, __ftol2, _floor|
|round26_magic_double_memcpy|rounded double mantissa extraction, little-endian finite-range diagnostic|DIFFER 2400/2344|integer/bit arithmetic; no matching integer-store instruction|
|round27_magic_float_memcpy|rounded float mantissa extraction, bounded positive-domain diagnostic|DIFFER 2403/2344|integer/bit arithmetic; no matching integer-store instruction|
|round28_binary_floor|bounded integer search avoids float-to-int conversion|DIFFER 2403/2344|calls ?GFFastFloatToLong@@YIJM@Z, ?GFFloatToLongNear@@YIJM@Z|
|round29_ieee_float_bits|integer IEEE binary32 decode with nearest-even|DIFFER 2403/2344|calls ?GFFastFloatToLong@@YIJM@Z, ?GFFloatToLongNear@@YIJM@Z|
|round30_guarded_portable_twin|full guarded established portable semantics|DIFFER 2403/2344|calls ?GFFastFloatToLong@@YIJM@Z, ?GFFloatToLongNear@@YIJM@Z, __ftol2|
|round31_sse_scalar_nearest|SSE nearest scalar; MXCSR rounding|DIFFER 2492/2344|cvtss2si eax, xmm0; cvtss2si ebx, xmm0; cvtss2si edx, xmm0|
|round32_sse_scalar_trunc|SSE truncating scalar|DIFFER 2492/2344|cvttss2si eax, xmm0; cvttss2si ebx, xmm0; cvttss2si edx, xmm0|
|round33_sse_scalar_load|SSE nearest from actual float argument slot|DIFFER 2510/2344|cvtss2si eax, xmm0; cvtss2si ebx, xmm0; cvtss2si edx, xmm0|
|round34_sse_broadcast_nearest|SSE broadcast then nearest low lane|DIFFER 2496/2344|cvtss2si eax, xmm0; cvtss2si ebx, xmm0; cvtss2si edx, xmm0|
|round35_sse_packed_nearest|MMX pair conversion with required state cleanup|DIFFER 2479/2344|cvtps2pi mm0, xmm0; emms |
|round36_sse_packed_trunc|MMX truncating pair with required state cleanup|DIFFER 2479/2344|cvttps2pi mm0, xmm0; emms |
|round37_sse2_double_nearest|SSE2 double nearest|DIFFER 2508/2344|cvtsd2si eax, xmm0; cvtsd2si ebx, xmm0; cvtsd2si edx, xmm0|
|round38_sse2_double_trunc|SSE2 double truncation|DIFFER 2508/2344|cvttsd2si eax, xmm0; cvttsd2si ebx, xmm0; cvttsd2si edx, xmm0|
|round39_sse2_packed_float|SSE2 packed float nearest plus integer extraction|DIFFER 2496/2344|cvtps2dq xmm0, xmm0|
|round40_sse2_packed_double|SSE2 packed double nearest plus integer extraction|DIFFER 2512/2344|cvtpd2dq xmm0, xmm0|

Additional flag diagnostic round41_signed_qifist uses the signed-long form with /QIfist; emitted qword stores are inspected separately. All source snapshots, compiler logs, object symbol tables, per-helper conversion instructions and target comparisons remain in immutable history.

ROUNDING_ORIGINAL_PROOF.json ties original PDB globals to complete actual donor routines: Near018C4020 and Fast02231910. Both perform the same bare dword integer-store operation seen natively. Existing shared headers provide their portable twins; ROUNDING_TWIN_CHECK.json records dual-path and independent binary32-oracle checks. The canonical runner is absent from the package, so a documented standalone equivalent is supplied at tests/portable/portable_twins_check.py.
