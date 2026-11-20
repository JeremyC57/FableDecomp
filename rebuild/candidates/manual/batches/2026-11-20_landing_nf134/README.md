# 2026-11-20 landing nf134: PRIORITY slice 9 third pass (14 rows)

Records: `naming/2026-11-20_agentNF134/` (selfcheck 0 problems). 9 entries: q4650 done; q6180 / q6181 done except 0x009B08C0 (landed with
nf133 on the HEAD stand-ins; queued); q6182 blocked (0x009FF590 port with its GraphicDataBank callees; the 0x00994930 self rename waits);
q5021 partial (0x00956FE0, 0x009551A0 left); q4993, q4994, q5140, q5030 not started. The open entries went to NF137.

## Rows (14): 14 RELOCATION_MATCH, every test PASS (`logs/recheck.log`)
- q4650: 0x00B5E700 onto engine/CShaderRenderManager.h with the shared CScopedShadersEnable / CScopedVertexShaderConstantLayout; the self row
  0x0098B5E0 rewritten on that header (leaf-only rename). All 17 referencing rows bind `?SetVertexShaderConstantLayout@CShaderRenderManager@@QAEXJ@Z`.
- q6180 / q6181: 0x009D0380 and 0x009D4C30 (held at nf131) ported onto the shared CAFile.h / CDiskFileWin32.h / CFileDataInputStream.h,
  no local re-base.
- q6182: 0x00A62120 binds `??0CMemoryDataInputStream@@QAE@PBXK@Z`.
- q5021: 0x00951B20, 0x00967A20, 0x0093A5C0, 0x0093A8C0, 0x00957A20, 0x00956380, 0x00920730, 0x0091F0D0, 0x009211D0 bind the PDB
  AIAction_PerformActionAndWait. 0x00920730 / 0x0091F0D0 / 0x009211D0 also change their own Update leaf UAE -> MAE (both PDBs; no landed
  row references them). 0x0091F0D0 / 0x00920730 move onto the shared CCreatureAction_* headers. 0x009211D0's header-less local
  CCreatureAction_BeetleLand / _BeetleTakeOff now derive from the shared CCombatActionBase (RTTI).

## Headers (add-only opt-in; the dropped lines are FABLE_STATIC_ASSERT lines pinned by pin_asserts)
engine/CVertexShaderConstantLayout.h FABLE_RETAIL_CVERTEXSHADERCONSTANTLAYOUT_GETLIGHTARRAY_INLINE (used by 0x0098B5E0; no landed includer
defines it).

## Checks
- Split gate (GOTCHAS nf120 line): three universally referenced targets dropped from the recompile, each already bound under its new spelling
  (0x0099A2E0 8 -> 8, 0x0062E900 4 -> 4, 0x0099A2F0 2 -> 2). 267 rows rechecked, 260 good, 7 pre-existing bad (`logs/includer_recheck.txt`);
  gate 0 created splits; bindings_multi_check 1.
- Table: 0x00987D70, 0x0098B420, 0x0098DA90 go 0 -> 1: row 0x0098B5E0 bound unrecorded stand-ins at HEAD and now binds the PDB spellings
  (one spelling each). Shrank: 0x0097DB70 9 -> 3, 0x00994700 3 -> 1, 0x00994780 2 -> 1, 0x00987FE0 / 0x009880E0 2 -> 1, 0x0098B5E0 2 -> 1,
  0x00994930 3 -> 2, and 0x008BB6F0, 0x013BC470, 0x00988140, 0x00988190, 0x00988110, 0x00988290.
- One-definition (`logs/symbol_diff.txt`): new symbols are own leaves, shared-header inlines and COMDATs (CAFile, CBaseClass,
  CShaderConstantRange vtables / destructors are all COMDAT sections, the same set the landed includer 0x00B3C5D0 defines), and the
  implicit virtual destructors of the header-less Beetle classes (bound nowhere). Link test over the 14 objects + 0x00B3C5D0 (`LINK=`
  cleared): no LNK2005.

## Coordinator answers (advisor rules, owner delegated)
- 0x00994930 rename (q6182): stays blocked until its last binders move; the three old no-Leaf landscape rows are ungraded and queued.
- 0x00920730 / 0x0091F0D0 keep their derived state-group tokens on the shared CAIStateGroupBase (no header for those classes): narrow rule.
- 0x009211D0's virtual implicit destructors for header-less local classes: accepted (pick-any, nothing binds or defines them).
