# 00ec4a00 follow-up (p25B, 2026-11-20): complete the `// Inline-budget:` marker

The landed row's line 2 named only the scoped `inline_depth(0)` specialization. The coordinator's landing gate now asks the marker to
name every inline device the ablation shows is needed, header FORCEINLINE_* opt-ins included. Line 2 now also names (grades from the
row's ablation/, batches/00ec4a00/README.md):
- FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR: without it DIFFER(1532v1540);
- FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY: without it DIFFER(1524v1540);
- FABLE_RETAIL_CDEFSTRING_OUTOFLINE_GETSTRING: without it DIFFER(1556v1540).

Only line 2 changes (src.diff against the landed file; the landed file also lacks the final newline). Code is unchanged.
Strict vland `--recheck-landed` (FABLE_HDR_OVERLAY=hdr/): `RELOCATION_MATCH PASS` (vland.log). The test is the landed one, unchanged.
