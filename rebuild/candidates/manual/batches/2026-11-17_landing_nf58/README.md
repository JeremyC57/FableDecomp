# 2026-11-17 landing nf58: name-fix round 7, NF58 (121 rows: STLport tree / list stand-in self names)

Owner: "Continue the naming fix queue with two agents until complete" and "Keep going until queue is done".

NF50 fixed the `_Rb_global<bool>::_Rebalance` binding in 79 rows, but 78 of them kept stand-in self names such as
`CRetailTree_longkey_buy004e4fef::Insert`, `CRbTree::_M_insert`, `CThingMap64::Insert` or `list_T_00a81470::~list_T_00a81470`,
which CLAUDE.md bars in landed code (queue entry q10437). NF58 took that family: 76 self rows (0x0076A110 / 0x00769980 went to
NF57) and their landed callers, 163 rows. Brief, list and records: `naming/2026-11-17_agentNF58/`.

## Rows (163, all terminal; STATUS.tsv)

| Status | Rows |
|---|---|
| ported | 121: 113 self renames + 8 hinted-insert callers whose local `_M_insert` became private (they now bind AAE, not QAE) |
| not-a-binder | 25: the landed copy already binds the new spelling (probe compile each) |
| blocked | 17 (unchanged) |

**How the self rows were named.** Each is now an explicit instantiation of the STLport member from `stlport/fable_map.h`:
`_M_insert` (60, the header's ORIGINAL_INSERT_BODY), `insert_unique` (51, including the hinted form and the /O2 out-of-line
iterator variants), one `insert_equal`, and 0x00A833B0. The key and value types come from the PDB member each caller fills
(Ego_r.pdb / FableWin.pdb struct layouts and `_Rb_tree<...>` name sets) and agree with the node size `_M_create_node` copies.
- 0x00A833B0 is `CNavRegion`'s scalar deleting destructor (was `CCountedPointerListOwner_00a833b0`). It is named from layout: the
  body destroys a `list<CCountedPointer<CNavRegionConnection> >` at +4 after a trivial word at +0, and Ego_r.pdb's only class with
  that member there is CNavRegion. No caller names it.
- Type correction (REJECTED.tsv): 0x00CD300B / 0x00CD3093 / 0x00CD34C5 are `map<int, CCharString>`, not long. The PDB's
  PlayCutsceneSound names that type, and map<long, CCharString> already lives at 0x00472A05.

**Blocked rows:**
- **No caller or data reference anywhere in Fable.exe (authoring):** 0x00A81470, 0x00A82230, 0x006311D0, 0x00631420. Each pair has
  identical bodies at two addresses, so `~_List_base` vs `~list` (or `~_Rb_tree` vs an owner's destructor) can not be told apart.
- **Map type unproven (authoring):** 0x00817D70 / 0x00818000 and 0x008B29A0, 0x008B2A60, 0x008B2B20 with their callers 0x008B2DD0,
  0x008B2E90, 0x008B2F50: global handler maps that no PDB names.
- **0x006DCB20 (port, DIFFER 227 v 198):** retail builds the node out of line on one branch and inline on the other, which the header
  body can not do (the same open problem as NF50's 0x0076A110).
- **0x00869020 (port):** models nine classes locally (CArmourDef, CThing, CHitParameters, ...). Its callee 0x00869CA0 is held back
  with it.
- **0x009EFB80 (port, DIFFER 202 v 254):** needs an opt-in in the root `fable_string.h` that keeps `CBasicString<char>::operator<`
  out of line but visible; agents do not overlay that header. Its callee 0x009EF750 is held back with it.
- 0x00869CA0 and 0x009EF750 already grade RELOCATION_MATCH re-authored (NF58 `work/ready/`, not landed); they land with their
  callers' ports so the binding is not split.

## Entries (APPLIED.tsv, 76)
- **62 done**, including 0x0069D520, which NF56 had already done (landing nf56).
- **3 done-already:** 0x00A84110, 0x00485E45 and 0x00630FE0 already had their STLport names.
- **11 blocked:** 0x00A81470, 0x00A82230, 0x006DCB20, 0x00817D70, 0x00869CA0, 0x008B29A0, 0x008B2A60, 0x008B2B20, 0x009EF750,
  0x006311D0 and 0x00631420.

## Rows and checks
- **Section 1 (113 rows; 226 file moves for sources and tests).** Section 2: 160 respelled bindings.
- **Coordinator correction:** 0x00CD34C5's catalog module spelled the same instance as `pair<const int, CCharString>`, where its
  siblings 0x00CD300B / 0x00CD3093 use `pair<int const, CCharString>`. The coordinator gave it the siblings' spelling, so one
  instance has one catalog name (catalog, manifest and build entry; no file moved).
- **Funclets:** no EH funclet includes a changed row. funclet_includes_check reports 1,355 checked, 0 broken.
- **Recheck (`logs/recheck_part0..2.log`):** all 121 changed rows graded from the repo after landing: 121 RELOCATION_MATCH, every test
  PASS. NF58's own final run agrees. No test file changed.
- **Checks:**
  - `naming/rebind.py` removed 165 binding lines and added 110;
  - bindings_multi_check is at its baseline (`NullCoord`).

## Headers (5 opt-ins in 4 files; lines added only, default path unchanged)
Details: `naming/2026-11-17_agentNF58/hdr/README.md`.
- `stlport/fable_map.h`: `FABLE_RETAIL_STLP_TREE_INC_OUTOFLINE` (the `operator++` twin of the existing DEC_OUTOFLINE) and
  `FABLE_RETAIL_STLP_TREE_CREATE_NODE_NOINLINE`.
- `engine/CWeaponDef.h`: `FABLE_RETAIL_CWEAPONTRAILGRAPHICSET_CLASS`. Both PDBs spell CWeaponTrailGraphicSet as a class; the header's
  default `struct` mangles `U` (queued: make `class` the default once every includer is checked).
- `engine/CTCInventoryExperience.h`: `FABLE_RETAIL_SEXPSPENDUNDO_ONLY` (only the global SExpSpendUndo record, without the CTCInventoryBase
  chain the full model needs).
- `engine/CDefString.h`: `FABLE_RETAIL_CDEFSTRING_COMPARE_FRIEND` (the PDB-named `CCompareDefString<T>` reads the private TablePos).
- One unrelated includer of each overlaid header (0x0042CB08, 0x004ABC70, 0x00839100, 0x00576C0B, 0x0045CFAD) still grades
  RELOCATION_MATCH, PASS (NF58 `work/recheck_unrelated.log`).
- Local definitions to review: CBlendedParticleEffectSet (no shared header defines it), `CCompareDefString<T>` (PDB-named, no header),
  a declaration-only `std::less<CDisplayModeDesc>` specialisation, forward enum / class declarations. No class with a shared header is
  modelled locally.

## Name fixes queued (`land1117fu`, 85)
From NF58's NAME_FIXES.tsv (84) plus one coordinator line:
- 35 out-of-slice binders that bind a stand-in or non-STLport spelling (GFSerialiseMapBinaryIn's `map::insert`, CUndoMap, CKillMap,
  CPauseSet, NF50's leftovers 0x0047E07F, 0x00A7A7F0, 0x00A84640, 0x00A84BE0, ...). NF58 checked that none bound an old self spelling,
  so its renames split no binding;
- 49 landed callee rows: the per-instance `_M_create_node` "buyNNNN" helpers (some wrong only in access: QAE where STLport has IAE)
  and the per-instance iterator `operator++` / `operator--` rows;
- coordinator: the CWeaponTrailGraphicSet class key (make `class` the default in engine/CWeaponDef.h once every includer is regraded).

## Rule slips (NF58 `slips.txt`)
- Two Bash calls exited 1 (a helper on a file not yet written, a trailing grep with no match).
- A generator failed and the grades chained after it with `;` ran on missing files, wasting 2 grades on 0x006AE9F0 / 0x006DB610.
