# Naming evidence

- Ego_r 0x007C6EA0 and FableWin 0x0271E7A0 give the exact leaf `?OnCollision@CTCHitOnCollision@@QAEXABVC3DVector@@0AAVCThing@@@Z`.
- The check order and final hit call corroborate Ego_d targetGameObject, doHit, blocked, creature, pAction, hit_pos, hit_direction, params, vector_to_target, left and pTCSolidToThing.
- Ego_d's component and hit-parameter types plus native vtable and contact field offsets identify CTCHitOnCollision and the inline SetBlockable operation.

## Behavioral fixture

The fixture executes the selected OnCollision body across 8,192 combinations, including type and repeat filtering, combat exclusions, both sideways directions, hit results, and solid-component lookup and attachment. The hit callback and component interfaces record the arguments they receive.
