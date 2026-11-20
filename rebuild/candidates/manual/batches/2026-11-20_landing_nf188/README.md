# Landing nf188 (Account A, 2026-11-20): landscape / water / stream binders (27 rows)

Agent NF188 (round 10) handed back early (context) with 64 entries: 6 done, 3 done-already, 10 blocked and 45 open (not reached;
each has its reloc_bindings census). Its files are in `naming/2026-11-20_agentNF188/`; the pulled-back 0x00C1ABB0 draft is in
`held/`. The open and partial entries go back to the queue unclaimed (48 entries).

- 27 rows land, all RELOCATION_MATCH, every test PASS.
- Most rows (15 + 2) drop row-local CMemoryDataInputStream / CDataInputStream / CBaseClassNonCopyable models for
  engine/CMemoryDataInputStream.h, or for the new candidates opt-in FABLE_RETAIL_CMEMORYDATAINPUTSTREAM_LOD_SHARED.
- Self renames (6): 0x005CC300 Transfer<CDefIndex> (explicit specialization on engine/CPersistContext.h, test rewritten), 0x00BEB490 the
  CWaterPatchMesh ctor, 0x00BF5A30 CWaterPatchMesh::CreateFromStream, 0x00BF6930 GetEdgeStrip, 0x00C22840
  PKCFrallocStatic<CSoundEvent>::Return, 0x00EE9590 CExpression_FishScript::OnPersist.
- Headers (add-only, `orig` equals HEAD, default paths token-identical, no landed includer defines the macros; asserts pinned are in
  `naming/.../hdr/`):
  - engine/CLandscapeBackgroundTreeNode.h GETEDGESTRIP (7 asserts pinned)
  - engine/CLandscapeBackgroundSubNodeSplitBranch.h SPLIT_BRANCH (4 pinned)
  - candidates/landscape_background_lod_compiled.h CMEMORYDATAINPUTSTREAM_LOD_SHARED
  - engine/CWaterPatchMesh.h CREATEFROMSTREAM
- Landing fixes:
  - The split gate first reported one CREATED at 0x00BF5A30. Its `// Leaf:` was the bare name `CreateFromStream`, so the gate counted
    two spellings for one row. The Leaf is now the full mangled name; both affected rows were rechecked
    (`logs/recheck_fix_00bde860_00bf5a30.log`) and the gate reads 0 created (GOTCHAS line added).
  - 0x00BDE860 needs the shared CMemoryDataInputStream::ReadConstData inline kept out of line (DIFFER 688v752 without a flag). Its flag
    is reduced to `/d2inlT91` with an `// Inline-budget:` line: T90 differs, T91 grades RELOCATION_MATCH. The catalog CompilerFlags are
    synced.
- Coordinator calls (advisor decisions, owner delegated):
  - The libpk entries (q4892 q4893 q5057 q5108 q5172 q12165) are deferred under the GameCODA / libpk vendor rule (light renaming only).
  - 0x00C22840's `V` key for CSoundEvent is accepted: the libpk headers declare `class CSoundEvent`.
  - The GETEDGESTRIP opt-in includes the quarantined engine/CEngineLandscapeMap.h. Nine landed rows already include it; accepted, and
    its promotion is queued as debt.
  - 0x00C2C750 models PKCDeferredBank locally (no shared header exists); accepted and queued as debt.
  - 0x00BEB4D0 `??_GCWaterPatchMesh` is queued as debt.
- Pick-any (`logs/pickany_check.txt`): the 17 DIFF lines are copies of `??1CDataStream` and its neighbours that do not own their
  address. The table lines come from other rows' stand-in binders; the rows bind ??1CDataStream at 0x00993B10, whose body stores the
  CDataStream vtable (ownership check, EXTRA_RULES pick-any section). By hand: 0x00BF6930's GetPatch / GetSplitBranch copies equal the
  owners 0x00BF6130 / 0x00BF6160 (36 = 36 bytes each).
- Link (`logs/link_landing.log`): 8 LNK2005, the same symbol set as HEAD (`logs/link_control_head.log`).
- Recheck (`logs/recheck.log`): rebind multi-check 1 (baseline); split gate 0 created after the Leaf fix. Includers and referencing rows:
  the 4 bad rows are the July landscape rows at their HEAD grades (0x00BDC510 1044v1131, 0x00BE7D70 1437v2983, 0x00BE8920 328v365,
  0x00BF3B60 825v1480); none got worse. Owner recheck good.
- tablecheck (`logs/tablecheck.txt`): five addresses shrink (0x00993B10 4 -> 2, 0x009964E0 5 -> 4, 0x00C14A50 2 -> 1, 0x009E84D0
  2 -> 1, 0x00993C10 3 -> 2), none grows. No out-of-image oracle line added.
