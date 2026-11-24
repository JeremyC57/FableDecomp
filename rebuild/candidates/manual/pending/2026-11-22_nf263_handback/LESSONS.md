# NF263 lessons
- A class operator delete with a placement twin: read the CALLERS' edx before choosing the spelling. `xor edx,edx` before every call means the
  two-argument `__fastcall` placement form; the one-parameter PDB form can be a separate `xor edx,edx ; jmp` stub elsewhere (0x00BDBF70).
- A brief's "same bytes" claim about two PDB forms is a claim too: FableWin's plain form forwarded (2 instructions), it was not a twin.
- Allocator quartets (??2 / ??3 / ??_U / ??_V) keep the PDB's order in retail; the neighbours of a row are a cheap second source.
- July three-line rows can define ONE symbol at two addresses (0x00BF36A0 / 0x00BF9920 both `?fn_00bf36a0`); a self rename fixes both.
- A table witness can be stale before you start (0x00BF9920's `CEngineSubPrimitiveGameMesh_dealloc`): compile the witness at HEAD and read
  the symbol it really binds before writing TABLE_CHANGES.
