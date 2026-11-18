# Naming evidence

- Ego_r 0x00B454F0 and FableWin 0x030A2200 give the exact leaf `?GetData@CGameEventDataPackage@@QAEXJPAX@Z`.
- Ego_d and both PDBs identify CGameEventDataPackage::GetData. Native uses byte_pointer, bytes_to_read and read_pos to consume ring contents for shipped game-event callers.
- The old generic reader aliases are inconsistent with the 32-byte ring and the native callers. The Ego_d overflow_check describes the signed bounds test.

## Behavioral fixture

The fixture executes the selected ring reader over start offsets and byte counts. Standalone overflow handling and sentinel memory expose both copy paths and write bounds.
