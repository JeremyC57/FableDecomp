# 0x00B625E0 CEngineSkyRenderer ctor: matched source not landed (coordinator decision, 2026-11-03)

The agent matched the row by writing the def lookup as `*def = reinterpret_cast<const CDefPointer<const T>&>(base)` (the header's
copy assignment). The PDB proves retail calls `CDefPointer::SetDynamicCast`, which re-reads the def pointer after releasing the
old one; our `engine/CDefPointer.h` keeps it in a register. Making the header re-read matches this row but breaks landed 0x0047A9F0
CHeroLogBook::AddEntry (1149 vs 1105). Owner rule: retail faithfulness, no pointer casts standing in for retail calls. So the row is
**parked**: the faithful source is `differ/00b625e0_setdynamiccast.cpp` (dl 48, differs only at the end) and the header option
`differ/CDefPointer_reread_option.h`. It joins the open CDefPointer load-before-dec investigation (3 weapon rows): one
SetDynamicCast form must match this row, AddEntry and the weapon rows together.
