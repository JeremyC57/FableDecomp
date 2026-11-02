# retry11b lessons (one line each)

- In a one-argument `persist.Transfer(member)` def body, a default whose constructor is out of line (CWideString) shows up as a ctor/dtor pair in the dead context-argument slot around that one call. A block-scoped default object reproduces it: `{ CWideString defaultTextValue; persist.Transfer(TextValue); }` (0x00631C60).
- CPersistContext vector members go through `TransferVector<T>`. Both PDBs instantiate only `??$TransferVector@K/M/J@`, never `Transfer<vector<T>>`.
- C2DVector is a packed class: the TPI packed bit, Ego_r offsets 0x15E and 0x11, and retail +0x142. Any class with a C2DVector after an odd-sized member needs `FABLE_RETAIL_C2DVECTOR`.
- A catalog extent can be one byte short when a function ends in a no-return call. VC7.1 emits an int3 after `call exit`, and that byte belongs to the function (0x0099DCB0 is 1455 bytes).
- q2-style graders read `// Flags:` from the source when no `--flags` is given. Never write a placeholder such as `// Flags: (none)`, because it reaches cl as an argument. Delete the line instead.
- An anonymous-namespace template argument (`?A0x<hash>`) never matches the retail leaf on this lane, because the hash follows the file path. Such rows grade the largest function and cannot pass vland's leaf check unless the build path is fixed.
- Trait-object declaration order in a CPersistTraits TransferIn (0x006159A0) has no effect on the frame. The empty traits get no slots; only the DefaultVal temporaries do.
- When Codex's selected context still carries its second CCharString model or per-feature macros, grade the parked shape there for continuity, but say so on the grade line. Those rows are not landable until they are rebased.
