# 2026-11-17 landing nf48: name-fix round 7, NF48 (86 rows: STLport vector members reached through local stand-ins)

Owner: "Continue the naming fix queue with two agents until complete".

NF48 took 16 entries from NF41's half, 87 binder rows from NF41's probe. They are STLport `std::vector<T>` members:
- operator= for float, long, CDefString, CArenaRoundDef, CConversation and CFireHeartPatternDef;
- erase for CEnginePrimitiveHandle, CCharString, CScriptThing and EHeroAbility;
- resize, ~vector, push_back and reserve.

The callers reached them through local stand-in containers (`CFloatVector`, `COpinionSet`, `Erase`, `EraseRange`, `Destroy` ...).

A container restart killed the first run with 48 rows done. The restarted agent resumed from its own files and ran the final
recheck over every changed row. Brief, worklist and records: `naming/2026-11-16_agentNF48/`.

## Rows (STATUS.tsv)

| Status | Rows |
|---|---|
| ported | 86 (83 worklist rows + 3 callers of renamed rows: 0x00891530, 0x00747B00, 0x00E81D10) |
| not-a-binder | 2 |
| blocked | 2 |

**Blocked rows:**
- **0x007E7C90:** 0x005ED940 must be identified first.
- **0x009CEAE0:** DIFFER(373v375) after 2 grades, with every vector binding correct. Its stand-in `CRuntimeData` has a zeroing
  constructor, but `CBankFile::CRuntimeData` in engine/CBankFile.h is a POD. That constructor must be proven and added to the header
  first.

## Entries (APPLIED.tsv)
- 13 done.
- 3 blocked: e1 and e28 by 0x009CEAE0, e67 by 0x007E7C90.
- The STLport spellings are descriptive-by-library (both PDBs are Dinkumware builds).

## Rows and checks
- **Self renames (section 1, 16 rows; 16 file moves):**
  - `vector<CScriptThing>::clear`, `vector<CCharString>::clear` and `vector<CCharString>::resize`;
  - `CArray<CCharString>::DeallocateMemory` 0x009D3FB0. It was catalogued as `~CArray`. Both PDBs name DeallocateMemory, and its only
    retail callers are CBankFile::CloseEntries / AllocateBuffers.
  - corrected signatures for the rest.
- **Section 2:** 113 respelled bindings. For the first run's 45 rows it lists only the worklist targets (NOTES.md).
- **Inline budget:** 0x00EDD060 and 0x00D18B20 gain `#pragma inline_depth` regions. These replace their old hand-written
  `DestroyInline` copies of the destructor. Each row carries an `// Inline-budget:` line with its evidence, the established form
  (233 landed rows); ASM_RULES says a pragma needs no portable twin.
- **Recheck (`logs/recheck_part0..2.log`):** all 86 graded from the repo after landing. All 86 are RELOCATION_MATCH, every test
  PASS.
- **Checks:**
  - funclet_includes_check reports 1,355 checked, 0 broken;
  - `naming/rebind.py` removed 131 binding lines and added 65;
  - bindings_multi_check is at its baseline (`NullCoord`).

## Headers (opt-ins only; default path unchanged)
- **stlport/fable_containers.h:**
  - `FABLE_RETAIL_STLP_VECTOR_DTOR_OUTOFLINE` makes `~vector` a declaration only;
  - `FABLE_RETAIL_STLP_VECTOR_DTOR_FORCEINLINE` keeps the same body but makes it `FABLE_FORCEINLINE`, like the header's existing
    `_UNINIT_*_FORCEINLINE` opt-ins.
- **engine/CTCAIScratchpad.h:** `FABLE_RETAIL_CTCAISCRATCHPAD_GOSSIP_MAPS` declares the static maps Gossip / GossipVillages /
  GossipFactions. hdrcheck2 cannot preprocess this opt-in standalone (cpp-err), but the default path is unchanged and the rows that
  define it compile and grade.
- **Overlay safety:** NF48 regraded one unrelated includer of each header under the overlay: 0x006B3B80 is RELOCATION_MATCH and
  0x007ACDD0 is MATCH.

## Name fixes queued (`land1116fu`)
- **0x00443F90 `vector<CEnginePrimitiveHandle>::resize`:** 8 landed callers outside the slice still bind stand-ins: 0x0064E1E4,
  0x0064ED06, 0x0064F6DC, 0x007E57C0, 0x00AFE9B0, 0x0043B050, 0x0072DC70 and 0x00BC65D0.
- **0x00433273 GFSerialiseVectorBinaryIn<CCharString>:** still binds a stand-in resize. Porting it changes its own leaf.
- **0x0143E90C pGlobalScriptDef:** four spellings in reloc_bindings.
- **0x013BAE44:** CTCAIScratchpad::Gossip, which one row binds as `g_gossipVillagesMap`.
- **0x00747B00 IsAvailableForAttack:** const in both PDBs.

## Rule slips
Three read-only or pre-write Bash calls exited non-zero. No git, nothing written under the repo, one compile at a time.
