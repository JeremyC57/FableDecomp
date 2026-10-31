# Chunk 636 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A FableWin-only inline accessor with an early `return 0` changes where the result lands. 0x00BA6DC0: the inline
  `GetHiddenMaterials(const C3DMesh2&)` assigns the table pointer straight into the caller's preset-0 register (`mov ebp,[esi]`).
  The same select written in the caller computed it in eax and needed a `mov ebp,eax`. Before parking a "one extra mov" row, grep
  fablewin_pdb_names.tsv for an inline accessor of the class the code reads.
- Use the PDB's inline helper even when the open-coded form is mathematically identical. 0x00A0C450: the landed inline
  `C3DVector::InitialiseFromAngles(xy, yz, 1.0f)` (named float cosYZ, `* length` products) gives retail's x87 stack shape
  (`fld st(2) ; fmulp` then `fmulp`). Open-coded cos*sin products multiplied in place and popped cos(pitch) afterwards.
  `x * 1.0f` folds away.
- Retail keeps unreferenced out-of-line copies of functions it also inlined (RGB2IMEColour at 0x00404920 has no callers).
  /O2 implies /Ob2, so a same-TU non-inline global is auto-inlined. Byte-search retail for such a copy to prove the helper's real
  shape and linkage.
- Byte-identical bodies in two TUs whose class is in an anonymous namespace (Ego_r `?A0x...` hashes) need no invented
  distinct names: `namespace { }` in each source gives each its own symbol (0x00978790 / 0x0097ABF0).
- engine/CScriptThing.h (FABLE_RETAIL_NSCRIPT) declares operator== out of line (0x008AAE20). A row that expands it inline
  (`PImp->IsEqualTo(*other.PImp)`) defines it inline after the include. Do NOT also define `~CScriptThing` inline unless retail
  expands it: that inlines the CCountedPointer release (+63 bytes, 0x00D9EB30).
- Second confirmation of agent634: VC7.1 canonicalises x87 operand order. `a + b` with either operand order, const or non-const
  operator+, a named temporary and a C2DVector step operator all grade the same on 0x00A198C0.
- A register-colour swap of two webs (ebx/edi: this v a short-lived pointer, 0x009210A0; fill_len v new_finish, 0x006A3A70)
  survived these: PDB respelling, real reference/const signatures, QFE 4035, helper functions and accessor forms. Park it as
  "register colouring tie" after two tries.

Review at landing:
- 2 matches (0x00A0C450, 0x00BA6DC0). Respell at landing per NAME_FIXES.tsv:
  - 0x00BEC8D0 in caller 0x00BD1F70 (InitialiseHiddenMaterials);
  - 0x00BA5260 in caller 0x00BA6FA0 (RenderMeshPrimitive by CEngineSubPrimitiveStaticMesh&, ERenderPassType);
  - the landed 0x00B38ED0 IsVisible (a __fastcall free function; the PDB has a private const member);
  - 0x00F35B30 / 0x0073A6E0 / 0x004C73D0, where many landed bindings carry non-PDB spellings.
- The landed push_back 0x006A3CD0 models its std types in a private namespace FablePush_006a3cd0. Its symbols for
  0x006A3A70 / 0x00A01B10 / 0x00A01B90 are therefore wrong.
- 0x00BA6DC0 uses a local primitive chain next to the flattened engine/CEngineInternalPrimitiveStaticMesh.h (header debt in
  NOTES.md). The engine/CBaseClassNonCopyable.h and CIVCountedPointeeBase.h (FABLE_RETAIL_DEF_CLASS_BASE) includes are real.
- Grades over 4: 0x00B39FB0 (6) and 0x00978790 (6), within the brief's ~6 per row; 0x00D9EB30 (5) counts the e1 regrade after
  removing the inline dtor.
