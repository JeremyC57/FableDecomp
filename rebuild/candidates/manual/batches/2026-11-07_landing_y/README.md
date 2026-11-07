# Landing 2026-11-07y: Fable expert retry chunk 847 (5 of 15 landed, 10 parked)

Fifteen 4-7-line near misses no expert had worked; 50+ attempts per row in this pass (up to 1071 on one row).

| Row | Grade | Note |
|---|---|---|
| 0x005029A0 GetRelevantBuilding | **RELOCATION_MATCH + PASS, landed** | 29 attempts: one flattened condition fixes the register colouring |
| 0x0063E590 CWorldMap::GetNavigationLayerAt | **RELOCATION_MATCH + PASS, landed** | 62: a `CThingSearchTools* tools` local + a named filter object put the Bind stores in retail's slots; opt-ins in CThingSearchTools.h / CMap.h |
| 0x00C71F80 CAudioSource::ComputePitchScale | **RELOCATION_MATCH + PASS, landed** | 43: the float pow overload (GameCODA, descriptive name) |
| 0x0075E270 CTCCameraPointScriptedSpline::OnInitialActivate | **RELOCATION_MATCH + PASS, landed** | 63: `long& end = NodeToEndOn` keeps retail's dead store |
| 0x00760A70 NShape::CSubdivisionTriangle::GetUpDirection | **RELOCATION_MATCH + PASS, landed** | 98: sequenced running-sum dot with `(float)(double)` identity wraps (noted on line 2) |
| 0x00C75F80 CCombFilter::Allocate | DIFFER, 4 | 71 attempts |
| 0x00A852B0 CVectorMap::Insert | DIFFER, 4 | 53; proof that no STLport copy-ctor shape loads begin()/end() before the sentinel store |
| 0x0065B142 CTCVillage::EnableVillagerDefTypes | DIFFER, 4 | 54 |
| 0x00A22E10 CRightHandedSet::MakePerpendicular | DIFFER, 2 | 73 |
| 0x00B92300 CEngineSceneGridCell::CreateOcclusionGroup | DIFFER, 4 | 77 |
| 0x00F36A30 CHeteroTerrainFractal::GetFractalHeight | DIFFER, 4 | 60 |
| 0x006AFE60 CTCPhysicsBase::RespondToCollision | DIFFER, 2 | 419; alias-driven (/Oa fixes it, diagnostic only) |
| 0x006A9690 ResolveRelativeMovementAcceleration | DIFFER, 2 | 85 |
| 0x00986ED0 CThingFilter_IsAICreatureMatchingTypeAndFactionNearby ctor | DIFFER, 8 | 1071; a 729-cell grid floors at 8 |
| 0x0085C7D0 NCameraModes::CMoveOnCylinder::OnInit | DIFFER, 8 | 220 |

- **Labels:** `labels_landing_2026-11-07y.tsv` (0x00C75F80 CCombFilter::Allocate, 0x00A852B0 CVectorMap::Insert). Queue `land0711y`: five respellings.
