# Inventory and verification boundaries

`SELECTED/matched` is the only finished set. `SELECTED/parked` includes ordinary mismatches and source/build failures; equal length does not mean equal bytes. `SELECTED/byte_match_hold` has exact-byte histories without final acceptance. `SELECTED/naming_hold` contains historical accepted byte/test records withheld under the owner's stronger naming rules. `SELECTED/blocked` contains identity, ABI, manual-instruction, split-span and other blockers. `SELECTED/prepared` is authored but ungraded. `SELECTED/unstarted` has no pass9 candidate/native grade.

Source/test files in an unresolved selection are the worker's explicit selection when recoverable; otherwise the native snapshot with the closest length is used and the provenance is recorded. An associated test artifact is not automatically a test pass for that exact snapshot. Read its receipts and source hash. All alternative experiment sources and original worker tables are retained so the coordinator can choose another model. No unresolved row is silently promoted.

All matched rows have exact frozen source/test hashes and a final successful native and GNU32 receipt. The stage payload's readable name field was produced with the original Microsoft symbol undecorator; source/test bytes, compiler flags and exact mangled leaves are unchanged. `FINAL_VERIFICATION/MSVC_UNDECORATED_TARGET_NAMES.txt` records this metadata operation.

The final full recheck initially ran GNU nullable-new fixtures without `-fcheck-new` for00516010/0099f2e0/00ac2160. Those three host runs faulted because GNU assumes throwing allocation. The same unchanged sources/tests passed with the documented `-fcheck-new` adaptation and actual VC7.1. `FINAL_RECHECK_INDEX.json` selects the corrected receipts. Initial records are preserved rather than erased.

Native test fixtures exercise the reconstructed target and independently check behavior, state, lifetime, guards or call order as appropriate. External engine methods, data lookup, string ownership internals or hardware interfaces are often controlled dependencies; row-specific limits remain in ROW.json, fixture notes and worker closeouts. These are not whole-engine integration tests.00929650 excludes inexact-float deadline cases and explicitly asserts original vector byte alignment.

`WORKER_CLOSEOUTS/` contains original final inventories and hash-verification provenance. Their historical manifests can refer to binaries deliberately excluded from this work-only archive. Only the root `SHA256SUMS` is the manifest for the delivered archive itself. Checkpoints with older handover prose are historical; current acceptance is decided by root ROWS/payload and final receipts.

Root updates were scratch-local. "Accepted" means locally reviewed for this handoff, not catalog-landed. The original Claude package reported51,161/57,267 covered functions; this archive does not update that global catalog or add pending candidates to that number.

Original absolute workspace paths are provenance. Use `ARCHIVE_PATH_MAP.json`; S=/workspace/work/pass9 maps mostly to WORK/, while capped native experiments map to HISTORY/. Canonical portable current selections are in SELECTED/, src/ and test/. A missing old absolute path is not a missing implementation when its immutable native snapshot and portable selected copy are present.

Readable assigned_name labels omit specialization arguments; exact original mangled leaf and full Microsoft-undecorated signature preserve the complete typed specialization. Anonymous namespaces remain explicit in the original symbol/signature; their unnameable compiler hash is not invented as a source namespace.

Final source-file crosscheck found an ungraded proposal005954AD outside the per-row ROW metadata. It is now a separate prepared/identity-review entry rather than strictly untouched. Thus the delivered untouched list has1,523 rows, and there are two prepared/ungraded rows; all1,108 grade counts remain unchanged.
