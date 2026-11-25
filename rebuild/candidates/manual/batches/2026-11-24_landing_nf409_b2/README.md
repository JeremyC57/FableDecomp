# Landing nf409 b2: the EObjectAugmentationType map's pair destructor and _M_erase, as one unit

- 0x004E41A1 is `??_G?$pair@$$CBW4EObjectAugmentationType@@VCObjectAugmentationParticleSet@@@std@@QAEPAXI@Z` (the exact Ego_r.pdb spelling), re-rowed in the pick-any form.
- 0x004E7772 is that tree's STLport `_M_erase`. Its old self was the shared stand-in `?BuildTreeArray@LTextTreeWalkThrough@@...`. It joins the unit because it is the only witness of the table line at 0x004E41A1.

TABLE_CHANGES replaces `?Destroy_004e41a1@CInner@@QAEXH@Z` with the pair `??_G` (witness 004e7772).

NF409 gates:
- vland: RELOCATION_MATCH PASS on both sides.
- pk: 0 NEQ, 0 reach flips.
- `~pair` has a unique tier-2 home at 0x004E3663. The other copies are tier 1. The tier tool's RESIDUE on 0x004EC33E's explicit instantiation is overridden, as stated in RESULTS.

Coordinator:
- 2/2 landed, 0 LITERAL_DIFF.
- Both selves applied, column 4 aligned. 0x004E7772 is a non-owner member; its self was hand-verified as defined in its object.
- Baselines OK.
