# NF64 lessons (round 7)

1. **Run the split check before the self rename, not after.** Six group-A operator= rows and 0x00467831 / 0x004E9021 / 0x00694020
   were finished and then had to be held in work/ready, because a landed caller outside rows.txt bound the old self spelling.
   Probe the self address's binders first (probe.sh), then decide between ported and ready.
2. **A first-pass "self row already right" verdict can be wrong when the claim itself is wrong.** 0x00694020 was marked
   not-a-binder because it defined the queued QAE spelling. Both PDBs give MAE, so the claim was rejected and the row corrected.
   Check the claim against both PDBs before classifying.
3. **Retyping a stand-in to the real STL type is cheap when the shared header has the per-TU out-of-line opt-in.**
   - The resize group (12 rows) and the CArray rows ported in one or two forms each, with FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE
     / _CLEAR_OUTOFLINE / FABLE_RETAIL_STLP_VECTOR(_C).
   - Rows whose own `namespace std` models clash with the shared STLport headers (C2955/C3203) are authoring jobs, not name fixes.
4. **Check every resized member's type in struct_layouts_egor.tsv before retyping.** One row's argument turned out to be
   CHistogram<long> (0x009FBBD0), not vector<unsigned long>, and its self leaf was wrong with it.
5. **The behaviour test is part of the port.** A row whose object starts carrying STL COMDAT bodies (0x0058148D) can grade RM and
   still fail to link its test. Run the vland recheck early enough to rewrite tests (0x008D5000's test needed the PDB names and
   the retail print pattern `OK_008d5000`).
6. **Give helper scripts absolute paths.** pay.py runs mkpay from the repo root, and g.sh resolves relative to it. Two calls went
   wrong on a relative path; both are listed in work/slips.txt.
