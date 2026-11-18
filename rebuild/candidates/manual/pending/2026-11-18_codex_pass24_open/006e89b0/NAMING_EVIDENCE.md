# Naming evidence

- Ego_r 0x00711BC0 and FableWin 0x024107B0 give the exact leaf `?GetAttackerPlacementVector@CCombatWheel@CDefendingCombatantInfo@@QBE?AVC3DVector@@ABVCThingCreatureBase@@0J@Z`.
- Native reads the selected ring, the two radii and the attacker's preferred distance. This corroborates Ego_d prong, inner_ring_rad, outer_ring_rad, pring and pref_dist.
- The same-TU CCombatRing and CCombatWheel siblings use the exact natural names from the PDBs and the native ring address calculation.

## Behavioral fixture

The fixture executes the selected placement body and its three companion members plus the anonymous helper. Seventy-two placements cover default and explicit prongs, preferred-distance fallback and boundary values, and inner and outer rings. The prong lookup, prong vector, and physics/definition accessors are fixtures.
