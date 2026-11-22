# 0x00839420 LOG (AUTH23)
1. r0 (recorded draft, today's headers): 00839420 NThingHelpers::CreateEffectOnHit DIFFER(1304v1308) flags= []
2. p1 = Ego_d locals / blocks / statement order (line_out, new_point_of_hit, new_direction_of_hit, perp, phitter, lod_centre, if (pmaterial_def) block, implicit CDefString conversion): 00839420 NThingHelpers::CreateEffectOnHit DIFFER(1323v1308) flags= []
3. p2 = p1 + IS_THING_ALIVE at line 110 (61 B), line_out.IsNonZero() (19 B), perp = line_out.CrossWithUp() (12 B), new_direction_of_hit = perp * -1.0f / perp * 1.0f (44 / 38 B): 00839420 NThingHelpers::CreateEffectOnHit DIFFER(1316v1308) flags= []
Grades used: 3 (r0 regrade 1304v1308 / 75 lines; p1 1323v1308 / 120; p2 1316v1308 / 29). Not grades: odall10 runs (od/). State and next steps: HANDOFF.md.
