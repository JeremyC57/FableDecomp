# ESCALATE 0x0059FEE3 CTCInventoryTrade::AddBuyItemToList (DIFFER 1552v1556; size TU, optimize("y",off)+("s",on))
Best source: differ/0059fee3.cpp (Ego_d names / scopes; shared headers). c2prio trace: tmp/prio_59f.txt.
Residue: retail `xor ebx,ebx` gives ptc_inventory its 0 in ebx (also the two CCountedPointer zero stores), the found component
goes straight into ebx (`mov ebx,[eax+4]`, no reload on the miss path); `this` is homed at [ebp-0Ch] (0x0059FEEC) and lives in
esi from 0x0059FF54. Ours (c2prio): `this` prio 17 -> ebx for the whole body, ptc_inventory prio 13 split (zero stored to
[ebp-8], `mov esi,[ebp-8]` on the miss), const 0 immediate; frame 60h v 64h.
Tried: Ego_d names, value_pair, three declaration orders (egod2a); PeekTC(GetInventoryType(item_def), &ptc_inventory) with no
inventory_type local (Ego_d has none): identical bytes to the HasTC/GetTC spelling.
Ask: what source shape gives ptc_inventory more weight than `this` in the entry block (e.g. an extra use / test of ptc_inventory,
a different GetTC form), or lowers `this` there; 1-3 hand candidates for the first 20 lines.
- (row 4 ruler) the dead-arm assert extra reference to ptc_inventory: no byte change -> not a reference-count gap.
