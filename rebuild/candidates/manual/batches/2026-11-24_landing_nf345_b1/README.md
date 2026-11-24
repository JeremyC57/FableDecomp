# Landing nf345 b1-b3 (Account A, 2026-11-24): leaf-vs-table conflicts with a stale table line (agent NF345)

Agent NF345 took the 143 census rows from `naming/audits/2026-11-24_leaf_table_nf344/`. Its hand-back is in
`pending/2026-11-24_nf345_handback/` (NOTES, STATUS with one line per row, evidence). Results:
- 5 rows respelled;
- 4 table fixes;
- 23 skipped-port;
- 111 research, of which 84 were not real conflicts: 45 `_$E2` / `_$E3` statics, which are local to each source file, and 39 exception
  funclets, which correctly carry their parent's name. The patched census_leaf.py labels both kinds; it is in agent_tools/nf345/.

## Landed (all RELOCATION_MATCH PASS before and after; objsec ALL_EQ; link clean; both pick-any checkers show no new NEQ copy; 5 NEQ copies fixed)
- **b1 U1**: 0x0063B890 / 0x0063B920 / 0x0063B9B0 / 0x0063BA30 swap their CWorldMap::PointIsWithinCell{TR,BL,TL,BR} names. Four sources
  agree: the ego_r call order, object order and sizes, and the fadd pattern. The coordinator renamed the files and the catalog entries
  to match: TR, BL, TL, BR in address order.
- **b1 U2**: the caller 0x0059697A is respelled to SetCameraBowYZInverted. The table moves SetCameraYZInverted from 0x0040D2F0 to
  0x0040B050. That line is witnessed by HEAD row 0x0041E6D3; it was applied separately in `TABLE_ADDS_HEAD_WITNESS.tsv`, because
  land_scripted applies only lines whose witness lands. This change makes 0x0040CCA0's copy EQ.
- **b1 U3**: the callers 0x004FEDE0 / 0x004FEEC0 are respelled to SetMapAsUnloaded. The table entry at 0x00640430 changes from RemoveMap to
  SetMapAsUnloaded, and RemoveMap is added at 0x006403D0, witnessed by HEAD row 0x004FF8B0.
- **b3**: 0x0042B587 _M_create_node of map<CCharString, unsigned long> (was `long`). The pair copy ctor at 0x004297DB is respelled with it.
  The coordinator renamed the file and the catalog entries to the unsigned long spelling.
- **b2** (`2026-11-24_landing_nf345_b2`, D1): the binders 0x0041E6D3 (PCSetToLoad -> LoadGame) and 0x00E1A990
  (Init -> InitialiseVariables) take their PDB names.
  - Coordinator ruling D1 extends NF302 Q2. Before the change each reference resolved to the WRONG landed body (0x0047DD9D, 0x00E1A870),
    a silent wrong link. Now it is unresolved until the owner is ported, which a link shows.
  - The owners 0x0047D008 / 0x00E1AD30 are queued as PRIORITY NAME_FIXES (nf345).

The coordinator rechecked every changed row from the repo after the renames: RELOCATION_MATCH PASS.

## Queued
- **D2**: 0x00643D8C (self respell) and 0x00437990 (forwarder port onto engine/CPlayerGui.h) land as one unit: NAME_FIXES nf345.
- **D3**: 10 identity pairs where two landed selves define one symbol: T3 nf345 `identity research`. Also 0x0085E200 with 0x00B3E390: a port.
- NAME_FIXES nf345: 0x007FC800 EndAttackBeam and 0x005EEDCF AddLearnedExpression.
