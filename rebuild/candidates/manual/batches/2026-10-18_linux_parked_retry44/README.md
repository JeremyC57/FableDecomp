# Dragon facing and thing-map cleanup

0094feb0 (311): actual JackDragon/state/brain headers, audited no-argument physics GetFacing slot72 and SetFacingAnglesFromRHSet slot75. Preserves world/frame calls, full orientation interpolation, do-while idle behavior, live-target checks and duration arithmetic. Six exact-source fixture scenarios cover complete orientation values, call order, target failure, stopping and zero/negative duration. One grade.

006380c0 (188): corrected propagated shader owner to NThingMap::CThingMapManager from Init caller and PDB evidence. Actual manager header with documented native12-byte containers. Genuine virtual CThingMap destructor and set erase helper; fixtures test empty/nonempty maps and tree, deletion-time backing-storage replacement, reset order, retained capacities/reference-count fields/world. Three grades.

All independently RELOCATION_MATCH + PASS, with verbatim-source host and canonical VC7.1 fixtures.
