# Landing hand3c3 b8: 0x00A781A0 CNavQuadTree::UpdateLines (Account B, 2026-11-20)

RELOCATION_MATCH, test PASS (24,000 cases), strict vland `--land`. Audited by the rolling auditor ($S/stage/audit_roll).

- **Audit changes:**
  - The delivered text carried three inline devices: a noinline sized constructor, and two FORCEINLINE fill helpers, one of them a new
    stlport/fable_containers.h opt-in. None is needed. The row, and its vector<C2DBoxF>(size_type) COMDAT (== retail 0x00A80360, 122/122), are
    the same bytes on the default path, so the set has no STLport change.
  - The four clamps are GFLimitUpper / GFLimitLower calls, as in Ego_d's 14/14/11/14-byte lines (exact at /Od; the same retail bytes).
- **Header:** engine/CNavQuadTree.h, one add-only block after IsAreaBlockedByLines (FABLE_RETAIL_CNAVQUADTREE_UPDATELINES, a declaration).
  hdrcheck2 default=same. The held navedge set adds another CNavQuadTree.h block; it merges by block.
- **Mutants:** 195 killed + 6 equivalent.
- **Name fix:** 0x00A80360 is vector<C2DBoxF>(size_type) (Ego_d local node_areas). It is landed as vector<C2DLineF>.
