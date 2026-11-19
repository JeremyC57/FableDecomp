# ESCALATE 0x005AC10E CTCInventoryTrade::ConstructActualBuyInventoryPC -- DIFFER(3532v3532), 2 hunks

Constraint: at 0x005ACB58 (merged SetName tail of the trading-goods loop's if/else-if chain) retail reloads
`mov esi,[ebp+74h]` (ui_trading_goods_list_children_buy) before `mov edi,[ebp+4Ch]` (pframe); ours the reverse. c2prio:
pframe (record 30, prio 197) split -> edi piece #67; children ref (record 51, prio 177) split -> esi piece #125. GOTCHAS
(0x00F3E120): reloads at a region entry come out in piece-id order, and split rounds go in ascending record id -- so retail
needs the children ref's piece numbered before pframe's (earlier record / earlier split round).
Measured, all the same 2 hunks: Ego_d scopes applied (goods lists + their refs at function scope, PC_TEXT blocks through the
block-scope `CText* pupper_menu_entry` with only `v` per block, `CComponent* pparent = ...FindChild(...)`, `CComponent* pgold`
with pgold_real inside the block -- all Ego_d locals now present); declaration reorders of pbuy_subtitle/_real and
ui_trading_goods_list (same / worse 3531). Candidate source with the Ego_d scopes: probe/5a_d.cpp.
