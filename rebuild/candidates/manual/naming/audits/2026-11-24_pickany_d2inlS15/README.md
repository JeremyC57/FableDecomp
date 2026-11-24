# Pick-any audit of the 63 landed /d2inlS15 rows (Account A, 2026-11-24)

Asked for by B (COORDINATION_LOG 2026-11-24, "Possible pick-any debt in landed BanditCamp rows"). Read-only: every row was rechecked on
the landed tree (`recheck.py` + `vland.py --recheck-landed`: 63 / 63 RELOCATION_MATCH PASS), then each object went through
`pending/2026-11-21_codex_pass26/tools/pickany_vs_retail.py` against `rebuild/oracles/reloc_bindings.tsv`. `PICKANY.tsv` holds every
line: row, EQ / NEQ, the table target, the copy length and the symbol, and whether the row's own leaf references the copy. It holds no
bytes.

## Result
- 396 mapped copies: 293 EQ, 103 NEQ.
- **11 NEQ are referenced by the row's own leaf.** EXTRA_RULES 2026-11-21 would hold such a row at landing, and these rows are already
  landed, so this is landed debt:
  - `vector<CArray<long>>::resize` (0x00AFE910): rows 0x00AFCC30, 0x00B03CA0, 0x00B1F200;
  - `map<CCharString, CScriptGameResourceObjectScriptedThingBase>::operator[]` (0x00CD3D2E): rows 0x00D13580, 0x00D19BD0, 0x00D1A740,
    0x00D1B200, 0x00D22F30, 0x00D2D930, 0x00EC4A00;
  - `CPersistTraits<long>::TransferBinaryIn` (0x0040FE60): row 0x00472896.
- **B's two cases confirmed:** `~CScriptGameResourceObjectMovieBase` (0x006E7B80) differs in 14 objects and `NScript::~CIsThingAlive`
  (0x00E241B0) in 7. Neither is referenced by its leaf, but either would break its owner if the final link picked it.
- **The rest:** the ScriptedThingBase map / tree helpers (0x00CD27C7, 0x00CD344A, 0x00CD3699), `~CSpawnedFuncBase` (0x00CDD4C0),
  `ScriptedThingBase::operator=` (0x008ABD10), the `vector<CArray<long>>` helpers, and the CScriptThing copy constructor / `operator=`.
  The copy constructor is agent NF328's unit.

## What follows
- No fix is attempted here. The MovieBase and CIsThingAlive copies come from the headers B24A2 is working on (0x00D09010), so the fix is
  coordinated with B.
- The leaf-referenced rows need a row-level decision: re-author the inline body so the copy equals retail, or keep the copy out of the
  row's object.
- The full-link pick of the final build decides whether this debt bites. Until then it stays a queue item: T3 `pickany_d2inlS15`.
