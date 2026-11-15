# acctC workflow hand-back wfE10 (unworked rows round 3, 2026-11-15)


`open/` holds the best drafts of the batch's unmatched rows (full residues in NOTES.md):
- 0x007A1B20: near_miss, DIFFER(2828v2840). Still 8 hunks and the frame is 0xbc, same as retail. The only residue is in the decapitating-hit copy: retail stores X with fstp and squares Z,Y,X (0x007A1E68..0x007A1EAE), while ours does fst then X,Z,Y, the same as the plain-hit copy, which matches. The loop
- 0x0078FD60: parked, DIFFER(9472v9268). Not reworked this pass. Twin of 0x00795300, generated from the same source by renames; grades the same (9472v9268, 139 masked hunks). Any fix to the twin applies here. Best source: differ/0078fd60.cpp.
- 0x00795300: parked, DIFFER(9472v9268). Not reworked this pass. The real name is CTCUnholyPower::FrameUpdate (vtable slot 9 plus Ego_r name), not CTCDivineWrath; it is in NAME_FIXES.tsv. Open, in order: the second cancel-sound arm must call GetString 0x00415DD0, then the tail merge, a duplicated CEv
- 0x00507C30: near_miss, DIFFER(7661v7661). Only residue: the swapped reload pair at loop exit 0x00509395 (0 hunks with registers masked; call targets right). New measurement (c2prio on probe/r3.cpp): local variable names change c2's original candidate ids. Renaming to the Xbox PDB names (file_length, m
- 0x00502E90: parked, DIFFER(13933v13622). Ran the C26 inliner trace (tools/inl.sh). Threshold T=74. The servant site expands with score 29; the apprentice gets the step penalty (score 151). In the fill-homes loop the push_backs score 124/97/159, all with the penalty. Without the penalty they would sco

