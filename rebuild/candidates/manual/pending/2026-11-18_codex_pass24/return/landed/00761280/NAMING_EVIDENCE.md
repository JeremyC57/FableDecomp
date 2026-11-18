# Naming evidence

- Ego_r donor 0x00770EA0 and FableWin donor 0x025DA5F0 have the exact fastcall leaf. Ego_d places the function in tc_geometric_shapes.obj and the landed caller at 0x00761560 carries the corresponding helper body and calls the same retail entry.
- Ego_d records `info`, `wmap`, `from`, `to`, `lod_centre`, `colour`, `start_pos`, `ave_dist`, `magnitude`, `weight`, `line`, `max_index`, `GRANULARITY`, and the nested `c1` and `end_pos`. The caller's text uses the same helper calculations and the retail disassembly corroborates the values and nested loop.
- The callee names `CWorldMap::GetGroundSizeZAt`, `CIEngine::AddPrimitive` and `CEnginePrimitiveLine` come from the landed 0x00761560 and the shared headers. Native calls GetGroundSizeZAt at 0x0063A300 for both endpoints and dispatches AddPrimitive through the engine vtable.
- The exact COFF literal and callee bindings are tabulated in RELOCATIONS.md. The macros for the CEnginePrimitiveLine and CIEngine models were already in the package.

## Behavioral fixture

The exact selected method uses the shared vector arithmetic. The fixture world supplies a sloped height field and the fixture engine records every submitted segment and validates its constructor arguments. VC7.1 RTM /Od /W3 and GNU C++03 both passed.
