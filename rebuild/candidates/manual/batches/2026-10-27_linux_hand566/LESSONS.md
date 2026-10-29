# Chunk 566 lessons (GOTCHAS one-liners)

- Relocation masking hides float constants too: read every `fld/fadd/fsubr/fmul/fcomp [abs]` operand from retail .rdata before trusting a RELOCATION_MATCH's arithmetic (0x008CC7A0's minuend is 1.0f, so its fade-out goes non-positive and the alpha byte wraps).
- A block-scoped temporary (`{ CCharString noise(...); f(noise); }`) shares the `push ecx` frame word with the lookup keys; at function scope it costs a second frame slot (0x008B7BA0: 340 vs 336).
- A one-expression setter `p->Get()->Set(p->Get()->Value() + k)` evaluates the object first; retail's getter-before-object order needs a named local (0x008C40F0).
- A raw member operand (`PBrain->Creature`, `m_PPhysics`) is re-read after a call and goes through eax; the PDB inline accessor (`GetCreature()` returning a reference, `GetPhysicsTC()`) keeps it in a callee-saved register as retail does (0x00934870, 0x008C40F0, 0x0097C5E0).
- An implicit dtor (no vptr store) whose class has a CEnginePrimitiveHandle member stores only the member's vtable; do not mistake that store for the class's own (0x009536D0).
- CTCCreatureModeManager 0x0073A840 is RefreshCurrentMode (both PDBs); RestartCurrentMode is invented.
- 0x006925E0 is CCreatureActionBase::GetPercentageComplete (QBE); the GetMovementRatio / GetProgress* labels are byte-twin propagation.
- Row extents can swallow following small functions after a jump table: compare the true extent with a masked prefix tool (tools/pfx2.py) and park with "re-split at" (0x0097C5E0 -> 0x0097C720).
- Behaviour tests on a 64-bit g++ host: model counted pointers and vectors by member, never by offset; float-to-byte through `(unsigned char)(long)` truncates the same way under g++ and VC7.1 /Od.

## Review at landing
- See NOTES.md "Review at landing" (alpha wrap at 0x008CC7A0, 0x013BA848 double binding, re-split 0x0097C720).
