# NF87 lessons

- **Check the whole family before you port a heap or sort helper.** The CKeyPairCompareLess heap families (0x00A3xxxx, 0x00A7xxxx) chain
  `__partial_sort -> __make_heap / __pop_heap / sort_heap -> __adjust_heap` and are called from `__introsort_loop`. Respelling one member
  changes what its neighbours must bind. Before the first grade:
  - Run `e8.py <addr>` to list the retail E8 call sites.
  - Check whether each callee's self row already has the right spelling. The `__adjust_heap` rows usually do; claim and port the ones
    that do not, such as `__make_heap` 0x00A344E0 or `sort_heap` 0x00A72AA0.
  - Stop at `__introsort_loop`. It needs a family port, so record it in NAME_FIXES.
- **STLport pair is a struct.** It mangles `PAU?$pair` / `U12`. Old rows that modelled `class pair` mangle PAV and need the shared
  template to fix them.
- **The heap recipes are header-only.** Each row below is one explicit instantiation and graded on the first try:
  - `FABLE_RETAIL_STLP_HEAP` (+`STLP_SORT_HEAP_BODY`): `sort_heap`.
  - `STLP_HEAP` + `STLP_MAKE_HEAP_BODY`: `__make_heap`.
  - `STLP_HEAP`: `__pop_heap`.
  - `STLP_PARTIAL_SORT_BODY` + `_INLINE_MAKE_HEAP`, without `STLP_HEAP`, so that `__pop_heap` stays a call: `__partial_sort`.
- **Out-of-line `_Rb_tree_iterator` ++/-- self rows** are explicit instantiations under `STLP_TREE_INC_OUTOFLINE` /
  `STLP_TREE_DEC_OUTOFLINE` on the map's real value type.
- **An implicit operator= that becomes a call in a big script function is an inline-budget split.** A five-probe bisect proved it:
  every half of the function inlines it alone, and the whole function does not. A per-row `/d2inlT` sweep upward from the default T74
  found the match (76). Budget the bisect: it cost 10 of the 16 grades. Next time, go to the `/d2inlT` sweep after two probes.
- **Cutscene rows with local CCutsceneCastMap / CCutsceneArgMap stand-ins port cleanly.** Use `std::map` from `fable_map.h`, plus
  `FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_FORCEINLINE_ASSIGN` and the shared `RunCutsceneMacro` declaration of
  0x00D6D670 / 0x00D33730. The inline map ctor/dtor come out right with no extra opt-in.
- **Pin before you grade.** I ran `pin_asserts.py` on a new overlay (CCS_OakValeRevisitedScript.h) only after its first grade. The pin
  does not change bytes, because `_AT` keeps the line-number typedef. Run it right after each overlay edit anyway.
- **`g.sh` resolves relative paths from the repo directory.** Always pass absolute row paths. A relative path printed a grep error and
  cost a grade.
- **Ego_d.pdb parameter names (egod_locals.py) give the argument order.** For SetBankAlias, `bank_handle` and `alias` settled which
  argument is the key and which is the value.
