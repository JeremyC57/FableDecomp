## 0x00867B40

# wf9 row 0x00867B40 header overlay (full copies of the current repo headers plus opt-ins)
- engine/CCreatureActionBase.h: `FABLE_RETAIL_CCREATUREACTIONBASE_PEEKWORLD` -- declares `const CWorld& PeekWorld() const;` (Ego_r ?PeekWorld@CCreatureActionBase@@IBEABVCWorld@@XZ, retail 0x00692950).
- engine/CCombatAction_GenericStrikeResponseKnockdown.h: `FABLE_RETAIL_CCOMBATACTIONGENERICSTRIKERESPONSEKNOCKDOWN_FINISHACTION` -- declares the public virtual override `FinishAction()` (0x00867B40) in the retail branch.
- engine/CCreatureAction_Die.h: `FABLE_RETAIL_CCREATUREACTION_DIE` -- the polymorphic retail class (derives CCreatureActionBase, ctor (CThingCreatureBase&, bool) 0x008ED650, 0xAC bytes).

## 0x00762BA0

# Header overlay for row 0x00762BA0 (wf9)
- engine/CTCShapeManager.h: `FABLE_RETAIL_CTCSHAPEMANAGER_ONSERIALISE` -- declares the public virtual override `OnSerialise(CPersistContext&, bool)` 0x00762BA0 (declaration only).
- engine/NShape_CClosedShapeBase.h: `FABLE_RETAIL_NSHAPE_CSHAPE_STATONSERIALISE` -- declares the public static __fastcall `NShape::CShape::StatOnSerialise` 0x00762620 (declaration only).

## 0x00A12AB0

engine/CPhysicsMesh.h: FABLE_RETAIL_CPHYSICSMESH_CONTAINMENTVOLUME -- declares CPhysicsMesh::IsInContainmentVolume (0x00A6CA20) and PeekRoughProportionOfSphereContained (0x00A6CA80), declarations only. Copied unchanged from the twin row's overlay ($S/w22/wf8/00a128e0/hdr) so both rows use one opt-in name.
engine/CPhysicsMeshObject.h: FABLE_RETAIL_CPHYSICSMESHOBJECT_CONTAINMENTVOLUME -- declares CPhysicsMeshObject::IsInContainmentVolume (0x00A128E0) and PeekRoughProportionOfSphereContainedInVolume (0x00A12AB0), declarations only. Copied unchanged from $S/w22/wf8/00a128e0/hdr.

## 0x00726160

engine/CTCSolidToThing.h: FABLE_RETAIL_CTCSOLIDTOTHING_ISSOLIDTOTHING declares ?IsSolidToThing@CTCSolidToThing@@QBE_NPBVCThing@@@Z (0x00770DE0), declaration only (row 0x00726160).

## 0x00A90900

- engine/C3DMeshFileTrianglesChunk.h: FABLE_RETAIL_C3DMESHFILETRIANGLESCHUNK_ACCESSORS declares GetList() const (0x00ABC440) and GetCount() const (0x00ABA370), both out of line in retail (row 0x00A90900).
- engine/C3DMeshFileXTriangleStripChunk.h: FABLE_RETAIL_C3DMESHFILEXTRIANGLESTRIPCHUNK_ACCESSORS adds the PDB inline accessors GetStaticChunkType() ("XTST"), GetCount() const, GetStripVertex(unsigned long) const (row 0x00A90900).

## 0x008DC720

No header overlay needed for 008dc720 (repo headers as they are).

## 0x006F3A30

# Header overlay for row 0x006F3A30 (wf9)
- stlport/fable_containers.h: `FABLE_RETAIL_STLP_LIST_SORT` -- STLport 4.6 _list.h/_list.c `_List_global<bool>::_Transfer`, `list::swap`, `list::splice(pos, list&, i)`, member template `list::sort(comp)`, and the free templates `_S_merge` / `_S_sort` (both with bodies, FABLE_STLP_FASTCALL); the opt-in is also added to the `#if` list at line 17 that defines FABLE_STLP_NOINLINE / FABLE_STLP_FASTCALL (the only changed existing line). Default path unchanged.
- engine/CThingCompare_Nearest.h: `FABLE_RETAIL_CTHINGCOMPARE_NEAREST_OUTOFLINE` -- the template `operator()` declared only (retail calls the `<const CThingCreatureBase>` instance out of line at 0x006EF290 from _S_merge 0x006F04C0). Default path unchanged.

## 0x0085D180

# wf9 row 0x0085D180 header overlay (full copy of the current repo header, additions only)
- engine/NThingMap_CThingMap.h `FABLE_RETAIL_CTHINGMAP_INLINE_CTORS` (inside FABLE_RETAIL_MAP_VALIDATE): inline bodies for the Ego_r.pdb default constructors `CThingMapCell() : PFirstNode(0) {}` and `CMapwhoLevelInfo() {}` (both expanded in retail 0x0085D180; declaration-only without the opt-in).
- engine/NThingMap_CThingMap.h `FABLE_RETAIL_CTHINGMAP_LAYERINFO` (inside FABLE_RETAIL_MAP_VALIDATE): `NThingMap::CLayerInfo` + `extern const CLayerInfo LayerInfo[]` (0x0137DAD8) without FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST's helpers (includes engine/CRGBColour.h).

## 0x005BE8F3

engine/CTCObjectAugmentations.h: FABLE_RETAIL_CTCOBJECTAUGMENTATIONS_SLOTS -- declares GetNumberOfSlots 0x00765D80 / GetMaximumNumberOfSlots 0x00765D90 / AddNewSlot 0x00766D70 / RemoveAugmentationFromSlot 0x007681D0 in the retail branch (row 0x005BE8F3).
engine/CTCInventoryItem.h: FABLE_RETAIL_CTCINVENTORYITEM_CONNECTTOINVENTORY -- declares ConnectToInventory(NInventory::CTCInventoryBase&) 0x005D8E40 (row 0x005BE8F3).

## 0x008C6860

- engine/CCreatureAction_TrollThrowProjectile.h: `FABLE_RETAIL_CCREATUREACTION_TROLLTHROWPROJECTILE` -- the polymorphic retail class on CCombatActionBase (Initialise / FrameUpdate / PerformAction / FinishAction; PShot +0x114, TargetPos +0x11c, UseTargetPos +0x128, ShotAlpha +0x12c; sizeof 0x130 asserted); default generated pad layout unchanged. Row 0x008C6860.
- engine/NThingShot_CTCShotDefinitionBase.h: `FABLE_RETAIL_CTCSHOTDEFINITIONBASE_INIT_POSITION` -- the target-position Init overload (`const C3DVector&`, 0x007BF790; Ego_r 0x007BE730 / FableWin 0x017D75C1 spelling), declaration only, beside the landed target-thing overload. Row 0x008C6860.
- engine/CThingCreatureBase.h: `FABLE_RETAIL_CTHINGCREATUREBASE_GETSHOTACCURACYPERCENTAGE` -- `long GetShotAccuracyPercentage() const;` (?GetShotAccuracyPercentage@CThingCreatureBase@@QBEJXZ: Ego_r 0x0053E950, FableWin 0x017CE205 / 0x01D8C0B0; retail 0x00663B10), declaration only. Row 0x008C6860.
- engine/CThingShotDef.h: `FABLE_RETAIL_CTHINGSHOTDEF_DEFSTRING` -- the +0x60 member as `CDefString ShotDefinitionClassName` (Ego_r name and type) instead of the flattened `long ShotDefinitionClassName_TablePos`; same offset and size (sizeof 0xac assert kept). Row 0x008C6860.

