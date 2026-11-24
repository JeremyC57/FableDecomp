# Landing nf371_b1 (2026-11-24): 0x00CD3F40 is CScriptDefinitionManager::Set, and its binder 0x00CB5D80 calls it by that name

Hand-back: `pending/2026-11-24_nf371_handback/` (NOTES.md, STATUS.tsv). Two rows, both RELOCATION_MATCH PASS before and after
(`logs/recheck.log`); objsec ALL_EQ under the renames; both pick-any checkers SAME with 0 reach flips.
- 0x00CD3F40: `?operator_new_variant@@YIPAXI@Z` -> FableWin.pdb + Ego_r.pdb `?Set@CScriptDefinitionManager@@SIXPAV1@@Z` (Ego_r Set is 12 bytes, as
  retail). Catalog Module `_global` -> `CScriptDefinitionManager`, oracle name `CScriptDefinitionManager::Set`, self_symbols updated
  (column 4 = the `// Leaf:` text). The file names stay (the generated indexes name them).
- 0x00CB5D80 CScriptInfoManager::Init: the callee is declared as the static member, not the free `GFSetPDefinitionManager`.
- Table: Reset<CScriptDefinitionManager> at 0x00CD43A0 (was `?Allocate@CFixedAllocator`) and Set at 0x00CD3F40.

Held (`pending/2026-11-24_nf371_handback/b2_held/`): the seven Reset<T> respells. Rulings (advisor decision, owner delegated): R2 yes, the
unreferenced pick-any `??0SCountBlock` stand-in copy becomes the header's `??0CCPPointerInfo` (equal to what header-form includers emit), once
pk_all shows it leaf=n; R1: 0x00A299C0 and 0x00F39CA0 take the explicit-instantiation form (no LNK2005), gated on the variant objects first.
