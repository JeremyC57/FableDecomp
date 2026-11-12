# Workflow wf18 (2026-11-12, Opus only): ten author rows, ten byte matches, nine landed (landing fk)

Every stage ran on Opus. Per-row notes are concatenated here (NOTES.md, LESSONS.md, DOSSIERS.md, ATTEMPTS.log, LABELS.tsv,
NAME_FIXES.tsv, STATUS.tsv, hdr/README.md, EVIDENCE_VERDICTS.md). 0x00959BF0 matched but is in repair. 0x007E6450 was rejected by
the landing verifier (wrong jump table: the switch mapped inputs to the wrong values), fixed by the coordinator and landed.
Batch: `batches/2026-11-12_landing_fk/`.
