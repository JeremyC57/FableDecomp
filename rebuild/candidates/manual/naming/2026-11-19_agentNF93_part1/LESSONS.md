# NF93 lessons (one line each)
- A self row whose callers already use a header inline ctor (FABLE_..._CTOR) becomes MATCH as the nf86 pick-any scaffolding (`new T` in an anonymous-namespace helper under optimize("s")): check the COMDAT selection (2 = any) in objdump -t aux.
- A non-const STLport twin (find_if / __find_if) of a ported const instantiation is the same explicit instantiation with the const dropped; each grades first time.
- Moving a find_if predicate into a named object built before the end pointer fixed a key/end load swap again (0x005BDFB1, 119v119 -> RELOCATION_MATCH).
- Generated flat action headers (engine/CCreatureAction_<X>.h) take a `_CLASS` opt-in with the generated body kept under `#else`; run pin_asserts.py after, it pins the moved asserts.
- An action header whose inline ctor calls GetSoundPair needs FABLE_RETAIL_CCOMBATACTIONBASE_SOUNDCTOR and engine/CSoundPair.h before engine/CCombatActionBase.h, or C3861.
- Unreferenced STL COMDAT twins (no call, jmp or absolute ref anywhere) cannot be told apart (~_Rb_tree vs ~map, clear vs map::clear): block on identity rather than pick.
