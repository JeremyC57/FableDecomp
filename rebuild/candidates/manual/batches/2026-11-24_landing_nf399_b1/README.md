# Landing nf399 b1: the EHeroTrainableStatType tree's _M_erase and destructor get their STLport names

- 0x00578FF5 was the stand-in `?BuildTreeArray@Tree@@QAEXPAUNode@@@Z` (5 other definers remain). It is now `_Rb_tree<EHeroTrainableStatType, pair<const EHeroTrainableStatType, NUISystem::CComponent*>, ...>::_M_erase`, the spelling nf396_b2's table line already binds there. The explicit-instantiation form on stlport/fable_map.h, as in 0x00527950.
- 0x005796AB was `??1list_T_005796ab@std@@...`. It is now that tree's `~_Rb_tree`, the table's existing spelling (witness 0057846f).
  - Reference sites decide ~_Rb_tree over ~map (NF389): ~CTCInventoryExperience calls it on `lea ecx,[esi+0x224]` (the UpgradableStats member) at 0x005784E8, and an uncatalogued 5-byte `jmp` at 0x00579887 (VC7.1's implicit ~map of this type compiles to exactly that jump) targets it.
  - The owner row's own match was not used as evidence.
- No table or header change.
- NF399 gates:
  - vland HEAD and new RELOCATION_MATCH, no LITERAL_DIFF;
  - pk_all / pickany_vs_retail with no NEQ and 0 reach flips;
  - extra COMDATs tier 2 only;
  - /FORCE link 0 duplicates, nothing left unresolved.
- Coordinator checks: landed 2 of 2; self = `// Leaf:` = `self_symbols.tsv`; the sources equal the graded sources; baselines: bindings multi 1, funclets 0 broken, S units 3 OK.
- T3 nf396 0x00578FF5 / 0x005796AB are closed. Follow-ups are T3 nf399 lines (the ~map thunk 0x00579887 and siblings, the 0x00579616 double name, and 0x006CC1E0's local ~vector copy). The 0x004DF54F unit (NF399 b2) waits on a ruling.
