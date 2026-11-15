# 00B484B0 identity, source and audit

The target is public virtual `?FrameStart@CEngineLightingManager@@UAEXABVCEngineComponentParams@@@Z`, native secondary-base slot012A2240. Both original PDB method records specify this-adjust12, the CEngineComponent base. The exact native body1177bytes ends after ret4 at00B48946. Module983 Debug\engine_lighting.obj donor02CED400 and its1261-byte trace prove the original parameter comp_params, const CIDrawEnvironmentParams& local params, and signed long channel. Full PDB records, original locals, donor trace and all earlier draft reading are retained.

The old local models were replaced before source experiments. Actual chain is GPEngineWorldMap->PeekGameWorld()->DrawGetEnvironment()->PeekInterpolatedParams(), not invented environment holder/source classes. Shared CIDrawWorld slot1 and CIDrawEnvironment slot1 correspond to original CWorld::DrawGetEnvironment and CEnvironment::PeekInterpolatedParams; real vtables and both PDB types agree. The three globals are original typed PUBLICs at donor04AC806C/04AC8084/04AC8040, read at the same positions in the original FrameStart donor. Native positions01436E8C/01436EA0/01436E60 are independently established by the corresponding call/access sequence. GLOBAL_PROOF.json records the exact original names.

Shared original types retain every manager member. New FRAMESTART option adds only the original virtual, public UpdateShadowScene and three private helpers. Existing full layout puts primary CEnginePrimitiveManager at0, secondary CEngineComponent atC, ThemeChannels3C, lookup-grid positionsC4/C8, and ShadowMode4694/spotlight parameters4698, exactly as native this-relative accesses after adjustment. CurrentThemeChannel is the actual descriptor, not flattened invented member names. Nested CThemeChannel is U-keyed,112bytes. resize's original STLport symbol is the existing bound vector<U CThemeChannel> two-argument overload; the original source's one-argument form expands its forwarding wrapper.

The original fog helper is reconstructed from donor02CF2120 and its own original local records: fog_start,fog_end,zscale,zofs,fog_transform. It constructs the actual12-float CMatrix3x4 and calls PreMultiply with PeekNonHomogenousViewTransform. This is the camera's+114 NoScaleViewTransform, not the old draft's incorrectly named inverse-view member. The shared helper body matches native00B451D0 exactly306bytes and is included in the carried audit. Original private access and class-valued return are preserved.

CEngineScene has no package header. Its new shared header preserves the original CBaseClassNonCopyable base and EVERY own member in both PDBs; the two maps are represented by opaque12-byte STLport storage and UpdateStats by its12-byte storage. No target code allocates this class or accesses these opaque members; only its proven public RenderEntireScene(EEngineRenderLayer) is declared/called. The source uses numeric cast1 rather than invent an enumerator. Scene full layout is derived from the original member order and retail container ABI; no independent constructor-size verification is claimed. No existing header model is replaced.

Original Normalise and GFFastOneOverSqrt are used from the shared C3DVector header. FableWin public symbols and donor calls prove these names. ReadingXYZ into locals preserves native product order. Existing NEWTON_GROUPED groups the same operations without changing arithmetic; this moves the frontend scheduling boundary and delays the resize temporary's push exactly to00B485B2. No scheduling pragma or compiler flag was added. Product-only and square/product forms moved the same boundary but did not match. These are one strategy family, not three independent strategies.

The two original GFFloatToLongFloor sites are00B4883B..00B48855 and00B4888F..00B488A9. Donor calls the exact FableWin name ?GFFloatToLongFloor@@YIJM@Z; native expands its distinctive half-subtract, dword integer store, integer reload, add-one, bit comparison and conditional increment. Thirty plain-C++ and ten intrinsic lowering probes were compiled against this full target before the existing shared helper was used. None emitted the native bare dword store. FLOOR_LADDER_CATALOG/RESULTS enumerate each. These include intentionally diagnostic direct casts and limited-domain short/intrinsic forms: they are NOT claimed to preserve floor semantics and were NOT selected. No new low-level helper implementation or macro was authored. The existing shared function has the original portable twin. Both real sites now match; use is exclusively for the proven original rounding blocks.

Behavior test pastes the target, original fog body and inline UpdateShadowScene body verbatim. Instrumented external dependencies cover environment present/absent,1..4channels,4shadow modes,4filter combinations,4camera positions and4diffuse-vector shapes:2048 complete cases. It checks all theme colours/scalers/fog transforms, primitive values, default state, descriptor padding, grid positions, external-call order/arguments, nulling before UpdateSIMDVarBlock and untouched fields. Original matrix constructor/PreMultiply and Normalise operations are retained in fixtures; synthetic inverse-square-root table inputs yield independently known exact binary results. The fixture's bit transfer uses explicit32-bit memcpy for LP64 rather than rely on the production32-bit header's unsigned-long reinterpretation. This is not a claim that the entire production header tree is LP64-layout compatible. Both VC71/Od/W3/GR- withoutEHsc and64-bit g++C++03 pass.

VC71 additionally compares the existing native floor helper with its existing portable twin for32768 deterministic float-bit samples, plus ten known corner values on both compilers. Runtime tests use CRT default x87 precision/rounding; no claim is made for a different precision-control mode (the existing twin documents that limit). Full-target grid cases avoid a negative signed left shift; original engine's signed-shift spelling is retained.

Only target1177 and required out-of-line fog306 count as the audited code closure. Uncalled shared COMDATs are not promoted. Matched progress counts target1177 only. Default/old header profiles and final source/test hashes are checked independently before acceptance.

## Exact dependency spellings

- 01436e8c: `?GPEngineWorldMap@NEngineInternal@@3PAVCEngineWorldMap@@A` — original PDB member/public and corresponding native/donor call sequence.

- 01436e60: `?GPShadowRenderer@NEngineInternal@@3PAVCEngineShadowRenderer@@A` — original PDB member/public and corresponding native/donor call sequence.

- 01436ea0: `?GPCamera@NEngineInternal@@3PAVCEngineCamera@@A` — original PDB member/public and corresponding native/donor call sequence.

- 00b451d0: `?CalculateFogTransformMatrix@CEngineLightingManager@@AAE?AVC4DVector@@MM@Z` — original PDB member/public and corresponding native/donor call sequence.

- 00b943f0: `?RenderEntireScene@CEngineScene@@QAEXW4EEngineRenderLayer@@@Z` — original PDB member/public and corresponding native/donor call sequence.

- 00b47bc0: `?UpdateShadowedSpotLights@CEngineLightingManager@@AAEXXZ` — original PDB member/public and corresponding native/donor call sequence.

- 00b46660: `?AddLightsToLookupGrid@CEngineLightingManager@@AAEXXZ` — original PDB member/public and corresponding native/donor call sequence.

- 00b44540: `?UpdateSIMDVarBlock@CEngineLightingManager@@AAEXXZ` — original PDB member/public and corresponding native/donor call sequence.

- 00b4f390: `?UpdateShadowScene@CEngineShadowRenderer@@QAEXABVCEngineCamera@@_NABUCEngineLightingManagerSpotlightParams@@@Z` — original PDB member/public and corresponding native/donor call sequence.

- 013CE558: `?GFastOneOverSqrtTable@@3PAKA` — both original typed data records(unsigned long[128]) and the native mantissa lookup.
