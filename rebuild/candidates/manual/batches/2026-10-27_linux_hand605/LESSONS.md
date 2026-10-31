# Chunk 605 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- RTTI before the manifest for CPersistContext_TransferableVector rows: read the vtable's type descriptor (vtable-4 -> COL ->
  TD); 0x0124F33C is `<long>` although the manifest says `<ECreatureGeneratorGenerateType>` (0x0057F70F).
- A traits helper that a size-optimised (`optimize("s")`) TU leaves out of line while retail inlined it wants `__forceinline`;
  forcing it also fixed the residual load scheduling (index before m_pFirst) that five operator[]/local/accessor spellings could
  not (0x0057F70F). For T = long, `(long)value` on a long lvalue makes no temporary; a named `long temp = value;` inside the
  helper gives retail's copy homed in the dead index slot.
- Reuse a landed sibling's model before tuning: the libpk SSE kernel row 0x00C88CD0 matched on the first grade with the landed
  CMixKernelsSSE `F32vec4` model and `*d++ += *s++;` after a parked raw-intrinsic form failed on the prologue order.
- A float functor field that retail promotes across a for_each loop: reading it into a float local, selecting into the local and
  writing it back unconditionally turns the integer `mov` copy into an FP store and removes the dead fxch pair (0x00754D80,
  17 -> 9 lines); the loop-level hoist itself was not reachable.
- Re-prove parked names: two parked rows carried wrong spellings that their substring `// Leaf:` hid (0x006AFE60 public
  pointers vs the PDB's protected const references; 0x0052EA30 `struct C2DVector` = ABU vs ABV, and `CFrontEndManager` /
  `ConvertX` callees). Always use a full mangled leaf.
- 0x0041E5F2 is NUISystem::CManager::GetInstance (the singleton's ctor stores .?AVCManager@NUISystem@@) -- eleven different
  spellings are bound to it in reloc_bindings.tsv.
- Pure scheduling residues that no spelling moved (park after 2-3 grades): epilogue `xor eax,eax` vs `pop edi` order
  (0x0070C4E0), memset count vs preceding member store (0x00C75F80), store sinking past a heap load (0x00A852B0), 8-byte copy
  load/store pairing around `sub esp,8` / push (0x0052EA30), x87 commutative operand choice (0x006AFE60).

Review at landing:
- 0x00C88CD0 matches but sits in name_review/ (descriptive libpk name CMixKernelsSSE::Add, already declared for this address by
  six landed siblings); move to src/ if the descriptive-libpk rule that landed the siblings applies.
- 0x0057F70F's source emits an unreferenced COMDAT `?TransferOut@?$CPersistTraits@J@@QAEX...ABJ@Z` (the forced-inline helper);
  the leaf is the TransferableVector symbol, so only that is graded/landed.
- NAME_FIXES.tsv: respell the landed callers of 0x009E2560, 0x0041E5F2, 0x0041CF47, 0x0052E580, 0x0052E9C0, 0x0070C4E0.
- 0x00C52BB0 has no cpuid (it only reads cached CPUID fields): not a retail_asm candidate.
- Rows that needed more than 6 grades: 0x00754D80 (11), 0x00C52BB0 (9), 0x006AFE60 (9), 0x0070C4E0 (8), 0x00C75F80 (7).
