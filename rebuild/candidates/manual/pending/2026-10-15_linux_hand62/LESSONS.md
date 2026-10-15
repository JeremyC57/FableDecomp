# Chunk 62 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- Retail `CParentDefClassBase` is a packed 0x39-byte polymorphic base (`engine/CParentDefClassBase.h` says 0x3d because
  the PDB vector map is 16 bytes): a derived `bool` lands at +0x39 and a dword at +0x3c.
- Inside def classes CRGBColour needs 4-byte alignment while C2DVector / C3DVector are byte-aligned
  (CHeroPostcardGeneratorDef has them at +0x39, +0x41, +0x49 and pads its float to +0x58); a C3DVector member copies as
  `movsd` x3, a `float[4]` member through an FPU element loop.
- The class behind a "CParentDefClassBase::operator=" row: a vtable whose slot 19 (`Copy`) is a 5-byte `jmp <row>` at
  row-5; then match the PDB layout with every container shrunk by 4 bytes (the agent's `probe/pdbmatch.py` in
  `$S/w22/agent62/probe/` automates it for a list of observed member offsets). All 15 such rows here are concrete def
  classes (CSoundThemeDef, CCameraManagerDef, CHeroPostcardGeneratorDef, the CSpecialAbilities*Def classes, ...).
- Bit-field initialisation in the mem-initialiser list is scheduled before the vtable store; as a body assignment the
  read-modify-write sinks to the end (00b18020).
- `GetX(a).Method(b)`: VC pushes `b` before evaluating the object expression, so `b` stays on the stack across the
  `ret 8` hidden-return call.
- A pointer stored through and then used as `this` that retail keeps in ecx across the store is a named local; reading
  the member twice reloads it after the store (00b215a0).
- New helper: `tools/decomp_pipeline/linux_lane/lane_tools/xref.py <va>` (callers / references of an address).

Review at landing: the agent ran one read-only `git status` against the wave rules (tree clean, nothing changed).
Parked rows are entry-block / register-colouring differences (likely lane gaps). Names inferred without callers:
00ec3230 (from neighbours); 009fcd40 / 00a69550 are CVolumeTexture forwarders named by address. 27 label corrections in NOTES.md.
