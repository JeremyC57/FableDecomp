# Naming pass, wave 8 (one agent; a chunk-authoring agent shares the lane)

Landed: operator=, waves 1-6, and wave 7 (applied in the repo WORKING TREE, being committed). Also landed: ChatGPT pass 9
(157 rows, rebuild/candidates/manual/batches/2026-10-29_chatgpt_pass9/). Records: `rebuild/candidates/manual/naming/`.
Owner decision (2026-10-29): wide text is `unsigned short` (`G`); never `/Zc:wchar_t` -- a proposed `_W` spelling lands
in the `G` form.

## Scope, in this order (stop at ~500 edited rows)
1. Three RELOC_CONFLICTs pass 9 exposed (the pass 9 side is right; fix the older landed rows and say which binding
   changes): landed 0072cba0 binds `??0?$CIntelligentPointer@$$CBVCThing@@@@QAE@PBVCThing@@@Z` to 0x004A96F0 (that is
   `CIntelligentPointer<CThing>`; the const-CThing ctor is 0x0056C860); CBankFileAsyncData: 0x009D5530 is GetData,
   0x009D5580 the const PeekData (Ego_r / FableWin declare Get then Peek) -- the landed defining rows 009d5530 / 009d5580
   and caller 00be04c0 have them swapped (catalog names too); landed 006821e0 binds
   `?resize@?$vector@_NV?$allocator@_N@std@@@std@@QAEXIAB_N@Z` to 0x006860F0 (`_M_fill_assign`; resize is 0x00467686).
   Verify each from evidence before editing.
2. `naming/w8/QUEUE_564_565.tsv` (chunks 564-565 NAME_FIXES) and `naming/w8/GPT_PASS9_NAME_FIXES.tsv` (465 proposals from
   the external pass, columns address / correct_symbol / evidence / wrong_spellings_seen; `GPT_PASS9_NAME_FIX_CONFLICTS.tsv`
   holds 25 it could not settle): merge into DECISIONS as earlier waves did, check every spelling against the PDB name
   lists yourself (the external evidence is a lead, not proof), apply the proven ones, `_W` in `G` form.
3. `naming/w7/DEFERRED.tsv` cheap items (CDefinitionManager const family first if it unlocks the GetDefinitionManager
   rows; GPreviousSelected; g_pPlayerGui only if one spelling is provable).

## Rules and hand-back
As `naming/BRIEF_W7.md` (idempotent scripts + verified file lists, a composed overlay re-made from the CURRENT repo
before the final recheck, BINDINGS.tsv action/symbol/target/by_row/note, LABELS.tsv proven only, DEFERRED.tsv,
RENAMES.tsv, recheck logs; final message = report). Work only in `naming/w8/`. Never run git; never write under
/home/user/FableDecomp; never kill a process (ask the coordinator); no recursive find/glob over `/` or the whole
scratchpad; `timeout` on every compile; leave no background job running when you report.
