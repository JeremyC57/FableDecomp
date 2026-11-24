# NF240 lessons (one line each)
- A private static (`0` in the mangling) read by non-members needs a public accessor from the PDBs (FableWin / Ego_d inline names such as SetAsRendered / IncrementFrameCounter): grep all three PDBs for `@<Class>@@` accessors before calling it an access-path blocker.
- A new vtable emitter binds every slot: read the retail vtable, then check each slot's landed self leaf; placeholder self leaves (vfunc_N, global stand-in classes) make the PDB slot spelling a new split unless those self rows move in the same unit.
- When a ported ctor's compiler-generated ??_G is shorter than the retail one, retail expanded the destructor: an implicit-dtor opt-in (the PASS22_IMPLICIT_DTOR pattern) makes the copy equal.
- A header-closure equivalence over the rows whose source or test names any header in the include closure keeps equiv_check to a few hundred rows instead of the whole tree.
- A local empty `enum ETCInterfaceType { }` blocks including any TC header; replacing it with engine/ETCInterfaceType.h is byte-neutral (same mangled name, int-sized).
