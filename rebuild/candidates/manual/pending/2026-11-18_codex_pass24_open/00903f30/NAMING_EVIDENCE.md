# Naming evidence

- Ego_r 0x0092A970 and FableWin 0x02B480F0 give the exact leaf `?FollowPreCalculatedRoute@CScriptGameResourceObjectScriptedThing@@UAEXABVCScriptThing@@W4EScriptEntityMoveType@@_N2@Z`.
- The native route key, movement enum selection, action queue and optional yield corroborate route, move_type, wait, avoid_dynamic_obstacles, proute, ai_move_type and ptc_script_control.
- The inherited fitted is_route spelling survives in the baseline. Scoped and inline predicate variants did not resolve the priority inversion.

## Behavioral fixture

The fixture calls the selected route body across owner and route lifetimes, wait and avoidance flags, current movement, and command outcomes. The thing interfaces and route command are supplied by the fixture.
