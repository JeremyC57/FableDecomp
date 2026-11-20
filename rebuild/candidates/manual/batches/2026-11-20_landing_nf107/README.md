# 2026-11-20 landing nf107: 20 CThingSearchTools search-template instances on their PDB spellings (20 rows)

Records: `naming/2026-11-20_agentNF107/` (selfcheck 0 problems). NF107 resumed NF106's search-tools ports. 3 entries: q11280 / q11283 (the
same claim) stay **blocked: port** with 20 binders ported this round and 5 rows left (0x008E60D0 after 3 failed forms, best DIFFER(704v704)
on one SIB operand order; 0x0078954D, 0x0078971D, 0x007FDB10, 0x006CE300 not attempted); q11281 stays **open** (not started).

## Rows (20): 20 RELOCATION_MATCH, every test PASS (`logs/recheck.log`)
Each row is an explicit instantiation of the shared `engine/CThingSearchTools.h` templates (GetAllThingsInRoughArea,
GetPBestThingInRoughArea, GetAllTCDsInRoughArea) on the define set of its caller's TU. Self leaves move off the `CThingSearchTools_Calls`
stand-ins (18) and QAE -> QBE (0x00937080, 0x009595C0) to the Ego_r.pdb / FableWin.pdb instance spellings; second source is the landed
caller that already binds the PDB spelling (NOTES "Split check"). Callee respellings (RENAMES s2): 0x0049C6F0 / 0x0049C700, the STLport
vector `_M_insert_overflow` instances, six anonymous-namespace filter callees.
- Headers: 5 add-only `FABLE_RETAIL_*` opt-ins (`naming/.../hdr/README.md`): CThingSearchTools.h BESTROUGH_THISEND (+ byte-offset cell
  branch), CThingMarker.h STATTYPEFLAGS, CTCDExperienceOrb.h STATGETDRIVERTYPE, CTCCarrying.h ISTHINGCARRYINGMELEEWEAPON,
  CThingDeadCreature.h STATTYPEFLAGS.

## Checks
assert_names_check 0; bindings_multi_check 1 (NullCoord); funclet_includes_check 1355 / 0. Includers + renamed rows + exe-scan referencing
rows: 461 good / 0 bad. No-new-split gate (before set compiled from HEAD): 0 created (`logs/split_gate.txt`). The first eval reported 6
false CREATED: the HEAD rows carried no `// Leaf:` line, so the self old leaf was empty, and a Leaf line cut at an anonymous-namespace
`?A0x` hash counted as a second new spelling. gate.py now falls back to the RENAMES old leaf and collapses a spelling that is a strict prefix
of another; nf104 / nf105 re-evaluate unchanged (0 created).
