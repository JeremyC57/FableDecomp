# Original fade conversion, native00BD76A6

Thirty distinct C++ forms and ten intrinsic forms built with the realRTM compiler in this completeparent; all differ. Diagnostic semantic limitations are stated below. No forty-parent-strategy claim. Native site is the existing real GFFastFloatToLong helper inside original GetAlphaIncludingDistanceFade; no source instruction block introduced.

|#|Kind|Attempt|Bytes|Hypothesis/limitation|
|---|---|---|---|---|
|1|cpp|round01_signed_long|2097 /2116|truncates instead of nearest|
|2|cpp|round02_signed_int|2097 /2116|different integer destination type|
|3|cpp|round03_unsigned_long|2097 /2116|unsigned conversion lowering|
|4|cpp|round04_unsigned_int|2097 /2116|unsigned32 conversion lowering|
|5|cpp|round05_signed_short|2100 /2116|narrow signed conversion lowering|
|6|cpp|round06_unsigned_short|2100 /2116|narrow unsigned conversion lowering|
|7|cpp|round07_signed_char|2098 /2116|narrow byte conversion lowering|
|8|cpp|round08_double_input|2097 /2116|double precision conversion input|
|9|cpp|round09_long_double_input|2097 /2116|long-double type lowering|
|10|cpp|round10_signed64_intermediate|2097 /2116|64-bit intermediate conversion|
|11|cpp|round11_unsigned64_intermediate|2097 /2116|unsigned64 intermediate conversion|
|12|cpp|round12_float_output_local|2097 /2116|explicit float local lifetime|
|13|cpp|round13_addressed_integer_output|2095 /2116|original pointer-out shape without special instruction|
|14|cpp|round14_floor|2109 /2116|floor intrinsic/library lowering|
|15|cpp|round15_ceil|2109 /2116|ceil intrinsic/library lowering|
|16|cpp|round16_floor_half|2115 /2116|positive nearest ties-up|
|17|cpp|round17_ceil_half|2115 /2116|positive nearest ties-down|
|18|cpp|round18_signed_half|2130 /2116|nearest ties-away|
|19|cpp|round19_truncate_remainder|2193 /2116|nearest-even via truncated remainder|
|20|cpp|round20_floor_parity|2174 /2116|nearest-even via floor and parity|
|21|cpp|round21_ceil_parity|2172 /2116|nearest-even via ceil and parity|
|22|cpp|round22_modf_parity|2099 /2116|nearest-even via split integral/fractional parts|
|23|cpp|round23_fmod_parity|2099 /2116|floating parity instead of integer parity|
|24|cpp|round24_absolute_even|2191 /2116|round magnitude then restore sign|
|25|cpp|round25_even_cell|2189 /2116|nearest-even in width-two cells|
|26|cpp|round26_magic_double_memcpy|2103 /2116|rounded double mantissa extraction, little-endian finite-range diagnostic|
|27|cpp|round27_magic_float_memcpy|2103 /2116|rounded float mantissa extraction, bounded positive-domain diagnostic|
|28|cpp|round28_binary_floor|2101 /2116|bounded integer search avoids float-to-int conversion|
|29|cpp|round29_ieee_float_bits|2101 /2116|integer IEEE binary32 decode with nearest-even|
|30|cpp|round30_guarded_portable_twin|2101 /2116|full guarded established portable semantics|
|31|intrinsic|round31_sse_scalar_nearest|2252 /2116|SSE nearest scalar; MXCSR rounding|
|32|intrinsic|round32_sse_scalar_trunc|2252 /2116|SSE truncating scalar|
|33|intrinsic|round33_sse_scalar_load|2269 /2116|SSE nearest from actual float argument slot|
|34|intrinsic|round34_sse_broadcast_nearest|2256 /2116|SSE broadcast then nearest low lane|
|35|intrinsic|round35_sse_packed_nearest|2209 /2116|MMX pair conversion with required state cleanup|
|36|intrinsic|round36_sse_packed_trunc|2209 /2116|MMX truncating pair with required state cleanup|
|37|intrinsic|round37_sse2_double_nearest|2262 /2116|SSE2 double nearest|
|38|intrinsic|round38_sse2_double_trunc|2262 /2116|SSE2 double truncation|
|39|intrinsic|round39_sse2_packed_float|2256 /2116|SSE2 packed float nearest plus integer extraction|
|40|intrinsic|round40_sse2_packed_double|2269 /2116|SSE2 packed double nearest plus integer extraction|
