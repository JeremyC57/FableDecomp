# Naming pass, wave 13 (one agent; one chunk-authoring agent, agent651, shares the lane)

Purpose: apply the 2026-11-01 owner decisions (`rebuild/candidates/manual/naming/2026-10-31_wave12/DECISIONS_W12_RESOLVED.tsv`,
canonical globals in `rebuild/candidates/manual/naming/ESTABLISHED_SPELLINGS.tsv`) family by family, then the queue rows
c643-c650 + codex50 and the wave 12 DEFERRED.tsv items that are PDB-proven. Wave 12 records (`naming/2026-10-31_wave12/README.md`,
`DEFERRED.tsv`) and the rules in `briefs/BRIEF_W10.md`, `BRIEF_W11.md`, `BRIEF_W12.md` are unchanged and apply. The plan text is
in `docs/journal/2026-10/2026-10-31_HANDOFF.md` "Wave 13 plan" (read it first).

Workspace: `$S/naming/w13/` (S is given in the agent prompt). `scripts/` is a copy of the wave 12 scripts: adapt them (w12 -> w13
in recheck.sh / recheck_fx.sh / compose12.py -> compose13.py, QUEUE_W12 -> QUEUE_W13 in inputs.py), build the baseline index with
`mkindex.py $S/naming/w13/idx/full.jsonl rebuild/src/compiled` (compile all landed rows once, in batches), and write QUEUE_W13.tsv
(columns chunk, address, correct_symbol, evidence, wrong_spellings_seen) from `naming/NAME_FIXES_QUEUE.tsv` rows whose chunk column
is c643..c650 or codex50, plus one row per DECISIONS_W12_RESOLVED item. Reuse famlib.run + fam_free / fam_base / fam_decl / fam_auto;
famrep.py lists a target's spellings; mkbind12.py (-> mkbind13.py) writes BINDINGS.tsv; recheck.sh rechecks a row list from an overlay.

## Scope, in this order (every family: edit the declaration and uses, replace the binding rows, recheck, record)
1. GGame 0x013B83D0 + GSystemManager 0x013CA618 (4 rows; respell 0x01223FF0 g_CStatic_... too per the decisions file).
2. TWO_PI 0x0124F2B8 (3 rows: 0x00A0C450 defines `const double g_twoPi` locally -> rename; 0x00853830 declares `Scale` inside
   `namespace NCameraModes` -> move to global scope as `extern const double TWO_PI;`).
3. CTCBase::GetThingSearchTools / PeekThingSearchTools 0x00686D80/90 (6 rows): a `class CTCBase { protected: ... }` base model (or the
   shared engine/CTCBase.h when it carries the slot) and `PeekThingSearchTools().X(`; 0x005BDC77's free function -> the non-const member.
4. CDefString 0x00415D70 / 0x00415DD0 (operator CCharString / GetString) and LowerBound 0x0040F020 / 0x004365B0 (non-const / const)
   bindings: every caller binds the right one of the pair.
5. PSoundManager 0x013B8394 (24 rows) + PDisplayManager 0x013B8390 / PInputManager 0x013B8388 / PGraphicBankManager 0x013B837C /
   PFontManager 0x013B838C (~20 rows): `class CASoundManager; extern CASoundManager* PSoundManager;` etc.; where a local class model
   is also the type of bound non-virtual callees (0x0088F620 CSoundMgr), keep the callee model and derive/static_cast (no byte change).
6. EPSILON 0x0129BA3C (49 rows): `extern const float EPSILON;` (non-const `@@3MA` rows become the `@@3MB` spelling).
7. SmoothFadeValue: 0x00AF9000 -> `?GFSmoothFadeValue@NParticleEngine@@YIMM@Z` (RENAMES.tsv: git mv of source + test, the coordinator
   moves; must stay byte-exact), 0x00B05E10 CPSCSingleSprite::SmoothFadeValue IBE, 0x00B1AD90 CPSCLight::SmoothFadeValue QBE.
8. IS_THING_ALIVE 0x006961F0 / 0x0069CE20: read 0x0097A630's argument type first, then the 3 callers bind the right instantiation.
9. PKCDeferredBank for vtable 0x012B29F8 (5 libpk rows) -- last of the decided families.
10. Anonymous-namespace rewrite (214 bindings, 18 rows): bind under the exact PDB spelling with the retail `?A0x<hash>`; confirm
    famlib.hashy's tolerance on the live reloc_bindings table first (a dry run that compiles one row and inspects the pair).
11. 0x00C351B0 Start callers: only if that row lands during this wave (check the catalog at the end); otherwise DEFERRED.
12. Then QUEUE rows c643-c650 + codex50 (~90, PDB-proven callee/leaf fixes only) and the PDB-proven items of wave 12 DEFERRED.tsv.
    Still-open owner items -- scope only in DECISIONS_W13.tsv with evidence and a recommendation, never respell: GetPWorld 0x008FCFC0
    (~45 spellings), VIRTUAL_SCREEN_* (20; VIRTUAL_SCREEN_WIDTH 0x013961E8 is established), vector<CCharString> dtor 0x00414EA0 (17),
    GetCurrentWorldFrame 0x004167C8 (8). Decide them the way DECISIONS_W12_RESOLVED did: callers first, PDB initializer list, layouts.

## Rules and hand-back (as BRIEF_W7..W12)
- All name-only: no byte change anywhere. Idempotent scripts plus verified row lists; a composed overlay (`ov_final/rebuild/...`)
  re-made from the CURRENT repo before the final recheck (the chunk agent's rows land every so often: re-copy sources right before
  each family's edit and again before the final recheck). BINDINGS.tsv (action/symbol/target/by_row/note), LABELS.tsv (proven names
  only), DEFERRED.tsv, RENAMES.tsv, DECISIONS_W13.tsv, recheck logs under logs/.
- A respelling that changes a row's leaf keeps `// Leaf:` and the header comment in step; one that changes a relocation symbol is
  mirrored in BINDINGS.tsv (RETIRE the wrong spelling, ADD the right one, same target). Full caller sets come from the relocation
  pairs of the compiled candidates (the index), not from reloc_bindings.tsv alone.
- Shared headers: prefer `rebuild/include/engine/<Class>.h` (the `#define Class ClassLayout` include pattern is accepted) over a
  local model unless the header flattens the vptr; say so.
- Purity: never introduce `__asm`, `_emit`, `naked`, `volatile`, `#pragma pack`; a pre-existing `#pragma pack` is left and listed
  in DEFERRED.tsv unless removing it keeps the bytes.
- Recheck: every changed landed row MATCH/RELOCATION_MATCH + PASS under `vland.py --recheck-landed`,
  `LANDVERIFY_WORK=$S/naming/w13/lv_*` (delete after each family), `QTAG=nw13_<family>` on every compile. `asm_bake`, `retail_asm`,
  `toolchain_library` rows are not compiled: names/catalog only. `bindings_multi_check.py` on the final bindings must print 0.
- Work only in `$S/naming/w13/`. Never run git. Never write under /home/user/FableDecomp. Never kill a process. Never touch
  `$S/w22/agent651*`. No recursive find/glob over `/` or the scratchpad. `timeout` on every compile; never repeat a failing command
  more than twice. Leave no background job running.
- Report (final message): rows edited per family, bindings changed, labels, renames, decisions scoped, deferred items with reasons,
  recheck totals, and the exact overlay path + file list for the coordinator to copy; one commit per family is the coordinator's job,
  so keep per-family row lists (`runs/<family>.rows.txt`) and per-family overlay snapshots (`ov_<family>/`) in step with ov_final.
