# Family pass 8 (external ChatGPT session, landed 2026-10-28): 914 of 979 rows landed

Pass 8 (chunk 957: 358 shape families, N001-N358, 979 rows) was reserved for an external ChatGPT session and handed to
it as a package with no game, no retail bytes and no oracle (`ChatGPT` cut its own oracle from its copy of the retail
exe, SHA-1 39fd416ba23a91a89713ec80c789f57ddb22e2c8). It reported 917 verified matches (912 C++, 5 retail_asm), 43
parked, 9 review holds and 10 ABI blockers, at most four source grades per row (HANDOFF_FROM_CHATGPT.md, SUMMARY.json).

Coordinator checks before landing:
- The delivery's 29,063 SHA256SUMS entries and its own `verify_package.py` (read first, run with `python3 -I`) pass.
- No game files, retail binaries, oracle bytes or links in the delivery; its 122 bundled headers are byte-identical to
  rebuild/include.
- A fresh oracle cut from our Fable.exe with our cutter (byte-identical to the one shipped), and an independent dry
  `vland.py` regrade on our lane: 912/912 main rows (157 MATCH, 743 RELOCATION_MATCH, 12 RELOCATION_MATCH[/EHsc]) and
  5/5 retail_asm rows PASS; no __asm / _emit / naked / volatile in the main payload.
- RELOC_CONFLICT: 13 rows bind the compiler-local static-initialiser symbol `_$E2` to their own targets; it is a
  per-translation-unit local, not a shared external symbol, so these land. Three rows (0x0093CFE0, 0x00959EE0,
  0x0095A270) call 0x006A4D00 as `?GetPosition@CThingCreatureBase@@QBEABVC3DVector@@XZ`, a spelling landed 0x008E07F0
  binds to 0x004C73D0. Retail 0x004C73D0 and 0x006A4D00 are byte-identical (`mov eax,[ecx+60h] ; add eax,0Ch ; ret`),
  so two source functions; neither PDB has that spelling (only `?GetPos@CThing@@QBEABVC3DVector@@XZ`). The three are
  held in pending/2026-10-28_chatgpt_pass8/held/ until cleanup pass 1 settles the pair.
- Three rows (0x009B6E00, 0x009B7B10, 0x00A39A00) carry a same-TU noinline body for STLport's real `_M_range_insert`
  (a known landed pattern, GOTCHAS), not an invented function.

Landed with `vland.py --land`: 909 main rows and 5 retail_asm rows (payload.json, payload_retail_asm.json; verify.log).
oracle.tsv holds the landed rows only. Pending (pending/2026-10-28_chatgpt_pass8/): the 62 unresolved rows' selected
sources and tests (differ/, STATUS.tsv: 43 PARKED, 9 REVIEW_HELD, 10 BLOCKED; four blockers have no source or test by
design) and the 3 held rows. Proven names: ghidra_out/labels_chatgpt_pass8_2026-10-28.tsv (382 rows whose
NAME_RECOVERY.md identity line says proven). ChatGPT's notes, per-row details, name evidence, lessons and next actions are
kept here; its 210 MB of attempt history, receipts and transcripts stay out of the repo.
