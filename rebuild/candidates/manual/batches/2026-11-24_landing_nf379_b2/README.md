# Landing nf379 b2: option S unit, ai_state_group_dragon.obj slice (owner 0x0094FBB0)

Owner 0x0094FBB0 holds IsTargetOnATower, IsStillRunnable and UpdateHover in retail order; 0x0094FDF0 and 0x00951FB0 are thin rows. 0x0094FBB0's own symbol moves to the PDB spelling `?IsTargetOnATower@CAIStateGroup_JackDragon@@AAE_NXZ` (QAE before; both PDBs say AAE). Catalog: 0x0094FBB0 and 0x0094FDF0 take the unit flag set `/O2 /Oy /W3 /d2inlT59` that 0x00951FB0 already carried. 0x0094FDF0 joined because 0x00951FB0's object held an EQ strong copy of it.
- Gates: every leaf RELOCATION_MATCH from one owner compile, strict landing recheck PASS; pick-any checkers 0 new NEQ, 0 reach flips; /FORCE link duplicates 6 to 5 (all five pre-existing); the QAE spelling is bound nowhere (compiled census).
- Ruling NF379-Q3 (advisor decision, owner delegated): a partial slice may land (NF376 precedent). The rest of the TU is queued: 0x00951070 S-ready, 0x00950CF0 pick-any, 0x00951430 / 0x00952890 NEQ copies (58 v 55), 0x00951D60 GetSubDef binder residue.
- T3 nf309 closed (T3_CLOSED.tsv). self_symbols: three lines added.
