# Chunk Q23 header overlay (grade with FABLE_HDR_OVERLAY=<this dir>)

Each file is a full copy of the current `rebuild/include/engine/<X>.h` plus `#ifdef FABLE_RETAIL_*` opt-in additions only; the
default (no opt-in) preprocessed text is unchanged. Nothing copied from the Q22 overlay (no overlapping headers). Re-diffed
against the repo at hand-back: additions only, plus the two `#ifdef` -> `#if defined()` rewrites noted below.

| Header | Opt-in | Addition | Used by | Evidence |
|---|---|---|---|---|
| CTCBuyableHouse.h | FABLE_RETAIL_CTCBUYABLEHOUSE_FRAMEUPDATE | Ego_r.pdb members after Rented with real types (CIntelligentPointer<CThing> MainSignPost/DressingSignPost/WifeLivingHere, FrameToNext..., InitiallySetupDressLevel, VirtualMoneyBags, HasPreviouslyBeenBuyable, IsResidential); FrameUpdate (slot 9) + SetupDressingAtLevel 0x006C0330, IsHouseBeingUsed(bool) const 0x006BFB10, GetRentsDue 0x006BF810, GetPriceForNextDressLevel 0x006BF920, private NeedsMainSignPost 0x006BFB90 / NeedsDressingSignPost / CreateMainSignPost 0x006C0F00 / CreateDressingSignPost 0x006C0F70 / CreateRentMoney 0x006C0C90. Structure: the former `#ifdef ..._WIFE` member block became `#if defined(..._FRAMEUPDATE) ... #elif defined(..._WIFE)` (WIFE path text identical). | 006c1250 | Ego_r + FableWin PDB, struct_layouts_egor.tsv |
| CThing.h | FABLE_RETAIL_CTHING_HASTC_FORCEINLINE | `#ifdef FABLE_RETAIL_CTHINGGAMEOBJECT_ONHIT` before HasTC's FABLE_FORCEINLINE became `#if defined(..._ONHIT) \|\| defined(..._HASTC_FORCEINLINE)` | 006c1250 | retail tests UsingComponents words in place (0x006C130C..0x006C1409) |
| CTCVillage.h | FABLE_RETAIL_CTCVILLAGE_PEEKVILLAGEPEOPLE | inline `const std::list<...>& PeekVillagePeople() const` | 006c1250 | FableWin ?PeekVillagePeople@CTCVillage@@QBE... |
| CSubtitleRenderer.h | FABLE_RETAIL_CSUBTITLERENDERER_PAUSE | `void Pause(bool)` 0x0048DD94 | 006c1250 | Ego_r ?Pause@CSubtitleRenderer@@QAEX_N@Z; manifest |
| CThingPhysical.h | FABLE_RETAIL_CTHINGPHYSICAL_STATTYPEFLAGS | `static unsigned long FABLE_FASTCALL StatGetTypeFlags() { return 3; }` | 0063f7c0 | FableWin ?StatGetTypeFlags@CThingPhysical@@SIKXZ; `and edx,3 ; cmp dl,3` 0x0063F92B |
| CPhysicsMeshObject.h | FABLE_RETAIL_CPHYSICSMESHOBJECT_NAVIGATION | GetNavigationLevelAt const 0x00A11D10, IsPointContainedInBoundingBox const 0x00A12890 (decls) | 0063f7c0 | Ego_r PDB; calls 0x0063F9EB / 0x0063F9A4 |
| CThingSearchTools.h | FABLE_RETAIL_CTHINGSEARCHTOOLS_FOREACHINROUGHAREA_BODY (needs ..._ORIGINAL_ROUGHAREA) | ForEachInRoughArea<T,Filter,Proc> body with the PDB `const Proc&` signature; placed as `#if BODY && ORIGINAL ... #elif defined(..._FOREACHINROUGHAREA)` so the existing declaration path is unchanged | 0063f7c0 (DIFFER) | Ego_r/FableWin: all instances QBE, ABV proc |
| CTCBoastingPosition.h | FABLE_RETAIL_CTCBOASTINGPOSITION_SETBOASTINGPODIUM | `void SetBoastingPodium(CThing&)` 0x008147D0 | 00814260 | FableWin ?SetBoastingPodium@CTCBoastingPosition@@QAEXAAVCThing@@@Z |
| NGraphicAppearance_CTCGraphicAppearance.h | FABLE_RETAIL_CTCGRAPHICAPPEARANCE_DUMMYOBJECTNAMES | `GetAllDummyObjectNames(std::vector<CCharString>&) const` 0x004BDB40 | 00814260 | Ego_r PDB; catalog agrees |
