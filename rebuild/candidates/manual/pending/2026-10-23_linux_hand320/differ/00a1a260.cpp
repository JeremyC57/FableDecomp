// CNavigatorAStar::IsRouteClear(...) -- walk the line between two positions through the quad tree and test each node and edge for clearance (137 x87 ops)
// Grade: DIFFER(-v1399) -- triage only, no source drafted: x87-dominated (137 ops) line walk; scheduling-sensitive, needs CANavQuadTreeNode virtuals
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A1A260, 1399 bytes. Decoded from dis_all.txt; see NOTES.md for the next step.
