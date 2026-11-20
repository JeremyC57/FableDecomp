# NF123 lessons (round 10)

- **Read the entry columns, not the header names.** For the chunk-796 (land0511x) rows the `correct_symbol` column holds the wrong
  spellings and the claim sits in `evidence`. Check each claim against the PDB name sets before you transform anything.
- **Probe first, then respell.** One exe-scan probe per REFS row (callee address, grade, bound symbol) settles every not-a-binder row
  cheaply. It also shows which binders carry which wrong spelling, so a family can be planned as one batch.
- **Stand-in classes decide the route.**
  - When the row already has a local model of the real class (even one that has a shared header), adding the member there is an
    in-place respell, and it grades first time (0x0078F4D0, 0x008CBB90, 0x00922600).
  - When the stand-in is a made-up class (CTCDriverED, CObstructionShape, CAIBrainX), the real class has to come from the shared
    header. That works only when the row does not also need members that the header leaves opaque (the ignore vector in the four
    blocked q6401/q6402 rows).
- **Self rows can carry a wrong name even when the file name is right.** 0x007FD3D0's file says SetLifeTimeSeconds, but its source
  defined BoughtSomething and a global. Read the source, not the catalog path.
- **The shared headers usually reproduce small accessors byte-exactly** (SetParams, SetLifeTimeSeconds and PeekCurrentStateGroupName
  matched on the first form). A port is cheaper than it looks when the body touches only the class's own members.
- **Keep shell quoting safe.** A heredoc that carries retail-byte comments with backticks must be quoted (`<<'EOF'`), with paths
  passed through argv. Otherwise the shell runs the backticks.
- **A local empty `enum ETCInterfaceType { };` blocks any CTC header that needs the enumerators.** Replace it with
  engine/ETCInterfaceType.h (C2011 otherwise).
- **pin_asserts matters.** Any opt-in block placed above a FABLE_STATIC_ASSERT moves its line: run pin_asserts after every
  overlay edit (CScriptedMapBrush.h pinned 14, CTCDPhysicalObstruction.h pinned 1).
