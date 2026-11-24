# 0x007634E0 NShape::CBasicShape::UpdateSubdivisions recheck after 0x00763060 becomes its own row (AUTH9)
- Declaration-only FractureShape (C) or all three NShape helpers declared (B): DIFFER(639v627), frame 0x34 v 0x30. The copy stays as a TU-mate.
- P1.patch: Fidelity comment only. rg1: RELOCATION_MATCH. pick-any 18 EQ / 0 NEQ. Repo test unchanged: PASS.
- P2.patch + P2_test.patch: P1 plus owner-verbatim copies of 0x00760020 / 0x00760190 (+FABLE_RETAIL_C3DVECTOR_CROSSWITHUP), the same text
  as 0x00763060's row. Leaf RELOCATION_MATCH; FractureShape / 0x00760020 / 0x00760190 copies RELOCATION_MATCH; pick-any 18 EQ; test PASS.
- orig/ = the repo files the patches were made from (git apply --check OK on HEAD).
