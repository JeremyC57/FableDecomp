# Original identity and bindings

`00BF7BC0` is private `void __thiscall CLandscapeBackgroundTreeNode::CalculateCameraUpdateThreshold(long current_lod)`, exact decorated spelling `?CalculateCameraUpdateThreshold@CLandscapeBackgroundTreeNode@@AAEXJ@Z`. Both raw PDB publics and complete class member records agree. Ego_r original segment1+0063E8C0 =00A3F8C0; FableWin segment2+01656120 =02DE5120. Original module938, Debug\Engine_Landscape_Background_Tree_Node.obj, actual locals and scopes saved in ORIGINAL_LOCALS.json. The assigned `CEngineLandscapePatch::DebugCalcInForgroundRange` identity is wrong. The native546byte extent ends atRET4,00BF7DDF; no tail reduction.

Original module disassembly proves distance local, two1.0e30f distances, GetPatch, patch_band/next_patch, GetLodBandInfo condition before next_patch<8, and GetLODZBandZ calls; debug assertion compares current_lod's remap with patch_band. Release removes the assertion and does not use current_lod. The source intentionally retains that original unused parameter.

Globals:

* `NEngineInternal::GPCamera`, `?GPCamera@NEngineInternal@@3PAVCEngineCamera@@A`, native01436EA0. Both typed PDB data records plus identical original and native camera-position chains. Native rereads it at00BF7D97 after virtual IsPatch.
* `NEngineInternal::GPLandscapeRenderer`, `?GPLandscapeRenderer@NEngineInternal@@3PAVCEngineLandscapeRenderer@@A`, native01436EA8. Both typed PDB records at the camera+8 donor offset, original calls GetLODZBandZ, and native renderer+498 indexed access. Old g_pLandscapeRenderer is descriptive and wrong.
* `GEnableSIMD`, `?GEnableSIMD@@3_NA`, native013D2880. Both typed PDB data records: Ego_r segment3+138005; FableWin segment4+17FB19. Original CalcShortestDistanceToPointSquared02CD3120 reads04BBFB19, exactly the latter original data VA. Native inlined same original kernel reads013D2880. Neither PDB contains g_SSEAvailable. That old binding is wrong.

The real member identities are CalcShortestDistanceToPoint / CalcShortestDistanceToPointSquared / PeekCameraPos / GetPatch / GetLodBandInfo / GetLODZBandZ; these are original PDB names. GetLODZBandZ is FableWin02DE53C0, renderer+0D10 in debug; native renderer+0498 in release. IsPatch is the PDB pure virtual introduction at slot16 and native indirect call00BF7D08.

## Release layout

The raw complete class records in *_NODE_TYPES.json name all members, their composite types, and their base. Retail RTTI from ghidra_out/rtti_bases.tsv: TreeNode -> CEngineLandscapeFreeableResource -> CMovableResource. The native tree constructor00BF6E20 zeros DynamicBuildTemplate+4, GPUUpdateTask+8, and eight complete CLODLoadInfo records starting+C, then sphere+C0, Map+A4, SubNode+A0, FileBlockPos+8C, and FirstNonSplitBand+E1. These independently prove a release FreeableResource base of4bytes and a release tree ofE8bytes. Debug-only LRUPrev/LRUNext do not occur in this release base: keeping them would shift every one of these independent native offsets by8. No new packing is introduced.

The typed tree overlay retains every named release member and real nested CLODLoadInfo: three long fields, one uchar, natural padding to16. Shared scalar/vector/box/sphere, patch, renderer and camera classes are used. A narrow IsPatch declaration adds no data and preserves slot16. Prior/default header paths remain unchanged and moved old assertion names are pinned with the mandatory tool.

## Observed lookup order and valid storage

The original debug loop and native00BF7D34 read OptimizedBandRemap[next_patch] before testing next_patch<8. For band7, or a remap chain reaching8, the native low byte is the following AsyncLoadTask field at+98. This is an observed native access; it does not imply a ninth CLODLoadInfo member in the retail class. Portable fixtures must provide real ninth backing storage for the semantic getter and must not index beyond a declared eight-element array. Retained source expresses the original GetLodBandInfo API; valid engine inputs normally select bands1..7, with the observed sentinel behavior documented separately.
