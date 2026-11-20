# NF164 lessons (one line each)
- When an earlier session's census objects survive in pH/, rebuild the census from them with binds.py (no recompiles): 70+ rows in seconds.
- A static data member's binder that models the map descriptively can move in place: include stlport/fable_map.h, give the static its real std::map type and use map::find with FABLE_RETAIL_STLP_TREE_MFIND_DECL; the local std::vector/allocator model must go (it clashes with fable_containers.h), and the shared vector matched first time.
- Pointer-returning stand-in accessors respell to the PDB reference return with no byte change (`CWorld* w = &GetWorld();` keeps a named-local shape); script it and check each row with an external-symbol diff against the HEAD object.
- A q.py DIFFER(N+1vN) that is identical at HEAD is an oracle cut; vland --recheck-landed is the judge.
- A combat-action row whose derived class has only a generated blob header ports cheaply when a landed sibling with the same body shape already sits on CCombatActionBase.h: copy its define block and add a minimal retail-class opt-in (base from rtti_bases.tsv, sizeof from the generated header).
