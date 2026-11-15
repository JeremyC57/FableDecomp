# Original GFFastFloatToLong sites: main independent ladder

Both sites were tested in every full-target compile: creation time00B0CBC8/00B0CBCC/00B0CBD0 and angle change00B0D308/00B0D30C/00B0D310. The original sequences are fld local, mov edx result-pointer, fistp dword[edx]. Original fastcall helper and its f/i/i_ptr locals are independently proved from PDB/donor body; worker helper_probe/PROOF.json reproduces all29 donor bytes.

These are diagnostic compiler-lowering probes, not40 new semantic source strategies. Several intentionally use truncation, restricted-domain arithmetic, or alternate FPU control domains; they are rejected, not claimed behavior-equivalent. Each probes both real target sites using the same frozen original final method and changes only the shared rounding routine. Intrinsic trials use/arch:SSE2. All sources, headers, compiler logs, object results and called symbols remain preserved.

|Trial|Kind|Hypothesis|Whole function bytes|Result|
|---|---|---|---:|---|
|main_round01_signed_long|cpp|truncates instead of nearest|2129|DIFFER|
|main_round02_signed_int|cpp|different integer destination type|2129|DIFFER|
|main_round03_unsigned_long|cpp|unsigned conversion lowering|2129|DIFFER|
|main_round04_unsigned_int|cpp|unsigned32 conversion lowering|2129|DIFFER|
|main_round05_signed_short|cpp|narrow signed conversion lowering|2129|DIFFER|
|main_round06_unsigned_short|cpp|narrow unsigned conversion lowering|2129|DIFFER|
|main_round07_signed_char|cpp|narrow byte conversion lowering|2129|DIFFER|
|main_round08_double_input|cpp|double precision conversion input|2129|DIFFER|
|main_round09_long_double_input|cpp|long-double type lowering|2129|DIFFER|
|main_round10_signed64_intermediate|cpp|64-bit intermediate conversion|2129|DIFFER|
|main_round11_unsigned64_intermediate|cpp|unsigned64 intermediate conversion|2129|DIFFER|
|main_round12_float_output_local|cpp|explicit float local lifetime|2129|DIFFER|
|main_round13_addressed_integer_output|cpp|original pointer-out shape without special instruction|2129|DIFFER|
|main_round14_floor|cpp|floor intrinsic/library lowering|2157|DIFFER|
|main_round15_ceil|cpp|ceil intrinsic/library lowering|2157|DIFFER|
|main_round16_floor_half|cpp|positive nearest ties-up|2169|DIFFER|
|main_round17_ceil_half|cpp|positive nearest ties-down|2169|DIFFER|
|main_round18_signed_half|cpp|nearest ties-away|2195|DIFFER|
|main_round19_truncate_remainder|cpp|nearest-even via truncated remainder|2315|DIFFER|
|main_round20_floor_parity|cpp|nearest-even via floor and parity|2277|DIFFER|
|main_round21_ceil_parity|cpp|nearest-even via ceil and parity|2277|DIFFER|
|main_round22_modf_parity|cpp|nearest-even via split integral/fractional parts|2353|DIFFER|
|main_round23_fmod_parity|cpp|floating parity instead of integer parity|2326|DIFFER|
|main_round24_absolute_even|cpp|round magnitude then restore sign|2310|DIFFER|
|main_round25_even_cell|cpp|nearest-even in width-two cells|2301|DIFFER|
|main_round26_magic_double_memcpy|cpp|rounded double mantissa extraction, little-endian finite-range diagnostic|2155|DIFFER|
|main_round27_magic_float_memcpy|cpp|rounded float mantissa extraction, bounded positive-domain diagnostic|2155|DIFFER|
|main_round28_binary_floor|cpp|bounded integer search avoids float-to-int conversion|2386|DIFFER|
|main_round29_ieee_float_bits|cpp|integer IEEE binary32 decode with nearest-even|2400|DIFFER|
|main_round30_guarded_portable_twin|cpp|full guarded established portable semantics|2459|DIFFER|
|main_round31_sse_scalar_nearest|intrinsic|SSE nearest scalar; MXCSR rounding|2283|DIFFER|
|main_round32_sse_scalar_trunc|intrinsic|SSE truncating scalar|2283|DIFFER|
|main_round33_sse_scalar_load|intrinsic|SSE nearest from actual float argument slot|2311|DIFFER|
|main_round34_sse_broadcast_nearest|intrinsic|SSE broadcast then nearest low lane|2295|DIFFER|
|main_round35_sse_packed_nearest|intrinsic|MMX pair conversion with required state cleanup|2337|DIFFER|
|main_round36_sse_packed_trunc|intrinsic|MMX truncating pair with required state cleanup|2337|DIFFER|
|main_round37_sse2_double_nearest|intrinsic|SSE2 double nearest|2289|DIFFER|
|main_round38_sse2_double_trunc|intrinsic|SSE2 double truncation|2289|DIFFER|
|main_round39_sse2_packed_float|intrinsic|SSE2 packed float nearest plus integer extraction|2291|DIFFER|
|main_round40_sse2_packed_double|intrinsic|SSE2 packed double nearest plus integer extraction|2297|DIFFER|

None of the30 C++ or10 intrinsic full-target trials match. Ordinary casts lower to __ftol2; intrinsic trials emit SSE/MMX conversions. Extra changes to root code from/arch:SSE2 are recorded, not attributed only to the conversions. No candidate supplied the native bare pointer-out dword conversion sequence at both real sites.

Macro rung main_round41_macro_direct:2191/2191bytes,565/565instructions,0masked residue. Existing named forced helper b11 also matches. Canonical ASM_RULES says use the first matching rung; selected main_review_03_macro_first therefore uses the shared GF_FAST_FLOAT_TO_LONG macro, documented under the original PDB routine GFFastFloatToLong, in the actual CParticle setters. Default helper/header behavior remains unchanged. The macro accesses the actual float and pointer locals directly and has a plain-C++ FABLE_PORTABLE twin.

No assembly text is present in row source or behavior test. Shared macro proof is specifically the original conversion instruction, never a scheduling workaround. Macro selection retains the same exact whole2191-byte target and original external symbols.

ROUNDING_TWIN_CHECK.json:65536binary32 inputs per nativeVC71,portableVC71,GCC64/O0 andGCC64/O2; all four outputs identical after newline normalization. Independent integer sign/exponent/significand oracle covers ties, signed zero, subnormals, range boundaries, infinities and quiet/signaling NaNs. Contract is nearest-even with masked invalid, signed32 indefinite for invalid input. Tests under tests/portable/rounding_twins.cpp; canonical checker absent from package, standalone runner review/main_00b0cad0/twin_check.py records commands/results.
