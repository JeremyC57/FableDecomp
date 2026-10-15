# Chunk 79 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A flag test giving `setg al` matched only as `cond ? true : false`; a bare comparison goes through ecx (007fde00).
- ParentThing passed straight into a call reuses the dead `this` register (edi) only when read into a named local first (007eb9b0).
- A `??_G` with the whole destructor chain inlined needs the intermediate base's empty destructor written `__forceinline`
  on a `__declspec(novtable)` model (0072f040); left implicit it was emitted out of line in this TU (it inlines in 007ebdd0).
- A counted def pointer copied into a local that is never address-taken stays in esi and matches (007fde70); one filled
  through `&local` by GetDef is re-read from its slot (007eac10) -- the 24k gap, now seen twice.
- Engine headers exist for many callee classes (CTCDoor, CDefClassBase, CCreatureActionBase, CPhysicsFacesSegmentLeaf); a
  local `struct X {` with such a name triggers the shared-header warning, so local models are named `X_Calls` / `X_Retail`
  (the `// Leaf:` mangled substring still selects the symbol). Review at landing: accept when the header cannot express the shape.
