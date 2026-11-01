# Chunk 688 notes

## Rows
| address | identity (two sources) | grade |
|---|---|---|
| 0x00B6CE00 | `std::vector<CTVertexLandscapeBackground>::_M_fill_insert` -- (1) sole caller the landed resize 0x00B6CD10 binding this spelling, (2) 24-byte stride = Ego_r CTVertexLandscapeBackgroundBase; STLport _vector.c text as the landed water twin 0x00B7FDE0 | RELOCATION_MATCH |
| 0x00CA0890 | libvorbis res0.c `_2class` (Leaf `__2class@16`) -- (1) sole caller the landed res2_class 0x00CA1310, (2) body = res0.c `_2class` verbatim (two _vorbis_block_alloc, fabs maxima, classmetric break) | RELOCATION_MATCH |
| 0x00AA5460 | `?ReadFromFile@@YIXAAVCVertexCompressionParams@@AAVCDataInputStream@@@Z` -- (1) Ego_r.pdb (donor 0x00C40310), (2) sole caller the landed C3DMeshFileXVertexDataChunk::ReadFromFile 0x00AA66B0; eight stores = CVertexCompressionParams layout | RELOCATION_MATCH |
| 0x00C75140 | `CStereoDelayLine::Process(float*, unsigned long)` -- (1) caller 0x00C6A080 on CReverbProcessor+0x25C (the CStereoDelayLine of the landed ctor 0x00C75410), (2) body reads two CDelayLine {data,size,pos} at +4/+0x14 exactly; libpk descriptive (no PDB) | RELOCATION_MATCH |
| 0x00678460 | `??1COpinionReactionManagerDef@@UAE@XZ` -- (1) stores vtable 0x0125B7DC (RTTI COpinionReactionManagerDef; slot 0 = its deleting dtor 0x0067C570, the only caller), (2) member sequence = Ego_r COpinionReactionManagerDef at retail STLport sizes (0x32C) | RELOCATION_MATCH |
| 0x00C88E60 | `CDspOps::FilterGain4(float(*)[128], float(*)[128], float* const*, const SGainParams* const*, unsigned int)` -- (1) landed caller CReverbStage94::Process 0x00C75CE0 binds the spelling, (2) body = four-wide two-pole filter over 0x200-stride blocks with the caller's states/params arrays; libpk descriptive | DIFFER 681v681, retail_asm kernel; parked (53 probes) |
| 0x00C76120 | `CCombFilter::Blend(SCombSettings*, const SCombSettings*, CRingBuffer*, float*, const float*, unsigned long, unsigned long)` -- (1) sole caller the landed ProcessCombs 0x00C69FB0, (2) scalar cross-fade of the landed CCombFilter::Process 0x00C75FF0 ending in the settings copy; libpk descriptive | DIFFER 623v641; parked (12 probes) |
| 0x00761280 | `?DrawPrimitiveLineOverLandscape@NShape@@YIXABVCEnginePrimitivePrepareInfo@@ABVCWorldMap@@ABVC3DVector@@22ABVCRGBColour@@@Z` -- (1) Ego_r.pdb, donor order 0x00770EA0 before CTrack::EditorDrawPreparePrimitivesForRendering 0x00771220 = retail 0x00761280 / 0x00761560 (its sole caller), (2) callees _CItanh, __ftol2, GetGroundSizeZAt x2, primitive slot 20 with the 0x3C CEnginePrimitiveLine | DIFFER 733v733; parked (12 probes) |
| 0x00AC9A60 | `?FindAdjacentFaces@COptimisedPrimitive@QuadricOptimiserInternals@@AAEXXZ` -- (1) Ego_r.pdb private member, sole caller Initialise 0x00ACA830, (2) callees GetIntersection 0x00AC8960 / GetFaceNormal 0x00AC7CB0 / JoinAdjacentFaces 0x00AC8AE0 on the Ego_r COptimisedFace / CUniqueVertex layouts | DIFFER 743v745; parked (16 probes) |
| 0x00501150 | `?LoadLevelFromFiles@CWorldMap@@IAEXJV?$CCountedPointer@VCAFile@@@@00W4ELoadType@1@@Z` -- (1) Ego_r.pdb (ret 0x20 = long + three 8-byte counted pointers + enum), sole caller CWorldMap::LoadLevel 0x00502620, (2) builds Ego_r CLevelLoader::CObject / CLevelFiles and calls SetAsLoading / Add / IsBusy | DIFFER 756v756 (chunk-149 lane gap); parked (8 probes, GOTCHAS: park after one try) |

## Label corrections (NAME_FIXES.tsv)
- 0x00C75140 is CStereoDelayLine::Process, not `PostStage::Process` (the caller's binding names a class that does not own the +0x25C member).
- 0x00C76120 Blend's first parameter is non-const (the settings copy at the end).
- 0x00B6D0B0 / 0x00B6D250 are vector<CTVertexLandscapeBackground>::_M_insert_overflow / _M_clear (landed as CRetailVector_SRecord24::InsertGrow / ReleaseFirstMember).
- 0x0051AF00 is CCountedPointer<CLevelLoader::CObject>::DeleteData (landed as CTCMapwho::OnDie); 0x00513570 the CLevelFiles __uninitialized_fill_n; 0x006C27A0 CLevelLoader::CObject::SetAsLoading (landed as SMappedRecords::Set); 0x0050F980 is nested CLevelLoader::CObject::CLevelFiles.
- The five map `_M_erase` instantiations 0x004507FB / 0x004DA360 / 0x0067A840 / 0x0067A800 / 0x0067A7C0 are COpinionReactionManagerDef's map members' erasers (landed as BuildTreeArray / FreeTree_CTreeOwner); 0x009FC550 is ~CResource (landed Leaf ??1CMemoryBlock); 0x00AD3A10 is the set<unsigned long> _Rb_tree copy constructor.
- Catalog placeholders replaced: SignalProcessing_InterpolateAndAccumulate, Vector_InsertFill_Vertices, Audio_ProcessFilter4Channel, FFT_RadixStride_Vectorized, QuadricOptimiser_PreprocessAdjacencyAndNormals, LoadLevel_ParseObjects, CMemoryBlock_Destructor_Extended, CurveInterpolation_ResampleCurveData.

## Descriptive names (libpk reverb, no PDB/RTTI)
CDspOps::FilterGain4 (parameter names dst/src/states/params/count; the params block fields read at +8/+0xC/+0x10..+0x18/+0 are the landed SGainParams m_current[1..2] / m_target[0..2] / m_master), CStereoDelayLine::Process with the file-local `Allpass` helper name, CCombFilter::Blend's parameter names. The LoadLevelFromFiles argument names (topologyFile / navigationFile / thingFile) are descriptive; the record fields they fill are the PDB's.

## Header debt
- engine/COptimisedPrimitive.h keeps UniqueVertices / Faces / Edges as opaque byte arrays and Vertices as a 12-byte array; the rows declare COptimisedPrimitive locally (as the landed UpdateContractions 0x00AC96E0).
- engine/CWorldMap.h is data-only (no virtual slots); 0x00501150 declares a local CWorldMap with the 27 slots (GetLoadableSections = slot 26) and PLevelLoader at +0xBC, as the landed CacheMaps 0x00502850.
- engine/CLevelLoader.h carries the PDB's 12-byte list (ObjectList +0x14 is right, ThingManager/WorldMap/World are shifted); 0x00501150 only needs the vtable, so a local two-slot class is used. CLevelLoader::CObject / CLevelFiles have no generated header (nested classes).
- engine/CDefClassBase.h opt-in + the local packed CParentDefClassBase of the landed ~CBlurredAttitudesDef model (its out-of-line ~CParentDefClassBase would be called instead of inlined) for 0x00678460.
- No C3DVector / CEnginePrimitiveLine / CRGBColour engine headers: local models from the landed NShape rows.
- The retail_asm row 0x00C88E60 lives in differ/ (parked) with its `// Retail-asm:` evidence line; if landed later it belongs to the retail_asm grade.
