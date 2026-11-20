# NF142 lessons (one line each)
- A propagated `_Dest_val` / `Dtor_*` label on a two-call destructor body: search struct_layouts for the class with exactly those member types at those offsets (CQuickDrawTriInfo, CMaintainedEvent) and type `self`; the typed form matched first time.
- An explicit destructor call (`p->~T()`) or `delete p` on a header class makes VC7.1 emit ??_GT as a pick-any COMDAT; that is also how to author an unreferenced compiler-generated ??_G row (FableWin.pdb has many ??_G names Ego_r lacks).
- A `mov ecx,offset g ; jmp T::Member` 10-byte body registered by an initializer is the static's atexit destructor: define the static in the row (leaf `_$E2`), as NF92's 0x01228CE0.
- refs.py exe hits are not bindings: compile the referencer and look for a PAIR before treating it as a binder (0x00784260 -> 0x0041BB00 was a stray immediate).
- Before respelling a row, grade its HEAD with an explicit Leaf: a no-Leaf row can be DIFFER at HEAD (0x009E15E0), and then no respell can be verified.
- Adding a Leaf line to a no-Leaf row is cheap and surfaces pre-existing STANDIN_LEAF warnings; say they are pre-existing.
