# NF157 lessons (one line each)
- A compiler-emitted ??_G can only be reproduced in a TU that compiles a constructor of the class; when every ctor is another row's leaf, a respell of the ??_G row's callee binding is the most that lands without a one-definition clash (form 1 emitted 0x008D8620's ctor name, form 2 four base-class ??_Gs).
- Diff the new object against the HEAD object (binding pairs + defined externals) for every row: it caught 0x0069C3D0's DeleteData COMDAT built from an incomplete class, under a retail name, which no grade shows.
- A pick-any explicit instantiation (`template void FABLE_FASTCALL CCountedPointer<T>::DeleteData(void*);`) is LINK_ONCE_DISCARD under VC7.1: it links beside every includer's implicit COMDAT of the same instance (control link proves detection).
- Respelling one binder to a callee's PDB access (QBE -> ABE) can create a split when the callee's own row still defines the old access: check the callee row's emitted leaf before respelling, and move both as a family.
- RTTI tsv drops template arguments; read the type descriptor (vtable-4 -> COL+0xC -> TD+8) from Fable.exe before deciding which side of a header/row disagreement is wrong (0x01237F1C = CIntelligentPointer<const CThing>).
- Pointer-to-reference self respells of mode constructors are byte-neutral when the body passes &x on to the row's pointer stand-ins: a one-grade change per row.
