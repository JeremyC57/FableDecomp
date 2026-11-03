# Chunk 722 — CEngine component forwarders (31 rows, 11 bytes each)

All 31 rows are `CEngine` public virtuals (CIEngine interface slots) of the shape
`mov ecx,[g_pComponent]; jmp Component::Method` — a thiscall tail call on a published
engine-component global. All 31 are RELOCATION_MATCH on the first grade; tests PASS.

## Model
- `class CEngine { public: virtual ... }` declared locally per TU, one virtual per row,
  so the leaf mangles `UAE`. `rebuild/include/engine/CEngine.h` flattens the vptr into a
  data member (`void* __vftable`), which cannot emit a virtual definition; the landed
  siblings 0x00B239C0 / 0x00B23CB0 / 0x00B25D20 use the same local-CEngine model.
- Callee classes are declared with only the called method (exact PDB signature, `const`
  where Ego_r says QBE). The overload of `CEnginePrimitiveRenderer::SetChildPrimitive` is
  told apart by argument bytes (ret 14h = 0x00B31C30 const-ref variant, ret 10h = 0x00B31C50).
- Globals use the spellings already in `reloc_bindings.tsv`; the one new spelling is
  `g_pPrimitiveAttachmentManager_01436e70` (0x01436E70, the CEnginePrimitiveAttachmentManager
  component pointer; only referenced by CEngine::SetAttachedPrimitive).

## Naming
21 landed callee spellings are wrong (invented `_Methods` / `C_xxxx` / `T::` classes, or a
catalog-guessed method name); see NAME_FIXES.tsv. Each correction is backed by Ego_r +
FableWin publics and the argument byte count of the forwarding jmp.

## Gotcha
- CEngine forwarders: declare a local polymorphic `CEngine`, not the flattened header, or
  the leaf mangles QAE and the row cannot match.
