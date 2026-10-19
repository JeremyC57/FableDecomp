# Hand chunk 196 (CEngineOcclusionManager .. CEngineScreenEffectDisplacementRenderer): 3 of 40 byte-exact

## What matched (3)
- 004c3d30 CEnginePrimitiveAnimation: resource pointer setup, reference increments, and two copied frame blocks; explicit assignments reproduce VC7.1 scheduling.
- 00bcea60 CEnginePrimitiveManagerAnimatedMeshesBase::FrameStart: mode-dependent pointer table; member declaration order follows retail store offsets.
- 00b9d100 CEnginePrimitiveManagerStaticMeshesBase::FrameStart: mode-dependent static pointer table; reordered members reproduce the nonsequential stores.

## Parked for Fable 5.1 (2)
- 0041beb0 CEnginePrimitive2DSprite DIFFER(192v171): constructor parameter layout and byte/pair argument mapping remain unresolved | tried: broad scalar/pointer model | next: recover exact 17-argument signature from callers/vtable
- 00bd27f0 CEnginePrimitiveManagerAnimatedMeshesBase::GetSupportedTypes DIFFER(182v176): vector helper temporary stack slots differ after the first append | tried: one reused local, three separate locals | next: model STLport push_back temporary lifetime

## Skipped
- Remaining 35 rows were not reached in this hand pass and remain available for the next lane pass.

## Label corrections
- None.
