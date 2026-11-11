# Start here, Claude: Pass20 handback (2026-11-11)

This is the complete current handback for the40 reserved functions in the owner's Pass20 package. No other rows were authored. No Git commands, commits or pushes were performed. The owner's original input archive remains the baseline.

## Result and what to land

**34/40 functions (85%) have verified relocation-masked byte matches and passing behavior tests.** Thirty match the supplied extent; four match corrected function extents. Six remain parked; none are untouched. All40 selected files were freshly recompiled with MSVC7.1 RTM13.10.3077 against the single returned header overlay. All40 standalone tests passed both that compiler (/Od /W3 /GR-) and64-bit g++03. Test cases exercise stubs and local behavior, not a full game integration run.

Matched function bytes: 17696/21137 (83.72%) across corrected extents for these40 rows. This is a local-pass statistic, not the project's overall completion percentage. The input README's overall baseline was55,612/57,835 functions; we have not changed the repository or recomputed its total.

STATUS.tsv uses RELOCATION_MATCH for30 normal-span matches and EXTENT_WRONG for5 corrections. Its additional function_status column identifies the4 corrected matches and1 corrected park. Do not count every EXTENT_WRONG row as matched or blocked. No exact raw call-address equality is claimed for relocatable objects: operands were audited separately.

1. Apply hdr/ as an overlay on this Pass20 input/include (168 headers). Do not replace the whole include tree. Read hdr/README.md and verification/MERGE_NOTES.md. The original default paths were checked in40 preprocessing graphs with55 new opt-ins disabled. Selected preprocessing tokens also agree for all40 rows. Header options remain per-row; do not enable all options globally.
2. Compile rows/<addr>/<addr>.cpp using /O2 /Oy /W3 plus its // Flags line, exact RTM compiler, no /Zc:wchar_t. Each // Leaf identifies the emitted target, with the normal compiler-generated anonymous-namespace hash caveat below. Check symbols with objdump -t and audit each relocation, including CRT array/scalar distinctions, virtual slots and branch destinations.
3. Re-run each self-contained test with VC71 /Od /W3 /GR- and g++ -std=c++03. Expected output is exactly <addr>_TEST PASS. A passing behavioral test does not turn a parked byte mismatch into a match.
4. Read each NAMING_EVIDENCE.md and the consolidated NAME_FIXES.tsv (176 addresses). Correct erroneous existing bindings when landing, rather than copying an old wrong alias. Every label is supplied in LABELS.tsv. Any descriptive name or incomplete inference is explicitly qualified in its row evidence.
5. Land the34 matching function bodies after your own validation. Four require the documented extent corrections. Keep all6 parks as work-in-progress; their selected .cpp is the closest retained version, not a submission claimed to match. PROBES.md records the source strategies and outcomes; GRADE_HISTORY.json indexes every saved compiler check, including failed builds, budget probes and final rechecks. Compiler checks are not all distinct authoring attempts.

## Corrected extents

| Address | Assigned | Function | Result |
|---|---:|---:|---|
|006d18a0|1344|1330|MATCH;14 trailing alignment bytes|
|00785620|1312|1301|MATCH;11 trailing alignment bytes|
|007677e0|1472|1467|MATCH;5 trailing alignment bytes|
|0076efa0|1888|1312|MATCH;following span contains24 other entrypoints plus padding; do not append them|
|007f6200|1472|1458|PARKED;14 trailing alignment bytes|

## Remaining work

- 0x004b2890: 569/569,189/189 instructions;4hunks: quest/list arguments use ecx/eax instead of edx/ecx, interface key address uses edx instead of eax. All20external operands correct;141byte GetAcceptedBoastsForScript and72byte list clear helpers exact.
- 0x0067e290: 10 strict instruction hunks; enum EAX/EDX swap begins+5b, then bool temporary rotation and sort operand order; same273bytes/102instructions
- 0x007f6200: parent/owner spill14/18 inverted; comparator store/call scheduling, filter temporaries and later parent caching;36 strict hunks
- 0x00919240: entry only: Creature load before filter179 initialization/vector zero stores; all subsequent instructions/operands and17branches exact
- 0x00a55f90: xy/xz commutative loads reversed; terminal three zero stores have different interleaving with final x87 subtract/store/pop
- 0x00bcf4f0: one adjacent independent load/store pair swapped at tail: retail loads StartPosition.Z before EndPosition.Y store; ours stores Y before loading Z.101/101instructions,99.01%strict,2hunks.

All6 parks have at least10 distinct source-strategy families documented; some saved variant counts are larger. Matches found earlier were not needlessly subjected to10 changes. Do not reset attempts or repeat discarded strategies without a new reason. No assigned row is untouched.

## Audit limitations and naming details

Final integration compared each selected leaf's raw object bytes and normalized relocation names/offsets/types to its independently reviewed object, and compared every previously emitted .text helper body and its relocations. All40 comparisons passed; the34 matched leaves remained matches. Branch/vtable displacement bytes are not masked. Prior whole-helper byte matches remain valid where individually claimed in row reviews. Other emitted genuine helper bodies are dependencies, not additional matched submissions: for0076efa0, the selected GetSubDef and list clear COMDATs have different TU codegen from native standalone copies. Do not land them as extra rows.

Anonymous-namespace hash components are generated from the TU path and cannot be recovered as exact source spelling. NAME_FIXES uses ?A<TU>@ as metadata notation only, never a literal declaration or mangled lookup. FINAL_GRADE records each actual emitted leaf. Preserve the original proven namespace/signature and review the target identity; do not treat two compiler hash strings as two semantic functions. For the trail module globals, original unqualified names/types and native bindings are proven; anonymous transport namespace is inferred and is disclosed in NAMING_EVIDENCE. GameDefinitionManager's PGlobalDefD0 field is explicitly descriptive where the input provides only an opaque manager declaration.

The complete selected sources, tests and corrected headers are included. Old failed candidate sources and binary/disassembly evidence stay local; their strategies, selected hashes, grade outcomes and remaining residues are preserved in PROBES/GRADE_HISTORY. Retail bytes, game files, compiler/Wine, object files, test executables and external links are excluded. SHA256SUMS covers every payload file except itself. Verify it before landing.
