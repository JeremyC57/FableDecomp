# START HERE — Fable pass13 return

This is the complete local-work checkpoint for the100 rows reserved in the supplied pass13 package. Work stopped for handoff at the user's request after the workers hit usage limits. No commit, push or remote repository change was made.

## Final result

- **51/100 independently verified matches (51%).** These represent116,243 of253,943 assigned retail bytes. Every accepted row has a real VC7.1 RTM cl13.10.3077 grade, exact Leaf, native extent and relocation/name review, and matching-source behavior PASS receipts on GNU C++03 and native VC7.1.
- **3 independently reviewed parked rows.** Passing behavior tests do not make them byte matches.
- **23 in-progress rows.** Their selected eligible or worker-selected nonmatching source and test are included. Some workers called them parked; main's final integration/review was not finished. They are not accepted matches and do not all have an audited minimum of10 eligible source families.
- **2 blocked rows:**0066b370 has unresolved original sound field/interface naming and no authored candidate;00db1f00 has a wrong-layout diagnostic and an unapproved alignment/packing proposal, with no behavior test.
- **2 bad assigned extents:**00ce6cf0 combines Init16 bytes and the next DeclareGossipCategories2221 bytes;00501d30 omits one final compiler INT3. Diagnostic corrected extents match, but are excluded from51.
- **19 rows without a compiled candidate.** See UNSTARTED_ROWS.tsv. Preliminary naming research for00ef9280 is preserved; an assignment or research read is not a completed attempt.

## What to verify and land

1. Run `python3 tools/verify_archive.py` to validate SHA256SUMS and the100-row inventory.
2. Read STATUS.tsv and ROW_DETAILS.md. Only VERIFIED_MATCH rows are submitted for landing. Review every name/callee/header and regrade/retest them using the owner's current policy before landing.
3. Per selected row, restore its exact tested headers with `python3 tools/restore_headers.py --row ADDRESS --output /your/empty/include-directory`. Compile rows/ADDRESS/ADDRESS.cpp under /O2 /Oy /W3 plus its recorded extra flags. Use your private matching retail executable; no game files or bytes are supplied.
4. The standalone test is rows/ADDRESS/ADDRESS_test.cpp. Historic actual build/run logs and hash-linked receipts are included under behavior_tests/ and research/workers/parked_expert/behavior_tests/. Tests marked NOT_RUN must not be presented as passing.
5. Review NAME_FIXES.tsv, original TPI/public/local-name evidence, per-row NAMING_EVIDENCE.md and reviews/. Catalog/manifest labels are only hints. All136 proposed address corrections are evidence-backed work notes; review them against your newer settled names before applying them.

## Continue unfinished rows

- 0050a650 CWorldMap::GetMapNavigationAreaInit: eligible expert28/main29 has2233/2233 bytes with exactly4 unmasked byte differences in final empty-path cleanup/register selection.
- 00ecd9e0 CSickChildsMother::Main: eligible expert30/main34 has2679/2679 bytes with exactly6 differences in two parent-resource address calculations. This improved private version is the canonical source in this archive, superseding the older root19.
- 00ecbdf0 CTalkingTrader1::Main:2948/2928; timer destruction expands at the wrong exit. Main17-19 added scope/return/definition-order probes without improvement; selected16 retained.
- 00500540 LoadRegion: corrected implicit CLevelFiles members in expert37, reproduced by selective engine integration39, replace ineligible older20. This is still2396/2412 and needs review; do not revive authored compiler-generated special members.
- The latest partial hard_batch6 expert work on00d21b40/00d19bd0/00cf0d20 is preserved as research. Its speculative CBaseObjectPointer/body context was not accepted. Keep the selected reviewed versions until the new evidence is complete.

Read PROBES.md in each row and history/ grade receipts before new attempts. The raw grade count includes repairs, flag diagnostics, repeated confirmations and ineligible experiments; it is NOT the number of genuinely different eligible strategies. Do not claim every row had ten attempts. Keep going where a real new approach is supported, without resurrecting disproven generated-member bodies or guessed names.

## Naming and policy details

The exact final function symbols are in STATUS.tsv; per-row names come from original PDB names/type records plus native callers/factories/vtables/literals and donor evidence. Script owner/child namespaces, access, reference members and data layouts were recovered and checked. Where an original inline keyword is unproven, the original body evidence and conditional stronger-expansion choice are explicitly documented; do not describe those keywords as recovered source.

Compiler-generated special members stay implicit or original external declarations. Historical authored generated-member experiments are retained only as clearly nonselected research.00db1f00 has no approval to activate new packing. The older pass12 exception does not grant one here.

## Archive layout

- rows/: all100 reservations, selected C++/test where available, final ROW.json, merged attempt/naming notes and original assignment notes.
- hdr/: current integrated headers plus hash-deduplicated exact selected/historical contexts.
- history/: every recorded graded candidate, compiler/symbol/relocation logs, strategy/result and header manifest.
- reviews/, evidence/, research/: main/worker findings, naming/layout proof, superseded versions, interrupted work and diagnostics. These directories contain historical status claims; STATUS.tsv and ROW.json override them.
- SHA256SUMS, CONTENTS.tsv, ARCHIVE_VALIDATION.json: integrity and inventory.
- FINAL_ACCEPTED_AUDIT.json: all51 selected objects independently re-extracted and compared with retail again during packaging; source/test hashes and passing receipts reconfirmed.

No Wine, compiler, PDB, game file, raw retail/object bytes, binary outputs or repository history is included. Native instruction listings and raw proof payloads were omitted; explanatory residues, names, offsets, hashes and original-type evidence remain. Source/header/test bytes are exact. Documentation paths and external links are normalized for portability. Compiler outputs can be reproduced locally with the owner's tools. Historical Python research scripts retain their original local-workspace assumptions and may need path edits; the two tools/ entry points are portable.


## Lossless archive compaction

Repeated historical research headers are stored once by hash under hdr/objects; RESEARCH_HEADER_INDEX.tsv preserves every original path and hash. Restore those paths with `python3 tools/restore_research_headers.py --output /your/empty/research-restore`. Canonical row headers still restore with tools/restore_headers.py. Historical overlay maps use checked deltas against hdr/CURRENT_MANIFEST.json; every original complete map was reconstructed and compared before packaging. No source, header version, test, attempt or finding was discarded by this compaction.
