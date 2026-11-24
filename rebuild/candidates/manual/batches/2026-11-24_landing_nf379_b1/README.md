# Landing nf379 b1: option S unit, CThingNoiseDef def TU (owner 0x0045C5F6)

Two thin rows, 0x00455EF0 (`??0CThingNoiseDef@@QAE@XZ`) and 0x0045C5DA (`??_GCThingNoiseDef@@UAEPAXI@Z`), drop their NF363 option A copies and include the owner row 0x0045C5F6, whose text already is the TU and is unchanged. The link drops the strong `?Alloc@CThingNoiseDef` duplicates (4 to 2; 1 with b3, the known `??0CTCNoise` pair).
- Agent NF379 (pending/2026-11-24_nf379_handback). Landed with the patched land_scripted (tools commit 4f72f62cb2): both members graded strict at their own leaf, RELOCATION_MATCH PASS.
- Rulings (advisor decision, owner delegated): NF379-Q2 yes, an S owner need not be the lowest address (that rule applies only when a new owner is authored); T3 nf334 stays open (owner not edited).
- s_unit_check --objdir: 3 units OK, 4 implicit members verified as DEF symbols.
