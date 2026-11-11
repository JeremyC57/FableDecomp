
- (agentNF23, 2026-11-11) For a pointer key, write the map value type as `pair<K const, T>`. Writing `pair<const K, T>` textually after macro substitution of `K = X*` gives `const X*`, which is the wrong type and the wrong mangling (`QAV` vs `PBV`).
- (agentNF23, 2026-11-11) STLport 4.6 `_Rb_tree::_M_erase` is private, so VC7.1 mangles it `AAE`. Put `private:` before `_M_erase` in a local tree model, and use explicit member instantiation to emit it as its own COMDAT.
- (agentNF23, 2026-11-11) A virtual destructor called explicitly on a member object (`m.~T()`) is dispatched through the vtable. Write `m.T::~T()` to keep the retail direct call; this is how the `_Dest_val`-shaped CBaseIntelligentPointer rows matched.
- (agentNF23, 2026-11-11) STLport `_Destroy(p)` (`p->~T()`) over a polymorphic element is a virtual call. A retail direct call means the element is a class that *holds* the pointer (e.g. 0x00440730 is list<CMiniMapDisplay::CMarkerInfo>), not the pointer class itself.
- (agentNF23, 2026-11-11) Including a header that pulls `stlport/fable_map.h` into a row that has its own local `std::pair` hung Wine cl past `timeout 240` (row 0x008D29C0). Check a header's include chain before you add it to a row with local std models.
- (agentNF23, 2026-11-11) A prefix `// Leaf:` can match two symbols (e.g. `erase` overloads). Set the full mangled leaf from `objdump -t`.
- (agentNF23, 2026-11-11) `rebuild/oracles/reloc_bindings.tsv` lists only the first caller. A scan of retail bytes for each target (`tools/refscan.py` → `refs.json`) finds every landed binder.
- (agentNF23, 2026-11-11) The landverify objects are stale, so they cannot be used as a binder or old-spelling index. To get the old spelling, grade the repo source (`gr.py --orig`).
- (agentNF23, 2026-11-11) A landed row can grade DIFFER in this lane even unchanged (0x00883180, 32v31). Grade the original before you blame the edit.
