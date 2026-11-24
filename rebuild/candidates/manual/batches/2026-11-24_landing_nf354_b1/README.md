# nf354 b1: 0x00E2A320 CQS_ScytheInfoScript::CScytheNearOracle::Main off the CCutsceneCastMap stand-in (1 row)

Agent NF354 (Account A naming queue; hand-back `pending/2026-11-24_nf354_handback/`). The row-local stand-in `struct CCutsceneCastMap`
becomes `std::map<CCharString, CScriptGameResourceObjectScriptedThingBase>` from `stlport/fable_map.h` with FABLE_RETAIL_STLP_MAP_INDEX_DECL_ONLY
(marked `// Fidelity: scaffolding, not retail`, a coordinator comment-only edit). Its operator[] reference now names the real 4-argument
STLport symbol, which is exactly the `// Leaf:` of the landed owner 0x00CD3D2E (nf350).

Coordinator verification (object rebuilt from the repo after landing):
- vland `--recheck-landed`: RELOCATION_MATCH PASS (HEAD: RELOCATION_MATCH PASS); warnings unchanged.
- objcmp HEAD v new: Main's `.text` bytes and section flags equal; its 77 relocations equal except the one intended rename; no other
  section differs except `.debug$S`; `.drectve` = HEAD's tokens plus `/DEFAULTLIB:"libcp"` (NF318 Q1 libcp exception); DEF sets equal; UND
  sets equal under the one rename.
- pk_all `--leaf`, HEAD v new: identical. pickany_vs_retail: the leaf-referenced NEQ ~CScriptGameResourceObjectMovieBase 0x006E7B80 is
  existing debt (T3 nf340 0x006E7B80), unchanged.
- No table change: the stand-in `??ACCutsceneCastMap` line keeps its witness 0x00E2A900.

Rulings (advisor decision, owner delegated, 2026-11-24): Q1 libcp yes; Q2 the partial unit u1 lands (the stand-in line coexists with the
real one, nothing resolved becomes unresolved). Q5: 0x00E2A900 is NOT landed. Both PDBs name the out-of-line clear of these two value types
only as Dinkumware `std::_Tree<_Tmap_traits<...>>::clear` (Ego_r 0x00cb576b / 0x00af9260, FableWin 0x035c6913 / 0x02fdb970), which cannot say
whether retail's STLport function is `map::clear` or `_Rb_tree::clear`; so the respell onto `?clear@?$map...` is not proved by two sources.
Queued as NAME_FIXES research on the two existing table lines (witnesses 0x00CFF9A0, 0x00D7C470). Q3 / Q4 queued on T3 (nf354).
