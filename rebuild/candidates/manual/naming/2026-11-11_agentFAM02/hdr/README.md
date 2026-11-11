# agentFAM02 header overlay

`stlport/fable_containers.h` -- full copy of the CURRENT repo header with one `#ifdef` opt-in added; the default path is
byte-identical (diff = two guarded blocks only).

- `FABLE_RETAIL_STLP_UNINIT_FILL_N_FORCEINLINE` (new): the non-POD `__uninitialized_fill_n(first, n, x, const __false_type&)`
  becomes `FABLE_FORCEINLINE` (and `fable_compiler.h` is included at the top for the macro). Retail /Os vector TUs keep the public
  `uninitialized_fill_n(first, n, x)` COMDAT out of line (`ret 4`) with the `__false_type` helper expanded inside it (one loop, no
  call); with plain `inline` under `#pragma optimize("s", on)` VC7.1 emits a call to an out-of-line `__uninitialized_fill_n`
  (19-byte body instead of 35..41). EXTRA_RULES already notes "under /Os put __forceinline on ... the uninitialized/fill helpers".
  Used by 11 rows: 0x00469FA5 0x0046B2E9 0x0046C5D5 0x00470BCD 0x00476001 0x004768F8 0x00476D9D 0x0047725F 0x00478104
  0x0047A2CF 0x004EE219 (all graded with
  `FABLE_HDR_OVERLAY=$S/w22/agentFAM02/hdr`).

Every other row grades against the repo headers unchanged (the overlay's default path is the repo's).
