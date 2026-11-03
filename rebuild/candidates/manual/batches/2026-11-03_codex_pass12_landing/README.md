# Codex pass 12 landing (2026-11-03)

Source: `rebuild/candidates/manual/pending/2026-11-02_codex_pass12/verified_pending/<addr>/` (50 rows, RELOCATION_MATCH + PASS in
Codex's header context). Landed on ONE header model (ours); no existing shared header changed. Per-row state: `STATUS.tsv`.

**Result: 20 of 50 landed (all RELOCATION_MATCH + PASS on our headers), 30 held, 19 landed rows respelled and rechecked,
`bindings_multi_check.py` = 0 after every wave.**

## Wave A (vendor zlib / libvorbis): 3 landed, 1 held
| addr | leaf | grade |
| --- | --- | --- |
| 0x00C01750 | `_deflate_slow` | RELOCATION_MATCH PASS |
| 0x00CA4160 | `@vorbis_lpc_to_lsp@12` | RELOCATION_MATCH PASS |
| 0x00C9F8C0 | `@mapping0_inverse@8` (+ new `engine/Vorbis.h`) | RELOCATION_MATCH PASS |

Held: **0x00CA2BB0** (`@floor1_encode@16`). It calls floor1.c's own `ilog` at 0x00CA1790, spelled `@ilog@4`, but
`@ilog@4` is already bound to 0x00C9FE70 (the landed 0x00C9FEA0's TU). A vendor file static has the same symbol in every TU, so binding
floor1.c's copy gives one symbol two targets (same shape as the held `@ilog2@4` triplicate, queue `retry1`). `record_bindings` keeps
the first binder and does not record the second target, so landing it would pass silently with a wrong table. The source also adds
`static __declspec(noinline)` to the vendor static (not in libvorbis). Queued REVIEW `cx12 0x00CA1790`.

## Wave B (ImeUi, DXUT): 2 landed + 17 landed rows respelled
Landed: 0x00CB38E0 `?ImeUi_RenderUI@@YIX_N0@Z`, 0x00CB48A0 `?ImeUi_ProcessMessage@@YIJPAUHWND__@@IIJPA_N@Z` (both Ego_r + FableWin).

Respelled the 13 descriptive pass 11 ImeUi globals to the DXUT `ImeUi.cpp` names (`g_CaretInfo` as `CompStringAttribute`, `g_screenWidth`,
`g_screenHeight`, `ImeUiCallback_DrawRect`, `ImeUiCallback_OnChar`, `g_szCompositionString`, `g_bImeEnabled`, `g_bInitialized`,
`g_hklCurrent`, `g_dwCount`, `g_dwSelection`, `g_uCandPageSize`, `g_bCandList`, `g_hwndCurr`, `_GetReadingString`, `_ShowReadingWindow`).
Row 0x00CB2EE0 (`SetCurrentPair00cb2ee0`) is rebuilt as `ImeUi_SetScreenDimension` (Ego_r 0x00C94D30, FableWin 0x03581520); its
file name keeps the old descriptive stem (catalog name now follows the Leaf). 0x00CB26B0 is `SendControlKeys` (Ego_r 0x00C92A80,
FableWin 0x0357E630), a `static __declspec(noinline)` fastcall in its caller 0x00CB2710 too.

**Static vs extern is byte evidence, not style:** `g_hklCurrent`, `g_dwCount`, `g_dwSelection`, `g_uCandPageSize`, `g_bCandList` are
file statics in retail. Declaring them `extern` DIFFERs (VC7.1 alias analysis keeps a static in a register across the calls; an
extern is reloaded). VC7.1 spells a file static's relocation undecorated (`_g_hklCurrent`), so that spelling is right and queue
entries 588/595 ("retire `_g_hklCurrent`") are superseded. Recheck: 17/17 respelled rows + the 2 new rows RELOCATION_MATCH/MATCH PASS
(`logs/imeui_respell_recheck.log`, `includer_recheck_grades.tsv`); four tests rewritten to link the source.

## Wave C (GameCODA EAX / SAL): 0 landed, 6 held
0x00C5FFB0, 0x00C60FF0, 0x00C66330, 0x00C66C90, 0x00C5E650, 0x00C71870.
- Codex's `CEAXListener` / `CEAXBuffer` are the classes pass 11 landed as `CEaxContext` / `CEaxSource` (same vtables 0x012B9340 /
  0x012B9344), and `CCodaObjectBase` is pass 11's `CPKObject` (root 0x012B5868). Codex's SDK comparison does support its names
  (`EAX4FXSlot` = 0x00C68F90, `EAX4Source` = 0x00C68510, `CommitEAX3` for 0x00C65A00; 0x00C67F60 is most likely the private
  `CEAXListener::CommitEAX3`, it commits the `m_dwEAX3Changes` bits). But one class model means rewriting the 4 pass 11 rows
  (0x00C67F60, 0x00C68F90, 0x00C65A00, 0x00C68510, ~500 lines on the SDK member model) **and** the ~79 landed rows that bind
  `CEaxContext_` / `CEaxSource_` local models, in one commit. Too big for this landing; queued REVIEW `cx12` (4 addresses).
- GUID spelling decision: **extern "C"** (`_EAX_REVERB_EFFECT`). Evidence: `DEFINE_GUID` expands to `EXTERN_C const GUID`, and the
  GameCODA 2.10 `EAX4Translation.obj` relocates `_EAX_REVERB_EFFECT` (same 2008-byte body as GetEAX4FXSlot). The landed C++ GUID
  spellings (0x00C62FF0, 0x00C67AE0, 0x00C71820, 0x00C56EC0, 0x00C65450, 0x00C64FB0) are queued `cx12` for respell. Merging the 4 GUID
  externs into `engine/EAX40.h` was not done: hdrcheck2 default=DIFF[201], and no landing row needed it.
- 0x00C5E650 adds `CodaMalloc` / `CodaFree` / `__uuidof` GUID second spellings; 0x00C71870 uses C++ `?SAL_EAX30_ListenerProperties@@3U_GUID@@B`
  (contradicts the decision above) and leaves `g_OpenAL` vs `g_AudioContext` open.

## Wave D/E (38 consolidation rows): 15 landed, 23 held
Landed (all NScript except 0x00F29530; leaf = Ego_r + FableWin spelling for each):
0x00CF9610, 0x00D13130 (OnPersist), 0x00D27460, 0x00E247F0, 0x00E377D0, 0x00E39B40, 0x00E92A50, 0x00E9B690, 0x00EE42A0,
0x00EF71E0, 0x00EF7BE0, 0x00EF80A0, 0x00E63890, 0x00E8A2B0, 0x00F29530 (`??1CScriptDef@@UAE@XZ`). Read-only grade on our headers
first (`logs/waveD_readonly_grade.log`), then `--land` (`logs/waveD_land.log`). 25 new headers, 0 changed (`HEADER_NEEDS.md`).

Rebase rules applied (all byte-neutral, proven by the grade):
- `CLevelScriptBase::Game` / `MasterScriptData` are pointers in our model (Codex: references): `Game.` -> `Game->`, `(Game)` -> `(*Game)`.
- `CPersistContext::Transfer` 2-arg calls -> our 3-arg form with an explicit typed default (Codex's `const T& = T()` default arg
  DIFFERs 700v1084 on ours).
- Codex per-feature macros dropped; one class macro each (`FABLE_RETAIL_CCHARSTRING`, `FABLE_RETAIL_CDEFSTRINGTABLE`,
  `FABLE_RETAIL_CPARENTDEFCLASSBASE` = Codex `UI_DEF_DTOR` for 0x00F29530).
- Codex's `fable_hanging_tree.h` prelude is inlined in 0x00E63890 / 0x00E8A2B0 (no second header).

Held (reason per row in `STATUS.tsv`):
- 0x00E0A820 DIFFER 1306v1304 on ours (call-argument push / lea order; the inlined `CScriptThing` prelude schedules differently).
- 0x00E5EBD0 DIFFER 1068v1064 (needs `CCharString::Truncate`; still 4 bytes off with it).
- 0x00DEFB10 DIFFER: one extra `movb $0,0x27(%esp)`; needs a user ctor on STLport `__false_type` in the default path (owner).
- 17 engine rows: 0x00402510, 0x0041D21B, 0x00522A20, 0x0057E2F1, 0x0058E99F, 0x006932B0, 0x00821010, 0x008EDFF0, 0x009BC120,
  0x009CF3A0, 0x00A9BBE0, 0x00AC2BE0, 0x00AE0070, 0x00AE0710, 0x00AE2E20, 0x00AE8D50, 0x00B3BE10 — each needs many members or
  a retail branch our headers do not have (compile sweep counts in `STATUS.tsv`); 0x00AC2BE0 compiles with additive C3DVector /
  C3DBoundingBox branches but DIFFERs 2775v2791 without Codex's second C3DVector ctor model.
- STLport (last, per brief): 0x00520D00 binds `??$_M_find@...` (member template) on 0x004B9370 while landed binders (0x00BEC8D0,
  0x009C43E0, ...) use non-template `?_M_find@`; our `fable_map.h` must pick one first. 0x0051FD80 (Codex `name_holds`
  0x00527D20 / 0x00527D80, 39 modified headers) and 0x00B2D9F0 (21 modified headers) depend on the same configuration.
  **Owner decision needed:** is STLport 4.6 `_Rb_tree::_M_find` a member template in the Fable build (`??$_M_find@`)? If yes, respell
  every landed `?_M_find@` binder in one commit and switch `fable_map.h`; then these 3 rows can be rebased.

## Name fixes (landed rows edited, rechecked RELOCATION_MATCH PASS, `logs/name_fixes_recheck.log`)
- 0x0041B800 `NUISystem::CSprite::CSprite`: base ctor 0x0052CC50 = `??0CChangingStateComponent@NUISystem@@QAE@J@Z` (was `CDraggableInto`).
- 0x004B4AA0 `CQuestManager::ActivateQuestCard`: 0x00415DD0 = `CDefString::GetString` (`?GetString@CDefString@@QBE?AVCCharString@@XZ`).
  Retail stores the by-value result in a named local (`lea ecx,[esp+20h]; push ecx`); a temporary DIFFERs 422v426.
- 0x0051C470 (`__destroy_aux<CMapDesc*>` at 0x0051AC70): NOT respelled. It needs one `CMapDesc` model (Ego_r: `{ set<long> VisibleMaps }`)
  across the whole vector<CMapDesc> chain (0x0051C470, 0x0051C920, 0x0051D030, 0x0051D240, 0x00518F30, 0x00513700); queued `cx12`.

## Naming queue (`rebuild/candidates/manual/naming/NAME_FIXES_QUEUE.tsv`)
- +55 `cx12` PRIORITY rows (Codex second spellings with a PDB entry), +10 more `cx12` rows (g_hklCurrent static, SendControlKeys,
  ImeUi_SetScreenDimension, CChangingStateComponent, CMapDesc chain, EAX class model, EAX GUID extern "C", ilog per-TU,
  `CThing::GetDefName` const (0x004C7CC0), `CV_FisticuffsClubScript::GetQuestText` NScript namespace (0x00E82F80)).
- Closed: cx11 0x0143CD54 / 0x0143CD60 (`g_screenWidth` / `g_screenHeight`), 540 (0x00415DD0), 588 / 595 (superseded), mig9 0x00415D70 annotated.
- Of Codex's 113 "already queued" rows only those two are closed by this landing: the other 111 still have landed binders in the
  wrong spelling (checked against `reloc_bindings.tsv`; a landed Codex row adds the correct spelling, it does not remove the old one).
- New second spellings this landing added to `reloc_bindings.tsv` (`bindings_delta_vs_HEAD.tsv`): 0x004C7CC0 `GetDefName` non-const
  (from the 0x004B4AA0 local model, as before) and 0x00E82F80 `GetQuestText` with `NScript` (correct; the old binder 0x00E82950 lacks it). Both queued.

Labels: `ghidra_out/labels_codex_pass12_2026-11-03.tsv` (41 rows), applied with `coordinator/apply_labels.py`; then
`sync_catalog_names.py`, `sync_manifest_from_catalog.py --labels`, `write_decomp_dashboard.py`, `update_readme_progress.py`.

vland `RELOC_NAME` warnings worth a look (catalog labels propagated across byte-identical bodies, not row errors): 0x00D27C00
labelled `CV_ChapelOfEvilScript::EndMission` but bound as `CQ_EndGameScript::CheckGuildEntering`; 0x006E7AB0, 0x00CDBFB0, 0x00433530
descriptive labels.
