// NScript::CQ_HangingTreeEvilScript::SpawnEnemies(const C3DVector&, CCharString, long) -- catalog row 0x00D6BB91 is not a function entry: it is the instruction at +0x11 inside SpawnEnemies 0x00D6BB80 (landed by this chunk as src/00d6bb80.cpp)
// Grade: DIFFER(0v645) -- bounds: no function starts at 0x00D6BB91 (no call or vtable reference; the bytes are the tail of 0x00D6BB80, 662 bytes, 0x00D6BB80..0x00D6BE16); retire the row
// Leaf: none
// Flags: none
// Evidence: retail 0x00D6BB80 `sub esp, ...` prologue runs straight through 0x00D6BB91 (mid-instruction stream of the same body); the
//   graded src/00d6bb80.cpp covers every byte of this row's range. Nothing to reconstruct separately.
