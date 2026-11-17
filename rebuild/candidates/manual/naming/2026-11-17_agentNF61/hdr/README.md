# NF61 header overlay (round 7, slice land1117fu)

Every file here is a copy of the repo header at the same path under `rebuild/include/`, with `#ifdef` opt-ins added and nothing
removed (`diff <repo> <overlay> | grep '^<'` is empty for all eight files, re-checked at hand-back). Without its macro each opt-in
is token-identical to the repo text. Check (work/overlay_token_check.txt): unrelated landed includers preprocess to the same tokens
with and without the overlay (0x0085D180, 0x0043F450, 0x004E9021, 0x00AD9F10, 0x005FCBE0 byte-identical; 0x006A02B0, 0x00576C4F,
0x00A78E20 identical except the `fable_static_assert_<__LINE__>` typedef names, which emit nothing).

| Overlay file | Opt-in(s) | What it adds | Rows |
|---|---|---|---|
| stlport/fable_map.h | FABLE_RETAIL_STLP_TREE_MCOPY_BODY | includes fable_compiler.h; STLport 4.6 `_M_clone_node` (FABLE_FORCEINLINE, Inline-budget) and the private `_M_copy(x, p)` declaration + STLport _tree.c body | the 27 `_M_copy` rows |
| stlport/fable_map.h | FABLE_RETAIL_STLP_MAP_CTOR_NOINLINE | `__declspec(noinline)` on the STLport `map()` (under FABLE_RETAIL_STLP_MAP); retail calls the map<long,long> ctor COMDAT 0x004DF86C | 0x005907AB |
| stlport/fable_containers.h | FABLE_RETAIL_STLP_PAIR_DEFAULT_CTOR_NOINLINE | `__declspec(noinline) pair()` (retail calls it at 0x00469435); a row using it includes fable_containers.h first | 0x00469381 |
| engine/CEnginePrimitiveHandle.h | FABLE_RETAIL_CENGINEPRIMITIVEHANDLE_COPY_DECL | declares the copy ctor `CEnginePrimitiveHandle(const CEnginePrimitiveHandle&)` (retail 0x0043F450) | 0x005BBBA5 |
| engine/CWeaponDef.h | FABLE_RETAIL_CWEAPONTRAILGRAPHICSET_CTOR | `CWeaponTrailGraphicSet() : Attack(-1), Knockdown(-1) {}` in the class variant (Ego_r ??0CWeaponTrailGraphicSet@@QAE@XZ; stores at 0x004E91D4/D8) | 0x004E9156 |
| engine/CTCHaste.h | FABLE_RETAIL_CTCHASTE_SETACTIONSPEEDMULTIPLIER_OUTOFLINE | SetActionSpeedMultiplier declared only (the inline body moves under `#else`, unchanged) | 0x005812AE |
| engine/CTCHaste.h | FABLE_RETAIL_CTCHASTE_SETACTIONSPEEDMULTIPLIER_FORCEINLINE | FABLE_FORCEINLINE on the same inline body (Inline-budget: retail expands it in 0x0078B410; VC7.1 calls it out of line, also under /d2inlS20, 469 v 493 bytes); includes fable_compiler.h under the opt-in | 0x0078B410 |
| engine/CTCHaste.h | FABLE_RETAIL_CTCHASTE_UNSET_INLINE | `UnsetActionSpeedMultiplier(void*)` with its body visible (`Multipliers.erase(key); UpdateActionSpeedMultiplier();`, the text of the landed out-of-line copy 0x0058148D); needs FABLE_RETAIL_STLP_TREE_ERASE_KEY; not with FABLE_RETAIL_CTCHASTE_UNSET | 0x00694020 |
| engine/CTCInventoryExperience.h | FABLE_RETAIL_CTCINVENTORYEXPERIENCE_BUY | declares Ego_r.pdb public `BuyStat(EHeroTrainableStatType, long)` / `BuyAbility(EHeroAbility, long)` | 0x005786B5, 0x005785B3 |
| engine/CTCHeroExperience.h | FABLE_RETAIL_CTCHEROEXPERIENCE_SPENDEXPERIENCE | declares both Ego_r.pdb `SpendExperience` overloads (EHeroTrainableStatType 0x006D6F60, EHeroExperienceStatCategory 0x006D6590; the landed rows' Leafs) | 0x005786B5, 0x005785B3 |
| engine/CNavQuadTree.h | FABLE_RETAIL_CNAVQUADTREE_CLEARNODES | declares the Ego_r.pdb private `ClearNodes()` (?ClearNodes@CNavQuadTree@@AAEXXZ, 0x00A792E0) | 0x00A7A7F0 |

Macro names are member-specific (FABLE_RETAIL_<CLASS>_<MEMBER>) so they merge with NF59 / NF60 overlays. All rows that use these
were rechecked together in one vland run with this overlay (../recheck_final.log: 107 rows, 6 MATCH + 101 RELOCATION_MATCH, all PASS).
