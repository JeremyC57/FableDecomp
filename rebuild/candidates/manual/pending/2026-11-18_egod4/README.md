# In-house Ego_d lane, round 4 (2026-11-18): finish the two rows a Fable review matched

Round 3 escalated 0x00BE2CD0 and 0x0059FEE3 (`../2026-11-18_egod3/return/escalate/`); a Fable reviewer (owner's rule: stuck rows go
to a Fable advisor) found the original source structure of both and grades them RELOCATION_MATCH with q.py:
- 0x00BE2CD0: the tri-list stores are written in each arm of one if / else (`poly_verts[0], [1], [2]`, destination indices swapped),
  not a shared `+2` store before the branch (retail 0x00BE2F74..95).
- 0x0059FEE3: the "unavailable" arm is written twice -- an outer if (affordable && (space || special) && quantity > 0) with an inner
  if (!already_have_special) whose else repeats the outer else (retail 0x0059FF9A..A0 enters the else one instruction past its head).
One author (egod4a) finishes them: ports 0x0059FEE3's local CThingPlayerCreature onto the shared header, writes both behaviour tests,
and runs the read-only strict-header vland check.
