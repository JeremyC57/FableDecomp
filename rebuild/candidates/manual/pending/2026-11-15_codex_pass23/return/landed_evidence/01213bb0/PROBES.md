# Parent source attempts for three generated rows

All three rows come from one448-byte CSegmentFile constructor. No cleanup code is authored by hand. Originalhand264 and recoveredhandQ9 drafts/notes retained; old443v448 was a local-model draft, not an original type definition.

1. main_01_shared_tree_string_range: genuine shared CString, PKCTRList and40-byte PKCTRRedblackTree replace local fake string/20-byte array/wrong-vtable models. No novtable.443/448, old branch-tail residue preserved.
2. main_02_full_failure_exits: give every failure its own range-clear call and return.449/448; ECX setup now in retail order, tail copies merge differently.
3. main_03_mixed_failure_exits: full returns only for signature/name/size failures; retain break after range/extent failures.454/448; desired two error paths appear in wrong order.
4. main_04_early_signature_mixed_exits: move invalid-signature earlyreturn before the loop; preserve mixed loop exits.448/448 masked exact, full40-byte generated cleanup group and52-byte EHmetadata exact. Only3disassembly-normalization differences are genuine ABS-zero FS relocations.
5. main_05_shared_native_construction_api: clarify chunkSize granularity parameter and add original mailbox/reader constructor declarations under the new opt-in for real-object tests.448/448 unchanged. Verification/control, not a separate source strategy.

Five graded builds, four source candidates; grouped into shared-type recovery and exit/control-flow families. Matching rows do not need ten speculative attempts. Final all_verified_headers_31 separately rebuilds each reserved row and all60accepted rows. Source and full immutable header history retained locally. No new inline devices or banned codegen keyword used.
