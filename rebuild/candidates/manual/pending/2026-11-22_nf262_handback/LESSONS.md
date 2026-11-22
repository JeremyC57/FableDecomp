# NF262 lessons
- A stand-in pointee for a data global can carry stand-in callee spellings in its member types (`NativeLandscapeRenderer::MemoryManager` was a
  `CEngineLandscapeMemoryManager_Methods*`): moving the global onto the real type moves those callees too, so census the callee spellings with
  the global before drawing the unit.
- A sibling row that already binds the right spelling is a recipe only for its statements, not for its local models: 0x00BFC140 carries a
  row-local `class CEngineLandscapeMemoryManager`, while the landed retail branch of the header declares the same members.
- Read the retail bytes of three-line July rows before planning a rename: 0x00BF36A0 / 0x00BF36E0 / 0x00BF9920 look like `operator new` in the
  catalog but are class operator deletes whose self leaves are placeholders, so their global cannot move without a self-rename family.
