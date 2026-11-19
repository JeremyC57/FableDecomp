# Landing hand4c b6 (2026-11-19): NUISystem::CMorphingSprite::GetDataFromDef (0x005542F0)

**Result:** strict vland --land gives RELOCATION_MATCH + PASS. No flags and no header overlay: only existing repo opt-ins are used.

**Source:** the parked draft was ported onto the shared headers (NUISystem_CMorphingSprite.h, CUIDef, CUIStateDef, CDefPointer) in the order of FableWin 0x022A8CB0, using its
locals pdefinition / count / map_entry / pinfo / frame.

**What closed it:** the map opt-ins of the landed 0x0061B610, STLP_MAP_ORIGINAL_INSERT_BODY + STLP_TREE_INSERT_UNIQUE_HINT_BODY. A c2 DAG dump showed retail's
pair stores carry no memory edges. With the _tree.c bodies visible, c2 sees that insert_unique only reads the pair.
- The same-TU helper 0x005537F0 (hinted insert_unique) grades RELOCATION_MATCH from this source.

**Test:** behaviour mutants (flags, key, operator[] overwrite, GetFrame flags) all FAIL.
- Test gotcha: VC7.1 /Od never finished compiling the test while a for loop preceded the class-object declarations in main, so the test declares them first.
