# Held batches (land only as stated)
- held/b5 (6 rows): coordinator D-1 accepted under NF321 Q1 (ALL_EQ + exactly `__fltused`), conditioned on two sources for each float
  member: evidence/b5_float_sources.tsv (Ego_r.pdb + FableWin.pdb TPI field lists). Lands from held/b5 only, with held/b5/TABLE_CHANGES.tsv
  (1 replace + 2 delete) and only held/b5/hdr/engine/CLensFlareElementDef.h.
- held/b6 (2 rows): the D-2 retry (operator= stays implicit pick-any; the forcing device in 0x0046F43E is the 0x00E95380-form scaffolding
  helper; 0x0046F43E on engine/CAbilityButtonGuiDef.h; 0x0046F49F unchanged). With held/b6/TABLE_CHANGES.tsv (1 delete) and
  held/b6/hdr/engine/CAbilityButtonGuiDef.h (the Copy row's opt-in). Evidence: evidence/b6/OBJCHECK_retry.txt, GRADE_g13_retry.tsv,
  linkcheck_b6_retry_with_witness.txt, census2_operator_family_HEAD.tsv. The rejected explicit build is kept in work/b6_explicit_rejected_rundir/.
- held/TABLE_CHANGES_held.tsv = the two batches' lines together (same content as their own files).
