# Landing nf199 (Account A, 2026-11-20): CCombatManager map finds, CCharString ctor / Compare binders (27 rows)

Agent NF199 (round 10) handed back early (context) after q6341 with 50 entries: 1 done-already, 6 blocked or partial, 2 deferred, and
41 open (not reached). Its files are in `naming/2026-11-20_agentNF199/`. Most entries go back to the queue unclaimed (48): the open ones;
the partial families q4389 (9 rows left) and q4456 (4 binders left); q4394; q6315 / q6319, whose binder was NF198's and has since landed;
and q6332 / q6341, where the existing fable_containers.h opt-ins are still untried. q4451 stays blocked for a family pass (82 binders of
varied shape left).

- 27 rows land, all RELOCATION_MATCH, every test PASS. No self rename, no file move, no header change.
- q4389 (7 rows): 0x006E9960, 0x006E9360, 0x006E9390, 0x006ED9E0, 0x006EB390, 0x006E97F0, 0x006EB600. Each swaps its stand-in map
  (SCombatantMap / CCombatantMap / CombatMap / CRetailRbMap_UID_CombatantInfo) for `std::map<unsigned __int64, CCombatantInfo>` with
  fable_map.h MFIND_DECL, and binds 0x006F0250's self Leaf exactly. 0x006ED9E0 also drops its local CCombatantInfo for
  engine/CCombatantInfo.h. The tests of 0x006E9360 / 0x006E9390 are rewritten on fable_map.h.
- q4456 (17 rows): the CCharStringMember stand-in rows, plus 0x004D3826, 0x009AA240 and 0x009A9930, move onto
  `??0CCharString@@QAE@XZ` from fable_string.h. The CCharStringMember spelling is gone from the tree.
- q4451 (3 rows): 0x0099E900, 0x006A7F30, 0x00A14EE0 drop their local CCharString models for fable_string.h (FABLE_RETAIL_CCHARSTRING)
  and bind `?Compare@?$CBasicString@D@@KIJPBD0@Z`.
- q6314 is done-already: 0x009A0EC0 binds `?AlphaMaskUVAdd@CStreamingFontBank@@2VC2DVector@@A`.
- Blocked rows left in the families: 0x006EA770 differs 796 v 799 after three forms (it keeps `this` in ebp, not esi). 0x009CB910 /
  0x00A16D50 are constructor bodies landed under other names, and identifying them is queued.
- Pick-any (`logs/pickany_check.txt`): `??8?$CBasicString@D@@QBE_NABV0@@Z` in 0x0099E900 / 0x006A7F30 / 0x00A14EE0 equals the owner
  0x004A93C0 (34 = 34). Every other new symbol is a shared STLport header inline with no owner row (agent check by table, exact and
  long-prefix Leaf).
- Queued: the stale table line naming 0x006E9360 as the witness of `?FindNode@CUIDMap@@` at 0x006F0250 (no landed row declares CUIDMap).
- Link (`logs/link_landing.log`): 0 LNK2005, the same as HEAD (`logs/link_control_head.log`).
- Recheck (`logs/recheck.log`): rebind multi-check 1 (baseline); split gate 0 created. Gate and referencing rows 28 good, 0 bad. The
  shared-header callee set has 917 referencing rows, so the run used the universal-binder exclusion.
- tablecheck (`logs/tablecheck.txt`): two addresses shrink (0x0099E4B0 8 -> 5, 0x006F0250 9 -> 4); none grows. No out-of-image oracle
  line.
