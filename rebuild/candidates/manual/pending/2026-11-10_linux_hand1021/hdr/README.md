# agent1021 header overlay (chunk 1021)

Every row was graded with `FABLE_HDR_OVERLAY=$A/hdr` exported. Each file here is a copy of `rebuild/include/<same path>` at HEAD
plus new opt-in blocks only: with none of the new macros defined, the preprocessed text of every existing includer is unchanged
(two new files are only reached through a new opt-in). Copy these over `rebuild/include/` when landing.

| File | New opt-in | Row | What it adds |
|---|---|---|---|
| engine/CNavigatorExternalInterface.h | `FABLE_RETAIL_CNAVIGATOREXTERNALINTERFACE_AGENTLISTS` | 0x00A47D10 | static `std::list<std::vector<const CSteeringAgent*>*> AgentLists` (0x013D2834) and the static `__fastcall` RemoveAgentFromLists 0x00A47D10 / RemoveAgentListFromLists declarations (Ego_r.pdb `SIX`) |
| engine/CTCEnemy.h | `FABLE_RETAIL_CTCENEMY_QUERIES` | 0x0076D2F0 | inline StatGetInterfaceType, GetPFaction, IsFriendsWithEverything; declarations IsEnemyOfFaction 0x0076C7D0, IsAlliedWithFaction 0x0076C7F0, IsEnemyOf 0x0076CCE0 (private), IsAllyOf 0x0076D460 (private), static CalculateEnemyTC 0x0076CFE0 (private), static AreThingsEnemies 0x0076D2F0 / AreThingsAllies 0x0076D4F0 |
| engine/ThingAlive.h | `FABLE_RETAIL_IS_THING_ALIVE_NOINLINE` (MSVC only) | 0x0076D2F0 | the same IS_THING_ALIVE body `__declspec(noinline)`: retail calls the COMDAT 0x0043DBF0 |
| engine/CThing.h | `FABLE_RETAIL_CTHING_HASBASEDEF` | 0x0066BC10 | FableWin.pdb inline `bool HasBaseDef() const { return !PBaseDef.IsNull(); }` |
| engine/CTCSoundPlayer.h | `FABLE_RETAIL_CTCSOUNDPLAYER_CRITERIA` | 0x0066BC10 | private GetPotentialCriteriaStrings(const CCharString&, std::vector<CCharString>&) declaration |
| engine/CEntitySoundDef.h (new) | (included by the row only) | 0x0066BC10 | `class CEntitySoundDef : public CSubDefClassBase`: StatGetTypeName, SoundMap (+0x28, opaque 0x20), SoundIdentifier (+0x48), VoicePitchOverride, AnimCriteriaClipDistance (Ego_r.pdb names) |
| engine/CDefClassBase_GetSubDef.h | `FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE` (MSVC only) | 0x0066BC10 | the real GetSubDef<T> template body declared `__declspec(noinline)` (retail COMDAT 0x0065F604 is called, and the visible body tells c2 the CDefPointer local is not captured) |
| engine/CBankFile.h | `FABLE_RETAIL_CBANKFILE_ENTRYDATASIZE` | 0x00BEC680 | `unsigned long GetEntryDataSize(unsigned long) const; // 0x009CC240` |
| engine/CMeshDataBank.h | `FABLE_RETAIL_CMESHDATABANK_TEXTUREBANK` | 0x00BEC680 | inline `GetTextureBank()` |
| engine/C3DMeshInfo.h | `FABLE_RETAIL_C3DMESHINFO_GETMESH` | 0x00BEC680 | GetMesh(CCountedPointer<const C3DMesh2>&, unsigned long, unsigned long) const 0x00A25E70; inline PeekTextureBank() |
| engine/C3DMesh2.h | `FABLE_RETAIL_C3DMESH2_STATS` | 0x00BEC680 | inline GetMaterialCount(); GetVertexCount 0x00A89320 / GetTriangleCount 0x00A89350 declarations |
| engine/C3DMeshMaterial.h | `FABLE_RETAIL_C3DMESHMATERIAL_MAPBANKINDICES` | 0x00BEC680 | the Diffuse/Bump map-bank getters now also under this macro (`#if defined(STATICBLOCKS_TEXTURE_HELPERS) \|\| defined(MAPBANKINDICES)`), plus GetReflectionMapBankIndex / GetIlluminationMapBankIndex |
| engine/CEngineInternalPrimitiveMeshBase.h | `FABLE_RETAIL_CENGINEINTERNALPRIMITIVEMESHBASE_COLLECTSTATS` | 0x00BEC680 | static `__fastcall` CollectStats(const C3DMeshInfo&, std::map<unsigned long, unsigned long>&, EEnginePrimitiveStatIndex, unsigned long) |
| engine/CGraphicDataBank.h | `FABLE_RETAIL_CGRAPHICDATABANK_BANKFILE` (default branch, without `_GRAPHICSIZE`) | 0x00BEC680 | `class CGraphicDataBank : public CBankFile` with ChangeScene() and the unmodelled tail to 0x30C, so GetEntryDataSize is reachable |
| engine/CThingPhysical.h | `FABLE_RETAIL_CTHINGPHYSICAL_CTOR` or `FABLE_RETAIL_CTHINGGAMEOBJECT_CTOR` | 0x0083D460 | public `CThingPhysical(unsigned long type); // 0x005296B0` |
| engine/CThingGameObject.h | `FABLE_RETAIL_CTHINGGAMEOBJECT_CTOR` | 0x0083D460 | `CThingGameObject(unsigned long type) : CThingPhysical(type) { }` |
| engine/CThingMarker.h | `FABLE_RETAIL_CTHINGMARKER_CONSTRUCT` | 0x0083D460 | `CThingMarker() : CThingGameObject(StatGetType())` and the protected inline ConstructFromParams(long, const C3DVector&, const CCharString&) (CThingGameObject::ConstructFromParams, then Health = MaxHealth = 1.0f) |
| engine/C3DVector.h | `FABLE_RETAIL_C3DVECTOR_GETANGLEYZ` (inside the `FABLE_RETAIL_C3DVECTOR` branch) | 0x00A0C570 | FableWin.pdb inline ?GetAngleYZ@C3DVector@@QBEMXZ: atan2(Z, sqrt(Y*Y+X*X)) * 1/(2 pi) wrapped into [0, 1) by fmod |
| engine/CRightHandedSet.h | `FABLE_RETAIL_CRIGHTHANDEDSET_NORMALISE` (inside `FABLE_RETAIL_CRIGHTHANDEDSET`) | 0x00A0C570 | FableWin.pdb inline ?Normalise@CRightHandedSet@@QAEXXZ: Up then Forward AccurateNormalise (0x00A14440) |
| engine/CCamera.h | `FABLE_RETAIL_CCAMERA_SETANGLEXY` | 0x00A0C570 | `void SetAngleXY(float angle); // 0x00A0C570` |
| engine/CEnvironment.h | `FABLE_RETAIL_CENVIRONMENT_THEMESTACKS` | 0x006B3E60 | PriorityStackChannels typed `CArray<NEnvironmentInternal::CEnvironmentThemePriorityStack>` and ThemeSet typed `NEnvironmentInternal::CEnvironmentThemeResourcePallette` (same 12-byte members, offsets asserted) |
| engine/CEnvironment.h | `FABLE_RETAIL_CENVIRONMENT_SETWEIGHTEDTHEMES` | 0x006B3E60 | `void SetWeightedThemes(long, long, long*, float*, long); // 0x006B3E60` |
| engine/CEnvironment.h | `FABLE_RETAIL_CENVIRONMENT_THEMEINDEX` (needs `FABLE_RETAIL_CDEFINITIONMANAGER_GETDEFCLASSINDEX`) | 0x006B3E60 | GetLocalEnvironmentThemeIndexFromGlobal with its body (`DefinitionManager->GetDefClassIndexFromGlobalIndex(i)`; retail's out-of-line copy 0x006B2C50 is `mov ecx,[ecx+4] ; jmp 0x009ACCA0`); includes CGameDefinitionManager.h |
| engine/CEnvironmentThemePriorityStack.h (new) | (reached through THEMESTACKS) | 0x006B3E60 | NEnvironmentInternal::CEnvironmentThemePriorityStack: `CArray<CEnvironmentThemePriorityStackEntry> Ents` (Ego_r.pdb), FableWin inline GetEntry(long), ProcessTransitions / CalculateTheme declarations |
| engine/CEnvironmentThemeResourcePallette.h | `FABLE_RETAIL_CENVIRONMENTTHEMERESOURCEPALLETTE_GETCOUNT` | 0x006B3E60 | FableWin.pdb inline `long GetCount() const { return DaySets.size(); }` (default: the declaration as before) |
| stlport/fable_algorithm.h | `FABLE_RETAIL_STLP_LOWER_BOUND` | 0x006B3E60 | `__fastcall` lower_bound(first, last, val) declaration (COMDAT 0x006B5A50) |
| stlport/fable_containers.h | `FABLE_RETAIL_STLP_VECTOR_INSERT_OUTOFLINE` | 0x006B3E60 | vector::insert(iterator, const T&) declaration only (retail calls 0x006B5ED0) |
| stlport/fable_containers.h | `FABLE_RETAIL_STLP_VECTOR_CLEAR_OUTOFLINE` | 0x006B3E60 | vector::clear() declaration only (retail calls 0x006B5B50) |
| engine/CTCSummonSpell.h | `FABLE_RETAIL_CTCSUMMONSPELL_TELEPORT` | 0x0079AA90 | private IsValidTeleportPos(const C3DVector&, const CWorldMap&, const CThing*) 0x0079A890 and GetValidTeleportPos(const CThing*) 0x0079AA90 declarations (Ego_r.pdb) |
