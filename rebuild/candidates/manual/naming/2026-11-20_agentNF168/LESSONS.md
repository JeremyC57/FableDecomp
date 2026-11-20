# NF168 lessons (one line each)
- A stand-in that names a callee after a byte twin can be the wrong member: 0x005EE136 / 0x005EDE93 differ only in the vector offset (+0x148 / +0x160); compare catalog bytes and the Ego_r layout before taking the catalog label, and read vland's RELOC_CONFLICT as "one symbol, two addresses".
- Local `struct X_Calls` / `X_Retail` / `XMethods` stand-ins for a header class move fastest through a declaration-only opt-in on the header's flat (generated) branch (CTCPerceiveThing FLAT_*): no layout change, the row keeps its other local models.
- A real STLport map with `FABLE_RETAIL_STLP_TREE_MFIND_NOINLINE` replaced a same-TU fake `find` body (the address-escape trick) in four rows at once, first grade.
- Copying a sibling row's whole define/include prologue (0x005B0AAC) is cheaper than building a minimal one: the minimal set failed twice on hidden prerequisite macros.
- Returning a derived pointer (CThingCreatureBase*) where the row needs CThing* fails on the default headers (incomplete / unrelated class): add the retail CThing chain defines (0x004FF750's set).
- WORKLIST is partial: the exe scan (REFS / xr.sh) found 0x00783380 and the six 0x0059Cxxx / 0x0059Dxxx rows the binding table hid.
