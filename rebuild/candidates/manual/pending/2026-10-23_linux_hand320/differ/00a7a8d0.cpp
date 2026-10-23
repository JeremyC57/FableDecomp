// CNavQuadTree::Initialise(...) -- build the quad tree over the map: size from the level bounds (__ftol2 + cdq/idiv), allocate the root and recurse, register node lists
// Grade: DIFFER(-v1665) -- triage only, no source drafted: 31 distinct callees incl. operator new/ctor pairs and STLport vector growth; engine/CNavQuadTree.h is PDB-shaped (retail vectors 12 bytes)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A7A8D0, 1665 bytes. Decoded from dis_all.txt; see NOTES.md for the next step.
