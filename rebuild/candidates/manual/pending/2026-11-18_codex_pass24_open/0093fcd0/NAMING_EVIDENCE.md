# Naming evidence

- Ego_r 0x00963A90 and FableWin 0x02B96C90 provide the exact public virtual leaf. The native vtable at 0x0128BAA0 points to it at 0x0128BAD0.
- Ego_d ai_state_group_vt_security_sweep.obj gives pvillage, ptc_perception, ret, pworld_seed, nav_route_calculated, pnav, t_angle, num_looks_before_moving, next_pos, vector_to_look, pos_to_look_at, frame_to_stop_looking and nav_route_calculated2. The native calls and temporary storage correlate them with the village task, perception, randomization, movement and gaze.
- Both PDBs and retail 0x00658586 identify SetSecuritySweepTaskAsAcceptedByThing. The Ego_r state-group members and the neighboring Cleanup use PTCVillageMember at +0x14 and PTask at +0x18. The four marker accessors are visible in the original FableWin PDB and expand the corresponding loads here.
- The second navigation bool is an output. The native row passes its address at 0x009400A7 without writing it. The callee at 0x0097EC00 initializes the referenced output before inspecting it.
