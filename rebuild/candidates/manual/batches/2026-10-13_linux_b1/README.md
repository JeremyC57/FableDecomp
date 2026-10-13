# Batch `2026-10-13_linux_b1` — 29 commented matching candidates (29 landed)

Authored 2026-10-13 in a Linux container that has the retail `Fable.exe` (16,666,624 bytes,
linker 7.10, entry `0x401067`) and a reproduced Ghidra 12.1 database, but **no VC7.1 compiler**.
Every function here was written from its retail instruction bytes against the shared engine
headers and the Ego_r.pdb layouts, with the codegen modelled on landed byte-exact siblings of the
same instruction shape. Nothing in this directory is counted as coverage until the parity oracle
grades it.

## Lane grading result (2026-10-13, Wine + DDK 13.10.4035 `cl.exe`, `/O2 /Oy /W3` + lander sweep)

Graded in the Linux container with `tools/decomp_pipeline/linux_lane/` and landed with `verify_and_land.py --land`:
**29 of 29 rows are byte-exact (MATCH / RELOCATION_MATCH) with behaviour PASS** and now live in
`rebuild/src/compiled/`, `rebuild/tests/` and the catalog. IsSigned, IsMainPlayer, CWStringParser::Init and the tattoo-path ctor needed a second shape (see each file's `Lane feedback` line).

The grade line below is the pre-grading state kept for provenance.

## Grade now: `candidate` (superseded by the section above)

| File | Address | What it is |
|---|---|---|
| `CEnginePrimitiveManagerOcclusionBase_RenderBoundingVolume_00b91340.cpp` | 00b91340 | sub-primitive -> parent forward (tail jump) |
| `CAIStateGroupServeFoodAtHome_GetBuilding_00902ca7.cpp` | 00902ca7 | brain -> villager -> home smart pointer |
| `CAIStateGroupServeFoodAtTavern_GetBuilding_00902cdc.cpp` | 00902cdc | same, tavern pointer |
| `CPixelFormat_IsSigned_009e3970.cpp` | 009e3970 | descriptor-table lookup, channel type 6 |
| `CGraphicInfoFrame_GetBlankTexture_009fd3f0.cpp` | 009fd3f0 | per-texture-type blank slot in the data bank |
| `CEngineLandscapeMap_IsBackgroundAtFullDetailInArea_00bdc150.cpp` | 00bdc150 | null-tree => true, else forward |
| `CGTDIMaze_Init_00e27c60.cpp` | 00e27c60 | InitialAngle from embedded virtual source |
| `CThingFilterIsHuman_operator_006a7b90.cpp` | 006a7b90 | predicate, int-sized bool |
| `CWStringParser_Init_00a0c8e0.cpp` | 00a0c8e0 | rebind + rewind |
| `CPlayer_IsMainPlayer_00487520.cpp` | 00487520 | Number == manager main number |
| `CClickable_Initialise_00546690.cpp` | 00546690 | base init + seed runtime field |
| `CCreatureActionBase_UpdateActionRunningTime_00692480.cpp` | 00692480 | RunningTimeSeconds += vslot62() |
| `CCreatureActionTrollWhackGroundBase_Initialise_00723730.cpp` | 00723730 | vslot95(); base Initialise (tail) |
| `CTCShop_OnInitialActivate_00775b20.cpp` | 00775b20 | cache CalcIsStallLike, tail to body |
| `CShaderBankStateBlock_Set_00a61f40.cpp` | 00a61f40 | copy CStateBlock (byte then dword) |
| `CEngineSceneGridCell_InvalidateAll_00b91710.cpp` | 00b91710 | clear validity bits up the parent chain |
| `CWorldMap_AllocateMapThingUID_004fb330.cpp` | 004fb330 | vslot18(map) -> CMap::AllocateMapThingUID |
| `CPlayer_ResetModes_00634870.cpp` | 00634870 | if Local: AddMode(0, true) |
| `CTCWife_ClearAppearanceChangedNoticed_006cecd0.cpp` | 006cecd0 | FrameNoticedChangeInAppearance[cat-2] = 0 |
| `CThingComponentNameTag_ctorCTCTargetingPlayer_0049d450.cpp` | 0049d450 | CCharString("CTCTargetingPlayer") ctor |
| `CThingComponentNameTag_ctorCTCAssassinRush_004d4df4.cpp` | 004d4df4 | CCharString("CTCAssassinRush") ctor |
| `CAIStateGroupNameTag_ctorSGPREPAREMEAL_008fdba2.cpp` | 008fdba2 | CCharString("SG_PREPARE_MEAL") ctor |
| `CAIStateGroupNameTag_ctorSGGAZEOUTSIDE_008fdbcb.cpp` | 008fdbcb | CCharString("SG_GAZE_OUTSIDE") ctor |
| `CAIStateGroupNameTag_ctorSGGAZEATHOME_008fdbf4.cpp` | 008fdbf4 | CCharString("SG_GAZE_AT_HOME") ctor |
| `CAIStateGroupNameTag_ctorSGMINIONHAWKINGFROMSTALL_008fdd10.cpp` | 008fdd10 | CCharString("SG_MINION_HAWKING_FROM_STALL") ctor |
| `CTattooDataPath_ctor_0041a6a0.cpp` | 0041a6a0 | CWideString(L"Data\Tattoos\") ctor |
| `CUserProfileManager_GetTextureDetail_0054a530.cpp` | 0054a530 | UseNewProfileValues ? new : current |
| `CUserProfileManager_GetMeshDetail_0054a590.cpp` | 0054a590 | same, MeshDetail |
| `CUserProfileManager_IsVerticalSyncEnabled_00548710.cpp` | 00548710 | same, VerticalSyncEnabled |

Seven of the 29 (the name-tag constructors, the tattoo path constructor, the three profile
getters) carry manifest labels that are propagated across byte-identical bodies; each file's header
comment says why the label was rejected and what evidence the replacement name rests on.

## How to grade (Windows box with `D:\Tools\vc71`, or the Linux lane: `docs/BUILDING.md`)

```
python tools\decomp_pipeline\verify_and_land.py ^
    rebuild\candidates\manual\batches\2026-10-13_linux_b1\payload.json ^
    rebuild\candidates\manual\batches\2026-10-13_linux_b1\oracle.tsv
```

Add `--land` once a row reports `MATCH` / `RELOCATION_MATCH` with behaviour `PASS`; the lander
writes `rebuild/src/compiled/...` and `rebuild/tests/...` itself and this directory can then be
deleted for the landed rows. `DIFFER(NvM)` rows are honest misses: iterate them with
`work/crawl_batch154/probe.py` and record the attempt in `tools/decomp_pipeline/crawl/gen_tried.txt`.

## Provenance

- Targets: `rebuild/manifest/functions.tsv` rows with no `compiled_status`, complete prototype,
  known calling convention, 12..96 body bytes, not in `gen_tried.txt`; bodies trimmed with
  `crawl/rowtrim.py`. Oracle spans in `oracle.tsv` are the trimmed bodies (max 20 bytes each).
- Codegen templates: an opcode-skeleton index over the landed `MATCH`/`RELOCATION_MATCH` set
  (registers kept, immediates and displacements masked). Where a template existed it is cited in
  the file header.
- Names and offsets: `rebuild/include/engine/*.h` where a header exists, otherwise
  `ghidra_out/struct_layouts_egor.tsv`; callee identities only from landed `PASS` rows
  (`CCharString::CCharString`, `CWideString::CWideString`, `CMainGameComponent::BeginInputLoading`,
  `CRenderStateManager::RestoreCaptureBlock`); propagated callee labels were not used for naming.
