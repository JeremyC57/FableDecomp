# NF278 lessons (one line each)
- A polymorphic class whose ctor and dtor are both out of line cannot emit its ??_G from a third TU (no nf86 scaffolding reaches the vtable): fold the ??_G row under an emitter's TU (0x00B2AB10 pattern), and pk both candidate TUs first: the ctor TU carried an NEQ copy the dtor TU did not.
- VC7.1 vtables name slot 0 `??_E...` (a weak alias, storage class 105) and define `??_G` beside it: census the ??_G spelling, not the slot symbol.
- Before respelling a stand-in template's arity, compare its copies with the real spelling's copies under a rename map (tools/cmpsym.py): here every copy was already byte- and relocation-identical to the landed owner's, which turns the question into a pure rule decision.
- A census of "rows with the stand-in model" by source grep finds more rows (8) than bind the stand-in (4); compile them before counting binders.
