# Naming evidence

- Ego_r 0x0096EE80 and FableWin 0x02BBC370 give the protected virtual leaf. Native vtable reference 0x0128BEF4 corroborates the slot.
- Ego_d ai_state_group_go_to_pub.obj names to_continue, ret, tc_tavern_game, tc_ai_scratchpad, ptc_mode_manager, tc_tavern_tankard, action, output and tc_tavern_table. Native calls corroborate their roles. The additional table-search locals spell the branch that appears expanded before WalktoSeatAndSit.
- Both PDBs identify ChooseAction, Drink, PayAtBar and GetSeat. The same-TU found-seat path and the native call to the seat-state setter establish the table/tankard component roles. The native by-value script string construction from the scratchpad at +0x74 and +0x70 corroborates the established GetSeatName and GetTableName accessors.
