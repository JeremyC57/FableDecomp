# Batch `2026-10-14_linux_hand32` — coordinator hand-authoring chunk 32 (CEngine .. CEngineInternalPrimitiveMeshBase): 33 of 40 landed

Chunk 32 of the class-labelled 17–96-byte pool, authored by the coordinator with the Wine lane: the internal-primitive
container family (group / changer / 2D viewport: GetChildPrimitive, PostRender, ClearAllChildPrimitives,
UpdateRenderLayerMask, GetAsOcclusionPrimitive, ResetOcclusionInfo, GetBoundingBoxTransform) over one shared model of
the CEngineInternalPrimitiveBase vtable (PostRender slot 14, GetBoundingBoxWorldSpace 17, SetAnimation 23, SetPosition
24, GetAsOcclusionPrimitive 27, ResetOcclusionInfo 28) and the VC6 handle vector (`sub ; sar 3` counts, `erase` of the
whole range), the 2D static mesh's identity transform and world-space box, the line primitive's midpoint, the attached
primitive's cached inverse transform, the camera's screen-to-homogeneous mapping, the base primitive's fade-distance
default and NotifyReleased, the animated mesh's SetAnimation copy, two mesh-base destructors, two scalar deleting
destructors, an `/EHsc` destructor with `delete[]` through the vector destructor iterator, three critical-section
guarded getters/setters on 0x00CAxxxx objects, and a packed-string constructor.

Label corrections: 009a07a0 "CEngineInternalPrimitiveChanger ctor" is a packed string record's constructor (named
CBasicString by address); 00ab9940 "GetObjectTransform" is a free `__fastcall` key builder (MakeRenderKey); 00bc6180 /
00be1d80 "ClearAllChildPrimitives" are UpdateRenderLayerMask (the OR of the children's masks); 00c48db0
"~CEngineComponent" is an entry-table owner's destructor (named by address; its leaf is `??1` so the lander picks the
45-byte body over the TU's `??_G`); 00ca4b60 / 00ca4ea0 / 00ca9a40 "CEngine::ClipBox" are critical-section guarded
members of 0x00CAxxxx objects (named by address); 00cab6d0 "CEngineComponent `scalar deleting destructor'" belongs to
a buffered reader (buffer at +0x3c).

Lessons (also in GOTCHAS, wave 24e): a `// Flags: /EHsc` line in a chunk source carries the flag into the payload, and
`delete[]` with the `eh vector destructor iterator` needs `/EHsc` plus an explicit `leaf` (`??1Class`) because the
`??_G` body in the same TU is larger; `verify_and_land.py` now honours the payload's `leaf` key (it used to derive the
symbol from the display name, which never matches a mangled `??1`); a `x ? first : this` select written through one
inline `GetData()` reproduces the `jbe ; mov eax,edx` idiom when it feeds a store directly in each arm.

Parked (4 attempts each, closest variants under the scratch `differ/`, evidence in the journal): 00b80f90 (the hidden
return pointer survives in edx across a same-TU callee: whole-TU register usage), 00b84120 / 00b84190 (the empty
allocator's address `lea eax,[esp+8]` from the dead flag slot: by value homes it but passes the value, by reference
gives it its own slot), 00bb1ef0 (a select-then-store that the lane duplicates into both arms), 00bba3e0 (the sqrt
result parked through an integer local word before the store), 00bc5620 / 00bc5660 (a dead `lea eax,[eax+edi*8]` of
the reference-bound handle).
