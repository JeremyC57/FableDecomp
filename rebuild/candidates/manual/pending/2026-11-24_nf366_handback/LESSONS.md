# NF366 lessons
- A CCountedPointer<T>::DeleteData row's T comes from the object its Reset's caller constructs (retail RTTI of the vtable the ctor
  stores), not from the Reset's catalog label: three Reset table lines here named made-up classes (CEntryCountedPointer, CAllocatorPointer,
  C3DOptimisedMeshBase) and one a different function (CFixedAllocator::Allocate).
- When the object's class has no counted-pointer name in any PDB set, the bytes cannot choose between it and its bases (0x009A6C00,
  0x00991E10): skip, do not pick.
- Retail keeps unreferenced template instances (0x0044AC20, Reset 0x00A0BC90 with its five Releases): an unreferenced DeleteData has no
  identity source at all.
- A vtable without RTTI (0x012C2648) can still be tied to its class through the shared header's slot map and the PDB's static Set.
- The scratch corpus missed every Reset row: census by HEAD-compiling the binders and the T's users, not from the corpus.
