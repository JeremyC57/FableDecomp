## 0x0091B3A0

# Header overlay for row 0x0091B3A0 (full copies of the current repo headers plus additions only)
- engine/CAIStateGroup_SeekPerceivedThing.h: `FABLE_RETAIL_CAISTATEGROUP_SEEKPERCEIVEDTHING` -- the polymorphic class on its CAIStateGroupBase base (FrameLastCaptured +0x14, sizeof 0x18), PDB members OnActivate / Cleanup / IsRunnable / Update (virtual), CapturePerceivedThing / RunToPerceivedThingPosition (private). Default (flattened) branch unchanged.
- engine/CTCPerceiveThing.h: `FABLE_RETAIL_CTCPERCEIVETHING_SEEKPERCEIVEDTHING_FRIEND` (inside the FABLE_RETAIL_CTCPERCEIVETHING_AWARENESS member block) -- `friend class CAIStateGroup_SeekPerceivedThing;` so the row reads the private LastPerceivedPos (+0x20) directly (0x0091B452 `lea edi,[ebp+20h]`); replaces the withdrawn invented GetLastPerceivedPos accessor (no such symbol in either PDB although FableWin emits this function's trivial accessors out of line). No layout change.
- engine/CThingPhysical.h: `FABLE_RETAIL_CTHINGPHYSICAL_ISINRANGE` -- declaration of `bool IsInRange(const C3DVector&, float) const` 0x006A4DF0 (?IsInRange@CThingPhysical@@QBE_NABVC3DVector@@M@Z, both PDBs).

## 0x00B81F30

# Header overlay for row 0x00B81F30 (full copies of the current repo headers plus `#ifdef` opt-ins; default paths unchanged)
- engine/C3DMeshInfo.h: `FABLE_RETAIL_C3DMESHINFO_DETAILLEVEL` (needs `_BOUNDINGBOX`) -- FableWin.pdb inline `HasLOD(unsigned long) const` and `GetBestCurrentDetailLevel() const`, and the nested pointee `C3DMeshInfo::CMeshData` (Ego_r.pdb: LODMeshes[4] +0x08, MeshStats +0x18, MeshID +0x1c) defined after the class. No layout change.
- engine/C3DPrimitive2.h: `FABLE_RETAIL_C3DPRIMITIVE2_CLOTHPRIMITIVES` -- types ClothPrimitives (+0x20) as Ego_r.pdb `CArray<CCountedPointer<C3DClothPrimitive> >` instead of 12 opaque bytes (not combinable with `_INDEXBUFFER` yet). No layout change.
- engine/CParticleProgram.h: `FABLE_RETAIL_CPARTICLEPROGRAM_UNSKINNED_SIMULATION` (inside the `FABLE_RETAIL_CPARTICLEPROGRAM` branch) -- declares `SetupSimulation(CParticleSimulation&, CMovableResourceMemoryPool&) const` 0x00AE0050, `SimulationStep(CParticleSimulation&, const CParticleContext&) const` 0x00AE2CB0, `SaveStateToInterpolateFrom(CParticleSimulation&)` 0x00ADF5B0. Declarations only.

## 0x008C61C0

- engine/CCreatureAction_TrollWhackGroundBase.h: `FABLE_RETAIL_CCREATUREACTION_TROLLWHACKGROUNDBASE_CLASS` -- the polymorphic retail class on CCombatActionBase (PerformAction, Initialise, protected virtual InitAnimation slot 81 / GetEffectName slot 82, private WhackGround); default generated pad layout unchanged. Row 0x008C61C0.
- engine/CCombatActionBase.h: `FABLE_RETAIL_CCOMBATACTIONBASE_OWN_VIRTUALS` -- in the opaque (pad) branch, the eleven virtuals CCombatActionBase adds (GetNextPos .. GetDamageMultiplier, slots 70..80; order checked against vtable 0x0128E0EC and the PDB branch); declarations only. Row 0x008C61C0.

## 0x006D0630

engine/CTCWife.h: FABLE_RETAIL_CTCWIFE_NICEGIFT -- declares CTCWife::ReceivedNiceGiftIsOpinionReactionHappening(long) 0x006D0630 (Ego_r.pdb / FableWin.pdb ?ReceivedNiceGiftIsOpinionReactionHappening@CTCWife@@QAE_NJ@Z); declaration only, no layout or vtable change. Full copy of the current repo header plus this block.
stlport/fable_map.h: FABLE_RETAIL_STLP_TREE_FIND_MEMBER_TEMPLATE_OUTOFLINE -- the STLport member template `_Rb_tree::find<KT>` with its body visible but `__declspec(noinline)` (retail's CTCWife TU calls the map<long,long> instance 0x005937E1 out of line); used by row 0x006D0630. Full copy of the current repo header plus this block, with ONE rewritten line: the repo's line 201 `#ifdef FABLE_RETAIL_STLP_TREE_FIND_MEMBER_TEMPLATE` becomes `#elif defined(FABLE_RETAIL_STLP_TREE_FIND_MEMBER_TEMPLATE)` behind the new `#if defined(FABLE_RETAIL_STLP_TREE_FIND_MEMBER_TEMPLATE_OUTOFLINE)` branch, so the existing FIND_MEMBER_TEMPLATE path and its `#else` / `#endif` (and the default build) are unchanged; only OUTOFLINE wins when both are defined. (A declaration-only `_DECL` branch tried during authoring, 477 v 459 bytes, was dropped before hand-back.)

## 0x0083D990

engine/CThingPhysicalSwitch.h: FABLE_RETAIL_CTHINGPHYSICALSWITCH_LEVELSCRIPT -- declares the virtual override ConstructFromLevelScript(const char*) 0x0083D990 (declaration only).

## 0x0072EB40

engine/CTCBuyableHouse.h: FABLE_RETAIL_CTCBUYABLEHOUSE_RECEIVEDCONFIRMATION -- declares GetPrice 0x006C0120 / SetRented 0x006C1150 / SetOwnedByPlayer 0x006C17B0 / IncrementDressLevel 0x006C04B0 (+ GetPriceForNextDressLevel 0x006BF920 when FRAMEUPDATE is off); declarations only.
engine/CTCHeroStats.h: FABLE_RETAIL_CTCHEROSTATS_PROPERTYOWNED -- new #elif branch: AddMoney(long,bool) 0x0057B327 + NumberOfShopsOwned +0xE0 / NumberOfHousesOwned +0xE4 (rest opaque, sizeof 0x1DC).

## 0x00BA4A40

# Header overlay, row 0x00BA4A40 (wf8). Each file = full copy of the current repo header + the opt-in below; default paths unchanged.
- engine/CEnginePrimitiveManagerStaticMeshesBase.h: `FABLE_RETAIL_CENGINEPRIMITIVEMANAGERSTATICMESHESBASE_BLANKTEXTURES` -- private declaration of SetupStaticBlocksBlankTextures (0x00BA4A40; Ego_r.pdb + FableWin.pdb mangled name), in both branches of the header.
- engine/C3DTextureStages.h: `FABLE_RETAIL_C3DTEXTURESTAGES_EQUALS` -- inline `operator==` (FableWin.pdb ??8C3DTextureStages@@QBE_NABV0@@Z; compares +0, +4, +8 byte). NOTE: the repo's one-line class definition `class C3DTextureStages {...bool UseBlankTextures;};` is split so the #ifdef block sits inside the class: the `};` moves to its own line (hdrcheck2 `default=DIFF[12]`); the default path is token-identical, only the static-assert counter changes (_6 -> _12, it is __LINE__-based).
- engine/C3DPrimitive2.h: `FABLE_RETAIL_C3DPRIMITIVE2_RECORDEDSTATE` -- inline GetRecordedFrame / PeekRecordedTextureStages / SetRecordedTextureStages (FableWin.pdb; +0x7C / +0x6C).
- engine/CGraphicInfo.h + engine/CGraphicInfoFrame.h: `FABLE_RETAIL_CGRAPHICINFO_TEXTURETYPE` -- inline CGraphicInfo::GetTextureType (+0x1C) and CGraphicInfoFrame::GetTextureType (GraphicInfo +0xC; declared in CGraphicInfoFrame.h, body in CGraphicInfo.h where CGraphicInfo is complete). FableWin.pdb names.

## 0x007BBE34

# hdr overlay (row 0x007BBE34)
- engine/NThingShot_CTCShotDefinitionSpreadShotProjectile.h -- NEW shared header (no repo header existed): class NThingShot::CTCShotDefinitionSpreadShotProjectile : CTCShotDefinitionProjectile, `protected: virtual void InitMovement();` (vtable 0x01240D84 slot 33), size 0xA4. No opt-in macros; no existing header changed.

## 0x00A128E0

engine/CPhysicsMesh.h: FABLE_RETAIL_CPHYSICSMESH_CONTAINMENTVOLUME -- declares CPhysicsMesh::IsInContainmentVolume (0x00A6CA20) and PeekRoughProportionOfSphereContained (0x00A6CA80), declarations only.
engine/CPhysicsMeshObject.h: FABLE_RETAIL_CPHYSICSMESHOBJECT_CONTAINMENTVOLUME -- declares CPhysicsMeshObject::IsInContainmentVolume (0x00A128E0) and PeekRoughProportionOfSphereContainedInVolume (0x00A12AB0), declarations only.

## 0x00B903D0

# wf8 row 0x00B903D0 header overlay (full copies of the current repo headers plus `#ifdef FABLE_RETAIL_*` additions only)
- engine/CShaderRenderManager.h -- FABLE_RETAIL_CSHADERRENDERMANAGER_INLINE_BUFFERDRAW (needs _INLINE_BLOCKDRAW + FABLE_RETAIL_CVERTEXBUFFERWIN32): inline bodies of `IsFixedFunctionVertexMode() const`, `SelectVertexBuffer(CVertexBufferWin32&)` and the seven-argument `DrawBuffersIndexed` (plain `inline`; bodies as landed 0x00B8EB00); includes engine/CVertexBufferWin32.h. Used by 00b903d0.
- engine/C3DPrimitive2.h -- FABLE_RETAIL_C3DPRIMITIVE2_VERTEXBUFFER (inside the FABLE_RETAIL_C3DPRIMITIVE2_INDEXBUFFER branch): `boost::scoped_ptr<CVertexBufferWin32> VertexBuffer` (+0x30, Ego_r.pdb type) instead of the opaque 4 bytes; forward-declares CVertexBufferWin32. No layout change. Used by 00b903d0.
- engine/CEnginePrimitiveManagerOcclusionBase.h -- FABLE_RETAIL_CENGINEPRIMITIVEMANAGEROCCLUSIONBASE_RENDERUNTEXTURED (inside FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS): private `void RenderUntexturedStaticMeshPrimitive(C3DPrimitive2*, const CMatrix3x4&)` 0x00B903D0. Used by 00b903d0.

