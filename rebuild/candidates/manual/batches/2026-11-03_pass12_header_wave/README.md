# Codex pass 12 header wave (2026-11-03)

Rows Codex pass 12 graded RELOCATION_MATCH that the first landing (`../2026-11-03_codex_pass12_landing/`) held for header
reasons. Each needed member went into the shared header as one `#ifdef FABLE_RETAIL_<CLASS>` branch (default path unchanged)
or into a new header; Codex's per-feature macros (FABLE_RETAIL_MAP_BRUSHES, _WILL_BUTTON_TIMER, _INIT_THING_TYPES ...) were
replaced by class macros. Sources: `rebuild/candidates/manual/pending/2026-11-02_codex_pass12/verified_pending/<addr>/`.

**Landed 11 of 23** (all RELOCATION_MATCH PASS, `vland.py --land`, logs/land.log); 12 held (HEADER_NEEDS.md, STATUS.tsv).
`bindings_multi_check.py` = 0 after landing.

| Row | Function | Headers (branch macro) | Note |
|---|---|---|---|
| 0x00AC2BE0 | C3DMeshPartitionBuilderElement::PartitionSpace | C3DVector, C3DBoundingBox (C3DBOUNDINGBOX), stlport/fable_containers.h (STLP_VECTOR: _vector.c `_M_fill_insert` body); new C3DBoundingSphere, C3DTriangle2, C3DVertex2, C3DMeshPartitionBuilderElement | 2775v2791 was the visible _M_fill_insert body |
| 0x006932B0 | CCreatureActionBase::RegisterActionEventTypes | CCreatureActionBase (RegisteredActionEvents map, DebugEventTrace); new NCreatureActionEvents | |
| 0x00AE0710 | CParticleProgram::CalculateAveragePatchSize | CParticleProgram (CPARTICLEPROGRAM retail class), C3DVector (GetSquaredMagnitude parenthesised); new CParticleSimulation, C4DVector, C3DQuad2 | |
| 0x00AE0070 | CParticleProgram::ApplyTransform | new CMatrix3x4, TConstraintType, CParticleDistanceConstraint, CParticleMinMaxDistanceConstraint | |
| 0x00AE8D50 | NParticleEngine::CParticleEmitterDatabase::Load | fable_string.h (CCHARSTRING: GetAsCharArray / NullChar / operator== / LoadFromFile), CStringParser, CPersistContext, CAFile; new CSymbolMap, NParticleEngine_CParticleEmitterDatabase | |
| 0x00B3BE10 | CEngineResourceManager::BuildAlphaStippleTexture | CEngineComponent (CENGINECOMPONENT), CEngineResourceManager (CENGINERESOURCEMANAGER), C2DVector (C2DVECTOR); new CAsyncLoadingProcessCallback, CCounter | GFRandom modelled in the row (GF*.h belonged to the asm-twins wave) |
| 0x00E5EBD0 | NScript::CV_BodyGuardScript::MakeHenchmanComment | fable_string.h (GetLength `PStringData==0 ? 0 :`, Truncate); new CV_BodyGuardScript | 1068v1064 fixed by the GetLength form |
| 0x00E0A820 | NScript::CQ_TraderEscortScript::TurnToBalv | stlport/fable_map.h (STLP_MAP: separate `map()` ctor); new NScript_CQ_TraderEscortScript | 1306v1304 fixed by the map() ctor |
| 0x008EDFF0 | CWillButtonHelperTimer::CWillButtonHelperTimer | CIntelligentPointer (retail branch: T* ctor, Set, private SetPointer), CTCInventoryAbilities (CTCINVENTORYABILITIES), CGameDefinitionManager (CGAMEDEFINITIONMANAGER hides the non-PDB GetDef), CSpecialAbilitiesForcePushDef (CSPECIALABILITIESFORCEPUSHDEF); new CSpecialAbilities{Berserk,BulletTime,Enflame}Def, CWillButtonHelperTimer, CGameComponent, CMainGameComponent | |
| 0x00522A20 | CThingManager::InitThingTypes | CThingManager (InitThingTypes, inline AddNewThingType, typed ThingTypeInfo / UpdateLists), CClassFactory (CCLASSFACTORY: typed ClassTypeInfo), Alloc on CThing (CTHING), CThingAICreature, CThingPlayerCreature, CThingBuilding, CThingMarker (each its class macro); new CClassTypeInfo, CClassTypeInit, CThingTypeInfo, CThingTypeInit, ThingLList, 9 CThing* subclasses | |
| 0x00821010 | CMap::LoadBrushesFromFile | new CFileFormatBrush (FABLE_RETAIL_CFILEFORMATBRUSH, the only pack(1) scope: serialized records, evidence comment from Codex PACKING_EVIDENCE.md), new CMap, CScriptedMapBrushInit, C3DAngles, CMapInfo; CScriptedMapBrush (CSCRIPTEDMAPBRUSH), CIDrawMap (CIDRAWMAP), CWorldMap (GetMapNumberAt overload order, vtable 0x01244AEC), C2DBoxI (C2DBOXI), CAFile (ReadVar), CThingManager (GetPThingWithUID), CThing (SetAsActivated); row needs FABLE_RETAIL_CARRAY | |

## Header changes (28 changed, 44 new)
`logs/hdrcheck2.txt`: default=same for 26 of 28. The two others: engine/CThingMarker.h (token-identical: only the one-line
class was split to host the CTHINGMARKER branch) and engine/CThingManager.h (a retail-only header with no default branch,
needs FABLE_RETAIL_CCLASSFACTORY: ThingTypeInfo / UpdateLists typed as std::vector, InitThingTypes, GetPThingWithUID and the
inline AddNewThingType declared). Retail-branch edits that touch rows already landed on them: CWorldMap overload order,
fable_string.h GetLength form (CCHARSTRING), CClassFactory typed ClassTypeInfo (CCLASSFACTORY), CIntelligentPointer ctors.

## Includer recheck
2301 landed sources include a changed header transitively (bin includers.py over every changed header, conditionals ignored).
2257 preprocess token-identically against the staged headers (fable_static_assert_N normalised; logs/ppcheck_diff.txt).
The other 44 (CCHARSTRING / CPERSISTCONTEXT / CWORLDMAP / CCLASSFACTORY / CTHING / CINTELLIGENTPOINTER definers) were rebuilt
with `vland.py --recheck-landed` on a full mirror (rebuild/include + stage): 44 RELOCATION_MATCH PASS
(includer_recheck_grades.tsv, logs/includer_recheck.log). The 11 rows were regraded together on the final stage before landing
(logs/final_regrade.log).

## Naming
`ghidra_out/labels_hdrwave_2026-11-03.tsv` (6 callee labels, applied). NAME_FIXES_QUEUE.tsv gets 6 `hdr1103` lines:
0x013BA8BC RegisteredActionEvents and 0x00CDBF70 map ctor (PRIORITY), 0x0082E8F0 / 0x0082E9A0 CScriptedMapBrush::SetThingToDelete /
SetThingToAdd (landed as CTCLook::IsTurningBody; PRIORITY), 0x0129AAF4 CCharString::NullChar and 0x009BBBF0
CPersistContext::CheckUnknownSymbols (REVIEW). Two-spelling targets left by ICF folding, not respelled: 0x00AE9CB0
(`__copy<pair*>` PAU / PBU), 0x004676EF (vector<bool>::_M_fill_insert, already queued).
