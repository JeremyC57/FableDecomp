# NF20 lessons
- A ??_G row emits only when the TU sees the vtable: a local derived class with an inline ctor, built in a helper, does it, but the class must declare CTCBase's pure virtuals.
- An implicit dtor (no user dtor + `o->T::~T()` helper) is the way to get retail's no-vptr-store shape; a declared virtual dtor stores the vptr.
- COMDAT/ICF: a binder whose element type differs from the address's catalog instance keeps its own spelling; never respell it to the catalog name.
- reloc_bindings.tsv and old object indexes go stale; grep rebuild/src/compiled before counting a binder as left.
- Old tests that link stand-in symbols LINK_FAIL after a self-rename; rewrite the test to call the row's real symbol.
- Do not name a helper script after a stdlib module (dis.py shadowed capstone's import of `dis`).
- Running an --orig grade writes into the same output folder; regrade the row afterwards.
