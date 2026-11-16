# GetSeat compiler history

All immutable candidate sources, selected headers, disassemblies, hunks and compiler logs remain in history/009482a0. PROBE_INDEX.json enumerates all local results, including failed fixtures. The package baseline and four prior attempts were read in full before new work; the best draft is identical to prior handQ6.

- port01/02: replaced historical local classes with actual shared models and original signatures. Missing list operations / pointer-null comparison options initially prevented compilation; these are setup failures, not matching strategies.
- port03: correct genuine full body, 1075B/352 instructions. Remaining mismatches included ESP 8-byte alignment, out-of-line loop map lookup, cached RNG values and return scheduling.
- diag04: compiler sizeof/alignment probes established intelligent pointer 8/1, list4/4 and iterators4/4. Original TPI packed-property bit is clear in both donor builds.
- port05/06: isolated suppress-pack experiment tested whether existing pointer packing caused ESP alignment. The first attempt failed preprocessing, the corrected build emitted the SAME1075 bytes; hypothesis disproved. Experiment fully removed, no new packing rule selected.
- diag07/08: separate explicit/random path diagnostics localized ESP realignment to random path. Partial-body results were diagnostic only and not claimed as row matches.
- port09: genuine original user pointer copy chain and mutable private SetPointer(T*) plus existing real CVectorMap LowerBound body visibility removed stack realignment and restored RNG/return code.1072B/359 instructions; two residues remained.
- port10: the first successful branch uses a named ordinary smart pointer confined to its original temporary lifetime. This discards the pointer-constructor return instead of using EAX, restoring native LEA. Its2B size effect also restores loop alignment; all1072B and359instructions match.
- port11: original public CTCTavernTable(CThing&) and virtual destructor declarations added under NEW opt-in for normal fixture lifetimes. Production remains exact1072B.
- final01: rebased inherited overlay onto latest aggregate, preserving only own tavern lifetime delta. Exact1072/1072,359/359, no flags. Audit53 assigned plus5 existing-helper operands passes.

Native fixture setup failures are preserved: first call lacked sourced Wine environment; native02 needed CWorld model opt-in; native03 used invalid component-present/null-component combinations and correctly failed assertions. The fixture now preserves actual component invariants and separately varies absent components, dead objects and seat failure. Native04 passed4112cases; final05 adds null-pub coverage for4120 valid cases and passes; final06 pastes selected source verbatim into one self-contained file, shared include/ only, native/GCC4120PASS. GCC64 same immutable tree passes4120cases.

No parking: exact match achieved after distinct real-model, body-visibility and temporary-lifetime strategies. No assembly, volatile, register, new pack or new force-inline was selected.
