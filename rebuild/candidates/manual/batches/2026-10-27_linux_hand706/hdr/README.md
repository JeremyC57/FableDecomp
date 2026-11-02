# Chunk 706 header overlays

Grade with `export FABLE_HDR_OVERLAY=$S/w22/agent706/hdr`. Eight headers: six existing ones get an opt-in retail branch (the
generated text stays unchanged under `#else`, or for CPlayer.h the addition is a guarded member-function declaration only), and two
are new hand-written retail-layout headers with no previous file. No default path changed, so no landed includer can change bytes and
no includer recheck was needed (`hdr/INCLUDERS_*.txt` not required). The coordinator should still copy all eight together: the
CLandscapeBackgroundPatch branch includes the CLandscapeBackgroundSubNodeBase / CMovableResource branches and the two new headers.

| header | change | macro | repo includers today |
|---|---|---|---|
| engine/CExplodingObjectDef.h | opt-in retail class | FABLE_RETAIL_EXPLODING_OBJECT_DEF | 0 |
| engine/CTCVolumeContainmentTrackerDef.h | opt-in retail class | FABLE_RETAIL_TC_VOLUME_CONTAINMENT_TRACKER_DEF | 0 |
| engine/CMovableResource.h | opt-in polymorphic root | FABLE_RETAIL_MOVABLE_RESOURCE | 28 (default path unchanged) |
| engine/CLandscapeBackgroundSubNodeBase.h | opt-in `class` : CMovableResource | FABLE_RETAIL_LANDSCAPE_BACKGROUND_SUB_NODE_BASE | 9 (default unchanged) |
| engine/CLandscapeBackgroundPatch.h | opt-in retail class, all Ego_r members, typed EdgeStrips | FABLE_RETAIL_LANDSCAPE_BACKGROUND_PATCH | 27 (default unchanged) |
| engine/CPatchTesselationEdgeStrip.h | NEW, retail layout | (always; `// Retail layout` mark) | 0 (new) |
| engine/CEdgeStipVertexArray.h | NEW, retail layout template | (always; `// Retail layout` mark) | 0 (new) |
| engine/CPlayer.h | guarded `bool IsFirstPersonTargeting() const;` inside the flat class | FABLE_RETAIL_PLAYER | 123 (default unchanged) |

## engine/CExplodingObjectDef.h -- opt-in FABLE_RETAIL_EXPLODING_OBJECT_DEF
- Change: new `#ifdef FABLE_RETAIL_EXPLODING_OBJECT_DEF` branch that models the class the way retail has it:
  `class CExplodingObjectDef : public CSubDefClassBase` (engine/CSubDefClassBase.h under FABLE_RETAIL_DEF_CLASS_BASE, which the
  branch defines when it is not already defined), `virtual void Transfer(CPersistContext&)` (slot 18), then
  TriggeredOnCreatureProximity bool +0x25, TriggerRadius float +0x28, ExplosionDef long +0x2C, ProximityParticleEffect long +0x30;
  `FABLE_STATIC_ASSERT(sizeof == 0x34)`. `CPersistContext` is only forward-declared (no shared CPersistContext header exists).
- Evidence: RTTI .?AVCExplodingObjectDef@@ : CSubDefClassBase : CDefClassBase : CDefPointeeBase : CResource : CIVCountedPointeeBase,
  vtable 0x0123AD7C slot 18 = 0x004DE253; Ego_r.pdb + FableWin.pdb both list ?Transfer@CExplodingObjectDef@@UAEXAAVCPersistContext@@@Z;
  the member offsets are Ego_r's and match the retail Transfer bytes (`lea eax,[esi+25h]` -> bool transfer 0x0043314A,
  +28h -> float 0x00431061, +2Ch and `add esi,30h` -> long 0x00431102).
- Used by: src/004de253.cpp (RELOCATION_MATCH with the overlay; SRC_FAIL C2039 without it, so the overlay copy is the one used).

## engine/CTCVolumeContainmentTrackerDef.h -- opt-in FABLE_RETAIL_TC_VOLUME_CONTAINMENT_TRACKER_DEF
- Change: the same shape: `: public CSubDefClassBase`, virtual Transfer slot 18, UpdateEveryFrame bool +0x25,
  CalculateBlendedLightingChannel bool +0x26, LightingChannelSphereModelRadius float +0x28, LightingChannelSphereModelZOffset float
  +0x2C; `FABLE_STATIC_ASSERT(sizeof == 0x30)`.
- Evidence: RTTI .?AVCTCVolumeContainmentTrackerDef@@ (same def chain), vtable 0x0123E0C4 slot 18 = 0x004DF32C; both PDBs list
  ?Transfer@CTCVolumeContainmentTrackerDef@@UAEXAAVCPersistContext@@@Z; the retail bytes transfer +25h/+26h as bool (0x0043314A) and
  +28h/+2Ch as float (0x00431061).
- Used by: src/004df32c.cpp. Overlay proof: one extra float member added after LightingChannelSphereModelRadius in this copy turned
  the row DIFFER(57v57); the restored copy is byte-identical to the backup (cmp) and grades RELOCATION_MATCH again.

Every edited header keeps a `// Retail opt-in` / `// Retail layout` line and a FABLE_RETAIL_ macro, so gen_class_headers.py's RETAIL_MARK keeps the hand edit.
Not touched: engine/CBankFile.h, rebuild_abi.h, and every other header (the remaining flattened headers are listed as header debt in
NOTES.md).

## engine/CMovableResource.h -- opt-in FABLE_RETAIL_MOVABLE_RESOURCE
- Change: `class CMovableResource { public: virtual ~CMovableResource(); };`, sizeof 4 (the vptr is a real vptr, not a data member).
- Evidence: RTTI .?AVCMovableResource@@ has no bases and is the root of the CLandscapeBackgroundSubNodeBase (vtable 0x012A8014) and
  CLandscapeBackgroundPatch (0x012A803C) hierarchies (read from the retail RTTI class hierarchy descriptors); Ego_r sizeof 0x4. Only the
  dtor is declared (its other virtuals are not modelled yet).

## engine/CLandscapeBackgroundSubNodeBase.h -- opt-in FABLE_RETAIL_LANDSCAPE_BACKGROUND_SUB_NODE_BASE
- Change: `class CLandscapeBackgroundSubNodeBase : public CMovableResource { };` (the branch defines the CMovableResource macro).
  The class key is `class` (RTTI .?AV...); the generated default path says `struct`, which is wrong but left untouched.
- Evidence: RTTI .?AVCLandscapeBackgroundSubNodeBase@@ : CMovableResource, vtable 0x012A8014 (slots 0..8); Ego_r sizeof 0x4.

## engine/CLandscapeBackgroundPatch.h -- opt-in FABLE_RETAIL_LANDSCAPE_BACKGROUND_PATCH
- Change: `class CLandscapeBackgroundPatch : public CLandscapeBackgroundSubNodeBase` with every Ego_r member at its PDB offset
  (Parent +0x04, EdgeStrips CPatchTesselationEdgeStrip[4] +0x08, Texture +0xB8 ... IsDefaultSection +0x11B; sizeof 0x11C, offset
  asserts), `GetEdgeStrip(LANDSCAPE_EDGE_STRIP_SIDE)` declared, and `enum LANDSCAPE_EDGE_STRIP_SIDE { }` (the enumerator names are in
  neither PDB name set; two landed sources invent different ones, Side0.. and LESS_LEFT..).
- Evidence: RTTI .?AVCLandscapeBackgroundPatch@@ : CLandscapeBackgroundSubNodeBase : CMovableResource, vtable 0x012A803C; both PDBs list
  ?GetEdgeStrip@CLandscapeBackgroundPatch@@QAEPAVCPatchTesselationEdgeStrip@@W4LANDSCAPE_EDGE_STRIP_SIDE@@@Z; the retail arms return
  +0x08/+0x34/+0x60/+0x8C (stride 0x2C = sizeof CPatchTesselationEdgeStrip).
- Used by: src/00be5c60.cpp. Overlay proof: an extra `long` inserted before EdgeStrips in this copy turned the row DIFFER(64v64), the same
  member inserted after EdgeStrips kept RELOCATION_MATCH; the restored copy is cmp-identical and grades RELOCATION_MATCH.

## engine/CPatchTesselationEdgeStrip.h (new) and engine/CEdgeStipVertexArray.h (new)
- CPatchTesselationEdgeStrip : CBaseClassNonCopyable (engine/CBaseClassNonCopyable.h), RTTI .?AVCPatchTesselationEdgeStrip@@ :
  CBaseClassNonCopyable : CBaseClass, vtable 0x012A8318; Ego_r members StartPos +4, Size +6, Verts / FanBaseVerts / WaterVerts /
  WaterFanBaseVerts +0x08..+0x27, Tesselated +0x28, Vertical +0x29; sizeof 0x2C (asserted). The vertex types are nested structs
  (class key U in both PDB name sets).
- CEdgeStipVertexArray<T> (Lionhead spelling "Stip", class key V): { T* Data +0; long Count +4 } (struct_layouts_egor.tsv).

## engine/CPlayer.h -- opt-in FABLE_RETAIL_PLAYER (member functions only)
- Change: inside the existing flat class, `#ifdef FABLE_RETAIL_PLAYER bool IsFirstPersonTargeting() const; #endif`. The layout is
  identical on both paths; nothing else touched.
- Evidence: Ego_r.pdb + FableWin.pdb ?IsFirstPersonTargeting@CPlayer@@QBE_NXZ = retail 0x004874D0 (the landed binding from caller
  0x008E9460 already uses this spelling); called from 0x00802610 on CThing::PeekPlayer's `const CPlayer&`.
- Used by: src/00802610.cpp (RELOCATION_MATCH with the overlay; SRC_FAIL C2039 without it). It replaces a local method-only
  `class CPlayer` that vland flagged as SHARED_HEADER_AVAILABLE.
