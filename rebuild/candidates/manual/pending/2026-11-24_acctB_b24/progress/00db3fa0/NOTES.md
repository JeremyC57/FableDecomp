# 0x00DB3FA0 CNOVI_BookTrader::Main -- B24B3: RELOCATION_MATCH STAGED, NOT a clean hand-back (payload.json + oracle.tsv written, vland PASS,
#   no WARN/RELOC lines) -- one unfixed pick-any NEQ copy introduced by the device; lander's ruling needed (see Pick-any)
## What made it match (lesson)
- Symptom: a forced predicate-temporary destructor (~CIsThingConscious) expands WITH its nested member dtor (~CScriptThing dec/test/delete)
  where retail expands only the outer vtable store and CALLS ~CScriptThing 0x004AA840 (while ~CScriptThing is expanded at other sites).
  Source form: `#pragma inline_depth(0)` ... `#pragma inline_depth()` round the FABLE_FORCEINLINE ~CIsThingConscious DEFINITION (not round the
  call statement): c2 compiles the body with no nested inlining and the caller inherits the nested call. (Statement-level depth 0 blocks the
  forced dtor and the ctor too; depth 1/2 = no effect.) Inline-budget device per owner 2026-11-08 (preference 3), marked in the source.
- Port: the old opt-in FABLE_RETAIL_CQ_NEWOAKVALEINTROSCRIPT_BOOKTRADER is now ..._ENTITIES3 in the header (SRC_FAIL before).
- Hygiene: local EHeroAbility / ETextGroupSelectionMethod enums -> engine headers; __forceinline -> FABLE_FORCEINLINE; EScriptAIPriority stays
  local (its header is gated by FABLE_RETAIL_PHASE1_ARCHER_PASS26; 381 landed rows define it locally).
## Pick-any (pickany_vs_retail.py on the final object)
- Fixed: ??1CScriptGameResourceObjectMovieBase (LEAF-REFERENCED, 0x006E7B80) was NEQ with the declared-only CP<Movie> dtor specialization
  (copy called 0x006E7AB0); with the visible header template (HEAD row 0x00D44CB0 spelling) it is EQ and the row still RELOCATION_MATCHes.
- OPEN (holds the row under EXTRA_RULES 2026-11-20): NEQ ??1CIsThingConscious@NScript@@UAE@XZ 0x00EF7AB0 (not leaf-referenced). 0x00EF7AB0
  HAS ITS OWN LANDED ROW (rebuild/src/compiled/00/ef/NScriptCIsThingConscious_CIsThingConscious_00ef7ab0.cpp), and the pre-device object
  (p1.cpp) emits an EQ copy (84 b), so the NEW differing copy is caused by the depth-0 device (the out-of-line body compiled under depth 0
  calls ~CScriptThing; retail's expands it). No sanctioned remedy found (the 2026-11-20 size-pragma scaffolding is for optimize pragmas;
  a DECL_ONLY opt-in cannot coexist with the expansion Main needs). Needs the lander's / owner's ruling; held until then.
## B24B4 (2026-11-24): searched for a clean form -- none; row stays HELD (RELOCATION_MATCH staged), ruling evidence for Account A
- Repo history: docs/pipeline/INLINE_DEPTH_REVISIT.md "Still unexplained (the 65)": retail calls ~CScriptThing 0x004AA840 / copy 0x004ABE90
  out of line inside inline-expanded CIsThingAlive / CIsThingConscious special members but expands them elsewhere in 16+ rows; no pragma-free
  form was ever found (/d2inlT55-65 reproduces 9, diagnostic only). Same mechanism here.
- Correction (B24B4, advisor check): Ego_r.pdb puts ~CIsThingConscious in .\Release\register_scripts.obj (index 99, 140 b) with the
  quest scripts, so ONE unity TU produced both forms (standalone copy: ~CScriptThing expanded; Main's expansion: ~CScriptThing called).
  The 'linker kept another object's copy' guess is withdrawn; the split is the unexplained c2 behaviour of 'the 65'.
- Ruling evidence: EXTRA_RULES 2026-11-21 clause "Not leaf-bound and bound at HEAD: debt, queued" -- the copy is not leaf-referenced and HEAD
  binds it (reloc_bindings.tsv: `??1CIsThingConscious@NScript@@UAE@XZ 00ef7ab0 00f1a990`). Counter-text: 2026-11-20 "debt, queued, not licence
  for more". Precedent: landed siblings of the same quest file with the same device emit a NEQ copy of the same symbol at HEAD (q.py rebuild +
  pickany_vs_retail.py, B24B4): 0x00DB5330 CNOVI_BarrelMan::Main NEQ 14 b, 0x00DAF080 CNOVI_TeddyGirl::Main NEQ 14 b (NOVTABLE form);
  ours is NEQ 25 b (calls ~CScriptThing, tail-jumps ~CPredicateBase; retail 0x00EF7AB0 = 84 b, both expanded). All other copies EQ.
- NOT marked ready: held until Account A rules on the clause above. payload.json / oracle.tsv / vland PASS from B24B3 unchanged.
