# Chunk 580 -- notes (names, evidence, label corrections, header debt)

9 of 10 rows RELOCATION_MATCH with PASS tests under g++ and VC7.1; 0x00F3D200 parked in differ/ (285v284).

## Names and evidence (two sources each)
| address | name | evidence |
|---|---|---|
| 0x00F37F60 | `std::vector<CGPUScratchBufferTextureManager::CResourceEntry>::_M_insert_overflow` | 44-byte element (0x2E8BA2E9 reciprocal + `sar 3`, `imul 2Ch`, Ego_r CResourceEntry); only caller is the vector's _M_fill_insert 0x00F381F0 (overflow arm), which the landed resize 0x00F385A0 binds; STLport 4.6 IAE body |
| 0x00F381F0 | `std::vector<CGPUScratchBufferTextureManager::CResourceEntry>::_M_fill_insert` | reloc_bindings binds this exact symbol from the landed resize 0x00F385A0; STLport 4.6 body verbatim; callees are the CResourceEntry helpers |
| 0x00F3D200 (DIFFER) | `CTriangleStripifier2::CheckForReshadedVerts(CArray<unsigned short>&)` | Ego_r.pdb CTriangleStripifier2 name set: CheckForReshadedVerts (0x180) and GetMeshIslandSubsection (0x630) sit right before CalcSectionScore; retail 0x00F3D200 (0x11C) and 0x00F3D320 (0x510) sit right before the landed CalcSectionScore 0x00F3D830; signature `AAEXAAV?$CArray@G@@@Z` = void, one word-index array, `ret 4`; no xrefs (debug check, result discarded). Probable rather than proven (two sources: name set + order/size/signature) |
| 0x00F36710 | `NFractal::CRidgedMultifractal::GetFractalHeight(const C2DVector&) const` | Ego_r donor order Ridged 0x00BC8020 / Hybrid 0x00BC8190 / Hetero 0x00BC8370 = retail 0x00F36710 / manifest CHybridMultifractal 0x00F36860 / CHeteroTerrainFractal 0x00F36A30; body is Musgrave's ridged multifractal (offset - abs(noise), squared, weight = signal * gain clamped); CFractal layout (ExponentArray +8, Lacunarity +0x408, Octaves +0x410, Noise +0x418) matches the Ego_r struct exactly |
| 0x00F34160 | `std::vector<CFireHeartPatternDef>::_M_insert_overflow` | 0x34 stride; per-element _Construct 0x00D34EC0 (bound by the landed _M_allocate_and_copy 0x00D35020); inline copy stores vtable 0x012CCC50 (slots 18-20 = CFireHeartPatternDef Transfer/Copy/GetSizeofClass) |
| 0x00F3AE50 | `CRangeCompressor::Compress(const void*, long, long, unsigned char*, long)` | Ego_r order: CalcCompressionScript 0x00BDCDE0 then this overload 0x00BDD0A0; retail CalcCompressionScript 0x00F3ABF0 is called here with its exact 5-argument PDB signature; `ret 14h` with the 5th argument (destSize) never read; callees bound Decompress 0x00F39ED0 and the landed private Compress 0x00F3A2E0 |
| 0x00E389E0 | `NScript::CV_AssassinAttacksScript::CAssassinChest::Main` | only reference is slot 1 of entity vtable 0x012E3738 stored by the landed Alloc 0x00E388F0; Ego_r name set (Alloc, Init, GetParentScript, OnPredicateFail, Main) |
| 0x00D27050 | `NScript::CQ_DragonBossFightScript::PlayCutscene(CCharString)` | Ego_r/FableWin PDB symbol; all five callers inside CQ_DragonBossFightScript::DoMission 0x00D26190; cs_flags at +0x84 (Ego_r +0xA0 minus CScriptBase/vector shrink) |
| 0x00ED7830 | `NScript::CV_TalentlessBardScript::Main` | slot 2 of quest vtable 0x012F3428 stored by the landed ctor 0x00ED76E0 (slots RegisterMain/OnPersist landed); binding/spawn names paired with their string literals |
| 0x00EF7600 | `NScript::CPersonalScript_EmptyScript::CStealthGuard::Main` | only reference is slot 1 of entity vtable 0x012F6238 stored by the landed Alloc 0x00EF2820; Ego_r name set |

Descriptive (no PDB name exists): globals 0x0143E980 `g_RangeCompressBytesIn` and 0x0143E97C `g_RangeCompressBytesOut`
(Compress statistics; they follow the landed Decompress statistics 0x0143E970 / 0x0143E978, which are also descriptive).
FableWin.pdb has only GetTotalDecompressTime/GetTotalDecompressData accessors for the class statics.

## Label corrections (manifest/catalog -> proven)
- 0x00F34160 `vector_reallocate_insert_ThingBuildingDef_map` -> vector<CFireHeartPatternDef>::_M_insert_overflow
- 0x00F37F60 `Vector_Reallocate_CameraTag` -> vector<CResourceEntry>::_M_insert_overflow
- 0x00F381F0 `Vector_Resize_CPreTransposedBoneMatrix` -> vector<CResourceEntry>::_M_fill_insert
- 0x00F37970 manifest `CPreTransposedBoneMatrix::Set` -> CGPUScratchBufferTextureManager::CResourceEntry copy ctor (callee of 0x00F381F0)
- 0x00F3D200 `NavigatorType_BuildFromIndices` -> CTriangleStripifier2::CheckForReshadedVerts
- 0x00F36710 `CNoise_EvaluateFractal` (autoname-t5) -> NFractal::CRidgedMultifractal::GetFractalHeight
- 0x00F3AE50 `RangeCompressor_Compress` -> CRangeCompressor::Compress (public 5-argument overload)
- 0x00E389E0 / 0x00ED7830 / 0x00EF7600 `sub_*` -> the three Mains above
- 0x00D27050 `DoMission_Setup` -> CQ_DragonBossFightScript::PlayCutscene
- 0x00E24760 label `CConsoleCommandParameters::CConsoleCommandParameters` -> CPredicate_And<CIsThingAlive, CIsThingAlive> ctor (binding already right)
- 0x00ED7A40 label `TalentlessBard_OnScript` -> CV_TalentlessBardScript::WatchForCompletedQuests
- 0x00F37D30 manifest `_Copy_backward<pair<unsigned long,C3DDummyObject>*>` -> __copy_backward_ptrs<CResourceEntry*, CResourceEntry*>
- 0x00CB8930 label `CTCVillage::OnInitialActivate` -> CScriptBase::PostAddScriptedEntities (bound spelling; vland RELOC_NAME warning)

## Header debt (local models where no retail header exists yet)
- CScriptThing, CScriptBase, CEntityScriptBase, CGameScriptInterfaceBase (padded `virtual void vNNN();` slot fillers),
  CPredicateBase / CPredicate_And / CIsThingAlive, CEntityScriptBinding<T>, CScriptGameResourceObjectBase /
  ScriptedThingBase / MovieBase, CBaseObject, CLevelScriptBase<CQ_SunnyvaleMasterData>, CScriptDef (pad 0x3a4): the
  migration agent's retail NScript headers should replace them (0x00E389E0, 0x00EF7600, 0x00ED7830, 0x00D27050).
- CFireHeartPatternDef (0x00F34160): local class over the shared `engine/CDefClassBase.h` (FABLE_RETAIL_DEF_CLASS_BASE), with
  its three slot-18..20 overrides declared; no per-class header yet.
- CGPUScratchBufferTextureManager::CResourceEntry (0x00F37F60, 0x00F381F0): local struct (Ego_r members, engine/CPixelFormat.h);
  local C2DExtentsI {long X, Y}.
- NFractal::CFractal / CRidgedMultifractal / CNoise and C2DVector (0x00F36710): local; CFractal has a retail vtable (Ego_r
  vptr at +0) that retail discarded (no data reference to any GetFractalHeight), so only the layout is checked.
- CRangeCompressor + CScriptEntry, CArray<T> : std::vector<T> (0x00F3AE50, 0x00F3D200): local (CRangeCompressor is empty in
  the PDB, no vtable).
