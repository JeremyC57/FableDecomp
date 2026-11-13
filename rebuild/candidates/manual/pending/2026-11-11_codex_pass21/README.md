# Codex pass 21 (2026-11-11)

The return is in return/. 26 of its 40 rows landed in landing cr (batches/2026-11-11_landing_cr/), and 14 are parked in
return/parked/. Its name fixes are queued as land1011cr. RESERVED.tsv stays in place, so local agents keep skipping the 14 parked
rows until they are released.

**Released 2026-11-13 (owner):** the Codex work for this pass has been handed back. `RESERVED.tsv` is renamed `RELEASED.tsv`, so the
reservation glob (`pending/*/RESERVED.tsv`) no longer holds these rows back. The rows still unlanded go to the normal parked pool with
their closest version on file: `../2026-11-13_codex_released/` (STATUS.tsv, differ/).
