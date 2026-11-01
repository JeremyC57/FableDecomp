# Chunk 697 (NA040) -- 10 of 10 byte-exact destructors; six catalog labels wrong (2026-11-01)

All ten rows landed (`verify.log`: WINS 10, RELOCATION_MATCH + PASS; bindings multi-check 0; no flags, pragmas, asm or volatile; 1-2 probes each):
~C3DClothPrimitive 0x00AC3B80 (the label was right here; chunk 696's twin 0x004AA840 is ~CScriptThing), ~C3DMeshObject 0x004C3200 (was
~C3DClothPrimitive), the implicit ~CScriptGameResourceObjectMovieBase 0x006E7B80 and ~CScriptGameResourceObjectScriptedThingBase 0x007E74D0 (byte
twins, two symbols: each class's ctor stores its own vtable and each ??_G calls its own copy), ~CEngineInternalPrimitiveMeshBase 0x00BEBC50,
~CTCPhysicsStandard 0x00723F90 (its callers are the ??_Gs of CTCPhysicsControlled / CTCPhysicsNavigator), the implicit ~C3DAnimationInterpolator
0x004C2E90, LHNetworkLib::LHHttpHeaderStatus::~ 0x00569D30, ~CLocalDetailCacheMap 0x00BDED60 and ~CChecksumCacheFile 0x00A60CE0 (the last three
were "~CPatchTesselationEdgeStrip"). Three implicit-dtor rows carry the one-line ungraded DestroyOne helper (chunks 656 / 657 / 696; accepted).

16 labels applied (`ghidra_out/labels_chunk697_2026-10-27.tsv`; the agent wrote addresses without the 0x prefix, normalised by the coordinator); NAME_FIXES.tsv 16
rows + 1 coordinator row queued c697 for naming wave 28: CEnginePrimitiveSimulationInfo::Clear 0x00B81E20 (landed as ClearSimulations), the
LHNetworkLib group (~LHHttpHeaders 0x00569C30 landed as CBlockVectorOwner_00569c30, LHHttpHeaders::Reset 0x0083FBC0, the STLport
vector<std::string> dtor 0x00569940 bound under three invented class names), ~CEngineInternalPrimitiveOcclusionBase 0x00B9C660 (landed non-virtual;
retail UAE), CLocalDetailCacheMap::CQuadTreeElement::~ 0x00BDDFB0, and wrong bound spellings for 0x006E7B80 / 0x007E74D0 / 0x004C2E90 / 0x00A60CE0 /
0x0099A430 / 0x0099A300. A fourth propagated "~CPatchTesselationEdgeStrip" label sits on 0x00B81CF0 (outside this chunk). Header debt (NOTES.md):
the CEngine*PrimitiveBase / CTCBase / CTCPhysicsStandard headers flatten the vptr; CLocalDetailCacheMap.h has opaque members; LHHttpHeaderStatus.h
is a `#pragma pack(1)` global-namespace record with opaque strings (vland SHARED_HEADER_AVAILABLE on 0x00569D30, answered by the local model); no
headers for C3DClothPrimitive, C3DMeshObject, C3DAnimationInterpolator, CChecksumCacheFile, CIVCountedPointer.
