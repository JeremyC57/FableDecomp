# NF209 lessons (one line each)
- VC7.1 ignores `template<> std::map<...>::~map();` declarations for STLport members (0x00E55CE0 still defines ~map / _M_erase copies); use fable_map.h's FABLE_RETAIL_STLP_MAP_DTOR_DECL_ONLY + FABLE_RETAIL_STLP_TREE_DTOR_DECL_ONLY instead.
- A vtable's ??_G is emitted only where a constructor is compiled; a qualified destructor call emits ??1 but no vtable. VC7.1 also emits an inline constructor's COMDAT even when every use is expanded (.debug$F only), FABLE_FORCEINLINE included, so make that copy equal the owner's (header-inline map ctors gave the owner's 113 bytes).
- A per-TU STLport opt-in (FABLE_RETAIL_STLP_BITSET_TEST_DECL) must be defined before the FIRST include of the header chain, not just before the header that uses it; check the defined-symbol diff, not only the grade.
- Header inlines with no catalog row can still have a table line (?HasTC@CThing -> 0x004118C0): run the table lookup on every newly defined symbol, then disassemble the target to see whose body it is.
- Comparing a new object against every built neighbour object (comm -12 of defined symbols, then cmpbody each) finds owner copies that stand-in leaves hide.
