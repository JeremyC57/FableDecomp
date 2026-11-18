# Naming evidence

- Ego_r 0x0082ADF0 and FableWin 0x027FEE40 give the exact leaf `?UpdateFacingDirection@CThingTrackNode@@QAEXXZ`.
- The link-direction load, normalized forward and the right/up basis in native code corroborate all three Ego_d vectors.
- FableWin getter bodies and native field accesses establish GetNextNode, GetPreviousNode and GetPhysicsTC. Native calls Rotate at 0x00A14250.

## Behavioral fixture

The fixture runs the selected UpdateFacingDirection body with no neighbor, previous-only, next-only, and paired nodes. It supplies a generated square-root table, a planar rotation helper, and captured physics updates.
