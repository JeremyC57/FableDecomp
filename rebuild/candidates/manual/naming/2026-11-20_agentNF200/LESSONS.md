# NF200 lessons
- `FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN_DIRECT` does nothing without `FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN`: the assignment silently falls back to the implicit memberwise copy.
- A named local in an inner block reproduces retail's register-pushed temporary string (frame size drops to retail's).
- `IS_THING_ALIVE(&PThing)` on a `CIntelligentPointer` member resolves to the template's own overload (C2039 IsDead); a stand-in pointer cast is needed, so treat it as a blocked port.
- ThingAlive.h with FABLE_RETAIL_IS_THING_ALIVE_NOINLINE gives a pick-any COMDAT byte-identical to its owner even under optimize("s").
- STLport self rows: an explicit instantiation (`template void std::vector<T>::push_back(const T&);`) emits the self leaf without a local model.
- For a funclet recheck, set source_cpp to the parent's new source, leaf `Unwind@<addr>`, funclet_offset 0 in the mkpay payload.
- libpk (0xC08AE0..~0xC40000) is Dinkumware; its pool copy CHandleBasedMemoryPool_012b4098 has no PDB: the vector-vs-CArray destructor name is not decidable from bytes.
