# Naming pass, wave 10 (one agent; the chunk 586 authoring agent shares the lane)

Landed: operator=, waves 1-9, LowerBound. Records are in `rebuild/candidates/manual/naming/`; wave 9 is in
`2026-10-29_wave9/`. Since wave 9 the class-model migration (waves 1-5, `rebuild/candidates/manual/headers/`) moved ~4,500
rows onto shared retail headers; several of its NAME_FIXES unblock further migration, so this wave runs before migration
wave 6. Owner decisions still in force:
- Wide text is `unsigned short` (`G`), never `/Zc:wchar_t`; a proposed `_W` spelling lands in the `G` form.
- Names are right the first time: exact PDB spelling, two sources per name, one retail address = one mangled name.
- Retail's calling convention wins over a PDB spelling when they differ (e.g. CMatrix3x4::SetFromVectorAndAngle is QAI in
  retail, QAE in Ego_r): the mangled name must compile to retail's bytes.

Workspace: `$S/naming/w10/` (S=/tmp/claude-0/-home-user-FableDecomp/a013f3ec-d440-5b47-87a1-ed975cb80dfb/scratchpad).
The prepared input is `$S/naming/w10/QUEUE_575_585_mig.tsv` (134 NAME_FIXES rows: chunks 575-585, migration waves mig1-mig5,
one misfiled 520 row; columns chunk, address, correct_symbol, evidence, wrong_spellings_seen).

## Scope, in this order (stop at about 600 edited rows)
1. **The migration-unblocking respellings** (their own sub-waves with their own recheck each):
   - 0x00CD23B9 `?IsValid@CScriptGameResourceObjectScriptedThingBase@@QBE_NXZ` -- 145 landed files spell it `IsScripting`
     (QAE / QBE / `ScriptResource::IsScripting`). No PDB has IsScripting.
   - 0x006E7510 `GetMinWorldFrameForMessages` (protected CGameScriptInterface member; FableWin order gives Min; Max is
     0x006E7530) -- 72 files spell it `GetCurrentWorldFrameForMessages` / `GetWorldFrame_006e7510`.
   - 0x00449970 `CPlayerManager::GetMainPlayer` returning `CPlayer&` (4 reverted `GetPlayer` rows + the `CPlayerManager_Retail`
     stand-in); 0x006E7B80 the MovieBase dtor; 0x007E74D0 / 0x007E72A0 the ScriptedThingBase dtor / ctor; the ScriptedThingBase
     slots' invented QAE spellings -> the PDB UAE ones (mig5 rows in the queue; evidence in
     `rebuild/candidates/manual/headers/2026-10-30_retail_models_wave5/HEADERS.md`).
2. **The rest of the queue:** chunk rows 575-585 (quest-script members landed in the global namespace or without `NScript::`,
   `PlayCutScene` / `FinishMazeStage` / `CRetailVector_*` / `CElement*` inventions, STLport `_M_clear` / `_Destroy` / `IAE`
   access fixes, 0x00411570 Compare and 0x0099E4B0 CCharString ctor junk bindings, 0x004292C0 `_Rb_global<bool>::_M_increment`
   with ~25 wrong spellings, 0x00CB7940 / 0x00CB7E50 / 0x00CB8930 CScriptBase members) and the mig1-4 rows (0x00431F60
   `??4CDefPointeeBase@@QAEAAV0@ABV0@@Z`, 0x004585BD `??1CArmourDef@@UAE@XZ`, 0x00430370 the CParentDefClassBase ctor with
   its 10-name sibling 0x00430300 and 7-name 0x00431ED0, 145 invented non-template `?GetSubDef@CDefClassBase@@QAE..` bindings).
   Check every spelling against both PDB lists yourself; rows marked probable in the queue stay open without a second source.
3. **0x0143E8F8** carries 9 bound names (g_pGameScriptInterface / g_Game / g_pScriptTimers / g_pThingManager* / g_timerMgr /
   g_pScriptTimerManager). There is no PDB data symbol. Decide ONE descriptive spelling from the evidence (what the retail
   code stores there and how the landed rows use it), record the decision with its evidence in DECISIONS_W10.tsv, and
   respell every row to it; if the evidence is split, scope it and DEFER with an exact plan.
4. **Wave 9's open decisions** (`2026-10-29_wave9/DECISIONS_W9.tsv`, 321 `open:` rows): proven-not-reached first; identity
   checks need two sources; probable stays open. Cheap DEFERRED items from waves 5-9 if budget remains (the mega families
   GetPItem 873 / ~CIntelligentPointer 676 / SetPItem 227 / LowerBound ~250 / CCharString dtor stand-ins 192 are NOT in this
   wave's budget: scope them only).

## Rules and hand-back
- Follow `briefs/BRIEF_W7.md` .. `BRIEF_W9.md`: idempotent scripts plus verified file lists; a composed overlay
  (`ov_final/rebuild/...`) re-made from the CURRENT repo before the final recheck; BINDINGS.tsv (action/symbol/target/by_row/
  note); LABELS.tsv, proven names only; DEFERRED.tsv, RENAMES.tsv (git-mv proposals with the target path; the coordinator
  moves) and DECISIONS_W10.tsv; recheck logs.
- A respelling that changes a row's leaf symbol must keep `// Leaf:` and the header comment in step; a respelling that
  changes a relocation symbol must be mirrored in BINDINGS.tsv (RETIRE the wrong spelling, ADD the right one, same target).
- **Recheck:** every changed landed row must stay MATCH/RELOCATION_MATCH + PASS under
  `tools/decomp_pipeline/linux_lane/lane_tools/vland.py --recheck-landed`, with `LANDVERIFY_WORK=$S/naming/w10/lv` (delete
  lv dirs after each batch; ~9 GB free). The payload builder is `tools/decomp_pipeline/linux_lane/lane_tools/coordinator/
  mk_recheck_payload.py` (compares against HEAD, so run it on your overlay copy); `recheck.py <dir> <addrs>` also works
  (mkdir the dir first). `bindings_multi_check.py` on your final bindings must print 0.
- **Where you may write:** work only in `$S/naming/w10/`. Never run git. Never write under /home/user/FableDecomp.
- **Processes:** never kill a process; ask the coordinator. **Searching:** no recursive find/glob over `/` or the scratchpad.
- **Commands:** `timeout` on every compile; never repeat a failing command more than twice with the same arguments.
- **Wrap-up:** leave no background job running.
- **Report** (your final message): rows edited per sub-wave, bindings changed, labels, the 0x0143E8F8 decision, deferred
  items with reasons, recheck totals.
