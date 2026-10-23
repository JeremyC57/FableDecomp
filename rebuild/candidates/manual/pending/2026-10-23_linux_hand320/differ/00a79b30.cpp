// CNavQuadTreeNode::Initialise(...) -- initialise a quad tree node: bounds, children, navigability sampling over the cell (39 x87 ops, 14 callees)
// Grade: DIFFER(-v1381) -- triage only, no source drafted: x87 sampling loop plus child allocation; engine/CNavQuadTreeNode.h does not name the retail members used
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A79B30, 1381 bytes. Decoded from dis_all.txt; see NOTES.md for the next step.
