# Chunk 2020 header overlay (`FABLE_HDR_OVERLAY=/opt/fable_lane/w22/agent2020/hdr`)

Every change to a copied repo header is an additive opt-in (`#ifdef FABLE_RETAIL_...`) or a new header; with none of the new macros
defined each copied file preprocesses to the repo text (copies taken from HEAD 2026-11-11), with one exception noted below
(NGraphicAppearance_CTCGraphicAppearance.h, a fix that only changes a combination that did not compile before).

Also in this directory, copied unchanged from the chunk 2012 hand-back (`/opt/fable_lane/w22/agent2020/prior/hdr`, its README kept as
README.prior2012.md; those files in turn carry chunk 2007's not-yet-landed headers): CAnimSet.h .. NGameText_CDataBank.h, including
CTCWife.h (APPEARANCEBOOL), CTCHeroMorph.h, CTCHeroStats.h (FATNESS), CTattooDef.h (MEMBERS), CWorld.h, CThingCreatureBase.h,
CThingAICreature.h. Only the parked row 0x006D1250 needs those; the two matched rows were re-graded with the whole overlay in place
(gall.sh RELOCATION_MATCH for both, vland RELOCATION_MATCH PASS).

## New headers
| header | content | evidence |
|---|---|---|
| engine/CAIStateGroup_GoToPub.h (FABLE_RETAIL_CAISTATEGROUP_GOTOPUB) | `class CAIStateGroup_GoToPub : public CAIStateGroupBase` with the Ego_r members (PPub +0x14 .. TimeBeforeAskingForBeer +0xA4, protected: CAIStateGroup_GameMaster derives from it), protected virtual IsRunnable / Update, protected helpers GetObjectWithScriptName 0x00947EB0, IsStaffed 0x00947FB0, GetSeat(CThing* = 0, const CCharString* = 0) 0x009482A0, FreeSeat 0x009486D0, GetBar 0x009490D0 | Ego_r struct_layouts (0xA8) + the landed implicit dtor 0x008FDCCA (members +0x14..+0x34) and GameMaster::GetSeat 0x0094CB40; both PDB name sets for every method spelling (IAE / MAE); GetSeat's NULL defaults: IsRunnable 0x00949D7C..0x0094A0B7 is GetSeat's no-table branch with the table / seat tests folded |
| engine/NGraphicAppearance_CGraphicAppearanceDrawInfo.h | `NGraphicAppearance::CGraphicAppearanceDrawInfo` (class), Ego_r layout 36 bytes: Interpolate +0 .. TextureMorphsUsed +0x21 | Ego_r struct_layouts; mangled `ABVCGraphicAppearanceDrawInfo@NGraphicAppearance@@` in DrawLight / DrawFlare / OnAppearanceDraw; DrawLight reads Interpolate at +0 (0x00641594 / 0x0064159C) |

## Opt-ins added to copies of repo headers
| header | opt-in | change | evidence |
|---|---|---|---|
| engine/CTCTavernTable.h | FABLE_RETAIL_CTCTAVERNTABLE_SEAT | GetSeat(CThing&, C3DVector&, CRightHandedSet&, long&, long&, const CCharString*) 0x007B8840 and GetTankard(long) 0x007B8590 declarations (+ `class CCharString;`) | both PDB name sets (the bound spellings at both addresses); `ret 18h` pushes at 0x00949FE5..0x00949FF8 |
| engine/CTCAIScratchpad.h | FABLE_RETAIL_CTCAISCRATCHPAD_TABLESEAT | inline by-value `GetTableName()` / `GetSeatName()` const | FableWin.pdb ?GetTableName / ?GetSeatName @CTCAIScratchpad@@QBE?AVCCharString@@XZ; retail copy-constructs TableName +0x70 / SeatName +0x74 into the caller's temporaries (0x00949B0D / 0x00949B35 / 0x00949B95) |
| engine/CTCVillage.h | FABLE_RETAIL_CTCVILLAGE_TAVERN | `IsTavernTime() const` 0x00658C0A, `GetNearestBuildingWithProperty(const C3DVector&, ESimBuildingProperty)` 0x00658E38 (+ MS forward enum) | both / FableWin PDB name sets; the landed rows at both addresses; calls 0x00949D1A / 0x00949D37 |
| engine/CTCLight.h | FABLE_RETAIL_CTCLIGHT_DRAWLIGHT | the nested `CLightInfo` (Colour +0, InnerRadius +4, OuterRadius +8, Flicker +0xC, LightingChannelsEffected +0x10, Inverted +0x14) and inline bodies of GetColour / GetInnerRadius / GetOuterRadius / IsInverted (`Overridden ? PLightInfo->X : PLightDef->X`) | Ego_r CLightInfo layout (24 bytes) + the landed SetColour 0x00640BB0 model; FableWin ?GetColour@CTCLight@@IBEABVCRGBColour@@XZ etc.; retail expands each in DrawLight (0x0064151E / 0x0064191E / 0x00641986 / 0x0064199F); no out-of-line copy bound |
| engine/CEnginePrimitiveLight.h | FABLE_RETAIL_CENGINEPRIMITIVELIGHT_INLINE | the constructor with its body visible (base type 0x0F + interpolation set via the ID_CTOR base, WorldPos, Colour, Radius, SpotRadius, LightingChannelMask) | the landed out-of-line copy 0x00642000 (same TU, right after DrawLight); DrawLight 0x00641A7F..0x00641AA2 stores the same fields into its argument object |
| engine/CGameTimeManager.h | FABLE_RETAIL_CGAMETIMEMANAGER_GETTIME (narrow branch) | inline `float GetTime() const { return Time; }` | both PDB name sets ?GetTime@CGameTimeManager@@QBEMXZ; retail `mov eax,[eax+1Ch] ; mov ecx,[eax+8]` after PeekWorld (0x006415C1) |
| engine/CEnvironment.h | FABLE_RETAIL_CENVIRONMENT_SUNLIGHTATTENUATOR (inside VIRTUALS) | inline PeekSunlightAttenuatorAngleFadeStart / End (+0x08 / +0x0C) and `const CRGBFloatColour PeekDiffuseColour() const` reading the 16 bytes at +0x58 (GameTickState +4, BlendedTheme[0].Colours[0]) through the opaque array; includes CRGBFloatColour.h under the macro | FableWin name set for all three; Ego_r CEnvironment / CEnvironmentGameTickState / CBlendedEnvironmentTheme layouts (retail GameTickState +0x54); DrawLight 0x00641884 / 0x006418A3 / 0x00641900 |
| engine/CRGBFloatColour.h | FABLE_RETAIL_CRGBFLOATCOLOUR_SCALE | inline `operator*=(float)` | FableWin ??XCRGBFloatColour@@QAEAAV0@M@Z; retail R, G, B, A multiplies 0x0064194A..0x00641982 |
| engine/NGraphicAppearance_CTCGraphicAppearance.h | (fix inside FABLE_RETAIL_CTCGRAPHICAPPEARANCE_LIGHTINGCHANNEL) | the opaque `LayoutMeshEffectsToComponents` + `LightingChannel` layout is skipped when FABLE_RETAIL_CTCGRAPHICAPPEARANCE_MOVEMENT already lays out LightingChannel at +0x40 (before: only when VOLUMECONTAINMENTTRACKER was also defined, otherwise C2086 redefinition + the size assert) | the MOVEMENT branch's members (LightingChannel at +0x40, sizeof 0x5C); DrawLight 0x006419CA `mov cl,[eax+40h]` through PeekLightingChannel |

Landing note: re-copy HEAD first and re-apply the blocks (other chunks may have changed the same headers since 2026-11-11). The
CTCGraphicAppearance fix changes the condition of an existing block: check that no landed includer defines both
LIGHTINGCHANNEL and MOVEMENT without VOLUMECONTAINMENTTRACKER (such a TU would not have compiled before).

# Chunk 3019 additions (w22 agent3019, row 0x006D1250 CTCWife::CheckAppearanceChanges)

| header | opt-in | change | evidence |
|---|---|---|---|
| stlport/fable_map.h (new overlay copy of the repo file, HEAD 2026-11-12) | FABLE_RETAIL_STLP_TREE_CONSTFIND_TEMPLATE_NOINLINE | the const member-template `find(const KT&) const { return const_iterator(_M_find(key)); }` with its body visible, `__declspec(noinline)` under `_MSC_VER` only (plain inline elsewhere); first branch of the existing find `#if` chain | retail calls 0x00616580 at all three sites of the row (0x006D134D / 0x006D1467 / 0x006D1734); the TU's out-of-line copy of this body equals 0x00616580 (and its twin 0x00B2EB20) instruction for instruction; the visible body is what lets VC7.1 forward the tattoo key across ~CCharString (0x006D170F..0x006D171E) and load covered_areas first in loop 1 (0x006D132E). Inline-budget device (EXTRA_RULES owner decision 2026-11-08, preference 3): the c2 trace scores the plain inline find 88 / 68 / 60 at the three sites while the LowerBound sites retail expands score 63 / 60, so no /d2inlT<N> can split them. |

With the macro undefined the copied fable_map.h preprocesses to the repo text (no landed row changes). The row also turns on the existing
repo opt-ins FABLE_RETAIL_STLP_MAP + FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY (the _tree.c insert_unique / _M_insert bodies; the TU's
out-of-line insert_unique copy equals retail 0x00512DA0 apart from relative branch / call targets) and no longer needs
FABLE_RETAIL_STLP_TREE_FIND_TEMPLATE (the new branch replaces that declaration). Landing note: re-copy fable_map.h from HEAD and
re-apply the one block if the file changed.
