# NF463 (2026-11-26): CModeStunned unit, particle factory debt, NF460 queue lines (MixFade, GetAttackerPlacementVector)

Scratch: `$S/w22/agentNF463`. Tools: NF462's (`tools/`, paths and lock owner moved to NF463) plus `census861.py` (retail call sites of a target ->
owner row, lock, source spelling by regex), `sandbox.sh` (fresh repo oracle copies -> apply_table --dry + apply per batch, SELF_SYMBOLS, clash,
descriptive_check, md5 of the repo oracles before and after), `mk_bS.py`. Header overlay: `hdr/engine/` (CPSCDecalRenderer.h, CSalRenderDisable.h,
CSalRenderPan.h; each batch also carries its own `hdr/engine/` and `orig/hdr_*.h`). HEAD 085b7e4d7b (FullDecomp, read from .git/refs), unchanged
during the run. No git; nothing written under the repo; repo oracle md5 identical before and after every sandbox run (evidence/sandbox_all.txt).

## Batches (NF462 layout: rows/, orig/ = exact HEAD copies incl. tests and hdr_*.h, hdr/engine/, head/payload.json, RESULTS, SELF_SYMBOLS,
## UNITS, CHECKS, TABLE_CHANGES, HEADERS, renames.tsv, evidence/)
| batch | rows | what |
|---|---:|---|
| bS_cmodestunned_unit | 3 | queue 13758 (nf455 identity 0x00740160). Ctor 0x00740160 respelt by parameter only to `??0CModeStunned@NCreatureMode@@QAE@AAVCThing@@@Z` (PDB members named; local model, one-slot vftables and the CModeBase_00861e00 base stand-in kept: r463b). Binder 0x008ED950: `CModeDazed(CThing*)` token -> `NCreatureMode::CModeStunned(CThing&)` on local CModeStunned : CModeOutOfActionBase deriving from engine/CModeStandBase.h (typed manager view kept for AddMode); +0x6c store = CModeOutOfActionBase::Recoverable. Factory 0x007427F0 onto `?Alloc@CModeStunned@NCreatureMode@@SIPAVCModeBase@CTCCreatureModeManager@@AAVCThing@@@Z` (bM pattern). TABLE 1 replace + 1 delete. |
| bP_psc_alloc_debt | 3 | NF462 resume item 3. 0x00AF7D80 CPSCRenderMesh::Alloc onto its PDB self, local class on engine/CPSCBase.h (r462a), ctor callee `??0CPSCRenderMesh@NParticleEngine@@QAE@XZ`. 0x00AF8100 CPSCDecalRenderer::Alloc + 0x00B1FF80 DatabaseClone (PDB virtual `UBEPAVCPSCBase@2@XZ`, was the unbound stand-in GetDrawableObject_00af8100) on engine/CPSCDecalRenderer.h + new add-only `FABLE_RETAIL_CPSCDECALRENDERER_ALLOC`. TABLE 2 replace. 0x00B0A190 needed nothing: already on engine/CPSCSingleSprite.h with PDB spellings at HEAD (Account B, 2026-11-20); note that self_symbols.tsv has no line for 0x00B0A190 or 0x00AF7DF0. |
| bF_salrender_mixfade | 4 | queue 13754-13757 (nf452). The four mixers onto the GameCODA SDK private statics `?MixFadeIn/MixFadeOut@CSalRenderDisable/CSalRenderPan@@CAXPBFPBUFilterPan@@PAH@Z` via add-only `FABLE_RETAIL_CSALRENDERDISABLE_MIXFADE` / `FABLE_RETAIL_CSALRENDERPAN_MIXFADE` (declarations, private section). Ends 2 HEAD duplicate definitions (?MixGainRampUp / ?MixGainRampDown defined by both twins). No table lines exist. CATALOG_CHANGES.tsv relabels the 4 rows (Module _global -> class, Name -> SDK name). |
| bQ_queue_single_binder | 1 | queue 13787 (nf458 0x006E89B0): binder 0x006EA770 `ComputeAttackVector` -> `GetAttackerPlacementVector` (same class / params / const). TABLE 1 replace. |

## Gates (all 11 rows)
- vland --recheck-landed HEAD v new at one LANDVERIFY_WORK path (runpair.sh / runpair_hdr.sh with the overlay on the new side): grade unchanged
  (RELOCATION_MATCH x7, MATCH x4), tests PASS both sides.
- WARN sets: bS identical except 0x008ED950 smaller (RELOC_NAME gone); bP 3 -> 0; bQ 11 -> 10 (RELOC_NAME gone); bF 0 -> 1 per row, STANDIN_LEAF
  "catalog _global::MixGainRamp*; names another function": a catalog-label artifact (the nf424 precedent): standin_leaf.standin() returns None
  under the CATALOG_CHANGES.tsv labels (evidence/standin_leaf_catalog.txt), and vland on the new rows with the relabelled oracle names prints
  no WARN (MATCH PASS x4, evidence/vland_bFn_relabelled.log). The relabel lands with the batch: RULINGS r463c.
- objproof (tools/objproof_map.py, on cc.py objects built with the row's flags and the overlay): ALL_EQ under exactly the renames in each
  renames.tsv; DEF/UND symbol diff shows only those renames; .drectve byte-identical everywhere (no new /DEFAULTLIB).
- link HEAD v new per unit (lk2.py, + HEAD objects of the callee / registry rows): 0 new duplicates (bF 2 -> 0); unresolved bS 110 -> 107,
  bP 52 -> 47, bQ 13 -> 12, bF 0 -> 0; none newly unresolved.
- sandbox (all four batches cumulatively, fresh repo copies): apply_table --dry 0 misses, applied, multi-target 1 -> 1; SELF_SYMBOLS 9 applied;
  clash 0; descriptive_check 2,026 lines 0 FAIL.
- Headers: g++ -E -P HEAD v new IDENTICAL under 3 macro sets (CPSCDecalRenderer) / 2 each + both together (Sal headers); the ALLOC #error guard
  fires; the Sal opt-ins rely on the header's existing macro guard. All asserts unchanged (CPSCDecalRenderer `_AT(64)` pinned; Sal size checks
  are typedefs). Coordinator runs the equiv check.
- Drift: every orig/ file equals the repo working tree at the end. No __asm / volatile. No new duplicate definitions: no other source defines
  the CModeStunned (CThing&) ctor or CModeStunned::Alloc, CPSCRenderMesh::Alloc, CPSCDecalRenderer::Alloc / DatabaseClone (grep of
  rebuild/src/compiled; RegisterPSCs 0x00AF8170 only takes their addresses), or the SDK mixer names.
- Names: Ego_r + FableWin for bS / bP / bQ. The four mixers are one-source (GameCODA SDK CodeView, the queue lines' evidence): bF/BEST_GUESS.tsv
  adds four descriptive_names lines in the landed CSalRender siblings' form (one-source:gamecoda-sdk); sandbox descriptive_check with them
  2,030 lines 0 FAIL.
- Pick-any: the ctor row 0x00740160 still emits the inline base ctor COMDAT `??0CModeOutOfActionBase@NCreatureMode@@QAE@PAVCThing@@@Z` as at HEAD
  (same spelling as the landed CModeDazed row 0x00740250). I first respelt it to (CThing&) too, which created a new COMDAT under the PDB name with
  no table line, and reverted that (objproof then shows exactly one rename).

## Holds / findings
- **0x00861E00 family (task 2): HOLD, RULINGS r463a.** Census (work/census861.tsv, source grep over the 85 retail call sites, not the object census):
  40 rows spell `??0CBase_00861e00`, 8 `??0CModeBase_00861e00` (incl. 0x00740160), 22 a local `CModeStandBase(CThing*)` (= the self's PAV spelling;
  one is 0x00740340 under COORD_HOLD_NF455), 7 include engine/CModeStandBase.h (PDB AAV), 3 (0x008DB030/0E0/190) already spell AAV through local
  models, 4 sites lie in no row. Blockers: the NF455 lock on 0x00740340, and the ctor rows' one-slot local vftables (header port = r459a; probe
  evidence/r463a_vftable_probe.txt: ??_7CModeStandHappy 4 -> 8 bytes plus a ??_G COMDAT).
- **0x00B93290 EnumerateScene:** not built; needs an EnumerateScene opt-in on engine/CEngineScene.h and a CEngineSceneEnumerator base under the
  enumerator that 0x00B320A0 builds inline (its vftable is emitted there).
- **WatchFor* (0x00D17090 / 0x00E39140 / 0x00F08590):** not built; every binder models CScriptThing locally (CScriptThingRuntime) and emits the
  CSpawnedFunc1<script, CScriptThing> instantiation, so the respell is a CScriptThing-header port of 4 quest rows (one 876 lines), not a rename.

## Resume (smallest first)
1. Rulings r463a / r463b; with them, the 0x00861E00 family (self + ~75 caller rows; start from work/census861.tsv, then confirm each caller's
   HEAD object spelling).
2. 0x00B93290 (2 binders + a CEngineScene.h opt-in), then the WatchFor* trio on engine/CScriptThing.h.
3. NF460 list items 2-4 unchanged: 0x0099A2D0 family (3 rows), 0x0076C830 for 0x00686870, 0x00686830 (8 rows), 0x004ABE90, the self-change lines
   (0x009FC210, 0x0064CC65, 0x0097DA40, 0x009D6490), then the large families.

## Locks (NF463)
Changed rows (11): 00740160 008ed950 007427f0 00af7d80 00af8100 00b1ff80 006ea770 00c6e790 00c6e820 00c6da40 00c6dad0.
Held unchanged (unit callees): 00b012e0 00b1dfd0 006e89b0. Not taken: 00740340 (COORD_HOLD_NF455), the 0x00861E00 caller rows.
