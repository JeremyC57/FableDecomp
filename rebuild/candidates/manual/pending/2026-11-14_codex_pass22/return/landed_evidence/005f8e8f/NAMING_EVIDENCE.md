# Identity evidence

The row is the STLport4.6 comparator overload `std::__pop_heap_aux` for `std::pair<long,long>` with `CKeyPairCompareLess<long,long>`. It is not an unwind funclet or a context-sensitive-item specialization.

Evidence1: original STLport4.6.2 `_heap.c` lines164–172 gives the exact source-level name, template parameter order, tag argument and by-value copy into `__pop_heap`. The native body has that complete15-instruction shape, an8-byte pair and fastcall/ret8 convention.

Evidence2: native callers005F9B1E/005F9B5A and the partial-sort family005F9D4E/005F9DA3 establish the pair specializations. FableWin PDB contains the corresponding `sort_heap` instantiations at01EEADF4/01EEB39C, `_Sort_heap` at01EEC12E/01EEC600 and `_Pop_heap_0` at01EED59D/01EED703. The donor uses another STL revision's algorithm spelling; its template types are evidence, not permission to rename native STLport algorithms. Ego_r also has the EHeroAbility pop/sort family at005867D1/00586C47. Exact native STLport symbol spellings already bind the callers in reloc_bindings.tsv, and the new object's full symbol table agrees.

The one callee is `??$__pop_heap@PAU?$pair@JJ@std@@U12@V?$CKeyPairCompareLess@JJ@@H@std@@YIXPAU?$pair@JJ@0@00U10@V?$CKeyPairCompareLess@JJ@@PAH@Z` at005F8D82, proven by the decoded REL32 and existing unique specialization binding. No callee implementation is carried. The real shared pair, comparator and enum definitions are used. No forced inlining is necessary.

The new opt-in adds the original auxiliary template and its external callee declaration. Removing only that block and its macro activation restores the original header exactly, including all old opt-in paths; no static assertions moved. Adjacent supplied sources were reviewed but their local class substitutes were not reused.
