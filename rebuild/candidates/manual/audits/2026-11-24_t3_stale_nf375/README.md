# T3 queue stale-line sweep v2 (agent NF375, 2026-11-24)

Second sweep after NF342's (`../2026-11-24_t3_stale_nf342/`), aimed at lines whose rows landings nf343..nf373 touched. 146 lines checked
by compile (393 row compiles, all RELOCATION_MATCH / MATCH; copy lines also with both pick-any checkers). Applied by the coordinator,
matching each line by exact text and its sha1 prefix:
- closed 2 (`T3_CLOSED_STALE.tsv`): nf297 0x009FF8D0 (binders 0x009DCA80 / 0x00BCB800 now bind only the PDB const GetFrame), nf340
  0x00B49DF0 (both landed retail callers emit a copy equal to retail since nf344_b1);
- rewritten 5 (`PARTIAL.tsv`, proposed_reason): nf303 0x00780400, nf303 0x009F7250, nf326 0x00CD3D2E, nf340 0x009AC700 (six landed
  callers still emit the 159 v 149 copy), nf349 0x0042AF3C.
125 lines confirmed open; 13 unproven because their deciding row was off-limits (`UNPROVEN.tsv`). Hit rate about one close per 200
compiles: a further blanket sweep is not worth it. Note: NF342's pre-filter reads the table, which keeps one witness per spelling, so it
can hide progress (how lines 470, 1010 and 1134 were missed).
