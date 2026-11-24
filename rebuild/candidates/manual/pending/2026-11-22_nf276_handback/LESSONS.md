# NF276 lessons
- A self rename QAE -> UAE on a destructor that moves onto a shared base with a virtual destructor makes the TU emit ??_7 / ??_G: give the local model every private virtual override with its PDB signature, or the emitted table binds __purecall / base members in the derived slots.
- Whether the TU's ??_G inlines the destructor is the deciding measurement: here VC7.1 kept the 93-byte destructor out of line (38-byte ??_G, pk EQ retail), unlike CWaterPatchMesh where the ??_G inlined it.
- A new vtable can bind a PDB spelling at an address whose self row still defines a stand-in; moving that self leaf in the same hand-back keeps the spelling count from growing (0x00BFDDB0's EBE_NJ).
- Using a PDB inline accessor in a row can make VC7.1 emit a pick-any copy of it even though every use is expanded: check objdump -t for new owner-less names after each form, not only the grade.
- An owner moved to pick-any (inline + anonymous-namespace address helper) only changes the COMDAT selection byte (NODUPLICATES -> ANY); objsec does not read selection, so check it from the section-definition aux record (tools/comdatsel.py).
- Pick-any owner plus NEQ pick-any TU-mate copies is worse than HEAD's strong owner: the linker then keeps the NEQ copy silently in some orders. Make every copy EQ first (S3), then drop the strong definition.
