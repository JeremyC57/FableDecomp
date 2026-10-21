# De-bake 0041be70 CEnginePrimitiveHandle::Clear (2026-10-21)

The catalog held this row only as an asm_bake (`_emit` body). Retry B wrote plain C++ that byte-MATCHes retail (test PASS). Procedure (as debake_family.py does it): removed the asm_bake catalog block, its auto-re-candidates.tsv oracle row and the baked src/test files (they stay in git history), then landed the genuine source through verify_and_land.py --land (MATCH + PASS). Catalog count unchanged; asm_bake count down by one.
