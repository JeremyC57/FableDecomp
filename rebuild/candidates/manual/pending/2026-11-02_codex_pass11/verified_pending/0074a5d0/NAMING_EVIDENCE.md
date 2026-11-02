# NCreatureHiding::GetValidPositions naming and validation

Identity: Ego_r0075c940 and FableWin025127e0 carry the exact emitted Leaf. The native0080f342 caller in CTCNymph::PickNewTargetPos passes the world map/search tools, centre, radius, hidden flag and exclusion pointer in the matching fastcall/stack positions. The assigned1065-byte extent is one complete function, ending at0074a9f9 after ret20; it contains no embedded table or data.

## Original local names

Unlike the previous creature trio, these local spellings are recovered: positions, wmap, search_tools, about_position, radius, get_hidden_positions, pExclusionArea, d_rad, thing_list, rad, n_thet, c1, thet, r, got_collision, ground_height, it, pthing, int_dat, hidable and usable. FableWin module debug/tc_creature_generator.obj stream132, procedure NCreatureHiding::GetValidPositions at segment2+0xd837e0 provides their types. thing_list is vector<const CThing*>, pthing is const CThing*, and thet is float. Proof is saved locally in evidence/naming_support/hiding_original_locals.json.

## Headers and inline APIs

All source receivers use real shared engine headers. CWorldMap retains the CBaseClassNonCopyable and CIDrawWorldMap bases and full native0xd8 extent. CThingSearchTools retains all original fields and real vector hierarchy at native0x18. CTCShapeManager derives from CTCBase and retains complete0x30 storage. CMeshIntersectionData is28 bytes and CPhysicsMeshObject is64 bytes.

C3DVector::ToC2DCoordI is proven by original TPI0x2783 method record0x277f, public const C2DCoordI return-by-value, and exact FableWin01addc10 symbol ?ToC2DCoordI@C3DVector@@QBE?AVC2DCoordI@@XZ. Native signed-long conversion order plus its independent debug caller types confirm the API. Ordinary inline retains the byte match (grade16); no stronger annotation is needed for this method. C2DBoxI(const C2DCoordI&,long) and Initialise(long,long,long,long) are TPI0xb186/0xb191, with native centre/radius arithmetic as independent evidence. Existing C3DVector arithmetic annotation follows explicit EXTRA_RULES100-104.

The clamp is spelled as the classic repeated by-value min/max ternary expansion with literal-first comparisons. This preserves the exact native unordered x87 behavior. STLport reference-returning min/max spills three extra addressable values and does not match. The visible genuine vector growth body reads the element input and never accesses the empty tag, allowing the native shared tag/frame slot. The collision flag, rather than an explicit break, is the original loop guard.

## Every direct relocation

- `0074ca20` -> `??$__copy@PAVC3DVector@@PAV1@H@std@@YIPAVC3DVector@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z`. Real STLport4.6 _algobase.h __copy signature and random_access_iterator_tag/Distance* dispatch; original locals prove C3DVector vector. Native helper copies 12-byte elements, uses ecx/edx first/last and stack result/tag/int pointer; existing bound instantiation agrees.
- `0123095c` -> `__real@3e000000`. Compiler literal 0.125; COFF data bytes were compared with the native data and agree exactly. No named global was invented.
- `0122f59c` -> `__real@3f000000`. Compiler literal 0.5; COFF data bytes were compared with the native data and agree exactly. No named global was invented.
- `0122dcdc` -> `__real@40000000`. Compiler literal 2.0; COFF data bytes were compared with the native data and agree exactly. No named global was invented.
- `00bfea70` -> `__ftol2`. Verified MSVCR71 _ftol2 helper; compiler generates the same helper for the original signed-long casts. Both donor name sets identify __ftol2.
- `0074d710` -> `??$GetAllThingsInRoughArea@$$CBVCThing@@V?$vector@PBVCThing@@V?$allocator@PBVCThing@@@std@@@std@@VCThingFilter_None@@@CThingSearchTools@@QBEXABVC2DBoxI@@ABVCThingFilter_None@@AAV?$vector@PBVCThing@@V?$allocator@PBVCThing@@@std@@@std@@@Z`. Ego_r0075f950/FableWin0251dcb0 exact const-CThing GetAllThingsInRoughArea template. FableWin original GetValidPositions locals independently prove vector<const CThing*> and const CThing* pthing; native helper traverses rough-area map and fills third stack-argument vector.
- `0129ba3c` -> `?DELTA_NOTIONAL_ZERO@@3MB`. Owner-settled extern const float DELTA_NOTIONAL_ZERO; native outer-radius guard reads this named data and adds radius at both loop tests.
- `0122dedc` -> `__real@00000000`. Compiler literal 0.0; COFF data bytes were compared with the native data and agree exactly. No named global was invented.
- `0122ded8` -> `__real@3f800000`. Compiler literal 1.0; COFF data bytes were compared with the native data and agree exactly. No named global was invented.
- `0124f2b8` -> `?TWO_PI@@3NB`. Owner-settled names/ESTABLISHED_SPELLINGS.tsv names extern const double TWO_PI. Exact native data is6.283185482025146484375, and angle construction multiplies the float fraction by this qword.
- `004fc600` -> `?IsPosInMap@CWorldMap@@QBE_NABVC3DVector@@@Z`. Ego_r004cfdb0/FableWin01c846b0 exact CWorldMap::IsPosInMap const signature; native caller passes radial C3DVector by reference to its const world-map parameter and branches on bool AL.
- `012462a8` -> `__real@40a00000`. Compiler literal 5.0; COFF data bytes were compared with the native data and agree exactly. No named global was invented.
- `006b0850` -> `?GetPhysicsMeshObject@CTCPhysicsBase@@QBE?AVCPhysicsMeshObject@@XZ`. Ego_r006ed7f0/FableWin024702c0 exact const CTCPhysicsBase::GetPhysicsMeshObject return-by-value; native hidden result builds full 0x40-byte mesh object. Original TPI and prior independently verified creature-row callee audit agree.
- `00a12450` -> `?GetPointIntersectionFromTo@CPhysicsMeshObject@@QBE_NABVC3DVector@@0AAVCMeshIntersectionData@@@Z`. Ego_r00b800d0/FableWin031d2780 exact const CPhysicsMeshObject::GetPointIntersectionFromTo; native takes two C3DVector inputs and CMeshIntersectionData output, returning bool. Original CMeshIntersectionData TPI0x3104 places IntersectionPosition at0.
- `00a127d0` -> `??1CPhysicsMeshObject@@UAE@XZ`. Ego_r00b80510/FableWin031d1210 exact virtual CPhysicsMeshObject destructor; native writes its RTTI vtable, releases Mesh+4 and chains CBaseClass. Full 0x40-byte temporary cleanup matches.
- `0122ee58` -> `__real@3dcccccd`. Compiler literal 0.10000000149011612; COFF data bytes were compared with the native data and agree exactly. No named global was invented.
- `0063a300` -> `?GetGroundSizeZAt@CWorldMap@@QBEMABVC3DVector@@@Z`. Ego_r006e7ec0/FableWin024a3500 exact const CWorldMap::GetGroundSizeZAt; native receives the radial point, calls world-to-map conversion and CMap::GetGroundSizeZAt, returns float in x87.
- `00760870` -> `?IsPosInAnyShape@CTCShapeManager@@QBE_NABVC3DVector@@@Z`. Ego_r00772080/FableWin025d6380 exact const CTCShapeManager::IsPosInAnyShape; native caller uses the original pExclusionArea const-pointer parameter and point reference; native callee scans shape storage.
- `004fcbd0` -> `?IsPosPassable@CWorldMap@@QBE_NABVC2DCoordI@@@Z`. Ego_r004d0190/FableWin01c8eee0 exact const CWorldMap::IsPosPassable(C2DCoordI const&); native calls CMap::IsPassableAt and receives the original two-long integer coordinate temporary.
- `0066e370` -> `?_M_insert_overflow@?$vector@VC3DVector@@V?$allocator@VC3DVector@@@std@@@std@@IAEXPAVC3DVector@@ABV3@ABU__false_type@2@I_N@Z`. Real STLport4.6 _vector.h/_vector.c protected non-trivial _M_insert_overflow and original vector<C3DVector> local type prove key/access/tag/signature. Native growth copies 12-byte elements via separate uninitialized_copy; canonical existing binding agrees.
- `00bfea14` -> `_free`. Verified MSVCR71 free import thunk. Native destruction of thing_list uses _free; no delete operator is substituted.

## Indirect call

CTCPhysicsBase::GetPhysicsMeshInfo() const is native slot93, byte offset0x174. RTTI table0125d2c4 points to006b13b0 (null base implementation); Standard01265f84 points to00724d20 (+0x40 mesh-info field), and RigidBody0126996c points to0075bfd0 (+0x64). PDB exact virtual names and original base TPI method order prove the slot after two editor-only predecessors are removed. Donor slot93 is a different method; it was not copied.

## Verification and fixture limits

Grade16 compiles under cl13.10.3077 RTM at base /O2 /Oy /W3 with no extra flags and gives RELOCATION_MATCH1065/1065. The emitted Leaf is unique. All33 relocation operands,21 target addresses, every compiler literal, both named globals, and the indirect0x174 call were reviewed independently. No symbol in this leaf is bound to another address in the supplied bindings.

The self-contained test expands34 genuine headers and embeds the selected leaf source. GNU64 strict C++03 and real VC7.1 /GR- both print0074a5d0_TEST PASS. Fifteen scenarios cover clearing preexisting outputs; negative, zero and positive radii; both clamp ends and interior; ring sample counts; outside-map rejection; visible/hidden passability; exclusion rejection; null candidates; missing mesh; failed and successful intersection; ground height below/above collision height; and hiding rejection after collision. A real CThing/CTCPhysicsBase hierarchy, genuine full CWorldMap and CTCShapeManager, and original STLport vectors are instantiated. Controlled engine boundaries replace game-world queries. Only CThingManager and CThingPhysicsMeshInfo are test-only opaque boundary completions: their fields/layout are never read or mutated by this leaf or the controlled search/physics calls. They are not production class models. The test does not validate engine search, map or mesh algorithms themselves.

Current source SHA256 bf2e3958a0e89f9b7dc5f5b9a02332747df6af181102d6710de3303bd48fc2fd. Test SHA256 cb7127c46aa960871d9fcc6085a486e31ea8bc8b77acb0f3bb0775941b1796b8.
