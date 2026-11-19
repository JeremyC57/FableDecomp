# Landing hand3c b17 (2026-11-19): CNavQuadTree::GetNodeAt (0x00A770B0)

Strict vland --land gives RELOCATION_MATCH + `00a770b0_TEST PASS`, after 3 grades. No flags.

**Source:** follows FableWin 0x03293760 statement for statement, with its local names.
- The box is C2DBoxF(MapPos, C2DExtentsF(MapWidth, MapHeight)), tested with the inline ContainsPoint.
- The blocked node comes from GetBlockedNode() and is set with Position.Initialise.

**Half-cell constant:** `MAX_NODE_SIZE * 0.5f` (`// Inferred:`). Retail adds the float 16.0f, whereas FableWin's double 0.5 gives `fadd qword`.

**Names:**
- ONE_OVER_MAX_NODE_SIZE is the PDB name (S_LDATA32 in FableWin and Ego_r).
- MAX_NODE_SIZE appears in neither PDB, so it is a descriptive name.

The `// Retail-asm:` line cites the three fistp addresses.

**New opt-in** FABLE_RETAIL_CNAVQUADTREE_GETBLOCKEDNODE: an inline static GetBlockedNode(), FableWin
?GetBlockedNode@CNavQuadTree@@SIPAVCNavBlockedQuadTreeNode@@XZ.
- A pure addition; no assert moves.
- Header-user recheck: 21 / 21 good.
