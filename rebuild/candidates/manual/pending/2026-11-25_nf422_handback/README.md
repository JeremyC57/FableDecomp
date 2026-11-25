# NF422 hand-back (2026-11-25): the 3D anim / mesh file units

12 rows landed as `2026-11-24_landing_nf422_b1` / `_b2` (RELOCATION_MATCH PASS; includer recheck of the six add-only header
opt-ins: 10 of 10 PASS). b1 re-forms lib_3d_anim_file and lib_3d_mesh_file as two five-member Option S units owned by their
default constructors (0x00A98E60 / 0x00A9AD80); the loaders call the private AAE root-chunk constructors, and the table's QAE
stand-in lines are replaced by the AAE spellings (one address, one name). b2 respells the two root chunks' GetChunkDescription,
which b1's vftables need. /FORCE census of 3,559 lib_3d_* / lib_chunked_file TUs (address order and reverse): duplicates 235 -> 231,
unresolved 1,882 -> 1,879, nothing new. 12 self lines applied.
RULINGS Q1 (coordinator decision, precedent GOTCHAS NF419 r2): the `??1CDataInputStream` copy pk now reads NEW_NEQ in the 8 rows
that compile to the whole unit is the same 5-byte jmp HEAD had at unit level, unreached, and reads NEQ only through a stale table
line; existing debt made visible, accepted. The stale line is queued (T3 nf422).
