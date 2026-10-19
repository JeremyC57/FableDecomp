# Final progress and continuation map

Accepted: **34,648 / 49,568 functions (69.8999%)** on FullDecomp. Claude baseline `fbfaf9f90`: 34,098 (68.7903%); **+550 functions**, +1.1096 percentage points. Latest source acceptance: root197 `759d5ead9`; archive metadata identifies the later documentation commit.

Accepted native function bytes: **1,715,457**; unique address-span union is the same. Added since Claude: **127,128 bytes**. These are function oracle spans, not the entire executable or a byte-coverage percentage. RELOCATION_MATCH permits only audited relocation operands; it is not literal equality of those relocated values.

## Acceptance and test evidence

`ACCEPTED_FUNCTIONS.tsv` is the flat per-function table of all accepted addresses, native byte lengths, current source/fixture hashes, inherited Grade tags and review links.

`baseline_audit/accepted_function_index.jsonl` enumerates all 34,648 accepted addresses with native lengths/oracle-byte hashes, selected build entries, current source/fixture hashes and historical review records. `baseline_audit/added_since_claude.tsv` lists all 550 additions. Every added address has source, fixture and STATUS evidence. Oracle membership equals unique build-catalog addresses; all catalog source and fixture paths exist. No baseline address was removed.

The inherited34,098 baseline was not collectively recompiled or retested. Recorded tests are isolated fixtures with the scenarios and dependency spies stated in each review, not whole-game integration. Accepted history includes inherited assembly bakes, separately enumerated in baseline_audit/assembly_classification.json; the new additions have no detected asm/naked tokens in that lexical audit. Known no-elision and ABI limitations, withdrawals, overwritten fixture history and ambiguous names remain explicit in CLAUDE_HANDOFF.md. A general all-tests-pass claim would be incorrect.

## Frozen whole-chunk inventory

| Lane | Accepted now | Worker READY awaiting review | Differ / near | Parked / skipped | Other unresolved |
|---|---:|---:|---:|---:|---:|
| astra235_whole | 31 | 0 | 9 | 0 | 0 |
| astra237_whole | 26 | 0 | 3 | 0 | 11 |
| astra238_whole | 29 | 0 | 8 | 3 | 0 |
| astra239_whole | 29 | 6 | 5 | 0 | 0 |
| astra240_whole | 14 | 9 | 17 | 0 | 0 |
| astra241_whole | 10 | 5 | 21 | 0 | 4 |
| astra242_whole | 10 | 16 | 13 | 0 | 1 |
| luna232_whole | 18 | 0 | 22 | 0 | 0 |
| luna233_whole | 23 | 0 | 13 | 0 | 4 |
| luna234_whole | 22 | 0 | 13 | 0 | 5 |
| luna236_whole | 21 | 4 | 0 | 0 | 15 |

These are 440 assigned rows in 11 existing whole lanes, including older completed lanes. They are not all historical project work. The latest two236/242 workers are frozen; no replacement chunks or retry sections were started. Canonical accepted membership overrides stale worker labels. Raw worker inventories retain their historical source hashes and claims.

- `READY_FOR_MAIN_REVIEW.tsv`: 40 matching worker candidates not counted as accepted. Main must verify full native identities, signatures, relocations and exact-source fixtures before landing.
- `NEAR_PARKED_SKIPPED.tsv`: nonaccepted current whole rows with mismatch/near/parked/skipped/partial statuses. Raw records give attempts, best variants, lengths, test gaps and next ideas where recorded. Equal code length is not a byte-match percentage.
- `BLOCKED_UNSTARTED_AND_UNRESOLVED.tsv`: current whole rows with blockers, no-grade work, withdrawals or unresolved evidence. Research-only blockers preserve their unused budgets.
- `closeout_index/current_whole_row_details.tsv`: all440 assigned records with raw budgets/notes. `whole_chunk_review_and_retry_details.jsonl` includes all linked variants and status histories for unresolved current whole rows.
- `status_audit/per_row_index.jsonl` and `phase_index.json`: all indexed historical phases and exact paths to their prompts, ledgers, source snapshots, failures and handoffs. An evidence-file count is never an invocation count.

Current worker source/test hash claims checked: 351; mismatches requiring interpretation: 0 (full details in worker_inventory_hash_checks.json). This is an archive-integrity check, not an independent compiler/ABI/test acceptance.

## Not yet started and older unresolved work

The discovery manifest has 57,144 distinct addresses; the union with accepted/assigned addresses has 57,183. This is a different universe from Claude's49,568 metric. Do not subtract discovery worklist counts from49,568 or call discovery counts the remaining percentage.

| Conservative indexed state | Addresses |
|---|---:|
| ACCEPTED_CATALOG | 34,648 |
| ASSIGNMENT_OR_STRUCTURED_EVIDENCE_ONLY | 701 |
| CANONICAL_SOURCE_OUTSIDE_ACCEPTED_CATALOG_CHECK_LEGACY | 57 |
| CURRENT_WHOLE_PHASE_RECORD | 206 |
| HISTORICAL_STATUS_OR_PLACEHOLDER_REQUIRES_TRIAGE | 765 |
| NO_AUTHORED_WORK_FOUND_IN_SCANNED_EVIDENCE | 19,474 |
| SOURCE_ARTIFACT_PRESENT_REQUIRES_REVIEW | 1,331 |
| WITHDRAWN_NOT_ACCEPTED | 1 |

`closeout_index/no_authored_work_found.tsv` is the explicit next-start list: no address-bound authored source/test or structured status evidence was found in the stated scopes. This does not prove nobody ever researched the row; generated lift alone is not a byte-match attempt. Search any older unbound payload before assigning a fresh budget. `all_unaccepted_rows.tsv` lists every indexed unaccepted address, while `worked_or_recorded_unaccepted.tsv` separates those with existing work or records. `explicit_unstarted_or_ungraded_claims.tsv` preserves historical claims, which later work may supersede. The full JSONL includes actual source paths/hashes and phase records.

## Retry stop and attempt accounting

Retry202I ended with0 accepted /3 DIFFER,16 new source grades and12 UNCERTAIN reservations:820B80 uses4new+4reserved=8 (2remain),8F70F0 uses6+4=10 (cap),C461E0 uses6+4=10 (cap). All nine selected behavioral configurations pass, but none of these three sources byte-matches. Retry240J was cancelled before release, with0new grades; its proposal does not reset earlier counts or establish a worked section. See the detailed handoff for each numerical/codegen gap and immutable histories.

Do not invent a global attempt total from duplicated logs/snapshots or add four historical reservations as four proven invocations. Ordinary chunks have10 counted source/flag grades; separately authorized older escalation phases retain their own limits. Compiler failures count; failures before compiler invocation do not. Frozen inventories, every immutable snapshot and root amendments are preserved.

## Names and restore

`NAMING_RECOVERY.md` explains proven original identities/member spellings, corrected propagated names, native-versus-donor offsets, and unresolved mappings. `names_audit` covers all added addresses with evidence links; original local-variable names remain NOT_RECOVERED. Worker name claims remain distinct from root proof.

Read `CLAUDE_HANDOFF.md` before resuming. The full text archive includes tracked decompiled work and in-scope untracked evidence from all waves; a separate Git bundle preserves FullDecomp history. External game/compiler/Wine binary dependencies are excluded explicitly, so the text package alone cannot execute native tests. The manifest and seal report verify the archived payload; no remote push is claimed.
