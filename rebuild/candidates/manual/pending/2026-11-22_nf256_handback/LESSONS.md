# NF256 lessons
- An empty filter passed as a named local costs its own 4-byte frame slot; retail's temporary sits in slack bytes (`lea ecx,[esp+1Bh]`).
  When every frame offset differs by 4 and the code is otherwise equal, try the argument as a temporary first.
- pk.py zeroes relocation fields and reads the retail target from the field, so a `symbol + addend` access (LayerInfo[i].ScaleShift)
  shows as NEQ:relocs at symbol+N. Read the object's addend (tools/addend.py) before calling the copy different.
- A HEAD object can carry an UND symbol that nothing defines (0x0063E480's same-TU stub called `?CollectThingsInRoughArea@@...`): grep
  the UND list of every stand-in row before trusting that it links.
- Linking the same object path twice is deduplicated by the linker: copy it to another name for a live LNK4006 control.
- The CTCCombat.h retail branch needs engine/ECombatAbilityDataType.h included before it unless FABLE_RETAIL_CTHINGGAMEOBJECT_ONHIT is on.
- The PDB S_GDATA32 records of one TU's globals keep their declaration order and spacing in both Ego_r and FableWin; with two neighbours
  already bound in the table that settles a data address without any referencer analysis.
