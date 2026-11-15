#0x0077C4D0 source strategies and diagnostics

| Tag | Strategy/diagnostic | Result |
|---|---|---|
|01|Initial class/header assembly; compile diagnostic only|Missing real persistent-pointee macro fixed before grading.|
|01b|Full original draw on real shared types, signed frame arithmetic|DIFFER605/611; loose0.95418. Alpha instruction and interpolation/frame residues.|
|02|Original Interpolate forwarding chain and unsigned long alpha|DIFFER617/611; unsigned conversion adds wrong sign correction; chain itself unchanged.|
|03|Named interpolation return; explicit float alpha conversion|DIFFER537/611; named return makes getter called, contrary to native expansion.|
|04|Narrow byte alpha|DIFFER605/611, wrong type/dataflow (later original PDB proves long alpha).|
|05|Original module BPREL locals/types, named pos, graphic reference and repeated alpha getter|DIFFER605/611; correct source structure but vector escapes opaque Sprite ctor.|
|06|Real Sprite constructor body visible and independently graded; original parameter order angle,size_x,size_y|DIFFER609/611, strict0.991957; constructor142/142. Every vector instruction/frame slot now exact.|
|07/08/09/10|Explicit getter float/unsigned/double conversion and product operand order diagnostics|All609/611, byte-identical. These are normalized spelling diagnostics, not four independent source strategies.|
|11|Scoped named fade factor|609/611, unchanged.|
|12|Scoped source_alpha float conversion before multiplication|RELOCATION_MATCH611/611 and constructor142/142, no flags. Native FILD+FMULP preserved; marked Inferred.|
|13/14|Portable pragma guards, address/inference comments; final source verification only|611/611;142/142. Not new source strategies.|

Eight substantive successfully compiled source strategies before match;15 compiler invocations total including the initial compile diagnostic and final metadata checks. No parking; no allocator/inline devices, no instruction helpers, no new packing. Each actual source/header/object/output history remains immutable under history/0077c4d0/sol2_*. Final selected sol2_14_final_inference_note.

Full standalone main and genuine constructor behavior test8640 combinations uses exact binary fraction inputs: face mode, orb component, additive flag, five transparency types, self lighting, fade enable, absent/existing primitive, alpha0/127/255, three interpolation values, three animation stages. Verifies position, frame truncation, width/height, native angle extrapolation, exact flag/blend/colour arguments, primitive creation/lifetime, update call, getter counts and unchanged sprite state. Real VC71/Od/W3/GR- without/EHsc and g++64C++03 both print0077c4d0_TEST PASS. Runtime/behavior_tests/SOL2_RESULTS.json records final test hash and successful runs.
