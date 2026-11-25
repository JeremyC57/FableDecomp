# Landing nf409 b1: 0x004DA6A9 is `??_G?$pair<long, CIntelligentPointer<CThing>>`

0x004DA6A9 was one of four rows that strongly defined the made-up `?vector_deleting_destructor@@YIPAXPAVCLandscapeBackgroundPatch@@HI@Z`. It is a compiler-generated, non-virtual scalar deleting destructor of 31 bytes, with no vftable slot. Its name is the one the existing table line already binds (witness 0x004DD79F); retail's caller and callee chain confirms it.

The row is re-rowed in the nf86 / nf88 pick-any form on the class's engine header, with `#pragma optimize("s",on)` kept. The table needs no change.

NF409 gates:
- vland: RELOCATION_MATCH PASS on both sides.
- pk: 0 NEQ, 0 reach flips.
- extra COMDATs: tier 1.
- census /FORCE link: duplicates 21 -> 18 with b2.

Coordinator:
- 1/1 landed, 0 LITERAL_DIFF.
- The first run held this row on T3 nf405; it landed on a rerun with that line set aside.
- Self respelt, column 4 aligned.
- Baselines OK.
