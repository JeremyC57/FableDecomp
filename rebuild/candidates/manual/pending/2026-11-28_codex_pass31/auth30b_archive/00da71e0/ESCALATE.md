# 0x00DA71E0 CQ_MinionClifftopChaseScript::GuardChatter -- ESCALATE (AUTH30, 2026-11-27)
- Bytes: c0 (Codex, four overlay opt-ins incl. the NEW header engine/EAIStateGroupType.h, /d2inlS64) RELOCATION_MATCH 1549/1549. pick-any all EQ.
- Blocker: `??_GCIsThingAlive` (vftable slot 2): 46 B v retail 45 at 0x00CDF160. It calls `??1CPredicateBase` instead of expanding it.
  - Landed same-script siblings 0x00DA7830 and 0x00DA7D30 emit a 39-byte copy at HEAD (head/), so ours is new.
- Codex's own record: predicate_owner/ and standalone_scalar_proof/.
- Question: same class as 00d2d1a0 (predicate destructor inline split between the leaf and the compiler-generated ??_G).
Grades: c0 only.
