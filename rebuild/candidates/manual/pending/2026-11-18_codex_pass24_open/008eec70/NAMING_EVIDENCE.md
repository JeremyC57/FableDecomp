# Naming evidence

- Ego_r 0x00914410 and FableWin 0x02AE9CF0 give the exact leaf `?Collide@Mass@@SI_NPAV1@0VC2DVector@@11MMMM@Z`.
- The native contact velocity calculations, friction bound and impulse/move dispatch corroborate Ego_d's arguments and local impulse values.
- Ego_d colocates GetVel, ApplyImpulse, ApplyMove and ForceToVel. Native inlines the first and third while calling 0x008EEA80 and 0x008EEB40 for the other two.

## Behavioral fixture

The fixture calls the selected collision body with synthetic positions, velocities, separation, and friction. Math and impulse application are implemented by local fixture types.
