# NF53 header overlay (round 7)

Each file is a copy of the CURRENT repo header `rebuild/include/engine/<name>` with `#ifdef` opt-ins added (lines added only;
without the opt-in macro every file is token-identical to the repo copy). Macro names are member-specific (`FABLE_RETAIL_<CLASS>_<MEMBER>`).

| Overlay | Opt-in macro | Adds | Rows |
|---|---|---|---|
| engine/CPlayerGui.h | FABLE_RETAIL_CPLAYERGUI_PEEKGLOBALDEF | `const CGlobalDef& PeekGlobalDef() const;` (Ego_r ?PeekGlobalDef@CPlayerGui@@QBEABVCGlobalDef@@XZ) in the flattened branch + `class CGlobalDef;` | 0x004384F0 |
| engine/CBankFileAsync.h | FABLE_RETAIL_CBANKFILEASYNC_ASYNCUPDATEPENDING | `void AsyncUpdatePending();` (0x009D68B0) for TUs without FABLE_RETAIL_NGAMETEXT_DATABANK_GROUPTEXT (guarded `&& !defined(...)` so the two never double-declare) | 0x009F8230, 0x00B3B7F0 |
| engine/CEngineResourceManager.h | FABLE_RETAIL_CENGINERESOURCEMANAGER_PROCESSLOADING_UNTIL | `void ProcessLoading(bool, double);` (Ego_r 0x009D9E50 / FableWin 0x02DD3C30) in the retail branch | 0x00B3B7F0 |
| engine/CMeshDataBank.h | FABLE_RETAIL_CMESHDATABANK_PROCESSLOADINGQUEUE | `void ProcessLoadingQueue(bool, double);` (0x00A23B00; Ego_r 0x00B5FD80 / FableWin 0x031333B0) in the BASES+CTOR class | 0x00B3B7F0 |
| engine/CEngineShadowRenderer.h | FABLE_RETAIL_CENGINESHADOWRENDERER_UPDATESPACE | the three private members SetShadowClipVolumesToNeverShadow / UpdateSpotLightShadowSpace / UpdateShadowSpace (Ego_r AAE spellings) | 0x00B4F390 |
| engine/CEngineShadowRenderer.h | FABLE_RETAIL_CENGINESHADOWRENDERER_PACKED_VECTORS | `#pragma pack(push,1)` / `pack(pop)` around StaticDataOrigin + CameraLookVector only. This is the one opt-in that changes LAYOUT, and only under the macro: Ego_r places them at +0x261 / +0x26D (retail 0x00B4F3B6 `lea esi,[eax+26Dh]`); default packing gives +0x264 / +0x270. CameraViewMatrix stays +0x27C, size stays 0x340. Without the macro the class is unchanged. | 0x00B4F390 |
| engine/C3DMeshInfo.h | FABLE_RETAIL_C3DMESHINFO_ARETEXTURESLOADED | `bool AreTexturesLoaded() const;` (Ego_r ?AreTexturesLoaded@C3DMeshInfo@@QBE_NXZ, 0x00A23C60) | 0x00A23C60, 0x00B9CE10 |
| engine/CEngineTreeFiler.h | FABLE_RETAIL_CENGINETREEFILER_LISTTREE | the three private CEngineTreeFilerBase members AddNode / ConvertTreeToList / ConvertListToTree (Ego_r AAE spellings, 0x00BF2A10 / 0x00BF2AD0 / 0x00BF2B50), declarations only | 0x00BF2E50 |
| engine/CEngineTreeFiler.h | FABLE_RETAIL_CENGINETREEFILER_FINDNODE_DECLONLY | an `#ifndef` around the inline FindNode bodies, so that TU calls the out-of-line FindNode 0x00BC9B30 (no declaration changes; without the macro the bodies stay) | 0x00BF2E50 |
| engine/CContainmentVolume.h | FABLE_RETAIL_CCONTAINMENTVOLUME_MOVABLERESOURCE | in the generated (flattened) branch, `class CContainmentVolume : public CMovableResource` (RTTI .?AVCContainmentVolume@@ base CMovableResource@0) in place of the own `__vftable` word; layout unchanged (the flattened CMovableResource is that 4-byte word), so `this` converts to the CMovableResource* owner of AllocateResource 0x00A62E40 | 0x00A754E0, 0x00A755C0 |
| engine/CEnginePrimitiveManagerStaticMeshesBase.h | FABLE_RETAIL_CENGINEPRIMITIVEMANAGERSTATICMESHESBASE_PROTECTED_TAIL | `protected:` before OverridePrimitive / RenderOnlyNMeshes / StippleAlphaSolidFactor (retail branch) -- access only, no layout change; the derived ctor 0x00BA1A70 stores 1 straight into +0x314 | 0x00BA1A70 |
| engine/CCreatureAction_ScorpionPrepareTailPlunge.h | FABLE_RETAIL_CCREATUREACTION_SCORPIONPREPARETAILPLUNGE_FINISHACTION | `virtual void FinishAction();` (Ego_r ?FinishAction@CCreatureAction_ScorpionPrepareTailPlunge@@UAEXXZ) in the retail branch, declaration only | 0x008C4990 |

Re-diffed against the repo before hand-back (`diff repo overlay | grep '^<'` empty for every file).
