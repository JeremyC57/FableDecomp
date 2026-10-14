# Family landing (shape templates)

Lands hundreds of byte-identical small bodies at once from one parametrised, commented source per
instruction skeleton. Used on 2026-10-13 in the Linux/Wine lane (`docs/journal/2026-10/LINUX_CONTAINER_LANE_2026-10-13.md`).

All three scripts read the scratch layout through `S` (the session scratchpad): the shipped
`Fable.exe` at `$S/dl/game/Fable The Lost Chapters/Fable.exe` (never committed) and
`$S/batch/families.json`.

1. `famscan.py <maxbytes>` — groups every un-landed manifest body of ≤ maxbytes (trimmed with
   `crawl/rowtrim.py`, trailing `CC`/`90` stripped) by opcode skeleton → `$S/batch/families.json`
   (list of `(skeleton, [(addr, module, name, len, hex)...])`, largest family first).
2. `fam.py <repo> <family-index|tiny> <batch-name> [limit|-] [template-id]` (`FAMFILE` env selects the scan file; the template id is the family index of the scan the template was written for, default = the family index) — writes
   `rebuild/candidates/manual/batches/<batch-name>/{payload.json, oracle.tsv, *.cpp, *_test.cpp}`
   for one family (templates keyed by family index; aliases share a template when the lander sweep
   covers the scheduling difference). Then `verify_and_land.py <batch>/payload.json <batch>/oracle.tsv --land`.
3. `famfinal.py <repo> [extra lane logs]` — after landing: rewrites the `Grade:` header of landed
   rows (batch file + `rebuild/src/compiled` copy), marks the rest with the lane verdict, ledgers
   DIFFERs in `crawl/gen_tried.txt`, writes one README per family batch.

Naming/trust rules (enforced in `fam.py`): class from `rebuild/manifest/rtti-vtables.tsv` (leaf of a
namespaced name); vtables without RTTI → `CVtable_<va>`; base/member constructor labels only when the
(module, name) is unique in the manifest, else `CBase_<va>`; by-value enums → `long`; template or
unknown argument types → `void*`. Open shapes: F15, F73, F148 (`docs/journal/FINDINGS_LOG.md`).

Scans used so far: `families.json` (64-byte cap, templates keyed by family index, batches `_f<N>`),
`families128.json` (128-byte cap, template ids `w*`, batches `_g<N>`), `families256.json` (256-byte cap,
template ids `w256_*`, batches `_h<N>`). The special family `tiny` collects every body of 16 bytes or
less for the generic `wtiny` synthesiser.

## EH unwind funclets (`funclets.py`, added 2026-10-14)

VC7.1 emits the unwind funclets of an EH-framed function (`push -1 ; push offset __ehhandler`)
together with its `__ehhandler$` thunk as one separate `.text` contribution headed by a `$L<n>`
label; the linker put Fable's blocks at `0x0121xxxx`, and Ghidra catalogues each funclet as its own
function (`Unwind@<va>`, 1,378 of them). They carry no source of their own: `funclets.py <repo>
<batch-name> [limit]` reads every landed `/EHsc` parent from `rebuild/build_candidates.ps1`, walks
the parent's retail `FuncInfo` (handler thunk → `mov eax, offset FuncInfo`; unwind map actions) and
writes one payload entry per funclet whose source is a one-line stub that `#include`s the parent's
landed file (path relative to `rebuild/include`), with `funclet_offset` (its offset in the block)
and `flags` (the parent's extra flags). `verify_and_land.py` compares `len(oracle)` bytes at that
offset of the object's `$L` block (relocation offsets shifted) and reuses the parent's behaviour
test. Re-run it after every wave that lands EH-framed parents; parents with try blocks
(`nTryBlocks > 0`) are skipped until catch funclets are modelled.

