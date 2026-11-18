# Naming evidence

- Ego_r donor 0x00A54AE0 and FableWin donor 0x02EA0580 both name the private `IsInsideFustrum` method with the same spelling and types. Retail expands the sphere setup and clipping at 0x00B9D730, and the surrounding static-mesh primitive render code performs the same center/scale preparation.
- Ego_d gives `mesh_prim`, `primitive`, `center`, and `radius`. The C3DPrimitive2 retail sphere at +0x0C/+0x18 and the CEngineInternalPrimitiveStaticMesh transform and scale offsets corroborate their roles. `sphere_radius` is descriptive and fitted. The const reference binds a returned float value and preserves retail's order at 0x00B9D7A4.
- FableWin's by-value GetBoundingSphere (donor 0x02E77C20), GetRadius (0x02526610), ClipSphere (0x02C9DF20) and the retail inline expansions corroborate those helper spellings. The landed CEngineCamera and C3DPrimitive2 shared headers supply the bodies.
- `NEngineInternal::GPCamera` is bound at 0x01436EA0 in the package's reloc_bindings, and the landed consumer 0x00BCA6B0 and the field accesses through that pointer corroborate the declaration.

## Behavioral fixture

The exact selected method uses the shared matrix transform and fixture sphere, mesh and camera types. The camera records the transformed center and radius and checks clipping against a deterministic frustum. VC7.1 RTM /Od /W3 and GNU C++03 both passed.
