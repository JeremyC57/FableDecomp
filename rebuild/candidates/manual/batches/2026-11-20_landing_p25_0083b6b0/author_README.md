# The static-mesh DrawPrepare family: 0x0083B6B0 / 0x0083DC80 / 0x00837B30 (p25N, 2026-11-21)

Three rows, one batch. All three are `DrawPreparePrimitivesForRendering` overrides (vtable slot 36), and retail has them instruction-identical
except for the graphic accessor. All three rows use the shared headers plus one add-only overlay (`hdr/`, `hdr.diff`: 39 added lines, 0 removed).

| row | class (Ego_r / FableWin) | grade | graphic accessor | test |
|---|---|---|---|---|
| 0x0083B6B0 | CThingVillage (0x0082C8C0 / 0x027F6E60) | RELOCATION_MATCH (309) | `GetCurrentGraphic()` 0x0083B520 (landed, returns a pointer) | `test/0083b6b0.cpp` |
| 0x0083DC80 | CThingPhysicalSwitch (0x00812D10 / 0x028013F0) | RELOCATION_MATCH (309) | `&GetCurrentGraphic()` 0x0083DB60 (landed, returns a reference) | `test/0083dc80.cpp` |
| 0x00837B30 | CThingTrackNode (0x0082B7E0 / 0x027FF610) | RELOCATION_MATCH (314) | `GetCurrentGraphic()`, a header inline: PeekGlobalDefinition 0x004C79A0 + 0xE6 | `test/00837b30.cpp` |

**Strict vland** passes on all three (`vland.log`): LANDVERIFY_STRICT_HEADERS=1, FABLE_HDR_OVERLAY=`hdr/`, a fresh LANDVERIFY_WORK. The
oracle is in `oracle.tsv`, with bytes read from Fable.exe. There is no flag.

The one warning is RELOC_NAME on 0x00837B30: `?PeekGlobalDefinition@CThing@@IBEABVCGlobalDef@@XZ` resolves to 0x004C79A0, which is
labelled `CAnimateThing::GetMeshBank` (a BSim propagation). This binding is not new: engine/CThing.h already documents 0x004C79A0 as
PeekGlobalDefinition (chunk 2039, with its body), and the landed rows 0x00837050 / 0x00837060 / 0x006A9590 bind it the same way. The label
needs a separate fix, outside this batch.

## The fix: a visible CEnginePrimitiveStaticMesh ctor body
- **Evidence it is inline.** In the original the ctor is an inline header function:
  - FableWin.pdb emits it as a COMDAT in tc_graphic_appearance_static_mesh.obj #10 (0x0263C1E0, 217 B debug);
  - retail's out-of-line copy 0x0077BD70 sits with that TU.
- **The opt-in.** The overlay adds the body under `FABLE_RETAIL_CENGINEPRIMITIVESTATICMESH_INLINE_CTOR`, with Ego_d's parameter names and
  the member order of the retail stores.
- **Effect.** The rows still call it out of line. But once c2 can see that the body keeps no address, it shares the temps: the
  interpolated position's diff and result, and the scaled temp with the uid copy. That gives frame 0x80, as retail.
- **Without the opt-in:** DIFFER(308v309), and (313v314) on 0x00837B30 (`evidence/ablation.txt`).
- **The ctor COMDAT is real.** The row object defines it, and the behaviour tests run it.

## The other overlay additions (each one opt-in, add-only)
- `CThingVillage.h` / `CThingPhysicalSwitch.h`: the `virtual bool DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&)`
  declaration. It is UAE in Ego_r, where the old drafts had QAE.
- `CThingTrackNode.h`:
  - the same virtual;
  - `const CEngineGraphic* GetCurrentGraphic() const` (Ego_r 0x0082A880, QBE);
  - its inline body `return &PeekGlobalDefinition().TrackGraphic;` (+0xE6, as the landed DrawGetMesh 0x00837050 reads it).

## Ego_d.pdb
- The three files have identical 8-record line tables: 36 / 26 / 14 / 186 / 280 / 4 / 2 / 16.
  - The 186 B record is `ASSERT(def_graphic)` (a mock debug assert measures 190 B).
  - The AddPrimitive statement is **one** line (280 B), so the sources write it on one line.
- The only local is `def_graphic` (const CEngineGraphic*). On the switch that is why the reference is taken with `&`.
- `FABLE_RETAIL_ASSERT(def_graphic)` uses the stock dead arm. It is byte-neutral, and so is its define (`evidence/ablation.txt`). It is kept
  for Ego_d's line 154.

## Inferred
- `GetUniqueID()` and `GetPhysicsTC()` are spelled as accessors: retail keeps the uid low word in ebx and loads PhysicsTC early, which is
  how those accessors schedule.
- `Scale` is read directly: GetScale() loads it before the ctor arguments, while retail loads it late.

## Defines
- Each file's define block came from a greedy minimisation (`evidence/define_greedy_*.txt`).
- Every kept define fails without it (SRC_FAIL or a DIFFER), except `FABLE_RETAIL_ASSERT_DEAD_ARM`, which is the stock release assert.
- The DIFFER ones:

  | define removed | result |
  |---|---|
  | INLINE_CTOR | 308 |
  | CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES | 290 |
  | CIVCOUNTEDPOINTER(_RELEASE_MEMBER) | 318 |
  | CTCPHYSICSBASE_INTERPOLATED_POSITION_BODY | 241 |

## Tests
- **Setup.** Each test links the real row object, including its ctor COMDAT.
  - CThingPhysical's DrawPrepare and the graphic accessor are `/alternatename` stand-ins that log. On the track node the accessor is
    PeekGlobalDefinition, which returns graphic - 0xE6.
  - The physics TC (GetRHSet(float), +0x11C, hidden return pointer), the engine (AddPrimitive +0x50) and the mesh effect (Destroy +4)
    are fake objects with hand-built vtables.
- **What they check:**
  - when the base declines: false, and no other call;
  - otherwise, every field of the primitive handed to AddPrimitive: type 8, interpolation set 0, mesh index, interpolated position
    O + (P - O) * t, the returned RH set, size = RenderSizeX * Scale, fade 0, null effect, zero theme bytes, white tint, alpha, lit,
    bullet-time cloth;
  - the temporary's release: Destroy runs only on the last reference;
  - `this`, `info` and the physics TC are unchanged.
- **Mutants:** 15/16 KILLED on 0x0083B6B0, 16/17 on 0x0083DC80 / 0x00837B30 (one extra mutant on the row-specific accessor), in `mutants/`.
  - The one survivor, on every row, is `GetUniqueID()` -> `0`. The retail CEnginePrimitive id ctor ignores the uid (0x0077BD70 never
    stores it; `FABLE_RETAIL_CENGINEPRIMITIVE_ID_CTOR`: "id is unused"), so nothing observable changes.

## NAME_FIXES.tsv
- The `CThingStaticMeshPrimitive_83b6b0` / `_83dc80` / `_837b30` labels (chunk 237 placeholders) become CThingVillage /
  CThingPhysicalSwitch / CThingTrackNode.
- The pending drafts' `QAE ... ABU` leaves become Ego_r's `UAE ... ABV`.
- The old pending drafts (2026-10-20 hand237, 2026-10-27 hand993, 2026-11-04 hand764, 2026-11-11 handQ14) are superseded.
