# Hand chunk 244 (CActiveFile .. CExplosionRing): 26 of 40 byte-exact

## What matched (26)
- **STLport vectors (labelled CActiveFile)** — 0x00460D75 `vector<CMusicEntry>::operator=` (CSoundDef::MusicEntries), 0x004E2F56 `vector<CExplosionRing>::operator=` (CExplosionDef::ExplosionRings): the landed 16-byte operator= body (0x004E29BA / 0x00461017 model), size TU, `__forceinline` copy helpers.
- **CEnvironmentThemeDef** — 0x00473046 operator=, 0x0047318D copy ctor: memberwise in a size TU over the retail 0x3c-byte CParentDefClassBase (members = PDB − 4).
- **CEngineThemeDef::Transfer** 0x0045979B — typed CPersistContext transfers in persisted order; the `Passable` bit via a bool local homed in the dead context slot.
- **CExplosionRing::TransferBinaryIn** 0x004EB2B8 — inline 64-bit-checked stream Read; the value must come from by-value `ReadLong()/ReadFloat()` helpers (a shared address-taken local puts `this` in eax).
- **VS constant layouts** — 0x00BDB400 Lights ctor, 0x00BDB080 RepeatedMesh ctor: `Range = CShaderConstantRange(first, count)` temporaries (landed sibling model); RepeatedMesh puts its four ranges at +0x14c (retail base is 0x14c, PDB 0x150).
- **Destructors** — 0x00B898D0 ~CEngineScreenEffectOutlineGlow (shader/render-target/capture-block member dtors inline, glow-renderer model), 0x00BC2280 ~CEnginePrimitiveManagerParticleSpriteGroup (implicit dtor, CArray<CVertexShader> virtual element loops, 12-byte retail CArrays), 0x00B8DA80 ~CEngineOcclusionManager (label "~ParticleLineGroup" wrong; user body = inline shader Release()).
- **CEngineWaterRenderer** 0x00B7F600 — background shader-constant setup (label "CEngineWeatherRenderer::RenderSnowBox" wrong): inline PrepareForRender(0x604), BoneMatrices.resize(4, T()).
- **CEngineSkyRenderer::RenderSky** 0x00B66190 and **CEnginePrimitiveManagerWeaponTrails::RenderSubPrimitive** 0x00BCD490 — the tracked render-state Set must test on the state and forward to an inline manager-side ChangeState(state, v); either side alone swaps registers.
- **Audio CEntity** 0x00C65870 — GUID-list property setter (label "SetPhysicsCollisionProperty" wrong): memcmp intrinsic over six GUIDs, index in the dead size slot.
- **CEngineWorldMap** — 0x00B40070 AutoBuildWorldMapGrid (comparison operand order follows source), 0x00B42B10 DeleteMap (inline `ReleaseGameMap()` on the map, STLport resize/__copy_trivial trim), 0x00B42D10 AddMap (inline STLport list insert at begin()), 0x00B42750 OpenStaticMaps (one shared state store after the if/else), 0x00B41FA0 really `LoadWaterData` (PDB) (CMemoryDataInputStream is 0x1c bytes; counted entry dtor zeroes both words).
- **CEngineScene/Grid** — 0x00B943F0 really `CEngineScene::RenderEntireScene`, 0x00B942A0 really `RenderSceneAndCheckVisibility`, 0x00B92500 `CEngineSceneGrid::RenderCellClipped`: landed RenderLayerCells model (operator[] stand-in); manager fetched into a named local before the chain; the visibility out-bool lives in an inline wrapper.
- **CEngineWaterBackgroundSubPatch::Save** 0x00BFD670 — byte-indexed jump-table switch for the stride, empty compressor object homed in the dead stream slot, engine header + derived shell.
- **CEnvironmentThemeSetDay::GetBlendedTheme** 0x00888260 — RTM unrolls the find loop by 4; declaring prevTime before nextTime fixes the x87 load order.
- **CEnvironmentLookupTexture** ctor 0x00885F50 — `/GS`; polymorphic CBaseClassNonCopyable root for the local CDiskFileWin32; size/format as unnamed temporaries.

## Parked for Fable 5.1 (14)
- 00bc6340 CEnginePrimitiveGroupManager::RenderPrimitive DIFFER(319v322): retail keeps a dead `lea eax,[eax+ecx*8]` (handle address) after the child load; count from begin in ecx | tried: cached count, handle reference/pointer/getter, reference after null test | next: an iterator-style element pointer kept alive past the loop body
- 00bcfb00 CEnginePrimitiveManagerAnimatedMeshesBase::RenderPrimitiveBlocksIndexBuffer DIFFER(329v336): inline indexed draw homes primitive count/start/vertex count in frame words, manager in ebx | tried: cached counts, inline DrawIndexed(ib, nv, start, count, type=-1) | next: draw helper on the render core with the args in its own parameter order, or out-of-line stand-in
- 00b99710 ClipSubSections DIFFER(335v335): centre/radius copied dword by dword through ecx | tried: member stores, C3DVector ctor | next: copy via a by-value sphere getter or user copy ctor
- 00babf70 CEnginePrimitiveManagerRepeatedZSprites::RenderPrimitive DIFFER(381v381): `mov ecx,[g] ; mov ebx,ecx` vs ours `mov ebx,[g] ; mov ecx,ebx` | tried: named manager local, assignment-in-expression, GetOccludedFrames stand-in, _M_insert_overflow stand-in (fixed the frame) | next: lane gap: register copy direction
- 00ba0fc0 CEnginePrimitiveManagerStaticMeshesBase::AddStaticMesh DIFFER(333v333): flags store placed first (retail: loaded first, stored after Alpha); handle slot +8 vs +4 | tried: novtable intermediate ctor base (fixed vtable order), store orders, register through handle | next: lane gap: zero-register hoisting vs flags register
- 00b324a0 CEnginePrimitiveRenderer::SetPersistentPrimitive DIFFER(328v354): handle Clear re-reads Data; re-assignment keeps a zero register and a dead second release | tried: one handle model | next: CEnginePrimitiveHandle operator=(CEngineInternalPrimitiveBase*) as an out-of-line-shaped inline with old-pointer local
- 00b93860 CEngineSceneGrid::MovePrimitivesDown DIFFER(382v381): Level+1 folded into dec; 3-byte GridPos copies packed at +0/+4 (retail +2/+5) | tried: inline AddPrimitive forms, level parameter | next: GridPos copy through a by-value getter
- 00b939e0 CEngineSceneGrid::CheckVisibilityResults DIFFER(316v336): retail pins layer in ebx, result bool in the dead layer slot, this/cell spilled | tried: mask local, named child coords, plain return | next: the inline-wrapper bool trick of 00b942a0 for the child recursion
- 00b92300 CEngineSceneGridCell::CreateOcclusionGroup DIFFER(335v335): layer edi/handle ebx swapped | tried: handle reference after test, pointer form | next: lane gap: register naming
- 00b93b30 CEngineSceneMap::AddPrimitive DIFFER(282v395): inline-asm fistp floor helper | tried: one compiled model | next: lane gap: asm_bake family
- 00be06b0 CEngineStaticMapFileBlockManager::AddRequest DIFFER(264v322): counted-pointer temporaries (result copy, record copy, three releases) | tried: one structural model | next: model the record as pair<const long, SBlock> built from a counted local
- 00b84580 CEngineSubPrimitiveRenderer::AddSubPrimitive DIFFER(332v340): inline-asm fld/fistp sort-key conversion | tried: one compiled model | next: lane gap: asm_bake family
- 00bf9290 CEngineTexturePalette::Load DIFFER(377v362): read count vs ReadString temporary slot packing | tried: by-value ReadLong, address-taken count, named string, early count decl | next: ReadString through an inline wrapper returning into the entry
- 00b6e260 CEngineWaterRenderer::RenderBackgroundList DIFFER(381v379): occlusion box built from integer locals (0x3c frame) | tried: one direct model | next: C2DCoordI/C2DExtentsI locals for map pos and size, MinZ/MaxZ loaded first

## Skipped
- none (no row holds two functions; 0x00BFD670's row carries its own jump/index tables).

## Label corrections
- 0x00460D75 "CActiveFile::CopyAssign_16" -> std::vector<CMusicEntry>::operator= (CSoundDef MusicEntries).
- 0x004E2F56 "CActiveFile::operator=" -> std::vector<CExplosionRing>::operator= (CExplosionDef ExplosionRings).
- 0x00B8DA80 "~CEnginePrimitiveManagerParticleLineGroup" -> ~CEngineOcclusionManager (vtable 0x012A3CD0).
- 0x00B7F600 "CEngineWeatherRenderer::RenderSnowBox" -> a CEngineWaterRenderer background shader-constant setup (no callers; PDB name unknown).
- 0x00C65870 "CEntity::SetPhysicsCollisionProperty" -> audio-library CEntity GUID-list property setter (name unknown).
- 0x00B943F0 "CEngineSceneGrid::RenderAll" -> CEngineScene::RenderEntireScene; 0x00B942A0 "CEngineScene::RenderLayered" -> RenderSceneAndCheckVisibility.
- 0x00B41FA0 "CEngineWorldMap::LoadMapThemeData" -> CEngineWorldMap::LoadWaterData.
- Callees: 0x00B2FA90/0x00BACDD0/0x00B98B10/0x00BDBFF0/0x00B59840/0x00B44170/0x009F3220 labels are propagated (see source headers); 0x00993E80 is WriteUShort (manifest "WriteCBYTE").
