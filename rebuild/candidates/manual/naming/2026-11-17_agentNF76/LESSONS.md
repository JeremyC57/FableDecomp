# NF76 lessons (one line each)
- Port from the retail disassembly, the Ego_d locals and a landed sibling's macro block, not from an inherited draft: four rows matched on the first grade that way, while grading NF73's draft cost three SRC_FAILs.
- engine/CTCBase.h names the owner ParentThing; a row-local `Thing` member is a stand-in, and VC7.1 reports its absence late and misleadingly (C3861 on a later line).
- An argument read through an inline accessor (GetParentThing()) is evaluated with the call arguments, so its load moves ahead of the other calls; a plain member read is loaded at the push (0x006D53C0 DIFFER 786v788 -> RM).
- Check a catalogue autoname against the PDB name set and Ego_d before porting: 0x00549B20 "Initialize" was GetDataFromDef (Ego_d locals pdefinition / paction_click / paction_accept), and the header already declared it.
- Read a component's vtable offsets against the header's slot comments, not the landed row's descriptive names: 0x140 is AddActionOnLeftUnclicked and 0x23C GetValueFromProfile, not "AddAction" / "RefreshSelection".
- A blocked row often has a landed sibling with the same inline sequence already on the headers (0x0041B550 <- CIconText::AddIcon 0x00542780): its macro block is the recipe, and the row matched on the first grade without any overlay change.
- An inline member's out-of-line COMDAT elsewhere (0x0053FDB0) does not mean the member is declaration-only: the CTextSlider TU expands AddParam(CActionParam*), so the opt-in makes it inline rather than declared.
