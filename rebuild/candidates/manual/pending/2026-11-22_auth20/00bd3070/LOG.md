# 0x00BD3070 LOG (AUTH20)
- g1 base: AUTH1's b6.cpp (= p25S b6: no row asserts + landed FABLE_RETAIL_CARRAY_INDEX_ASSERT) with AUTH1's overlay copied to hdr/, stock flags,
  QT=_a20: DIFFER(11288v11288), built bytes identical to AUTH1's b6.out. Grading takes ~4 s.
Next (p25S open item): move the ~15 row-local FABLE_FORCEINLINE member definitions into their shared headers as add-only opt-ins, then
regrade at stock flags. The remaining hunks are c2 numbering ties (same class as 0x00B4D370's residue).
