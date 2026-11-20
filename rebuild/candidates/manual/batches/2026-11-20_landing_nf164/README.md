# Landing nf164 (Account A, 2026-11-20): creature action / AI state group names (16 rows)

Agent NF164 (round 10, resumed after a container restart, then handed back early on a long slice) handed back 25 entries:
2 done, 8 done-already, 1 set aside for a lock, 14 open (q5101 and q5102 partly respelled). Its files are in
`naming/2026-11-20_agentNF164/`. The 15 unfinished entries go back to the queue unclaimed.

- 16 rows. q4516: all four binders of 0x013BA8BC use the private `0V?$map` spelling of CCreatureActionBase::RegisteredActionEvents.
  q5017: 0x0092D600 and 0x0092D7C0 change QAE to IAE in place. q5102: 10 rows respelled in place on their local
  CCreatureActionBase, and 0x008B93D0 ported onto the shared headers. q5101: 0x00849140.
- Self renames (2): 0x0121E030 / 0x01229280, the map's dynamic initializer and atexit destructor.
- Header: engine/CCreatureAction_ReleasePhysicalShieldSpell.h opt-in RELEASEPHYSICALSHIELDSPELL (the retail class on
  CCombatActionBase, from RTTI; used by 0x008B93D0). Its two existing asserts are line-pinned; `orig` equals HEAD.
- Advisor decision (owner delegated): 0x0121E030 and 0x01229280 each define the static, so together they give LNK2005 on
  RegisteredActionEvents (`logs/link_landing.log`). A TU that emits the initializer or the destructor has to define the static,
  so no row-level spelling avoids it. The landed NF89 pair 0x0121F0E0 / 0x01229BE0 clashes the same way at HEAD
  (`logs/link_control_head_nf89_static_pair.log`). Both rows land, with a DEBT line and an EXTRA_RULES entry (narrow scope:
  one static's initializer / destructor pair, landed together, no other definer).
- Recheck (`logs/recheck.log`): 16 RELOCATION_MATCH, every test PASS. rebind multi-check 1 (baseline); split gate 0 created;
  includers 41 good, 0 bad. tablecheck: 0x013BA8BC 4 -> 1, 0x00692940 8 -> 3, 0x0092D1F0 2 -> 1 spellings.
- Link: 7 LNK2005, the six of the HEAD control (`logs/link_control_head.log`) plus RegisteredActionEvents above.
- Queued: the DEBT line and 0x00846100's oracle (cut one byte short). NAME_FIXES (auto-queued) has 0x00696340's ??$_M_find self
  rename, now unblocked, and the three stand-in binders of 0x00696900.
- Queue upkeep: the "deferred" notes from nf159 / nf161 are reworded and those entries unclaimed (q11666, q4303, q6167), so
  mkagent can pick them again.
