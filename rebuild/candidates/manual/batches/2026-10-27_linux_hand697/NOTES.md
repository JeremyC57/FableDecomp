# Hand chunk 697 (C3DClothPrimitive .. CChecksumCacheFile): 10 of 10 byte-exact

All ten NA040 rows were named from their own vtable / ??_G / caller / PDB evidence before authoring. 10 RELOCATION_MATCH,
every test PASS under g++ and VC7.1, read-only vland: WINS 10 (no RELOC_IDENTITY / RELOC_RENAMED; one
SHARED_HEADER_AVAILABLE warning on 0x00569D30, answered below). Six of the ten catalog labels were wrong: the three
"~CPatchTesselationEdgeStrip" rows are LHHttpHeaderStatus / CLocalDetailCacheMap / CChecksumCacheFile, the second
"~C3DClothPrimitive" is ~C3DMeshObject, the second "~CScriptGameResourceObjectMovieBase" is ~...ScriptedThingBase, the
second "~CEngineInternalPrimitiveMeshBase" is ~CTCPhysicsStandard. No row needed more than 2 probes; no flags, no pragmas.

## What matched (10)
- **C3DClothPrimitive::~C3DClothPrimitive 0x00AC3B80** / **C3DMeshObject::~C3DMeshObject 0x004C3200** -- empty user dtors
  (vtable store first) over engine/CCountedPointer.h: the inline release of the CCountedPointer member (ParticleProgram +4 /
  Mesh +8), then ~CBaseClassNonCopyable 0x0099A300 / ~CBaseClass 0x0099A2E0. Same shape as ~CScriptThing (chunk 696).
- **CScriptGameResourceObjectMovieBase / ScriptedThingBase implicit dtors 0x006E7B80 / 0x007E74D0** -- byte twins, two
  symbols: each class's ctor (0x006E7B60 / 0x007E72A0) stores its own vtable and each ??_G (0x006E7BD0 / 0x007E7520) expands
  its own copy. Straight from the shared retail headers (FABLE_RETAIL_BASE_OBJECT): implicit derived dtor, PImp's inline
  CCountedPointer release, the inline user `~CScriptGameResourceObjectBase() {}` keeps the 0x0126008C store; DestroyOne
  emission helper (chunk 656/657).
- **CEngineInternalPrimitiveMeshBase::~CEngineInternalPrimitiveMeshBase 0x00BEBC50** -- `if (!SimulationInfo.IsNull())
  SimulationInfo->Clear();` then MeshEffect and SimulationInfo CIVCountedPointer releases (named local, chunk 696), then
  ~CEngineInternalPrimitiveOcclusionBase.
- **CTCPhysicsStandard::~CTCPhysicsStandard 0x00723F90** -- `if (PPhysicsMeshInfo) delete PPhysicsMeshInfo;` (trivial type: the
  test is the source's), CTCPhysicsBase's implicit dtor expands the PhysicsDef CDefPointer release (engine/CDefPointer.h), ~CTCBase.
- **C3DAnimationInterpolator::~C3DAnimationInterpolator 0x004C2E90** -- implicit; the two implicit C3DAnimationBlendState
  members (EndState, StartState) inline their CIVCountedPointer<CSharedData> release and call ~CBaseClass out of line.
- **LHNetworkLib::LHHttpHeaderStatus::~LHHttpHeaderStatus 0x00569D30** -- `{ Reset(); }`, three STLport strings, then the inline
  user `~LHHttpHeaders() { Reset(); }` (its own out-of-line copy is 0x00569C30) with the header string and the out-of-line
  ~vector<string> 0x00569940. Real STLport 4.6 _String_base / basic_string / _Vector_base / vector templates.
- **CLocalDetailCacheMap::~CLocalDetailCacheMap 0x00BDED60** -- empty user dtor; palette entry CArray and AreaUpdateList
  vector free, then the nested CQuadTreeElement's out-of-line virtual dtor as the tail jump.
- **CChecksumCacheFile::~CChecksumCacheFile 0x00A60CE0** -- `{ Close(); }`; STLport 4.6 ~_Rb_tree = clear() (leftmost, root,
  rightmost, count order) and ~_Rb_tree_base's null-tested header free; then ~CWideString.

## Names and evidence (all proven, two sources each; none descriptive)
| row | symbol | evidence |
|---|---|---|
| 0x00AC3B80 | ??1C3DClothPrimitive@@UAE@XZ | vtable 0x0129DF74 RTTI; sole caller ??_G 0x00A96000 (slot 0); Ego_r + FableWin name; Ego_r members |
| 0x004C3200 | ??1C3DMeshObject@@UAE@XZ | vtable 0x01239860 RTTI (??_G 0x004C3250 expands it); 7 callers destroy C3DMeshObject locals (landed 0x004BC670 binds this spelling); Ego_r + FableWin |
| 0x006E7B80 | ??1CScriptGameResourceObjectMovieBase@@UAE@XZ | ctor 0x006E7B60 vtable 0x01260EF4, ??_G 0x006E7BD0; Ego_r + FableWin; engine header |
| 0x007E74D0 | ??1CScriptGameResourceObjectScriptedThingBase@@UAE@XZ | ctor 0x007E72A0 vtable 0x0127094C, ??_G 0x007E7520; Ego_r + FableWin; engine header |
| 0x00BEBC50 | ??1CEngineInternalPrimitiveMeshBase@@UAE@XZ | vtable 0x012A41D4 RTTI; ??_G 0x00BA7FF0; Ego_r 0x00AA3E50 + FableWin 0x02EE37A0; landed 0x00BD9250 binds it |
| 0x00723F90 | ??1CTCPhysicsStandard@@UAE@XZ | vtable 0x01265F84 RTTI; callers are the ??_G of the derived CTCPhysicsControlled / CTCPhysicsNavigator (rtti_bases); Ego_r + FableWin |
| 0x004C2E90 | ??1C3DAnimationInterpolator@@UAE@XZ | ??_G 0x004C3D10 of vtable 0x01239840 (RTTI : CBaseClass); Ego_r + FableWin; Ego_r members StartState/EndState |
| 0x00569D30 | ??1LHHttpHeaderStatus@LHNetworkLib@@QAE@XZ | Ego_r 0x005E04D0 + FableWin 0x022E4280; first call is the landed LHHttpHeaderStatus::Reset; Ego_r layout with retail strings |
| 0x00BDED60 | ??1CLocalDetailCacheMap@@QAE@XZ | Ego_r 0x00A92180 + FableWin 0x02E3AE50; landed callers DeleteMap / ~CEngineLocalDetailGenerator bind it; Ego_r members |
| 0x00A60CE0 | ??1CChecksumCacheFile@@QAE@XZ | Ego_r donor order Close 0x00C006D0 -> ??1 0x00C00700 = retail Close 0x00A60C30 -> this; FableWin 0x033A6870 |

Callee names (checked by address with calls.py; every target is retail's): operator delete 0x00BFE9BC, free 0x00BFEA14,
~CBaseClass 0x0099A2E0, ~CBaseClassNonCopyable 0x0099A300 (MAE), ~CBaseObject 0x0099A430 (MAE), ~CTCBase 0x00686830,
~CWideString 0x0099B510, CChecksumCacheFile::Close 0x00A60C30, the landed _Rb_tree<CWideString,..CEntry..>::_M_erase 0x00A60A00
(same mangled spelling as its landed leaf), LHHttpHeaderStatus::Reset 0x0083FB60. New / corrected bindings (each proven from two
sources, none collides with another address's spelling):
- 0x00B81E20 ?Clear@CEnginePrimitiveSimulationInfo@@QAEXXZ (landed leaf says ClearSimulations: respell, NAME_FIXES).
- 0x00B9C660 ??1CEngineInternalPrimitiveOcclusionBase@@UAE@XZ (bound QAE and a free stand-in: respell).
- 0x00BDDFB0 ??1CQuadTreeElement@CLocalDetailCacheMap@@UAE@XZ (bound ??1QuadTreeElement@@UAE@XZ: respell).
- 0x0083FBC0 ?Reset@LHHttpHeaders@LHNetworkLib@@QAEXXZ (the landed row's own leaf; bound by 0x00569C30 as ?Clear@CBlockVectorOwner_00569c30).
- 0x00569940 ??1?$vector@V?$basic_string@DV?$char_traits@D@std@@V?$allocator@D@2@@std@@V?$allocator@V?$basic_string@...@std@@@2@@std@@QAE@XZ
  (three invented class names bound; STLport V char_traits as in the landed vector<string> rows).

## Parked for Fable 5.1 (0)

## Skipped
None. Extents: all ten oracle extents are right; each ends with its tail `jmp` followed by int3 padding.

## Label corrections
- 0x004C3200 "C3DClothPrimitive::~C3DClothPrimitive" -> C3DMeshObject::~C3DMeshObject (0x00AC3B80 is the real ~C3DClothPrimitive).
- 0x007E74D0 "CScriptGameResourceObjectMovieBase::~..." -> CScriptGameResourceObjectScriptedThingBase::~... (0x006E7B80 keeps MovieBase).
- 0x00723F90 "CEngineInternalPrimitiveMeshBase::~..." -> CTCPhysicsStandard::~CTCPhysicsStandard (0x00BEBC50 keeps the mesh base).
- 0x00569D30 / 0x00BDED60 / 0x00A60CE0 "CPatchTesselationEdgeStrip::~..." -> LHNetworkLib::LHHttpHeaderStatus::~LHHttpHeaderStatus,
  CLocalDetailCacheMap::~CLocalDetailCacheMap, CChecksumCacheFile::~CChecksumCacheFile. (A 4th propagated
  "~CPatchTesselationEdgeStrip" label sits on 0x00B81CF0, not in this chunk.)
- 0x00569C30 catalog "CBlockVectorOwner_00569c30::~CBlockVectorOwner_00569c30" -> LHNetworkLib::LHHttpHeaders::~LHHttpHeaders.
- 0x00B81E20 catalog "CEnginePrimitiveSimulationInfo::ClearSimulations" (manifest "CParticleProgram::ClearGroups") ->
  CEnginePrimitiveSimulationInfo::Clear.
- Wrong bound spellings for 0x006E7B80 (4), 0x007E74D0 (2), 0x004C2E90 (3), 0x00A60CE0 (1), 0x0099A430 (4), 0x0099A300 (many):
  NAME_FIXES.tsv.

## Header debt
- engine/CEngineInternalPrimitiveBase.h, CEnginePersistentPrimitiveBase.h, CEngineInternalPrimitiveOcclusionBase.h: flattened
  vptr (`void* __vftable`) and opaque members; 0x00BEBC50 uses a local polymorphic chain (CIVCountedPointeeBase from the shared
  retail model) with the Ego_r offsets. No CEngineInternalPrimitiveMeshBase header exists.
- engine/CTCBase.h / engine/CTCPhysicsStandard.h flatten the vptr: 0x00723F90 models CTCBase / CTCPhysicsBase /
  CTCPhysicsStandard locally (`// Layout:`).
- engine/CLocalDetailCacheMap.h: flattened record with opaque QuadTree / AreaUpdateList / CollectionTypePalette (no nested
  CQuadTreeElement / CObjectTypeCollectionPalette, no element types): 0x00BDED60 local model with the header's offsets.
- engine/LHHttpHeaderStatus.h (vland SHARED_HEADER_AVAILABLE): flattened, global namespace (retail is LHNetworkLib::), opaque
  strings and LHHttpHeaders, `#pragma pack(1)`: 0x00569D30 local model with the same offsets.
- No engine headers for C3DClothPrimitive, C3DMeshObject, C3DAnimationInterpolator / C3DAnimationBlendState, CChecksumCacheFile,
  CIVCountedPointer (each row carries a local CIVCountedPointer<T> template with the chunk 696 Release body).
