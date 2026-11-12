# agentFAM11 lessons (one line each)
- reloc_bindings.tsv columns are symbol, target, first caller: grep column 2 for a row's address to find the real spelling its callers bind.
- A landed real-typed caller binding plus a PDB _Tmap/_Tset_traits name set proves most iterator-op / __distance rows in one grade each.
- VC7.1 accepts incomplete pair members in iterator ops and __distance; host g++ instantiates pair through ADL, so ignore those syn errors.
- Spell a pair value as ` ::std::pair<...> ` (leading space): `<::` is a digraph hazard, and VC7.1 needs `> >`.
- map's _Rep_type is private in fable_map.h: spell the _Rb_tree explicitly for an explicit instantiation.
- Nested-class elements cannot use the local STLport text; the shared fable_map.h route plus the owner header works (4 rows, first try).
- Check the binder's traits and arity: two placeholder "operator++" rows were const post-increments, two "find" rows member templates.
- A row whose element needs a header that pulls fable_containers.h cannot keep a local std text model; leave it or move it to the shared route.
