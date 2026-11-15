# 0x00C41060 — CCoda3DSource::Create

Selected `sol05_final_latest09`: 621/621 bytes, 136 instructions, zero hunks, nine exact relocations. Five genuine strategies; six target compiler invocations (final regrade excluded), one layout build. Three behavior builds (GCC initial warning-producing storage projection, warning-free corrected fixture, VC); seven prior-row regression compiles are separate. No parking threshold was used after matching.

| Tag | Strategy | Result |
|---|---|---|
|sol00_original_3d_source|1: real owning class/data, native virtual interface, original Set/Zero helpers|616/621; SetError incorrectly expands and implicit base-vector copies differ.|
|sol01_original_vector_ctors|2: restore three genuine SDK vector constructors|616, unchanged; keep honest no-effect result.|
|sol02_original_assignment_body|3: reconstruct the original emitted 25-byte vector assignment body|637/621; vector copies and most constant/register roles now match.|
|sol03_external_error_boundary|4: declare existing SetError without its body in the new row-only profile|622/621; correct direct call boundary, only tail lifetime/order remains.|
|sol04_group_all_gain_defaults|5: place fMacroFx with the other four floating gain defaults|621/621 exact.|
|sol05_final_latest09|same5: final source metadata and latest main merged09 header union|621/621 exact; regrade excluded.|

The real SDK operator= body copies x/y/z sequentially and returns this. The initial implicit base-object copy model used different address/register webs; restoring constructors alone did not fix it. The exact genuine emitted assignment body fixed those webs. The source's external error call is an existing declared real function, not a new wrapper or noinline device. Grouping macro gain with the other gains ends the 1.0f value lifetime before the dynamic HF-reference load and restores the final byte.

No scheduler pragmas, flags, forced-inline additions, casts over layout, asm, padding or raw register tricks. All new API/body visibility is isolated by FABLE_RETAIL_PASS22_CCODA3D_CREATE; older definitions remain untouched when it is off. Immutable attempts retain every source/header snapshot.
