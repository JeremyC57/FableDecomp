# NF178 header overlay (2026-11-20)

## stlport/fable_map.h
- Base: the repo working tree copy at the start of this pass (`orig/stlport/fable_map.h`, byte-identical to rebuild/include/stlport/fable_map.h).
- One opt-in, add-only, member-specific: `FABLE_RETAIL_STLP_MAP_CLEAR_DECL_ONLY` declares `void clear();` (declaration only) in
  place of the inline `void clear(){_M_t.clear();}`. User: row 0x00E3E720 (CV_BordelloScript::PlayCutscene), whose `csargs.clear()` retail
  calls out of line at 0x009AACE0 (`?clear@?$map@VCCharString@@V1@...@QAEXXZ`, the spelling landed 0x00D7C470 already binds).
- Default path (macro undefined) is token-identical to the base: the `#else` branch is the original line, checked by stripping the
  block and comparing the files (equal).
- pin_asserts.py: `stlport/fable_map.h pinned 0` (the header's only assert is `FABLE_STATIC_ASSERT_AT(699, ...)`, explicit line, untouched).
- Header users: 923 landed rows under rebuild/src/compiled include fable_map.h directly (more through 136 headers); none defines
  FABLE_RETAIL_STLP_MAP_CLEAR_DECL_ONLY (grep of rebuild/), so the opt-in changes none of them.
- Sibling overlays read first: NF176 (engine/CMeshDataBank.h), NF177 (engine/CBankFile.h, fable_retail_banks.h), NF175 (engine/CCreature*,
  CCombatActionBase.h, CScriptedMapBrush.h): none touches stlport/fable_map.h.

# NF178b follow-up (2026-11-20): two more declaration-only opt-ins in stlport/fable_map.h

Same overlay file, base still orig/stlport/fable_map.h (= repo working tree copy). Both add-only, member-specific, in the existing
`FABLE_RETAIL_STLP_TREE_*` family. Why: EXTRA_RULES 2026-11-20 (pick-any copies of functions that have their own landed row). VC7.1
instantiates ~_Rb_tree (and through it clear and _M_erase) from the map constructor and emits all three as pick-any COMDATs even when
nothing calls them (only .debug$F refers to them). In the NF178 objects that gave `?_M_erase@?$_Rb_tree<CCharString,...>` (0x00CD344A,
70 / 125 / 59 bytes v retail 123) and `?clear@?$_Rb_tree<CCharString,...>` (0x00CD3699, 51 bytes v retail 41, q.py DIFFER(51v41);
retail's clear is a /Os body) under retail names with non-retail bodies.

- `FABLE_RETAIL_STLP_TREE_MERASE_DECL_ONLY`: `#ifndef` guard around the whole out-of-line `_Rb_tree<...>::_M_erase` template definition
  block (both its PASS22 and default branches), with a 3-line comment inside the guard. The in-class declaration is unchanged, so with
  the macro defined `_M_erase` is declared but has no body in the TU. (MERASE_NOINLINE keeps the body visible and still emits the
  COMDAT, so it did not fit.) Defined by all 34 NF178 rows (30 in rows/, 4 in held/).
- `FABLE_RETAIL_STLP_TREE_DTOR_DECL_ONLY`: `#ifdef ... ~_Rb_tree(); #else <the original CTOR_DTOR_OUTOFLINE block> #endif` around the
  tree destructor. With it the tree destructor is declared only, so neither it nor clear nor _M_erase is instantiated. Only usable in a
  TU that already takes `FABLE_RETAIL_STLP_MAP_DTOR_DECL_ONLY` (the leaf calls ~map 0x00CDBFB0; nothing inlines the tree destructor).
  Defined by the 30 rows in rows/. Not usable in the 4 held rows (they leave MAP_DTOR_DECL_ONLY out, and their emitted implicit ~map
  COMDAT, RELOCATION_MATCH v 0x00CDBFB0, inlines the tree destructor and clear): those are held.
- The NF178 opt-in `FABLE_RETAIL_STLP_MAP_CLEAR_DECL_ONLY` stays; its only user, 0x00E3E720, is now in held/.
- Default path token-identical: tools/defpath.py strips the three opt-in blocks (keeping each default branch) and compares with
  orig/stlport/fable_map.h: equal (1051 lines).
- pin_asserts.py (`... rebuild/include agentNF178b/hdr stlport/fable_map.h`): `pinned 0`. The only assert is
  `FABLE_STATIC_ASSERT_AT(699, ...)` with an explicit line number (now at overlay line 1068); no `__LINE__` use; nothing to pin.
- Header users: 925 files under rebuild/src/compiled include stlport/fable_map.h; no file under rebuild/ mentions either new macro,
  so the opt-ins change none of them.
- Sibling overlays of this header read first: agentFAM12 / agentFAM13 (old 482-line copies, no such macros); no clash.
