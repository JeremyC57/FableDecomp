# NF378 — real PDB names for landed stand-in rows (Account A naming lane)

114 targets (`targets.txt`). Final: **24 done** (27 changed rows with 3 binders that move with their units), 42 skipped-port,
30 skipped-identity-unproven, 18 skipped-conflict. `STATUS.tsv` has one line per target.

## Batches (land_scripted run dirs)
| batch | rows | units | headers (add-only opt-ins) | table |
|---|---|---|---|---|
| b1 | 21 (17 selves, selfbinder 009ea9e0, binders 0041d122 / 0041d165 / 00a07900) | 18 | CAIBrain, CPlayer, CGamePlayerInterface, CCreatureActionBase, CTCBulletTime, CTexture, CRenderManagerCore, CHandleBasedMemoryPool, CRenderStateManager | 3 replace |
| b2 | 5 selves (00a0ae10, 009f3390, 0093f8a0, 00914b20, 006cd3b0) | 5 | CSurface, CThingSearchTools | none |
| b3 | 1 self (00b5bd30, local class model; no header). Graded as run b3r after a row-comment wording fix | 1 | — | none |

Gates for every changed row (`batches/bN/evidence`, `logs`):
- vland HEAD and new: MATCH / RELOCATION_MATCH + test PASS, the same grade at the same path (RESULTS reason column; logs/vland_*).
- objsec ALL_EQ on every row (evidence/objsec_*.txt); the only symbol changes are the intended renames (CHECKS.tsv).
- Both pick-any checkers, HEAD v new (pk_all with --row-length and --leaf; pickany_vs_retail with python3 -I): pkcmp GATE PASS, 0 turned/newly NEQ;
  reach flips n->y = 0 (evidence/pk/*_PKCMP.txt, *_REACHFLIP.txt).
- /FORCE link of each batch's objects (units + their binders; the selves of b2 / b3 have no landed caller in reloc_bindings): 0 duplicates
  HEAD and new; b1 has the same single unresolved (`?TextureTransformStates@@3QBJB`) on both sides (evidence/link_*.txt).
- apply_table.py --dry on each TABLE_CHANGES.tsv: 0 misses, multi-target 1 before / 1 after, no new one.
- Headers: every edit is add-only under a new `FABLE_RETAIL_*` macro (grep: unused anywhere in rebuild/ before this work); pin_asserts pinned 0.
  equiv_check (gcc -E, repo rebuild/include v overlay tree) over the reverse include closure: b1 2,314 rows -> 0 differing, b2 618 rows -> 0.
  A planted control (work/treeP, CTCBulletTime) was flagged (7 rows), so the check sees changes (evidence/eq_ctl.*).
- head/payload.json is built from the repo sources; mkbatch378.py asserts payload == repo == orig/ for every row and test and orig/hdr_* == repo header.
- Repo tests are unchanged (each test is a standalone behaviour mirror; none includes the row).

## Done rows: the two sources
Identity rule, applied the same way to done, port and unproven rows:
- Source 1: BSim >= 0.95 onto the PDB symbol. The exact spelling must be in both the Ego_r.pdb and FableWin.pdb name sets.
- Source 2: Ego_r.pdb object membership (egor_obj_functions.tsv) with >= 2 landed same-object neighbours and mostly consistent VA order.
- With only 1 neighbour, source 2 also needs one more independent fact:
  - a retail caller whose site lies inside the same object's contiguous retail span (00692400: site 0x006949A0 sits between
    InitialiseActionEvents 0x006946C0 and Initialise 0x00694D10, both creature_action_base.obj; 0093f8a0: sites 0x0093F031 / 0x0093F0A8
    inside CAIStateGroup_VillageTaskPatrol::Initialise's ai_state_group_vt_patrol.obj span; port row 005bc729: caller 0x005BF97D inside
    tc_inventory_base.obj), or
  - for a template instantiation, the row's own relocation callees spelling out the template arguments (00914b20 calls
    CThingFilter_IsNot::operator() and CThingCompare_Nearest::operator()<CThingAICreature>; 0093f8a0 calls the HasTC / In2DRange filters
    and CThingCompare_Nearest<CThing>).
- Rows with only vftable pointers (007f5050, 007f5070, 00880800; not tied to RTTI here) or only membership of the giant register_scripts.obj
  (00e83990) are identity-unproven.
Conflict check: the new symbol is carried by no other landed address (self_symbols.tsv) and bound to no other target (reloc_bindings.tsv); rechecked
against the current repo just before assembly (b3 included: the uncapped recheck found no self_symbols / reloc_bindings line for
`?FinishWithDynamicTextures@CEngineScreenEffectRadialBlurRenderer@@AAEXXZ`, no binding by target 00b5bd30, and `ReleaseTextures_00b5bd30` only
in its own row and test. engine/CEngineScreenEffectRadialBlurRenderer.h does not declare the member).

Stay-off screen of all 27 changed rows: COORDINATION_CLAIMS.tsv, T3_PORT_QUEUE.tsv, every pending/*acctB* payload and rows/ folder, and the NF379/NF380 targets.
- No changed row is staged or claimed by Account B, NF379 or NF380.
- The acctB hits are mentions only: header comments, dossiers, and callers' rows.
- 00a07900 appears in T3_PORT_QUEUE only inside the nf289 reason text of rows 00a04630 / 00a059e0.
- 0041d165: see Other notes.

Includers: no file in rebuild/src, rebuild/tests or rebuild/include `#include`s any of the 27 changed sources, so no thin row or includer
needs a regrade. The 27 tests are standalone, and vland WARN lines are identical between HEAD and new.

| row | batch | new symbol | source 1 | source 2 |
|---|---|---|---|---|
| 00445ae0 | b1 | `?SaveInitialInfo@CGamePlayerInterface@@AAEXXZ` | BSim 1.0000; both PDB name sets | Ego_r game_player_interface.obj#29, 9 same-object neighbours, order ok 6/bad 5; retail callers calls=0 ptrs=0 landed=- |
| 00445b40 | b1 | `?LoadInitialInfo@CGamePlayerInterface@@AAEXXZ` | BSim 1.0; both PDB name sets | Ego_r game_player_interface.obj#30, 9 same-object neighbours, order ok 6/bad 5; retail callers calls=0 ptrs=0 landed=- |
| 004874a0 | b1 | `?IsTargetLocked@CPlayer@@QBE_NXZ` | BSim 1.0000; both PDB name sets | Ego_r player.obj#9, 8 same-object neighbours, order ok 11/bad 0; retail callers calls=2 ptrs=0 landed=- |
| 00692400 | b1 | `?InitialiseActionTime@CCreatureActionBase@@AAEXXZ` | BSim 0.9767; both PDB name sets | Ego_r creature_action_base.obj#3, 1 same-object neighbours, order ok 3/bad 0; retail callers calls=1 ptrs=0 landed=- |
| 006cd3b0 | b2 | `??$GetPBestThing@V?$CFilter_Bind2@V?$const_mem_fun_ref_t@_NVCThingGameObject@@@std@@V?$CFilter_Not@VCThingFilter_In2DRange@@@@@@VCThingCompare_Nearest@@VCThingGameObject@@@CThingSearchTools@@QBE_NABV?$CFilter_Bind2@V?$const_mem_fun_ref_t@_NVCThingGameObject@@@std@@V?$CFilter_Not@VCThingFilter_In2DRange@@@@@@ABVCThingCompare_Nearest@@PAPAVCThingGameObject@@@Z` | BSim 1.0000; both PDB name sets | Ego_r tc_creature_navigation.obj#47, 2 same-object neighbours, order ok 4/bad 0; retail callers calls=1 ptrs=0 landed=- |
| 0079c5f0 | b1 | `?GetBulletTimeTicksFromAbilityLevel@CTCBulletTime@@ABEJXZ` | BSim 0.9999; both PDB name sets | Ego_r tc_bullet_time.obj#1, 2 same-object neighbours, order ok 4/bad 0; retail callers calls=0 ptrs=0 landed=- |
| 0079c6d0 | b1 | `?GetBulletTimeElapsedTicks@CTCBulletTime@@ABEJXZ` | BSim 1.0; both PDB name sets | Ego_r tc_bullet_time.obj#5, 6 same-object neighbours, order ok 8/bad 0; retail callers calls=0 ptrs=0 landed=- |
| 0088b8c0 | b1 | `?SetAllStateGroupsEnabled@CAIBrain@@QAEX_N@Z` | BSim 1.0000; both PDB name sets | Ego_r ai_brain.obj#17, 3 same-object neighbours, order ok 5/bad 1; retail callers calls=1 ptrs=0 landed=008302e0 |
| 00914b20 | b2 | `??$GetPBestThing@V?$CFilter_Bind2@VCThingFilter_IsNot@@VCThingFilter_IsChild@@@@VCThingCompare_Nearest@@VCThingAICreature@@@CThingSearchTools@@QBE_NABV?$CFilter_Bind2@VCThingFilter_IsNot@@VCThingFilter_IsChild@@@@ABVCThingCompare_Nearest@@PAPAVCThingAICreature@@@Z` | BSim 1.0; both PDB name sets | Ego_r ai_state_group_kid_wander_near_kids.obj#6, 1 same-object neighbours, order ok 1/bad 0; retail callers calls=1 ptrs=0 landed=- |
| 0093f8a0 | b2 | `??$GetPBestThing@V?$CFilter_Bind2@VCThingFilter_HasTC@@VCThingFilter_In2DRange@@@@VCThingCompare_Nearest@@VCThing@@@CThingSearchTools@@QBE_NABV?$CFilter_Bind2@VCThingFilter_HasTC@@VCThingFilter_In2DRange@@@@ABVCThingCompare_Nearest@@PAPAVCThing@@@Z` | BSim 1.0000; both PDB name sets | Ego_r ai_state_group_vt_patrol.obj#6, 1 same-object neighbours, order ok 1/bad 0; retail callers calls=2 ptrs=0 landed=- |
| 009ea5a0 | b1 | `?ResetCommandLineBuffers@CConsole@@AAEXXZ` | BSim 1.0000; both PDB name sets | Ego_r lib_console2.obj#13, 5 same-object neighbours, order ok 5/bad 2; retail callers calls=0 ptrs=0 landed=- |
| 009ea660 | b1 | `?RevertToUserEntry@CConsole@@AAEXXZ` | BSim 0.9999; both PDB name sets | Ego_r lib_console2.obj#21, 3 same-object neighbours, order ok 3/bad 4; retail callers calls=1 ptrs=0 landed=009ea9e0 |
| 009ea980 | b1 | `?PrevInHistory@CConsole@@AAEXXZ` | BSim 1.0000; both PDB name sets | Ego_r lib_console2.obj#36, 5 same-object neighbours, order ok 2/bad 5; retail callers calls=0 ptrs=0 landed=- |
| 009ea9e0 | b1 | `?RevertToUserEntry@CConsole@@AAEXXZ` | BSim 1.0; both PDB name sets | Ego_r lib_console2.obj#37, 6 same-object neighbours, order ok 2/bad 5; retail callers calls=0 ptrs=0 landed=- |
| 009eab80 | b1 | `?SetHistoryMode@CConsole@@AAEXXZ` | BSim 0.9947; both PDB name sets | Ego_r lib_console2.obj#40, 7 same-object neighbours, order ok 7/bad 2; retail callers calls=0 ptrs=0 landed=- |
| 009f3390 | b2 | `?GetPitch@CSurface@@QBEKXZ` | BSim 1.0000; both PDB name sets | Ego_r lib_surface.obj#23, 5 same-object neighbours, order ok 8/bad 0; retail callers calls=0 ptrs=0 landed=- |
| 009f9d60 | b1 | `?GetRefCount@CTexture@@QAEJXZ` | BSim 0.9999; both PDB name sets | Ego_r lib_texture.obj#3, 5 same-object neighbours, order ok 7/bad 0; retail callers calls=0 ptrs=0 landed=- |
| 00a02680 | b1 | `?ResizePool@CHandleBasedMemoryPool@@QAE_NKM@Z` | BSim 1.0; both PDB name sets | Ego_r lib_handle_based_memory_pool.obj#24, 4 same-object neighbours, order ok 6/bad 0; retail callers calls=2 ptrs=0 landed=0041d122,0041d165 |
| 00a044e0 | b1 | `?ForceResendRenderStatesToHardware@CRenderStateManager@@QAEXXZ` | BSim 1.0; both PDB name sets | Ego_r lib_render_state_manager.obj#3, 4 same-object neighbours, order ok 5/bad 0; retail callers calls=1 ptrs=0 landed=00a07900 |
| 00a0ae10 | b2 | `?DrawCurrentBuffersAsTris@CRenderManagerCore@@QAEXKK@Z` | BSim 0.9999; both PDB name sets | Ego_r lib_render_manager_core.obj#23, 8 same-object neighbours, order ok 10/bad 0; retail callers calls=0 ptrs=0 landed=- |
| 00a0b1e0 | b1 | `?SetWorldTransform@CRenderManagerCore@@QAEXKABVCMatrix3x4@@@Z` | BSim 1.0; both PDB name sets | Ego_r lib_render_manager_core.obj#34, 6 same-object neighbours, order ok 10/bad 0; retail callers calls=0 ptrs=0 landed=- |
| 00a0b280 | b1 | `?SetTextureTransform@CRenderManagerCore@@QAEXKABVCMatrix3x4@@@Z` | BSim 0.9999; both PDB name sets | Ego_r lib_render_manager_core.obj#35, 6 same-object neighbours, order ok 10/bad 0; retail callers calls=0 ptrs=0 landed=- |
| 00a0b3e0 | b1 | `?SetPerspectiveTransform@CRenderManagerCore@@QAEXMMMMMMMMMMMMMMMM@Z` | BSim 1.0; both PDB name sets | Ego_r lib_render_manager_core.obj#37, 6 same-object neighbours, order ok 8/bad 0; retail callers calls=0 ptrs=0 landed=- |
| 00b5bd30 | b3 | `?FinishWithDynamicTextures@CEngineScreenEffectRadialBlurRenderer@@AAEXXZ` | BSim 0.9999; both PDB name sets | Ego_r Engine_Screen_Effect_Radial_Blur_Renderer.obj#8, 6 same-object neighbours, order ok 7/bad 1; retail callers calls=0 ptrs=0 landed=- |

Mixed VA order inside an object (00445ae0 / 00445b40: 6 ok / 5 bad) was accepted because membership is strong (9 neighbours) and the
mangled names are unique in both PDBs.

## Forms used (please rule if any is unwanted)
1. **Helper-cast form** (00445ae0, 00445b40, 00a0b1e0, 00a0b280, 00a0b3e0, 009f3390, the three GetPBestThing rows): the method is now defined
   on the real class, but the inline helpers that HEAD emitted as COMDATs of the stand-in (`CGamePlayerInterface_Methods::GetPacketFile`,
   `CRenderManagerCore_Methods::SetD3DMatrix`, `CSurface_Methods::GetWidth/GetPixelFormat`, `CThingSearchToolsMethods::PeekAllThings`) stay on the
   stand-in struct and are reached by `static_cast<Stand_in*>(this)->helper()`. Turning them into free functions changed the COMDAT names
   and failed objsec (first b1 run: 5 REJECT; rerun b1r with this form: 5 ACCEPT). The leftover stand-in names are helper COMDATs, not the
   row's own symbol. The template rows also need a `const_cast` because PeekAllThings is non-const on the stand-in.
2. **DrawCurrentBuffersAsTris (00a0ae10)**: the stand-in returned `long` (QAEJKK); the PDB spelling is `void` (QAEXKK). Retail sets no return value
   after the call, and the bytes are identical (objsec ALL_EQ), so the void form is the PDB spelling, not a type change of the body.
3. **Local class models** (00a02680, 00a0ae10, 00b5bd30): these rows already define their own class with the real class name (not a shared
   header); only the member's name / access changed. Porting them onto the shared header is separate work.
4. **CConsole rows** (009ea5a0 / 009ea660 / 009ea980 / 009ea9e0 / 009eab80): there is no engine/CConsole.h; the local `struct CConsole_Retail` became
   `class CConsole` with `private:` methods (PDB AAE). The other CConsole_Retail stand-ins (RunCommandLine 009eb430, AddCommandLineToHistory
   009eaf80, FindInput 009eb220) are not targets and are unchanged.
5. CThingSearchTools: the existing PASS26 GetPBestThing opt-in is only in the retail branch, so a flattened-branch twin
   (`FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTTHING_DECL`) was added. CEnginePrimitiveRenderer.h was edited for 00b32100 and then set aside (held/).

## Skips (details per row in STATUS.tsv)
- **skipped-conflict (18)**: the PDB name is already carried by another landed address, with no ICF fold proven (GOTCHAS nf104):
  0044c57d 004cb6e0 00640460 00ab9f70 00b6d600 00b9d650 00be4400 00c28080 00c283c0 00c283e0 00c338f0 00c39f10 00c3c590 00c44250 00c54bd0 00c6bff0 00cd45e0 00a3a380.
- **skipped-identity-unproven (27)**: 10 Dinkumware VC10 donor names (NF301: _Cons_val / _Dest_val / $$Q / _Copy_backward are not STLport);
  15 with no second source (no Ego_r neighbour or no consistent donor); 008f5c20 (no landed neighbour in hero_stat_modifier_events.obj);
  007f5050 / 007f5070 / 00880800 (1 neighbour + unverified vftable pointers); 00e83990 (register_scripts.obj only).
- **skipped-port (42)**: the name is proven (NAME_FIXES.tsv), but respelling it needs a port, under the following rules:
  - A standalone `_Retail` model where the shared header exists.
  - Virtual slots.
  - A parameter or return type change (NF305 Q1).
  - A namespaced PDB class (NInventory::CItem, NUISystem::CComponent, NThingMap::...).
  - A private PDB member called from a derived stand-in (00b32100 / binder 00b32230, texts in held/).
  - The anonymous-namespace TU hash ?A0xca0d66cb (007f7040).
  - Account B's live package (00a23f30, C3DAnimationInfo.h, b24). This is a stay-off, not a queue item: it is in NAME_FIXES only.
  - QUEUE_PROPOSALS.tsv lists the other 41.

## Other notes
- 0041d165 appears in a stale 2026-11-21 **Account A** claim (COORDINATION_CLAIMS.tsv line 216, round-10 agent NF218). The stay-off rule covers Account B, and the nflocks lock was free, so I claimed it. Please confirm that claim is closed.
- 00a07900's own self (`CRenderStateManager::Init`) stays a stand-in. It is named only in the nf289 T3 reason text, which is not a reservation (NF316 Q3).
- Compiles: about 72 of the 150 budget.
- No retail bytes are in the hand-back. oracle.tsv and the wd/ objects stay in work/.

## Rulings needed
- R1: Is the helper-cast form (form 1) acceptable? The row symbol is real; the stand-in helper COMDATs remain. (9 rows)
- R2: 00a0ae10 return type `long` -> `void`: is this a respell or a type change? Discriminator against the skipped 006b0190 / 00a0ab50: objsec ALL_EQ
  (identical bytes, retail never sets eax after the call) and no landed caller is bound to it. 006b0190 changes the returned value's type,
  and 00a0ab50 changes a parameter type that callers would see.
- R3: Local-class respells (00a02680, 00a0ae10, 00b5bd30, the five CConsole rows) were done in place, against "prefer the shared header".
  The local models were already at HEAD; a port onto the shared header is separate work. There is no CConsole.h at all.
- R4: 0041d165 carries the stale 2026-11-21 Account A claim (line 216). Please confirm it is closed.
- R5: Should 007f7040 (anonymous-namespace hash in the PDB template argument) get a reproducible-spelling exception, or stay a port?
