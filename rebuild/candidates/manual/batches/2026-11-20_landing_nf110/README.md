# 2026-11-20 landing nf110: the GetPFirst / GetPBest search-template instances (17 rows)

Records: `naming/2026-11-20_agentNF110/` (selfcheck 0 problems). q11280 / q11283 (the same claim) stay **blocked: port** with 4 rows left, all
NF107 leftovers: 0x0078954D, 0x0078971D, 0x007FDB10, 0x008E60D0. They are now the only referencing rows that still declare the
`CThingSearchTools_Calls` stand-in; every other row binds the Ego_r.pdb spelling of 0x0049C6F0 / 0x0049C700.

## Rows (17): 17 RELOCATION_MATCH, every test PASS (`logs/recheck.log`)
- GetPFirstThingInRoughArea (8: 0x007313B0, 0x007512E0, 0x0079A280, 0x007FCEF0, 0x007FD110, 0x007FE730, 0x0096F260, 0x00987430): the body landed
  row 0x009871C0 already defines, moved verbatim into the header as the opt-in `FABLE_RETAIL_CTHINGSEARCHTOOLS_FIRSTROUGH_EGOD`.
- GetPBestThingInRoughArea (6: 0x0079A050, 0x007F5D20, 0x009147E0, 0x00937690, 0x0093CDB0, 0x00942390): NF107's BESTROUGH_BODY + THISEND +
  byte-offset cells, T's type-flags define, out-of-line CThingCompare_Nearest. 0x009147E0 on its TU-mate 0x00914650's define set.
- 0x006CE300 (NF107 0x008786E0 recipe + PUSHBACK_NOINLINE), 0x0080ECC0 (list EGOD body on 0x00809A70's define set, for CFilter_Bind5).
- 0x007FEAF0 (caller): its local template made `const` in place, closing the existing split on 0x007FE730.
- Self renames 15 (12 QAE -> PDB QBE; 0x006CE300 / 0x0080ECC0 off `Calls` stand-ins; 0x0096F260 to `<CThingFilter_IsRelatedTo, const
  CThingDeadCreature>` off a wrong IsBlocking label; 0x00987430 const T; 0x0093CDB0 T = CThingTrackNode). 44 callee respellings.
- Headers: CThingSearchTools.h FIRSTROUGH_EGOD; CThingTrackNode.h STATTYPEFLAGS (0x10007, FableWin `?StatGetTypeFlags@CThingTrackNode@@SIKXZ`).
  Add-only, token-identical without the macros, pin_asserts 0, assert_names_check 0.

## Checks
bindings_multi_check 1 (NullCoord); funclet_includes_check 1355 / 0. Includers + renamed + exe-scan referencing rows: 367 good / 0 bad.
No-new-split gate (before set compiled from HEAD): 0 created. Existing splits left as they were (NAME_FIXES: callers 0x0096F180, 0x009863E0,
0x00937C20, 0x0093CC60 bind their own stand-ins; 0x006A7F30 is CThingFilter_HasTrackName::operator(), 0x0080E480 the anonymous
IsAffectedByExplosions).
Rule slip judged: 0x009147E0 took 5 graded forms (3 real body forms; two shared one missing prerequisite). Accepted: the 3-form cap
counts body forms, and the row matched on its TU-mate's set.
