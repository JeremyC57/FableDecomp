# NF242 header overlay (2026-11-22)

## engine/NCreatureMode_CModeSitFloor.h (NF240's held q12674 opt-in, carried unchanged into q12677)
- Base: the repo working tree (`orig/engine/NCreatureMode_CModeSitFloor.h`, byte-identical to the tree today and to NF240's orig/).
- The overlay is NF240's hand-back copy (`pending/2026-11-22_nf240_handback/hdr/engine/`), unchanged: the existing line
  `virtual ~CModeSitBench();` (inside the PASS27 class) sits under `#ifndef FABLE_RETAIL_NCREATUREMODE_CMODESITBENCH_IMPLICIT_DTOR`;
  with the macro defined the class has no user-declared destructor (the implicit one is still the virtual
  ??1CModeSitBench@NCreatureMode@@UAE@XZ, FableWin.pdb 0x024B9E50), so the vtable emitter's ??_G expands ~Filter + ~CModeSitBase in
  place as retail 0x007400E0 does (`call 0x00739DB0 ; call 0x008D71D0`). Used by row 0x00740040 and its new test only. No layout change.
- Default path token-identical. pin_asserts (repo tree v hdr/): pinned 0.
- Includers today: 22 landed rows (sources / tests; the same 22 as NF240's); no header includes it. None defines the new macro.
- Equivalence rerun today: equiv_check (repo include tree v a copy with this header) over the 22 rows: 0 differing
  (work/equiv1.tsv empty, work/equiv1.log); assert_names_check (repo tree v that copy): 0 mismatches.
- NF241's overlay (`agentNF241/hdr/`) holds only stlport/fable_containers.h: no overlap.

## No other header changes
- The nine slot-swap rows use the existing `FABLE_RETAIL_CMODESTANDBASE_PASS22_VTABLE` opt-in of engine/CModeStandBase.h (seven rows
  define it, as the landed 0x00743600 / 0x008E3A20 do); the header itself is not touched.

