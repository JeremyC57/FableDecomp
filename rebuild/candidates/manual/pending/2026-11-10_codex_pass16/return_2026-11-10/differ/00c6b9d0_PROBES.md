# Final handoff status: PARKED

Selected source: 245/269 bytes. Final RTM grade and both selected behavior-test runs are recorded in FINAL_GRADE.json and TEST_RESULT.json. Historical statements below do not override this final disposition.

# Current attempt history

Read all three prior files (including its fixture), both earlier source mechanisms and all six landed neighbors. The inherited draft embeds prohibited instructions and invented class names. The current row uses the real SDK CSalSwFilter::ProcessEQ with its nested PARAMS, fields fF/fQ/fG, m_fZ0/m_fZ1 and m_nFadeSamples. Original SDK debug calls sal_ftoi and clip16, not the old guessed FloatToLong names.

1. Initial shared-header body with the old external rounding helper:254/269. Superseded after SDK identification; not selected and not considered a settled helper name.

| Probe | Bytes | Result and reason |
|---|---:|---|
|002_integer_cast|248|Direct integer cast: correct ordinary truncation, but does not preserve original exact-half quirks; compiler cannot emit the native round-near instruction.|
|003_shifted_cast|259|Original subtraction/negative increment around integer cast; wrong rounding mode at the cast.|
|004_floor_path|276|Floor helper changes rounding direction; rejects on boundary semantics.|
|005_ceil_path|276|Ceil helper changes rounding direction; rejects on boundary semantics.|
|006_floor_ties_even|245|Faithful C++ nearest-even via floor/fraction/parity; correct default-state behavior, much larger than native.|
|007_trunc_ties_even|245|Faithful C++ nearest-even using signed truncation and parity; avoids floor but still ordinary conversion call.|
|008_absolute_ties_even|336|Faithful C++ nearest-even through absolute magnitude; distinct sign/control-flow shape.|
|009_sse_nearest|270|Scalar SSE nearest conversion; correct default rounding but different instruction family and MXCSR state.|
|010_sse2_nearest|270|SSE2 double conversion; still not the native x87 instruction.|
|011_mmx_nearest|245|Packed conversion via MMX; extra vector operation/state cleanup, not native.|

Selected007_trunc_ties_even is fully portable nearest-even followed by the original negative-result increment. It produces245 bytes for the target because the real compiler keeps its larger sal_ftoi body out of line. The target is NOT byte matched. Inlining or changing that helper cannot erase the fundamental instruction-family difference: retail00c6baa7 performs a bare x87 32-bit integer store under the x87 rounding mode. Normal casts call the runtime conversion; scalar SSE and MMX use other opcodes and state. Header-owned assembly from older rounds is not copied into this submission under the current explicit no-assembly-anywhere rule.

The intrinsic helper objects retain their own bodies where the compiler does not inline them. A low target hunk count there does not mean the conversion matched. None is accepted on diagnostic instruction similarity.

Behavior:8,193 exact fractional rounding inputs and348 filter/ramp cases. Controlled ProcessEQFade checks requested prefix and commit logic; it does not reimplement the separate native ramp kernel. Domain assumes valid finite audio intermediates in signed-int range, matching the SDK debug assertions and default nearest rounding.

## Main-only continuation (latest)
Selected portable body unchanged. /QIfist was now checked at this exact site:264/269, and the compiler emits fistp qword rather than the native dword conversion. It is a diagnostic, not an extra distinct source family or an accepted match.

Each saved build below is a reproducible trial, not necessarily a new independent strategy. Corrections and flag diagnostics are explicitly excluded from the source-family total.

| Trial | Grade | Bytes | Strict hunks |
|---|---|---|---|
| 101_qifist_integer | DIFFER | 264/269 | 9 |
