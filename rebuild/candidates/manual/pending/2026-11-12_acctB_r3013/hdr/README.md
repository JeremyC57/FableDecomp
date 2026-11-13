# Chunk 3013 header overlay = union of prior/2028/hdr and prior/2027/hdr (no file overlaps except README)

## From chunk 2028
# Chunk 2028 header overlay (FABLE_HDR_OVERLAY=/opt/fable_lane/w22/agent2028/hdr)

Every change is an opt-in (or a new header); the default text of every copied header is unchanged (one exception noted: the new
`#include "fable_compiler.h"` at the top of CIVCountedPointer.h). Copies taken 2026-11-11/12 from rebuild/include or the named hand-backs;
re-base on HEAD before landing. Row 0x00B8DF50 (RELOCATION_MATCH) uses the first block; the parked 0x00B9A4A0 the second.

| header | opt-in | what | evidence |
|---|---|---|---|
| engine/CEngineOcclusionManager.h | `FABLE_RETAIL_CENGINEOCCLUSIONMANAGER_MEMBERS` | own members typed (Ego_r names, retail offsets; nested COccluderSlot / COccluderEntry / COccluderFlareEntry), FrameStart / FrameEnd virtuals, private SelectFlareTestVertexBuffer | struct_layouts_egor.tsv + fable_types.h; landed 0x00B8D280 / 0x00B8DBE0 offsets; Ego_r / FableWin names |
| engine/CEngineCamera.h (c2008 copy) | `FABLE_RETAIL_CENGINECAMERA_CLIPSPHERE` | FABLE_FORCEINLINE ClipSphere(const C3DVector&, float) const, PeekViewTransform, FABLE_FORCEINLINE TransformWorldToView(const&, &), PeekNonHomogenousViewToViewScaleX / Y | FableWin-only inlines; landed CEngine::ClipSphere 0x00B24150; 0x00B8E0F6.. / 0x00B8E2CB.. |
| engine/CEngineCameraDesc.h | (c2008 copy, unchanged) | | |
| engine/CMatrix3x4.h | `FABLE_RETAIL_CMATRIX3X4_SETTRANSLATEONLY_XYZ` (nested in _SETTRANSLATEONLY), `FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_COPY_SUMS` | SetTranslateOnly(float,float,float) + the vector form forwarding to it; FABLE_FORCEINLINE TransformPoint(const&, &) with Z-term-first rows | FableWin ?SetTranslateOnly@CMatrix3x4@@QAEXMMM@Z; 0x00B8E231..0x00B8E27B, 0x00B8E2CB..0x00B8E2EA |
| engine/CEngineStateBlocks.h | `FABLE_RETAIL_CENGINESTATEBLOCKS_APPLY_INLINE` | inline ApplyDiffuseOnlyStateBlock (DiffuseOnlyMode.Apply()) | FableWin-only; 0x00B8DFDB virtual call on +0x28 |
| engine/CRenderManagerCore.h | `FABLE_RETAIL_CRENDERMANAGERCORE_DRAWASTRIFAN` | DrawCurrentBuffersAsTriFan(long, long) 0x00A0AE40 | Ego_r / FableWin; body DrawPrimitive type 6 |
| engine/CRenderStateCaptureBlock.h | (c2024 copy, unchanged) | scoped capture block | |
| engine/CEnginePrimitiveRenderUtil.h (c2011 copy) | `FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_MESHRENDER` | static GetCurrentOcclusionLayer / GetOcclusionEnable / GetLODErrorTolerance / GetLODVirtualScreenSizeFOVAdjusted, member CalculateSubPrimitiveRenderFlags (bodies in the row) | FableWin names; COMDAT 0x00B98CC0 |
| engine/CEnginePrimitiveManager.h | `FABLE_RETAIL_CENGINEPRIMITIVEMANAGER_BLOCKONLOAD` | static GetBlockOnLoad() | FableWin; COMDAT 0x00B98C10 |
| engine/CEnginePrimitiveRendererSharedData.h | `FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_RENDERSETTINGS` (new first branch) | Ego_r members to +0xC4 + inline getters | struct_layouts_egor.tsv; 0x00B9A4DB.. reads |
| engine/CEngineInternalPrimitiveBase.h | `FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ISPERSISTENT` | IsPersistent() | FableWin; 0x00B9A528 |
| engine/CEngineInternalPrimitiveOcclusionBase.h | `_ISOCCLUDED`, `_SETUP_FORCEINLINE` | HasOcclusionInfo inline, IsOccluded declared (row body); SetupOcclusionInfo forced | FableWin; trace scores 138 / 80 |
| engine/CEnginePrimitiveOcclusionManager.h | `FABLE_RETAIL_CENGINEPRIMITIVEOCCLUSIONMANAGER_OCCLUDED` (with _SET) | GetOccludedFrames(info&) 0x00B2FAC0, inline IsPrimitiveOccluded | FableWin; 0x00B9A5A3 |
| engine/CIVCountedPointer.h | `FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_FORCEINLINE` (nested in _RESET_ADOPT) | FABLE_FORCEINLINE Reset | trace score 80; 0x00B9A564.. |
| engine/C3DMeshInfo.h (c2011 copy) | `FABLE_RETAIL_C3DMESHINFO_LODERROR` | GetLODError, BeginLoadingTextures(EBankLoadPriority) 0x00A23C50 | FableWin; Ego_r donor order |
| engine/CShadowClipVolume.h, CShadowRendererSceneData.h, CEngineShadowRenderer.h, CEngineLightingManager.h, CRenderTarget.h (c2024 copies) | `FABLE_RETAIL_CSHADOWCLIPVOLUME_CLIPBOUNDINGBOX`, `FABLE_RETAIL_CSHADOWRENDERERSCENEDATA_COPY`, `FABLE_RETAIL_CENGINESHADOWRENDERER_PEEKSCENEDATA` | FABLE_FORCEINLINE ClipBoundingBox (landed 0x00B9C2E0 body), declared ctor / copy ctor 0x00B38600, PeekSceneData by value | Ego_r / FableWin; 0x00B9A71D..0x00B9A946 |
| engine/CEngineInternalPrimitiveRepeatedStaticMesh.h | `FABLE_RETAIL_CENGINEINTERNALPRIMITIVEREPEATEDSTATICMESH` | the retail class over the occlusion base, Ego_r members, GetRenderInfo | RTTI chain (tools/rtti.py), Ego_r layout = retail |
| engine/CEnginePrimitiveManagerRepeatedStaticMeshes.h | `FABLE_RETAIL_CENGINEPRIMITIVEMANAGERREPEATEDSTATICMESHES` | the retail class over CEnginePrimitiveManagerMeshBase (STATICBLOCKS chain), typed shader members, RenderPrimitive | RTTI chain; landed ctor 0x00B99D90 |
| engine/CEngineRenderInfoMeshBase.h, CEngineSubPrimitiveBoundingBox.h, CEngineSubPrimitiveRepeatedStaticMesh.h (new) | default | render info (Lights / Mesh / LOD), bounding-box sub-primitive (inline ctor), mesh-base + repeated-mesh sub-primitive (ctor 0x00B98BC0) | fable_types.h layouts; RTTI vtables 0x012A3E84 / 0x012A3E94 |

## From chunk 2027
# Chunk 2027 header overlay (grade with `FABLE_HDR_OVERLAY=/opt/fable_lane/w22/agent2027/hdr`)

Every change to an existing header is an additive opt-in: with none of the new macros defined each copied file preprocesses to its
source text (CWillButtonHelperTimer.h is the pending hand-back 2026-11-11_linux_handH40j's version, whose STATE_INLINE block is
itself an opt-in). Two headers are new. Only engine/CQ_OrchardFarmRaidScript.h is needed by the matched row 0x00DCD350; the rest
serve the parked row 0x007C6E50. Landing note: re-copy HEAD first and re-apply the blocks.

## New headers
| header | content | evidence |
|---|---|---|
| engine/CQ_OrchardFarmRaidScript.h (row 0x00DCD350) | `NScript::CQ_OrchardFarmRaidScript : CLevelScriptBase<CQ_SunnyvaleMasterData>` (needs FABLE_RETAIL_NSCRIPT): the Ego_r.pdb members at retail offsets (PDB - 0x14, - 0x18 past the 12-byte CrateList), sizeof 0x10c; nested `ERaidComment` (enumerators unproven), `CCrateTeamManager` (Ego_r 64 bytes: MemberCount +0x18, TeamReinforcementsTimer +0x1c ...), forward declarations of the seven entity classes; opt-in FABLE_RETAIL_CQ_ORCHARDFARMRAIDSCRIPT_CTEAMSPAWN_MAIN: `CTeamSpawn : CEntityScriptBase` (ParentClass pointer +0x14 .. BanditsLeftID +0x3c, sizeof 0x40) with the Ego_r / FableWin method set | struct_layouts_egor.tsv (CQ_OrchardFarmRaidScript 292, CCrateTeamManager 64, CTeamSpawn 64); Main 0x00DCD350 reads +0x50 / +0x52 / +0x78 / +0x7c / Teams +0x8c (shl 6, +0x18 / +0x1c), stores +0x64; the landed MakeTeamMemberComment 0x00DCDA80 (+0x4c / +0x84), Main 0x00DCC770 (+0x78 / +0x8c), CTeamSpawn::Init 0x00DCD1D0 / ctor 0x00DCD160 agree; vtable 0x012DB2D4 slots 1 Main / 2 Init / 3 GetParentScript. MissionFailed's PDB type is `<unnamed-type-MissionFailed>` (4 bytes, no enumerator names): modelled as `long` (header debt) |
| engine/CSpecialAbilitiesTurncoatSpellDef.h (row 0x007C6E50) | `CSpecialAbilitiesTurncoatSpellDef : CParentDefClassBase` with the five Ego_r vector<float> members at retail offsets StaminaCostPerSec +0x3c .. TurncoatRange +0x60 .. TurncoatXPUpEveryNSecs +0x6c (12-byte STLport vectors, sibling CSpecialAbilities*Def layout) | struct_layouts_egor.tsv (0x90, 16-byte vectors after a 0x40 base); FrameUpdate reads TurncoatRange at +0x60 (0x007C6F7E); landed SetPTarget 0x007C6940 reads TurncoatTime at +0x54 |

## Opt-ins added to copies of repo headers
| header | opt-in | change | evidence |
|---|---|---|---|
| engine/CTCTurncoatSpell.h | FABLE_RETAIL_CTCTURNCOATSPELL | the polymorphic retail class on CTCBase (vtable 0x0126E5C4 overrides, Ego_r / FableWin method set incl. private Start/StopTurncoatingEffects / FinalCleanup), members at Ego_r order with the 0x34 retail timer: PDef +0x40 .. XPFrameCounter +0x94, sizeof 0x98; the generated flat layout stays the default | rtti-vtables.tsv; both PDB name sets; FrameUpdate reads every member offset; landed Start/StopTurncoatingEffects, SetPTarget, CancelMove agree |
| engine/CWillButtonHelperTimer.h (H40j copy) | FABLE_RETAIL_CWILLBUTTONHELPERTIMER_SETSTATE_INLINE | inline `SetCurrentButtonHeldState(EButtonHeldState)` storing CurrentState | FableWin.pdb 0x0271B8C0 (COMDAT right after the spell's FrameUpdate), no Ego_r / Fable.exe copy; retail stores `mov [esi+20h],1` in place (0x007C6E91 / 0x007C7528) |
| engine/CTCSpecialAbilities.h | FABLE_RETAIL_CTCSPECIALABILITIES_TURNCOAT (inside the FULL branch) | StatGetInterfaceType + inline UnLockAbilitiyMutex (unless CANCELFORCUTSCENE already declares them) and the static __fastcall `FadeOrKillEffect(CThing*)` | FableWin.pdb ?UnLockAbilitiyMutex@CTCSpecialAbilities@@QAEXXZ (retail stores +0x0c / +0x10 in place, 0x007C6EF4); Ego_r / FableWin ?FadeOrKillEffect@CTCSpecialAbilities@@SIXPAVCThing@@@Z = the bound spelling of 0x0072AC40 (body: null / dead test, emitter fade) |
| engine/CTCHeroStats.h | FABLE_RETAIL_CTCHEROSTATS_MODIFYSTAMINA (needs _STAMINA) | `ModifyStaminaBy(long)` declaration + FableWin inline `GetStamina() const` | Ego_r / FableWin ?ModifyStaminaBy@CTCHeroStats@@QAEXJ@Z (0x0057B1DB, bound); FableWin ?GetStamina@CTCHeroStats@@QBEJXZ, retail reads +0x58 in place (0x007C73A0) |
| engine/CTCTurncoat.h | FABLE_RETAIL_CTCTURNCOAT | selects the existing complete retail class branch (until now only under FABLE_RETAIL_CTCTARGETINGAI_ATTACK) without the targeting-AI pointer variants | Ego_r / FableWin records; IsTurncoated 0x007C7890, ModifyTurncoatedness 0x007C7A00, SetTurncoatOriginator 0x007C76D0 |

## Added in chunk 3013
| header | opt-in | what | evidence |
|---|---|---|---|
| engine/C3DMeshMaterial.h (copy of rebuild/include, default text unchanged) | `FABLE_RETAIL_C3DMESHMATERIAL_FLAG_ACCESSORS` | inline `bool IsTransparent() const`, `bool IsBooleanAlpha() const`, `unsigned long GetMapFlags() const` returning Transparent +0x29 / BooleanAlpha +0x2A / MapFlags +0x20 | FableWin.pdb ?IsTransparent@C3DMeshMaterial@@QBE_NXZ / ?IsBooleanAlpha@C3DMeshMaterial@@QBE_NXZ / ?GetMapFlags@C3DMeshMaterial@@QBEKXZ (no out-of-line copy in Fable.exe or Ego_r.pdb); members from struct_layouts_egor.tsv (C3DMeshMaterial +32 MapFlags, +41 Transparent, +42 BooleanAlpha); retail 0x00B9AA17 / 0x00B9AA1A / 0x00B9A99A read them in place. Used by 0x00B9A4A0 (with the existing STATICBLOCKS_TEXTURE_HELPERS GetDiffuseMapBankIndex). |
