# Workflow wf22 (2026-11-13, Opus only): ten author rows, eight byte matches landed (landing fx), two still differ

Every stage ran on Opus. Per-row notes are concatenated here (NOTES.md, LESSONS.md, DOSSIERS.md, ATTEMPTS.log, LABELS.tsv,
NAME_FIXES.tsv, STATUS.tsv, hdr/README.md, EVIDENCE_VERDICTS.md). Not matched after three author passes each: 0x00646F67
NPlayerGui::CDrawExperience::Draw (7 hunks, argument-setup order) and 0x00F3BA70 CTriangleStripifier2::BuildCacheOptimizedVertexList
(8 hunks, inlined push_back growth path); their best sources are under `differ/` and the next ideas are in NOTES.md.
Batch: `batches/2026-11-12_landing_fx/`.
`differ/hdr/engine/CTriangleStripifier2.h` is the header overlay the 0x00F3BA70 source needs (opt-in
FABLE_RETAIL_CTRIANGLESTRIPIFIER2_BUILDCACHE; not in `rebuild/include` because the row has not landed).
