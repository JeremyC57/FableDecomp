// InvalidDecode_HaltStub -- catalog row with no bytes (not code)
// Grade: DIFFER(not code) -- 0x0132D470 lies in .rdata (0x0122D000..0x01373A4C, characteristics 0x40000040, not executable) and the row length is 0; there is nothing to reconstruct
// Leaf: none
// Flags: none
// Next idea: drop the row from the catalog (a decode artefact over read-only data), or relabel it as the data object at that address.
