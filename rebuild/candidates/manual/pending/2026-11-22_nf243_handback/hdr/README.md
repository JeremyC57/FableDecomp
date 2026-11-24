# NF243 overlay headers

## engine/CBankFile.h (base: orig/engine/CBankFile.h = the repo working tree at start)
One add-only, per-TU opt-in, `FABLE_RETAIL_CBANKFILE_PROGRESS_WRAPPERS_DECL` (CODEXP27 L108-L110): inside the existing
`FABLE_RETAIL_CBANKFILE_UPDATE_PASS27` block, the inline SetBankUpdateStatus / WriteBankLog (protected) and SetBankUpdateProgress (private)
bodies are wrapped as `#ifdef FABLE_RETAIL_CBANKFILE_PROGRESS_WRAPPERS_DECL <declarations> #else <the unchanged original lines> #endif`.
`diff orig hdr`: 14 added lines, 0 removed or changed; with the macro undefined the preprocessed text is identical.
- pin_asserts.py: pinned 0 (every assert in the file is already FABLE_STATIC_ASSERT_AT with an explicit line).
- Equivalence: work/eq/equiv.tsv, equiv_check.py over the 517 landed rows whose source or test reaches CBankFile.h (26 headers, transitive):
  0 differ. No landed includer defines the new macro (only the three new rows and their tests do), so none of them changes.
- Users: the three self rows 0x009CBD30 / 0x009CBD50 / 0x009CBD70 (+ tests). 0x009CEC60 (CBankFile::Update) keeps the inline bodies.
No other header was changed (stlport/fable_containers.h, engine/CStringParser.h untouched).
