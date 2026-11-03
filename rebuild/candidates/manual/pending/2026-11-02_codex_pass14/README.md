# Codex pass 14 reservation (2026-11-02)

100 never-attempted rows (RESERVED.tsv: pool chunks NA097-NA106 of `pending/never_attempted/CHUNKS.tsv`, 1044-3962 bytes, 200331 bytes in all)
packaged for an outside ChatGPT Codex pass at the owner's request, one of three sections (passes 12-14) handed out in turn as Codex
finishes the previous one: `fabledecomp_codex_pass14_100_2026-11-02.zip` (scratch only, `$S/codexpass/`, built by
`$S/codexpass/build.sh 14 NA097 NA106` with the pass 11 row packer). Same contents and brief as pass 11
(`../2026-11-02_codex_pass11/README.md`) with the headers, bindings, name queue and GOTCHAS refreshed after chunks 713 / 714 and the
lessons of chunks 709-714 added. No retail bytes (checked: 0 of 200 sampled 24-byte row windows found in the package), no links.
Reserved until 2026-11-30: in-house chunk cuts skip NA097-NA106. Returned rows are regraded with vland.py and every bound symbol checked
against the PDB name sets before landing (precedent: `batches/2026-10-31_chatgpt_pass10/README.md`).

**Released 2026-11-03 (owner, with the pass 13 return):** pass 14 was never sent to Codex. `RESERVED.tsv` is renamed
`RELEASED.tsv` so the reservation glob (`pending/*/RESERVED.tsv`, read by `analyze_never.py`) no longer holds NA097-NA106 back;
the 100 rows go to in-house chunk cuts.
