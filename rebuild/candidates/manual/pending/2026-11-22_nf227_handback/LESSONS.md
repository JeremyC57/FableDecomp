# NF227 lessons (one line each)
- A non-template inline's pick-any copy takes the optimize mode in force where the header DEFINES it (not where it is used, not the end of the TU): put a row's size / frame pragmas after its includes and those copies come out in the /O2 owner's shape (0x0041E3B2 / 0x0041DED2 ??9_Rb_tree_base_iterator, 22 B EQ 0x0041C830).
- Template members are the opposite: their COMDATs take the mode at the end of the TU, so a row-tail pragma reaches them (0x0055C950 clear) and an above-include pragma does not stop them.
- Vtable-slot census: no table lines exist for CManager slots; compile every object that can emit the vtable (ctor / dtor / local models with a ctor) and read the ??_7 COMDAT relocations (tools/vtsyms.py).
- A user-declared empty destructor of a polymorphic class stores the vptr; a compiler-generated one does not, so an implicit destructor cannot be reproduced by declaring it.
- STL template self rows (~_Rb_tree, map()) port cleanly as an explicit instantiation of the header member with the owner TU's defines and pragmas.
