# NF251 lessons (one line each)
- A ??_G row whose class has out-of-line ctor and dtor rows can fold under one of them (include it, no code of its own, the funclet pattern): stage the new parent where `../src/compiled/...` resolves (overlay dir's sibling src/), and prove the include took the draft from the object's symbols (HEAD ctor = ??0EngineNative).
- A per-row link double-counts a fold row (its object is its parent's TU): run the link both ways (naive and one object per TU) and report both.
- A protected inline ctor of an abstract class is emitted pick-any by an anonymous-namespace derived class with an out-of-line ctor (never instantiated): no strong definition, and the TU also emits the header vtable and ??_G.
- A strong definition of a symbol that other rows emit pick-any is LNK2005, not LNK4006: never move a self row onto a header-inline spelling as a strong definition.
- VC7.1 neither folds intrinsic cos / sin of a literal 0.0 nor uses fldz for a double 0.0 store: both load __real@0000000000000000, so a stand-in global for 0x0122ED70 is always replaceable by the literal.
- Census a data address by source grep of `extern` declarations that name it, not only by table lines: the table missed ?g_ProgressTimeZero@@3NB and kept a stale ?g_clearDouble@@3NB.
