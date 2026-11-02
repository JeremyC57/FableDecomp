# Naming pass wave 30 (2026-11-02): 46 landed rows edited, no shared header; 46/46 rechecked (1 MATCH + 45 RELOCATION_MATCH, all PASS)

## Inputs and index
- Brief naming/briefs/BRIEF_W30.md; QUEUE_W30_input.tsv (122 rows: 73 carried, 24 c700 and 25 c701 appended mid-wave); DEFERRED_W29_INPUT.tsv.
- idx/full.jsonl is HEAD: the wave 29 after.jsonl (the wave 29 ov_final was verified identical to the repo, renames included) plus chunk 700
  (idx/delta_c700.jsonl, 10 rows) and chunk 701 (idx/delta_c701.jsonl, 10 rows), found by modification time (no git). No c700 / c701 row is
  edited by this wave. idx/after.jsonl is HEAD plus ov_final. Before hand-back nothing under rebuild/src, tests, include is newer than
  idx/.c701_marker except the c701 landing itself.

## Commands (from $S/naming/w30, S exported, $S/wine/env.sh sourced)
1. `python3 scripts/compose30.py ov_final` runs scripts/hand30/h30_<fam>.py on fresh repo copies; writes runs/<fam>.rows.txt and ov_<fam>/.
   (h30_csm.py reads scripts/hand30/csm_leaf.tsv: each re-authored row's own symbol, taken from its compiled object, for the Leaf line.)
2. `bash scripts/chain30.sh` (SKIP_RK=1 reuses the recheck): recheck of runs/final_plus.rows.txt (= the 46 family rows; runs/hdrdep.rows.txt is
   empty: no shared header edited), mkindex, after.jsonl, qscan30, mkq30 (+ mkq30_notes), mkdef30 (+ runs/deferred30_manual.tsv), mkbind30
   (+ runs/reloc_bindings.after.tsv), bindings_multi_check (0), mkren30 + fixren30, mklabels30 + fixlab30.

## Families (compose order; runs/<fam>.rows.txt)
- **cst (15)** -- queue row 23 (~CScriptThing 0x004AA840): engine/CScriptThing.h's retail branch replaces the local non-virtual stand-ins
  (??1CScriptThing@@QAE@XZ: 00cbeed2 00cbef30 00cbefc1 00cbf9de 00d7ae80 00d81820 00dbde40 00dbf2d0 00df36e0 00eeb7b0), the derived second views
  over the flattened header (ScriptThingCopy 00d2c530 00e11bb0, NScript::ScriptThing 00d9c4a0, Row00e15500::Handle 00e15500) and
  Row00e1e530::Handle (00e1e530).
- **csm (18)** -- queue row 23: the CTCAIScratchpad / script-API rows of the repeated body shape (008a23b0 .. 008a77f0, 008976f0, 0089faa0)
  re-authored as the landed sibling 0x008A1E50: the ScriptMethods projection is the header's CScriptThing (copy / dtor / GetPThing defined
  inline after the include, as the projection had them), CThingNative is CThing (CThing::GetScriptThing const, both PDBs), and the own row is
  the retail virtual ?X@CGameScriptInterface@@U... (was the invented CGameScriptInterfaceMethods). RENAMES 1 (0089faa0).
- **c699 (7)** -- 0x0051ED60 ?PeekPThingWithUID@CThingManager@@QBEPBVCThing@@_K@Z (was "CStopWatch::GetTicks") + binders 0x0057ABD6 / 0x0057AE53
  (const GetThingManager() -> const CThingManager&); 0x006868E0 ?OnDie@CTCBase@@UAEXXZ (was C006868e0) + binders 0x00713740
  ?OnDie@CTCScriptedControl@@UAEXXZ (was vfunc_13 / "OnRemove") and 0x007F0890 ?OnDie@CTCKrakenTentacle@@UAEXXZ (was Dest_val over Dtor_first /
  Dtor_node; test rewritten); 0x00A9D3A0 ??1C3DMeshFilePrimitiveChunk@@UAE@XZ (was "CTCInventoryItem::OnDie"; sibling 0x00A9D3C0 model; test
  rewritten, still prints the catalog pattern ONDIE_OK). RENAMES 3.
- **stl (4)** -- the byte-identical SortTreeRecursively_00436900 stand-ins are STLport ~_List_base<T>: 0x0052D5C0 (EChangingStateComponentState),
  0x00B43BB0 (CEngineMap*), 0x00B09E40 (CEnginePrimitiveSpriteTrail::CSpriteTrailEntry); the ??_G 0x0052D770 binds its list dtor by that name
  (was ??1CStateList@NUISystem). RENAMES 3.
- **lhn (2)** -- 0x00569C30 ??1LHHttpHeaders@LHNetworkLib@@QAE@XZ (was CBlockVectorOwner_00569c30) and 0x00569940 STLport ~vector<std::string>
  (was CRetailVector_00569940::Destroy...). RENAMES 2.

## Not done (DEFERRED.tsv / DECISIONS_W30.tsv)
- 32 ~CScriptThing binders (runs/deferred30_manual.tsv by reason: 17 two-view rows -- D1; 4 setters needing the CCharString copy ctor in
  fable_string.h -- D2; 5 CThing-derived / vector rows; 6 quest-script ScriptMethods rows).
- ~CTCBase binders, the rest of the c697-c699 families, the libpk spelling (needs the coordinator's choice), all c700 / c701 rows, scope 2
  (DECISIONS_W29 D2 / D6 / D3) and scope 3: not reached.

## Outputs
- QUEUE_W30.tsv: 122 rows -- 8 respelled, 8 respelled-partial (3 this wave + 5 carried), 106 deferred. DEFERRED.tsv: 104 queue + 5 wave-30
  findings + 587 carried (8 wave-29 entries resolved: runs/w29_deferred_resolved.tsv; holds never resolved by the index test).
- BINDINGS.tsv: 18 RETIRE, 6 ADD, 7 KEEP, 0 CONFLICT; runs/reloc_bindings.after.tsv; multi-binding check 0. RENAMES.tsv 9. LABELS.tsv 3.
- DECISIONS_W30.tsv: 7 items. runs/hdrdep.rows.txt: empty (no shared header edited).
