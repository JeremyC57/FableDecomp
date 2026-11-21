# NF210 header overlay (2026-11-20)

Every overlay header is a copy of the CURRENT repo working-tree header (base copy in `orig/`), with add-only `#if` opt-ins under
member-specific macro names. Without the macros each header is token-identical to its base (`diff orig/<h> hdr/<h>` shows only the
added blocks). pin_asserts.py was run after each edit: `engine/CCombatManager.h pinned 0`, `stlport/fable_map.h pinned 0` (both use
explicit-counter FABLE_STATIC_ASSERT_AT lines; no assert line was moved).

## engine/CCombatManager.h
- Added `FABLE_RETAIL_CCOMBATMANAGER_ISDEFENDERDUEFORATTACK` (after the FABLE_RETAIL_CCOMBATMANAGER_ISATTACKERON block): declarations of
  the two public const PDB overloads (Ego_r.pdb 0x00712BC0 / 0x00713BF0 + FableWin.pdb 0x024187C0 / 0x024185D0):
  `bool IsDefenderDueForAttack(const CCombatantInfo& info) const;` (0x006E97F0) and
  `bool IsDefenderDueForAttack(const CThingCreatureBase& defender) const;` (0x006EA050). Declarations only: no layout change.
  Drafted by NF207 (naming/2026-11-20_agentNF207/work/hdr_drafts/).
- Added `FABLE_RETAIL_CCOMBATMANAGER_GETCOMBATDEBUGINFO` (before the FABLE_RETAIL_CCOMBATMANAGER_UNSHEATH block): a declaration of the
  public const `CCombatDebugInfo GetCombatDebugInfo(const CThingCreatureBase& creature) const;` (Ego_r.pdb 0x00715140 + FableWin.pdb
  0x0241C2D0 `?GetCombatDebugInfo@CCombatManager@@QBE?AVCCombatDebugInfo@@ABVCThingCreatureBase@@@Z`; out of line at 0x006EAC50).
  CCombatDebugInfo has no shared header: the including row supplies the class before the include. Declaration only: no layout change.
- Users: rows 0x006EA050 (ISDEFENDERDUEFORATTACK), 0x006EAC50 (GETCOMBATDEBUGINFO).
- Landed includers (`grep -rlE 'engine/CCombatManager\.h' rebuild/src/compiled`): 38 rows; none defines the new macro (grep), so the
  opt-in changes none of them.

## engine/CThingCreatureBase.h
- Base: repo working tree (copy in `orig/engine/CThingCreatureBase.h`).
- Added `FABLE_RETAIL_CTHINGCREATUREBASE_ISINMELEEASATTACKED` (right after the `IsInMelee / IsInMeleeAsAttacker` line): a declaration of
  `bool IsInMeleeAsAttacked()const;` (Ego_r.pdb 0x0053D2C0 + FableWin.pdb 0x01DA3370 `?IsInMeleeAsAttacked@CThingCreatureBase@@QBE_NXZ`,
  out of line at 0x00662430, already bound under this spelling in reloc_bindings). Declaration only: no layout change.
- User: row 0x006EAC50. pin_asserts.py: pinned 0.
- Landed includers (`grep -rlE 'engine/CThingCreatureBase\.h' rebuild/src/compiled`): 614 rows; none defines the new macro (grep), so the
  opt-in changes none of them.

## stlport/fable_map.h
- Added `FABLE_RETAIL_STLP_RB_GLOBAL_DECREMENT_SIZE_BODY` (right before `typedef _Rb_global<bool> _Rb_global_inst;`, guarded
  `!FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ && _MSC_VER`): `Fidelity: scaffolding, not retail`. An inline explicit specialization
  `_Rb_global<bool>::_M_decrement` with the same STLport 4.6 _tree.c body as the in-class one, under `#pragma optimize("s", on)` ...
  `#pragma optimize("", on)` (the nf184 SIZE_BODY pattern: FABLE_RETAIL_STLP_LESS_CCHARSTRING_SIZE_BODY). The block ends with
  optimize("", on), so a row that uses it must not carry a row-head optimize pragma before its include of fable_map.h.
  EXTRA_RULES 2026-11-20 size-pragma conditions, each checked on 0x006EDA80: (1) the row's own body and every other defined symbol are
  identical with and without the macro (objcmp of both objects: only _M_decrement differs, 64 -> 57 bytes); (2) the owner row
  0x004294E2 sets `#pragma optimize("s", on)` itself; (3) the comment states the reset. The copy then equals the owner object's
  (cmpbody 57/57, bytes True, relocs True). The owner's copy is a COMDAT (from `template class _Rb_global<bool>;`), so no LNK2005.
  An out-of-class template definition under the pragma does not work (NF207 _d: the instance is compiled at the end of the TU, at /O2).
- Added `FABLE_RETAIL_STLP_TREE_ERASE_KEY_DECL_ONLY` (guarded `!FABLE_RETAIL_STLP_TREE_ERASE_KEY && !FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE`):
  `size_t erase(const K& key);` declared only in _Rb_tree (after `insert_unique(iterator,const V&)`), and the _map.h wrapper
  `size_t erase(const K& key){return _M_t.erase(key);}` in map (after the FABLE_RETAIL_STLP_TREE_ERASE_KEY block). The TU calls the owner
  rows' out-of-line instances (0x006F50B0, 0x006F3200) and emits no pick-any copy of erase(key) or its helpers. The FABLE_RETAIL_STLP_MAP_CLEAR_DECL_ONLY /
  FABLE_RETAIL_STLP_TREE_MERASE_DECL_ONLY pattern. Same function; declaration only.
- Users: rows 0x006EDA80 (SIZE_BODY) and 0x006ED510 (ERASE_KEY_DECL_ONLY).
- Landed includers (`grep -rlE 'stlport/fable_map\.h' rebuild/src/compiled`): 1008 rows; none defines either new macro (grep), so the
  opt-ins change none of them.

No other agent is running (single-agent round), so no other overlay was read for these headers.

## Not overlaid: engine/CThing.h
- The FABLE_RETAIL_CTHING_HASTC_DECL_ONLY draft for q11772 (an `#elif` declaration-only HasTC branch) is NOT in hdr/: regraded on the held
  0x0047DB73 draft it gives DIFFER(65v69) (HasTC called out of line instead of expanded). It is kept in work/hdr_drafts/engine/CThing.h
  (base copy work/hdr_drafts/orig_engine/CThing.h) for the record, and no handed-back row uses it.
