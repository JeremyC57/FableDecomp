# Retail class-model migration, wave 7 (2026-10-30): 1,667 landed rows onto shared retail headers, bytes unchanged

Brief BRIEF.md; per-row ROWS.tsv (addr, file, families, change, mcheck, regrade); REVERTED.tsv (123 family edits, each
reverted from its own pre-image so a row's other passing edits stay); HEADERS.md; NEXT.md (wave 8 counts and plan, the
CScriptThing shape breakdown); LESSONS.md; NAME_FIXES.tsv; bind_add.tsv (empty); logs/ (recheck1..5.log, mcheck and apply
logs); scripts/.

Headers changed (no new class headers): `engine/CLevelScriptBase.h` and `engine/CEntityScriptBase.h` each turn a
declared-only constructor into the inline constructor retail expands. Evidence: every level-script ctor and Alloc runs
`call 0x00CB8110` (CScriptBase()) then stores +0x40 Game and +0x44 MasterScriptData directly (0x00D61C30, the 0x00EED220
Alloc family); every entity Alloc stores the 0x012C3224 vtable and Game, then calls the out-of-line CScriptThing copy
0x004ABE90 (0x00F1B800). Ego_r.pdb (release) has neither ctor out of line; FableWin.pdb (debug) has both. All 55 unchanged
includers compile to the same bytes. The five unreferenced out-of-line CLevelScriptBase<T> ctors 0x00CDD160..0x00CDD220
look like an explicit instantiation but have no caller: `// unproven`, their 5 rows keep local models.
Rows per family (a row can carry more than one): CScriptThing 1,462 (local model -> engine/CScriptThing.h; implicit copy /
dtor / operator= defined inline after the include where the row expands them), CEntityScriptBase 829 (entity classes now
declare the PDB `Main` / `GetParentScript` overrides the pure header slots need), non-template CLevelScriptBase ->
`CLevelScriptBase<T>` 180 (T recovered from retail CScriptInfoManager::RegisterScripts 0x00CD52D0: each script's master-data
Alloc stores one of the five data vtables 0x012C2718..0x012C2748, in the order of the five bound CLevelScriptBase<T> vtables
0x012C2EEC..0x012C2F3C), CScriptBase full-member / CBaseClassNonCopyable 88, template inline-ctor 26, 0x48 / methods-only
CScriptBase 24, `CGameScriptInterface* Game` -> `CGameScriptInterfaceBase*` 46, CIsThingAlive / CIsThingConscious 8.
Agent recheck 1,667/1,667 PASS (1,562 RELOCATION_MATCH, 105 MATCH), every row keeps its HEAD grade; payload_check 0 stale.
Five rows differ only in their own leaf spelling (DoEntityScriptPersist / DoScriptDataPersist QAE -> IAE, three entity Main
QAE -> UAE). Coordinator: clean cherry-pick onto 9b184de90; respelled the two persist callers (00cb7ee0, 00cb8690) to the
protected IAE spelling and rebound 0x00CB7DA0 / 0x00CB7CB0 (4/4 PASS, spot2/); independent spot regrade
183/183 PASS (153 edited rows across all 8 families + 30 unedited includers of the two changed headers; coordinator_spot_recheck.log); multi-check 0. The three Main rows are queued for naming wave 11
(NAME_FIXES_QUEUE.tsv, prefix mig7: nest the entity classes in their NScript script classes).
Reverted 123 family edits: CEntityScriptBase 92 (blocks inside a namespace; derived classes that model the base's vptr /
Game / Me words themselves), CScriptThing 24 (std vector<CScriptThing> helpers 0x008AC2A0 / 0x008AC140 / 0x008AD5F0 that
call the copy out of line, 0x00AF69D0, vector/list and enum-typedef clashes), 0x48 stand-ins whose rows construct the base 7.
Open: MovieBase 85 rows still unexplained (LESSONS 6: every CCountedPointer model inlines the release, 385 v 349;
inline_depth(1) overshoots to 347; retail inlines the same release inside the ScriptedThingBase dtor expansion and calls
0x006E7AB0 inside the MovieBase one; lead: Ego_r's out-of-line `??1CScriptGameResourceObjectMovieBase` 0x007E2EA0);
master-data T conflicts in 6 landed template rows (byte-neutral respellings, NEXT.md item 3); not reached: the
CGameScriptInterface remainder (220), CPlayerManager (487), CTimer (201), wave 4 leftovers.
Remaining local models: CScriptThing 1,045, CEntityScriptBase 466, CScriptBase 93, CIsThingConscious 40, CLevelScriptBase 6.
Bindings: no relocation symbol changed by the wave; the coordinator's two IAE respellings replaced the QAE lines.
