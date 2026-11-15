# Rounding-site diagnostics:0x00779F32

Native0x00779F2C..0x00779F49 is the complete original GFFloatToLongCeil sequence: add0.5, store signed32 with nearest-even rounding, convert back, subtract1, compare float bits and conditionally decrement the integer memory. Original FableWin OnInitialActivate calls ?GFFloatToLongCeil@@YIJM@Z at0x01F57744. Existing shared engine/GFFloatToLongCeil.h already supplies the isolated original helper and its portable twin. No helper or intrinsic was invented for this row.

These40 compiler diagnostics are distinct tested spellings, not40 independent full-row source strategies. They cover ordinary casts, explicit temporaries and storage, biased conversion, standard math, correction/guard logic, ordinary helper visibility and the available scalar/packed float/double SIMD conversions. Baseline flags/O2/Oy/W3; final/QIfist is a flag diagnostic, not another strategy. All sources, compiler logs and instruction output remain in private history/00779ef0/rounding_site_00779f32; no object or executable is a hand-back artifact.

| Diagnostic | Site spelling | Compiler result |
|---|---|---|
| 01 | long_cast | call 9; native bare dword store absent |
| 02 | int_cast | call 9; native bare dword store absent |
| 03 | short_cast | call 9; native bare dword store absent |
| 04 | named_float | call 9; native bare dword store absent |
| 05 | named_double | call 9; native bare dword store absent |
| 06 | float_half_bias | call 0xf; native bare dword store absent |
| 07 | double_half_bias | call 0xf; native bare dword store absent |
| 08 | standard_floor | call 0xf; call 0x17; native bare dword store absent |
| 09 | standard_ceil | call 0xf; call 0x17; native bare dword store absent |
| 10 | floor_then_increment | call 0xf; call 0x17; native bare dword store absent |
| 11 | ceil_float_result | call 0xf; call 0x17; native bare dword store absent |
| 12 | trunc_correct_up | call 0xa; native bare dword store absent |
| 13 | sign_split | call 0x1a; call 0x2c; native bare dword store absent |
| 14 | modf_fraction | call 0x18; call 0x36; call 0x4e; native bare dword store absent |
| 15 | fmod_remainder | fmod overload ambiguous; corrected to explicit(double) then compiled: no fistp dword; native bare dword store absent |
| 16 | floor_half_bias | call 0x15; call 0x1d; native bare dword store absent |
| 17 | ceil_double_parameter | call 0xf; call 0x17; native bare dword store absent |
| 18 | member_destination | call 9; native bare dword store absent |
| 19 | array_destination | call 9; native bare dword store absent |
| 20 | cast_through_int | call 9; native bare dword store absent |
| 21 | cast_through_short | call 9; native bare dword store absent |
| 22 | unsigned_cast | call 9; native bare dword store absent |
| 23 | wide_integer_cast | call 9; native bare dword store absent |
| 24 | subtract_half | call 0xf; native bare dword store absent |
| 25 | negative_floor | call 0x11; call 0x19; native bare dword store absent |
| 26 | guarded_ceil | call 0x2b; call 0x33; native bare dword store absent |
| 27 | binary_round | call 0x12; call 0x28; call 0x41; call 0x53; native bare dword store absent |
| 28 | helper_call | call 9; native bare dword store absent |
| 29 | inline_helper_call | call 9; native bare dword store absent |
| 30 | positive_integral_test | call 0x10; native bare dword store absent |
| 31 | sse_nearest_ss | cvtss2si ecx, xmm0; native bare dword store absent |
| 32 | sse_truncate_ss | cvttss2si ecx, xmm0; native bare dword store absent |
| 33 | sse2_nearest_sd | cvtsd2si eax, xmm0; native bare dword store absent |
| 34 | sse2_truncate_sd | cvttsd2si eax, xmm0; native bare dword store absent |
| 35 | sse_mmx_nearest | cvtps2pi mm0, xmm0; native bare dword store absent |
| 36 | sse_mmx_truncate | cvttps2pi mm0, xmm0; native bare dword store absent |
| 37 | sse2_packed_float_nearest | cvtps2dq xmm0, xmm0; native bare dword store absent |
| 38 | sse2_packed_float_truncate | cvttps2dq xmm0, xmm0; native bare dword store absent |
| 39 | sse2_packed_double_nearest | cvtpd2dq xmm0, xmm0; native bare dword store absent |
| 40 | sse2_packed_double_truncate | cvttpd2dq xmm0, xmm0; native bare dword store absent |

/QIfist on the direct long conversion generated fistp qword. All10 intrinsic spellings compiled and produced SSE/MMX conversions. No plain-C++ or intrinsic diagnostic produced the native bare32-bit store or full fix-up. The final row therefore calls the existing helper per ASM_RULES.

The original helper and portable twin were compared in00779ef0_rounding_test.cpp under real VC71/Od/W3/GR- without/EHsc, RC nearest-even and precision53, across fixed boundary cases and20000 random bit inputs (excluding the shared twin documented tiny-magnitude precision caveat). Both paths agree. The same test compiled and ran the portable twin under g++64C++03. Log history/00779ef0/dual_rounding_test/RESULT.json.
