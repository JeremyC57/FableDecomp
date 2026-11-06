# Chunk 783 header overlay

Every change is an opt-in `#ifdef FABLE_RETAIL_<X>` branch or a new header with no existing includer: the default
path of every repo header is unchanged, so no INCLUDERS list is needed. Siblings regraded read-only with this overlay
are in ../SIBLINGS.tsv (all MATCH / RELOCATION_MATCH).

| header | change | evidence | used by |
|---|---|---|---|
| engine/CDefClassBase.h | `FABLE_RETAIL_CDEFCLASSBASE_IMPLICIT_ASSIGN`: drop the declared operator= (0x00431F10) so the implicit one expands | 0x004321FB: call 0x00431F60, PDefManager +0x1c, GlobalIndex +0x20, the three bits of +0x24 as xor/and/xor merges, inline in the derived assignment; landed CFrontEndDef 0x00432E60 / CEngineVideoOptionsDef 0x00434450 show the same | 0x004321F0 |
| engine/CResource.h | `FABLE_RETAIL_CRESOURCE_ASSIGN`: declare `CResource& operator=(const CResource&)` (0x00431F60) | FableWin.pdb ??4CResource@@QAEAAV0@ABV0@@Z; called from 0x00431F19 / 0x004321FB / 0x00432E6B | 0x004321F0 |
| engine/CParentDefClassBase.h | `FABLE_RETAIL_PDCB_FORCEINLINE_ASSIGN` (with FABLE_PDCB_TYPED_MEMBERS): a `__forceinline` member-wise operator= | implicit and plain inline both leave a call to the COMDAT 0x00431ED0 (attempts a-c, 2031v2117); retail expands it with the CDefClassBase one inside | 0x004321F0 |
| engine/CRenderManagerCore.h | `FABLE_RETAIL_CRENDERMANAGERCORE_DRAWASINDEXEDTRIS`: declare DrawAsIndexedTris(const CTexture**, unsigned long, const void*, long, long, void*, long) 0x00A0AEA0 | Ego_r 0x00B74460 / FableWin 0x030EC200 `?DrawAsIndexedTris@CRenderManagerCore@@QAEXPAPBVCTexture@@KPBXJJPAXJ@Z`; retail `ret 1Ch` | 0x009FA780 (parked) |
| engine/CDisplayManager.h | `FABLE_RETAIL_CDISPLAYMANAGER_GETVIEWPORT`: inline `const C2DBoxI& GetViewport() const { return ViewportBox; }` | FableWin ?GetViewport@CDisplayManager@@QBEABVC2DBoxI@@XZ; 0x009FA7FE..0x009FA828 copies ViewportBox +0x1F4 field by field | 0x009FA780 |
| engine/CTexture.h | `FABLE_RETAIL_CTEXTURE_BLTTORENDERTARGET`: declare BltToRenderTargetWithGPU(bool, bool, bool) const | Ego_r 0x00B6A840 ?BltToRenderTargetWithGPU@CTexture@@QBEX_N00@Z; `ret 0Ch` | 0x009FA780 |
| engine/CTVertexRHWColTex1.h | new: X, Y, Z, RHW, DiffuseColour, U1, V1 (0x1C) | Ego_r CTVertexRHWColTex1Base (struct_layouts_egor.tsv, 28 bytes); retail stride 1Ch in 0x009FA780 | 0x009FA780 |
| engine/CQ_BanditCampBossBattleScript.h | new: the quest script (CLevelScriptBase<CQ_SunnyvaleMasterData>, members +0x48..+0x6d, sizeof 0x70) with nested CGate3Guard (ParentClass& +0x14, PMasterData +0x18) | Ego_r layout - 0x14 (struct_layouts_egor.tsv), the offsets the landed Init 0x00D03610 / CCampHostage2::Main 0x00D08790 read | 0x00D07490 (parked) |
| engine/CQ_PrisonRaceScript.h | new: the quest script (members +0x48..+0x5c, sizeof 0x60) with nested CPrisonRaceGuard (ParentClass& +0x14, PMasterData +0x18, DistanceToPlayAnim +0x1c) | Ego_r layout - 0x14; landed Alloc 0x00DDDC30 `push 60h`; landed Init 0x00DDB570 writes +0x1c | 0x00DDB720 (parked) |

The two quest headers declare only the nested entity class the chunk's rows need; later chunks add the other entity
scripts (Alloc / Init / GetParentScript addresses are in each header comment).
