# Chunk 3014 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- (c2 rename rotation) A split read-modify-write temporary (`mov r,[m] ; or r,imm ; mov [m],r`, 0x00B65E5A) gets its register from a
  per-region ROUND-ROBIN, not the allocator nor the 0x10775ACC expression temporaries: c2.dll 0x10785179 returns the next register
  after the global index [0x108b022c] in the table 0x108a9738 (eax ecx edx esi edi ebx ebp), skipping busy ones; the index resets to 7
  at each scheduling region, and the region's tuples are visited in REVERSE (cmp, push, add reg,imm, or, inc, dec all advance it).
  The register therefore depends on how many such tuples follow it in its region. Hook: agent3014 tools/sched/t18.py + rr.py.
- (c2 scheduler) Ready test is c20 (remaining preds) == c24 (remaining "soft" preds); RAW edges from `add reg,imm` to a load through
  that register are soft: the load may issue first with the displacement folded (`mov ecx,[eax+0D0h] ; add eax,0D0h`), and loads get
  +32 priority (fl&4), so a load whose only outstanding pred is the add always overtakes it (0x00B65E94..0x00B65E9C).
- (0x00B65A20) Every local spelling of a C4DVector temporary built from one float (ctor, named, C3DVector + w, member-wise) and every
  reference / pointer / block-scoped form of a 16-byte colour copy yields the identical c2 DAG: c2 canonicalises them; dead names
  (enums, unused references) do not move this row's ties.

- (0x00B65A20, c2 rename walk) The round-robin split register is decided by which tuples of the region the reverse walk VISITS: only
  arithmetic opcodes (class flag 0x400: cmp / push / add / or / inc / dec), never opcode 18 (the 3-address address form emitted as `lea`
  or `add r,imm`). A struct copy from `arr[0].Member[12]` through the plain `_M_start[i]` operator[] (or a named reference) makes the
  address a visited two-address `add`; FABLE_RETAIL_CARRAY (operator[] through begin()) makes it opcode 18, which also turns the soft
  add->load scheduler edge into a hard one. Measure the opcode with agent3014 tools/walk2.sh / w2s.sh before trying spellings.
- (0x00B65A20) Three tail residues (a dead-store tie, a split RMW register, an add/load order) had ONE cause in the IL of an expression
  far from two of them: try container-access opt-ins (FABLE_RETAIL_CARRAY) on the direct spelling, not only on the spelling that
  matched most instructions.

Review at landing:
- 0x00B65A20 RELOCATION_MATCH needs the c2031 overlay headers (hdr/) plus the existing FABLE_RETAIL_CARRAY opt-in.
- 0x00B70A90 CEngineWaterRenderer::RenderEnvironmentMap (unlanded) is the same-shape sibling of 0x00B65A20's colour tail.
