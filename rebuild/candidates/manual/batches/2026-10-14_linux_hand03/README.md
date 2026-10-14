# Batch `2026-10-14_linux_hand03` — hand-authored C3DMeshFilePartitionChunk .. C3DMeshMaterial functions: 40 bodies (33 landed)

Delegated hand-authoring pass (compiler in the loop, Wine lane) over the fourth chunk of
class-labelled open functions of 17–96 bytes: mesh-file chunk `WriteToFile` / `CreateNewInstance`
/ `ReadFromFile` bodies (sub-mesh, transformation, smoothing groups, unique vertices, bounding box
and sphere, mapping info, texture index, triangle strip, UV tangent, animated primitive), two
scalar deleting destructors, an audio-library setter under the global queue lock, the
`CCreatureAction_CastWillDrainLifeSpell` constructor, `C3DMeshLODInfo` resource callbacks and
`Sizeof`, `C3DMeshPartitionElement::Sizeof`, the `C3DMeshMaterial` constructor and the
`CMaterialDef` constructor (`#pragma optimize("s", on)` in the source). Most manifest labels were
propagated and were corrected from the stored vtables and PDB layouts.

Lessons recorded by the pass: chained assignments (`a = b = c = 0.0f`) store right to left, giving
the descending-address identity-matrix stores of an embedded `CMatrix3x4` constructor; a named
`bytesRead` local stops the compiler re-associating `size - (now - start)`; a `CArray` list accessor
as `Size() > 0 ? m_begin : (const T*)this` gives the load-then-`ja` shape; the implicit inline
destructor of a class derived from a polymorphic base without a virtual destructor stores no
vtable; `CCountedPointer` release is `--Info->RefCount; if (Info->RefCount == 0) { Info->DeleteFunc(Info->Data); delete Info; }`
with a `__fastcall` delete function; intrinsic `memset` zero registers never merge with earlier
null stores. Retail layouts differ from the PDB headers for `CParentDefClassBase` (0x3c), `CArray<T>`
(0x0c) and `std::vector` (12 bytes). Seven bodies differ by scheduling only (`GetChunkInfo`,
`IsLoaded` / `IsLoadedStats` loop rotation, `Reset`, `UnloadResource`, two def constructors).

All landed bodies graded `MATCH` / `RELOCATION_MATCH` + `PASS` on 2026-10-14.
