# Chunk 879 header overlay (additive opt-ins only; every default path unchanged)

All additions are for row 0x00502E90 CWorldMap::PopulateRegionWithVillagers (parked DIFFER, differ/00502e90.cpp).
Diff each file against rebuild/include/<same path>; nothing is removed or reordered.

| File | Macro | Adds | Evidence |
|---|---|---|---|
| engine/CThingCreatureBase.h | `FABLE_RETAIL_CTHINGCREATUREBASE_SEXTYPE` | decls `ESex GetSex() const` (0x00661FF0), `ECreatureType GetCreatureType() const` (0x00662000) + `enum ECreatureType;` fwd | Ego_r 0x0053CE40/50, FableWin 0x01D9A430/50; out of line in retail |
| engine/CThingCreatureDef.h | `FABLE_RETAIL_CTHINGCREATUREDEF_SEX` | `Sex` +0xC0, `CreatureType` +0xC4 inside the retail layout (pads around them) | Ego_r Sex +0xC4 / CreatureType +0xC8 shifted by the 4-byte smaller STLport vector; retail getters read +0xC0 / +0xC4 |
| engine/CThingAICreature.h | `FABLE_RETAIL_CTHINGAICREATURE_RELATIONS` (needs `FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN`) | inline `SetHomeBuilding/SetWorkBuilding/SetFatherCreature/SetMotherCreature/SetSpouseCreature` (assign from a CIntelligentPointer temporary) and inline `GetFatherCreature/GetMotherCreature/GetSpouseCreature` | Ego_r 0x004E0DA0..0x004E0EE0 + FableWin; retail expands SetHomeBuilding at 0x005052AC, COMDATs 0x00512210..0x00512350 |
| engine/CThingBuilding.h | `FABLE_RETAIL_CTHINGBUILDING_SIMDEF` | decls `PeekPSimDefinition() const` (0x0082D2D0), `GetIsAvailableForResidency() const` (0x0082E0E0) | Ego_r + FableWin (FableWin spells the latter const) |
| engine/CTCVillage.h | `FABLE_RETAIL_CTCVILLAGE_GAYVILLAGERS` | decl `long GetNumberOfGayVillagers() const` (0x00658F41) | Ego_r + FableWin |
| engine/CWorld.h | `FABLE_RETAIL_CWORLD_POPULATE` | decl `bool PopDontPopulateNextLoadedRegion()` (0x0049DBD0); inline `PeekDefinitionManager() const` (returns `*DefinitionManager`) | Ego_r 0x004A6F50 / FableWin 0x01B50F70; FableWin/Ego_d ?PeekDefinitionManager@CWorld@@QBEABVCGameDefinitionManager@@XZ (retail 0x00505E31 reads [world+0x10] in place) |
| engine/CWorldMap.h | `FABLE_RETAIL_CWORLDMAP_POPULATE` | decl `void PopulateRegionWithVillagers(CThing& village)` | Ego_r 0x004D6E30 / FableWin 0x01C97C30 |
| engine/CMainGameComponent.h | `FABLE_RETAIL_CMAINGAMECOMPONENT_WORLDSEED` (inside `FABLE_RETAIL_CMAINGAMECOMPONENT`) | inline `unsigned long* GetWorldSeed() const` (address of WorldSeed) | FableWin 0x01B3FBA0 ?GetWorldSeed@CMainGameComponent@@QBEPAKXZ; inline in retail |
| stlport/fable_map.h | `FABLE_RETAIL_STLP_TREE_ERASE_KEY_CALLS` (nested in `FABLE_RETAIL_STLP_TREE_ERASE_KEY`) | `_Rb_tree::equal_range(const K&)` and `erase(iterator,iterator)` declaration-only (retail calls 0x00514730 / 0x0051AF80); `_Rb_tree::erase(const K&)` and `map::erase(const K&)` `__forceinline` under `_MSC_VER` (plain inline otherwise) | retail expands map::erase(key) in 0x00502E90 phase 13 and calls only the two helpers. Inline-budget device: both levels must be forceinline, forcing only the tree level leaves map::erase out of line (md 5743) |

Behaviour: every inline body above is retail's own logic (field read / CIntelligentPointer assignment / STLport erase-by-key);
nothing is a stand-in.
