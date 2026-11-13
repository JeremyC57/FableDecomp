# Hand chunk 2054 (CNavQuadTree): 1 of 1 byte-exact

## What matched (1)
- **0x00A7AF60 CNavQuadTree::LoadFromFile** (src/00a7af60.cpp, test/00a7af60.cpp): RELOCATION_MATCH 2503v2503, 770 of 770
  instructions, `// Flags: /Gr`; behaviour test PASS under g++ and VC7.1; read-only vland RELOCATION_MATCH PASS (two RELOC_NAME
  warnings only: the catalog labels of 0x00A85870 and 0x00A83420 are the demangled form of the same symbols). rel.py: every call /
  data target equals retail (35 same, 34 not bound yet, 7 name-only differences against stale bound spellings -> NAME_FIXES.tsv).
  34 full-row grades in all (interrupted session 9, scout 13, this session 12: grades.log).
  What the function does: loads a navigation map through a 307200-byte CCachedFile -- file size, version (below 5: return false),
  map size, the region list, map-relative action points, a map<long, CANavQuadTreeNode*> index table seeded with 0 -> the shared
  BlockedNode, the layer tables ((width / 32) * (height / 32) nodes each), the node records (switchable flag from version 6, a plain
  blocked record is the shared BlockedNode with NavLayer = current layer, otherwise a CNavSwitchableLeafNode / CNavNavigableLeafNode /
  CNavQuadTreeNode built from the file, preferability from version 7), the end marker, ConvertIndexesToPointers over the top-level
  nodes, SwitchableNodes.Update() and SetAsBlocked(id, false) for every switchable entry, Initialised = true. GFProcessBreath (landed
  0x00A762A0) is expanded five times.
  What closed it (in order):
  1. (interrupted session) the retail class models: CCachedFile (new overlay header, INLINE destructor), the three node classes as
     opt-ins, `MapPos = area.GetTLPos().ToC2DCoordF()` (535 -> 707 equal).
  2. (scout) the region loop `{ unsigned long i = 0; if (no_regions != 0) { for (; i < no_regions; ++i) Regions.push_back(
     CNavRegion(MapIndex, i + 1)); } }` and the CNavRegion(long, long) text `: MostRecentSearchID(0) { RegionID = ...; }`.
  3. (this session) the forced `__declspec(noinline)` std::__copy opt-in of the earlier candidates was a symptom of a WRONG HELPER
     TEXT: without FABLE_RETAIL_STLP_VECTOR_ASSIGN the shared vector has no operator=, CNavigationLayer's implicit operator= is a
     bitwise `rep movsd`, and the TU's __copy<CNavigationLayer*, ..> is 68 bytes (retail 0x00A85870: 97 bytes, three vector operator=
     calls + the Dirty byte). Found by grading the row TU's own out-of-line __copy against retail 0x00A85870 (free, q.py). With the
     existing repo opt-in FABLE_RETAIL_STLP_VECTOR_ASSIGN the body equals retail and c2 keeps it a call by itself (score 137 / 103
     against threshold 74), erase and resize expand as in retail. The noinline opt-in, the LIST_ALLOC_CREATE opt-in and the overlay
     copy of stlport/fable_containers.h are withdrawn; the row uses the unmodified repo STLport headers and forces no inline decision.

## Fitted v proven
- PROVEN by bytes / two name sources: every Read in file order and its size, the version thresholds 5 / 6 / 7, the member offsets
  (engine/CNavQuadTree.h, Ego_r.pdb), `!switchable && blocked` (0x00A7B4AD..0x00A7B4BB), the 1/32 scale (0x0129DD98), the layer
  wrap (`jl` at 0x00A7B702), the five breath copies (same text as the landed 0x00A762A0), all callee names (LABELS.tsv).
- FITTED: (a) the region loop spelling. The bytes prove ONE guard (`xor edi,edi ; test eax,eax ; jbe`, 0x00A7B04A), the CNavRegion
  copy constructor CALLED (0x00A7B09F) and NavigationLayers.resize EXPANDED after the loop. Measured: plain `for` -> copy expanded,
  frame 0xF4 (grades 9, 29); `if` + `for (unsigned long i = 0; ..)` -> two tests + a loop-head pad (grades 8, 14, 30); do-while forms
  -> copy expanded or resize called (11, 12, 15); a declaration-only copy constructor with a plain `for` (scout diagnostic 13) gives
  the right frame but an entry jmp. Only "counter set ahead of the guard, then `if`, then `for`" gives all three. VC7.1 removes the
  redundant `if` test (no bytes); it halves the static frequency of the copy site (0.625: cost 76, score 82 >= 74 -> called).
  (b) the CNavRegion(long, long) text: `: MostRecentSearchID(0)` + body RegionID; a body `MostRecentSearchID = 0; RegionID = ...;`
  matches too (scout s8); the landed 0x00A78680 text (RegionID first) gives the wrong store order at 0x00A7B07D / 0x00A7B085 in the
  expanded copy. All three texts give the 0x00A78680 bytes. (c) `GetTLPos().ToC2DCoordF()` over the PDB inlines (a member-by-member
  conversion compiles differently). (d) `(MapWidth / 32.0f) * (MapHeight / 32.0f)` (a multiplication by 0.03125f is the same bytes).
- Not on an edge: the three inline decisions that carry the frame have margins of 7 or more score units (copy ctor 82, resize 67,
  erase 37, __copy 103 against 74; tools/inl/v13gr.txt).

## Names
- Row and engine callees: Ego_r.pdb + FableWin.pdb name sets (ghidra_out/egor_pdb_names.tsv, fablewin_pdb_names.tsv) and the landed
  rows; STLport helpers: STLport 4.6 source names + landed rows / reloc bindings, and for eight of them the row TU's own body
  equals the retail copy (tmp/sibs_final.txt). See LABELS.tsv (15) and NAME_FIXES.tsv (11 stale spellings: seven the row meets,
  the placeholder Leaf of 0x00A81A70, and the three sibling BlockedNode member addresses of the landed 0x00A76340).
- Descriptive (no original name known): every LOCAL variable of the row (file_size, version, no_regions, no_action_points,
  action_point, nodes, no_layers, no_nodes_per_layer, no_nodes, layer, node_in_layer, switchable, top_level, blocked, leaf_node,
  quad_tree_level, nav_layer, position, map_position, index, switchable_node, node, end_marker, last_breath_time), the parameter
  names, and the empty enum body of ENavQuadTreeNeighbours (PDB name, enumerators not needed). No made-up function, global, class
  or member name.

## Header debt (hdr/README.md has the evidence)
- NEW engine/CCachedFile.h; opt-ins FABLE_RETAIL_CANAVQUADTREENODE_LOADSLOTS, FABLE_RETAIL_CNAVQUADTREENODE,
  FABLE_RETAIL_CNAVNAVIGABLELEAFNODE, FABLE_RETAIL_CNAVSWITCHABLELEAFNODE. Nothing on a default path. The other files under hdr/ are
  chunk 2051's overlay, untouched and unused by this row (prior/2051 holds that chunk's hand-back).

## Parked for Fable 5.1 (0)
None.

## Skipped
None.

## Label corrections
- NAME_FIXES.tsv. Catalog: 0x00A841F0 "Sort_SElement16_00a841f0" is std::sort over pair<unsigned __int64, CANavQuadTreeNode*> with
  CKeyPairCompareLess; 0x00A7E340 / 0x00A81A70 are the map<long, CANavQuadTreeNode*> tree's _M_erase / insert_unique; the landed
  0x00A76340 "SetBlockedNode" writes the members of CNavQuadTree::BlockedNode (Position, QuadTreeLevel, NavLayer), not four globals.

## Tools and scratch kept
- tmp/sibs.py (compares every function of a q.py object that has a retail address with Fable.exe, relocation-masked) and
  tmp/sibs_final.txt; tools/inl/v13gr.{json,txt} (final inline trace, /Gr), v9gr (the noinline candidate); tmp/rel_final.txt;
  tmp/vland.log; scout/ (the scout's notes, candidates and traces); cand/v9..v16 (this session), cand/v1..v8 (interrupted session).
