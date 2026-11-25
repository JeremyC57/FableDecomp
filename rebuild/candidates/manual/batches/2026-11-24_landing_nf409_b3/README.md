# Landing nf409 b3: 0x005C1390 is `??_GCItem@NInventory@@QAEPAXI@Z`

The exact spelling comes from FableWin. Retail's call chain confirms it: 0x005BF969 -> erase 0x005C1ECC -> `__destroy_aux` 0x005C18C6 -> 0x005C1390. The row is in the pick-any form and replaces the shared stand-in self.

Ruling R-NF409-1 (advisor decision, owner delegated): ACCEPT. The other rows' unchanged pick-any copies now read NEQ at this address. They are booked on two T3 lines:
- the unreached pair 0x005BF91A / 0x00621A20 gets a port with the implicit-dtor opt-in;
- the framed pair 0x005BF969 / 0x005C1ECC gets the `optimize("y", off)` placement lever and a link-order note.

Coordinator: 1/1 landed, RELOCATION_MATCH PASS, 0 LITERAL_DIFF, self applied, T3 nf391 0x005C1390 and nf405 0x004DA6A9 closed.
