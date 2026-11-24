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
