# Landing nf284 (Account A, 2026-11-22): CCountedPointer DeleteData binders, continuation of NF282 (31 rows)

Hand-back: `pending/2026-11-22_nf284_handback/`. Tracker batch T1 standin_alias (NF282's ENTRIES, its work list lines 61-91). No header,
flag, test or catalog change.

- 31 `CCountedPointer<T>::DeleteData` targets, each with one stand-in binder: a generated Reset row (local `SCountBlock` model, free
  `?Destroy_<addr>@@YIXPAX@Z`). Each binder moves to the landed opt-in `FABLE_RETAIL_CCOUNTEDPOINTER_DELETEDATA_DECL_ONLY` +
  `engine/CCountedPointer.h` (the hybrid form ruled for NF282), the count block's initialiser `&CCountedPointer<T>::DeleteData`.
  Targets: 0x00B2DE20, 0x00B36060..0x00B361B0 (22, the CEnginePrimitiveRenderer managers), 0x00B4B020..0x00B4B060 (5), 0x00B94F90,
  0x00B95000, 0x00BB6330.
- Proof (hand-back `evidence/`): objsec HEAD v new ALL_EQ under the one rename per row (`.drectve` included); both function COMDATs SAME;
  no tables. Two sources per spelling: the FableWin.pdb name set (31/31) and the retail binder body storing the target as an imm32 (31/31),
  T tied by the binders' real-typed callers (31/31 agree). No direct call / jump sites to any target. Link: HEAD leaves all 31 `Destroy_`
  references unresolved; the new set resolves 30, and the 31st (`CCountedPointer<CEngineSceneGrid>::DeleteData`) waits on 0x00B94F90's
  self row (it still defines the AutoTinyVirtualObject stand-in). 0 duplicates.
- Each row gains a pinned `// Leaf:` line (the binder's own stand-in Reset leaf, NF282 ruling D3).
- Table: 31 `?Destroy_<addr>` lines deleted, 30 PDB lines added (0x00B94F90 already had its PDB line, witness 0x00B94540).
- NAME_FIXES: 32 T3 ports (the 31 binders' self names onto `?Reset@?$CCountedPointer@V<T>@@@@QAEXPAV<T>@Z`, FableWin; and 0x00B94F90's self
  row), queued `land1122nf284`, not respelled here.

## Rulings on NF284's questions
- D1: 0x00B94F90's self-row port is queued as a T3 unit with NF282's four AutoTinyVirtualObject rows (0x0067F650, 0x00A110D0, 0x00A99FE0,
  0x00A9CB20).
- D2: standalone tests that define stubs under stand-in names stay for now (a later test pass).
- D3: an address bound under two stand-in spellings, one of them with no table line (0x009E1DD0 `GFAYToVY_009e1dd0` `YG` and `YI`), is one
  entry; both are fixed together.
- D4: the continuation (NF285) works in ENTRIES order from line 1; the 12 deferred CCountedPointer entries (nested / template T, multiple
  binders, ReleaseData / dtor stand-ins) are a later T3 unit.

## Recheck
`logs/recheck.log` (landed tree, 31 rows): 31 RELOCATION_MATCH PASS, 0 DIFFER, 0 RELOC_CONFLICT. Rule slip: one early command wrote a
harmless file outside the agent folder (`/tmp`); noted, the continuation brief repeats the rule.
