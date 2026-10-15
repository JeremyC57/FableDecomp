# Chunk 66 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A named pointer local (`float* v = Object;`, `T* first = TransVector->m_pFirst;`) makes VC load into the dead `this`
  register (ecx) like retail; reading through the member picks eax or swaps the two loads (fixed 0040f870, 005663f0, 00614450).
- A 12-byte POD vector filled field by field in a named local and returned keeps retail's copy through the frame
  (`mov ecx,eax` + three load/store pairs); a memberwise user copy constructor folds it into three x87 `fstp`.
- `add ecx,0Ch` between the first and second dword copies of a hidden-pointer return = an inline `const C3DVector&`
  accessor plus a user memberwise copy constructor (integer moves, not x87; 0x008FA7E0).
- STLport `map::operator[]`: the default mapped value must be a NAMED local (retail re-addresses it with `lea`; a bare
  `T()` pushes the constructor's eax, 80 v 83 bytes); the by-value iterator is a 4-byte class with a user copy
  constructor; insert's hidden result reuses the dead key slot.
- The 0x009E2590 long reader: a plain `long value;` out-local is homed in the dead parser slot automatically.
- `CPersistTraits<T>` is `{ T DefaultVal; }` with `: DefaultVal() {}` -- reproduces the early `mov dword ptr [esp],0`
  for float / CDefIndex and the CCharString constructor/destructor pair for the string traits.
- `engine/CPhysicsFacesSegmentBranch.h` flattens the vptr into a `void*` member, so it cannot produce vtable stores; its
  destructor rows use a local polymorphic model with the PDB member names/offsets.
- Three sources need in-source pragmas `optimize("y",off)` + `("s",on)`: 00cd3da4, 0047cf3f, 0042d5b1.

Skipped 00455fa0: 42 bytes = three functions with NO padding (a `jmp [eax+48h]` thunk, a bit getter, a bit setter) --
the catalog row needs splitting before it can be authored. 18 label corrections in NOTES.md.
