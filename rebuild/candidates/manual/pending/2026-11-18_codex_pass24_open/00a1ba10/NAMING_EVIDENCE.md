# Naming evidence

- Ego_r 0x00B866A0 and FableWin 0x031AD860 give the exact leaf `?GetRandomNavigablePos@CNavigatorAStar@@UBE?AVC3DVector@@ABV2@MABVC2DVector@@M@Z`.
- Ego_d's function record names a separate bool overload. The selected four-argument type is independently present in both PDBs and the retail vtable. Its lower-case local spellings are descriptive.
- The native vector-return copy, projection/height check and four consumed argument slots identify this overload. The interior local and constant spellings in the older landed contexts are preserved.

## Behavioral fixture

The fixture runs the selected GetRandomNavigablePos body against synthetic navigation nodes and height checks. It exercises failed candidates and confirmed outputs with deterministic randomness.
