# Parked 0x005B37F7 CTCInventoryClothing::InitialiseMannequin (agentNF12) -- DIFFER 759 vs 760

Best source `005b37f7.cpp` (7 grades). It needs the two headers here, which were NOT installed in rebuild/include because no landed
row uses them yet: `CInventoryDef.h` (repo header plus the FABLE_RETAIL_CINVENTORYDEF_DOLLS opt-in: Ego_r members after
MeshAreaBRPos through StatsPersonalityDoll) and the new `CTCDNone.h`. Install both when the row (or 0x005CD767, same residue) lands.
Residue: the TCI_SKELETAL_MORPH (0x78) GetTC lookup's `cmp [eax],ebx ; jg` layout (see ../NOTES.md "Left, with reasons").
