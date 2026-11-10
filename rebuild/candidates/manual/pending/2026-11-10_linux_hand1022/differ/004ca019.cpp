// CThing::ConstructFromParams -- PHANTOM ROW: 0x004CA019 is not a function start
// Grade: DIFFER(not graded) -- 0 grades: 0x004CA019 is the interior of the landed CThing::ConstructFromParams 0x004CA010 (437 bytes, ends 0x004CA1C5)
// Evidence: retail 0x004CA010 is the real entry (landed row CThing_ConstructFromParams_004ca010.cpp, neighbours/004ca019/);
// 0x004CA019 falls 9 bytes into its first block (inside the instruction stream of the 0x004CA010 body, not after a ret / int3
// run; a whole-image xref scan for 0x004CA019 was not run). The catalog length 439
// is the landed body's extent measured from the wrong start. Nothing to reconstruct; drop the row from the catalog.
