// CNavigatorAStar::GetRandomNavigablePos(...) -- random navigable position near a point for the ground navigator (same scheme as the flyer 0x00A19430, 128 x87 ops, 7 rsqrt-table loads)
// Grade: DIFFER(-v1136) -- triage only, no source drafted: same family as the parked flyer row 0x00A19430 (x87 temporaries and dead-argument slot reuse); attempt after that one matches
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A1B5A0, 1136 bytes. Decoded from dis_all.txt; see NOTES.md for the next step.
