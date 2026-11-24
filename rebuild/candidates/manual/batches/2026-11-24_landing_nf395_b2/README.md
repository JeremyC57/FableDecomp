# Landing nf395 b2: binder 0x009B77B0 onto the shared CArray header (rulings R1, R10, R11)

0x009B77B0 (`_Push_heap` over CDefinitionManager pairs) replaces its row-local `N009b77b0::CArray` with `engine/CArray.h` and `FABLE_RETAIL_STLP_VECTOR_ASSIGN`. Its assignment binds STLport `vector<unsigned long>::operator=` at 0x0045BC09, as in nf390_b1. Its self is unchanged; the `__push_heap` respell is queued (T3 nf395 0x009B77B0, NAME_FIXES_QUEUE nf395).
- **Extras:** the row is /Gr, so its free helpers mangle YI. All 10 new COMDATs are EQ by COMDAT to the /Gr shared-header sibling 009b80c0 (coordinator `comdat_cmp`), which is tier 1. `~CArray` pairs with HEAD's local one (R11).
- **Table:** delete `??4?$CArray@K@N009b77b0@@QAEAAU01@ABU01@@Z -> 0045bc09`. No other source spells that namespace.
- **Coordinator checks:** landed RELOCATION_MATCH; the self is defined in the new object and equals `self_symbols.tsv`; the source equals the graded source. Baselines as in b1. NF395: link duplicates 0 -> 0, no NEQ.
