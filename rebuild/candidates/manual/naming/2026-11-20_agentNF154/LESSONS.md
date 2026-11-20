# NF154 lessons (one line each)
- Compile the whole REFS census before anything else (tools/census.sh: each row once, every target it references): the binding table missed one wrong binder (0x005DAB10) and carried two stale witnesses (0x007E27F0 PeekWorldMap, 0x0057CB7E GetDefinitions); 7 entries closed as done-already on that evidence alone.
- The census is a compile: start it in the background only while you edit, never while you grade (one compile at a time); 387 rows took about 35 minutes.
- engine/CTCCreatureNavigation.h's default path names C3DVector without declaring it: include engine/C3DVector.h first in a row that has not.
- The flattened CTCWeapon (default branch) has no CTCBase base, so a protected CTCBase accessor needs FABLE_RETAIL_CTCWEAPON; that alone turned a C3861 into RELOCATION_MATCH with no byte change.
- Access-only respells inside an existing local model (public -> private) never moved a byte in 9 CTCLook rows: do the whole family in one pass, self rows and callers together, and check every referencer with an exe scan.
- A stand-in container passed to a PDB `vector&` parameter: when retail's element is a packed 9-byte pair, keep the row's stand-in and hand it over by reference rather than porting the vector (0x008006D0).
- Before a self rename to an access the shared header uses, compile every includer of that header and read its COMDATs: an in-class noinline body (CTCLook::GetTurningAngles) is emitted pick-any by any includer that calls it, and the strong self row then clashes (llvm-readobj --symbols shows Selection Any vs NoDuplicates).
