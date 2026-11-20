# 2026-11-20 landing nf145: follow-ups owed by the nf137 / nf140 landings (21 rows)

Records: `naming/2026-11-20_agentNF145/` (selfcheck 0 problems). 6 entries: q11780, q11784, q11808, q11809, q11810 done; q11785 open
(0x00956960's Clone port and the override opt-in went to NF148).

## Rows (21): all RELOCATION_MATCH, every test PASS (`logs/recheck.log`)
- Onto shared headers (6): 0x0122A9A0 (CMemoryAllocator.h _CTOR + _DEALLOCATE + new _DEALLOCATOR), 0x009BFE70 / 0x009BFF10 / 0x009BFF40 /
  0x009C0020 (CDisplayManager.h new _RESETDEVICE + _CREATEDEVICE), 0x009566C0 (CCreatureAction_ThrowShotObject.h _CLASS; own leaf takes
  the PDB ctor spelling).
- Inline control only (13): 0x009D52D0 now carries `// Flags: /d2inlT78`; 7 of the 12 Deallocate users carry `/d2inlT114`, 5 match with no
  flag. The catalog CompilerFlags of those 8 rows were set from their Flags lines.
- In-place respells of HEAD local models (2): 0x00B24690, 0x008E8830 (binds the InvulnerableTeleport ctor spelling 0x008E8C50 defines).

## Headers
- engine/CMemoryAllocator.h: the nf140 opt-in FABLE_RETAIL_CMEMORYALLOCATOR_ALLOCATE_FORCEINLINE is removed (its only user is 0x009D52D0,
  in this landing); Deallocate / Destroy under _DEALLOCATE are plain `inline` (were FABLE_FORCEINLINE; every user rechecked, with its flag);
  new add-only _DEALLOCATOR (nested CDeallocator, static Deallocator, operator delete's body for that TU). Token-identical without the
  changed macros.
- engine/CDisplayManager.h: new add-only _RESETDEVICE.

## Decisions (advisor, owner delegated)
1. q11785 stays open; NF148 has the override opt-in and the 0x00956960 port.
2. 0x008E8830: accepted as an in-place token respell (header-less class, sibling 0x008E8C50 has the same shape, base model as at HEAD).
3. The ALLOCATE_FORCEINLINE opt-in removal is accepted (inline control moves to per-row flags, EXTRA_RULES 2026-11-08 preference 2).

## Checks
- 156 rows rechecked (gate referencing rows + renamed rows + includers of both headers), 156 good. Gate 0; bindings_multi_check 1.
- Table: no set grew. Shrank: 0x013CAA90 8 -> 7, 0x013CAA94 2 -> 1, 0x009BFF10 2 -> 1.
- One definition (`logs/symbol_diff.txt`): new COMDATs are own leaves or shared-header inlines. The owners whose names also appear
  (0x008E8CB0 dtor, 0x009C11A0 C2DExtentsI copy ctor, 0x00405C10 placement new) were compiled for the context link.
- Landing link (21 objects) and context link (161 objects): the only LNK2005s involving a landing object are the CEngineLandscapeMemoryManager
  four (FreeVirtual / DeleteIndexBuffer / DeleteVertexBuffer / DeleteTexture) among 0x00BD9B90, 0x00BD9CE0 and 0x00BDA150, which the HEAD
  objects of the same three rows reproduce (`logs/link_control_head.log`); already queued as debt at nf139.
