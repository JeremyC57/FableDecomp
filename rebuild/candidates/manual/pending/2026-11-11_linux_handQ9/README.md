# Chunk Q9 (parked near misses, Opus 5.5)

4 of 40 rows matched, and all four landed in landing co: 0x009F3010, 0x006AB430, 0x00E1CAE0 and 0x0092B900. The other 36 are in
differ/, each with its residue on line 2. The closest are:
- **0x0092ACE0 GoToLibrary (389v389).** Only the LookAtPos argument setup is left.
- **0x0047042A TransferBinaryIn (716v716).** One SIB operand order.
- **0x005A6769 ConstructNormalShopListPC (1336v1336).** The operand order of one sum.
- **0x004FEB70 HeightAreaChanged (260v261).** Retail keeps the constant 0 in ebx.

The global `g_LibraryLookTurnSpeed` in these drafts is respelled `g_LookAtTurnSpeed` (one name for 0x0138CBA8).
