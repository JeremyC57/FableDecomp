# NF136 lessons (one line each)
- A RTTI-less component vtable names its T through the function that stores it: owner.py on the non-constructor store, then that owner's Leaf line or the spelling its landed callers bind (callers.py); catalog T labels were wrong for 6 of 23 rows here.
- Two rows with one catalog name (0x005D8167 / 0x00679910, both "CPersistTraits<EInputKey>::TransferIn") are never one function: callers.py on each gives the spelling its caller already binds.
- Rows that model CCharString / CStringParser locally cannot include a header that needs the retail string models; a partial header mode (FABLE_RETAIL_CPERSISTTRAITS_SLONG_ONLY) moves the callee spelling without touching the models.
- The explicit `template void X<T>::f(...)` form of a component member emits only the leaf; the traits-object form also matched but emitted CPersistTraits<T>::TransferIn as an extra COMDAT.
- genA-style generators: detect the old stand-in class from the whole row, not the first `struct` (0x007AD7B0's first struct was a local CPersistTraits).
- A STLport member stand-in (struct SubBuffer { void Clear(); }) is respelled in place by the real std::list<T> member plus the header's CLEAR_OUTOFLINE opt-in; the member's size does not matter when nothing follows it.
- An Ego_r.pdb member offset (MaintainedEventsList at 3340 = 0xD0C) identifies a stand-in member in a mislabelled row faster than the catalog does.
- Re-probe a held binder at HEAD before recording a dependency: nf133 had landed by mid-session (0x00B4F510).
