# 0x00978B90 std::__adjust_heap

Landed 2026-11-27 on FullDecomp-B under VC7.1 RTM cl 13.10.3077. Grade RELOCATION_MATCH, behaviour test `00978b90_TEST PASS`. Retail extent 142 bytes.

The body is the STLport 4.6 `__adjust_heap` for `CThing**` and the 16-byte anonymous-namespace comparator `CThingCompare_InDirectionOfAndNearestToHeuristicBitch`, the same spelling as the landed twin 0x0097AF70. A same-TU noinline `operator()` is what lets VC7.1 pipeline the 16-byte comparator copy. Callees: `operator()` 0x00978790 and `__push_heap` 0x00978A00.

PDB spelling, both tables: `??$_Adjust_heap@PAPAVCThing@@HPAV1@VCThingCompare_InDirectionOfAndNearestToHeuristicBitch@?A0xffab493a@@@std@@YIX...` (Ego_r 0x0099FF00, FableWin 0x02C176F0). That Dinkumware label is what the manifest already uses. The note for Account A is in `NAME_FIXES.tsv`. The unmatched length estimate was 192.

The verifier warns that `CThing`, `C2DVector`, and the comparator have shared headers. The local models are the ones that reproduce the pipelined copy, as on 0x0097AF70.
