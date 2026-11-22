# 0x008E18D0 -- AUTH15 log (S/w22/auth15/BRIEF.md: land-ready, no re-fit)
Base: AUTH14 e5n.cpp (copied as e5n_auth14.cpp); 008e18d0.cpp = e5n with comment-only edits (non-comment lines identical, checked).
Overlay hdr/ = ONLY engine/NParticleEngine_CParticleUpdateParameters.h (HEAD + the add-only IMPLICIT_DTOR opt-in, comment rewritten to
the PDB evidence). CThing.h (C5) is left out. orig/ == HEAD (checked).
## PRE-REGISTERED (each once; no variant, no re-fit)
- G1: 008e18d0.cpp, q.py --flags /Gr, FABLE_HDR_OVERLAY=hdr/ (acceptance MATCH / RELOCATION_MATCH).
- G2: TU-mate 0x008DD7E0 (landed flagless) at /Gr, repo source, no overlay. Report only.
- G3..: every landed binder of 0x006E2500 (binders.txt, 23 rows incl. the owner row and sibling 0x007A1380), repo source untouched, landed
  flags + /DFABLE_RETAIL_CPARTICLEUPDATEPARAMETERS_IMPLICIT_DTOR, FABLE_HDR_OVERLAY=hdr/, leaf from `// Leaf:`. A break gets ONE
  control grade without the define.
- D1: vland dry run (LANDVERIFY_STRICT_HEADERS=1, flags ["/Gr"], overlay hdr/, LANDVERIFY_WORK under W).
- Header proof: pin_asserts (pinned 0), assert_names_check (0 mismatches), equiv_check old v new (eq/old, eq/new), recheck listed rows.
- Wine grades run only after equiv_check has finished (CPU).
## Results
- G3c (pre-registered control for a COMDAT break, once each): 007be3e0 / 007be4e0 without the define
- G1 008e18d0 /Gr, overlay hdr/: RELOCATION_MATCH.
- G2 008dd7e0 /Gr (landed flagless): RELOCATION_MATCH. Not changed.
- G3 x23 (binders, define on): all RELOCATION_MATCH (grades.txt). The leaf grade does not cover the emitted dtor COMDAT, so it was checked
  readout-only with vsretail.py (dtor_comdat_check.txt): 12 of 14 live rows + this row emit 73 B = 0x006E2500 (0 masked diffs);
  007be3e0 / 007be4e0 (optimize("s",on) TUs) emit a 33 B copy (calls the inner ~vector): NEQ = pick-any break with the opt-in.
- G3c x2 (pre-registered control, no define) 007be3e0 / 007be4e0: RELOCATION_MATCH, no dtor body emitted (external reference only).
- D1 vland dry run, strict headers, flags ["/Gr"]: 008e18d0 RELOCATION_MATCH PASS (2 informational RELOC_NAME warnings, std::sort labels).
- Header proof: pin_asserts pinned 0; assert_names_check 0 mismatches; equiv_check 57020 rows, 0 differ -> nothing to recheck.
- Locals per block (coordinator ruling): all four scopes equal Ego_d (locals_per_block.txt). No text change.
- STOP.
- Post-check (advisor): d1_payload.json source_cpp / test_cpp == 008e18d0.cpp / 008e18d0_test.cpp byte for byte. d1_oracle.tsv (retail bytes)
  deleted from the hand-back; regenerate from S/batch/auth1_oracle.tsv (row 008e18d0, trailing CC/90 stripped, 2073 B).
- Readout (not a grade): auth9 leafrel.py on the G1 object (wine/q/008e18d0/008e18d0.obj, same mtime as G1): PROBLEMS 0; EQ 0x0040F020,
  0x0076ADA0, 0x006E2500 (g1_leafrel.txt, identical to AUTH14's e5n_t74.leafrel.txt).
