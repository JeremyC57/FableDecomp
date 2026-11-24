# Landing nf374_b2 (2026-11-24): two NF374 held binder rows released by the rulings

Run dir built by the coordinator from `pending/2026-11-24_nf374_handback/b1_hold/` (the rows' orig texts equal HEAD). HEAD and new
compiled from the repo at one path: both RELOCATION_MATCH PASS; land_scripted rechecked both on the landed tree.
- 0x006C7960 TransferOut<COpinionDeed*>: calls TransferOut<CIntelligentPointer<CThing>> 0x006C7B00 by its defined spelling (by reference).
  Held only for binding NF373's operator+= unit, which has landed. `.drectve` byte-equal; symbols differ only by the rename.
- 0x00939E70 IsPlayerCarryingWeapon: calls FindMostRecentDeed<COpinionDeedFilter_IsStillHappening> 0x0093A3C0 by its defined spelling
  (W4EOpinionDeedType). Ruling (advisor decision, owner delegated): `.drectve` equal under the rename is acceptable here: 4 tokens, same
  order; the one changed token is the `/alternatename` whose left operand is the renamed callee (in the rename map) and whose right operand
  is its plain-member twin renamed the same way; the left operand is now defined by 0x0093A3C0 itself, the address the table bound the old
  spelling to, so the alias resolves to the same address as at HEAD.
Table: two `replace` lines (0 misses, no new multi-target symbol).
