# Questions for the next Codex package (append; the package builder copies this into its brief)

1. **0x00CF4EC0 CFanMarker::Main, FableWin line 887** (`pending/2026-11-22_auth16/00cf4ec0/TERMINAL.md`): read FableWin's code for one
   macro expansion (Mod 0011) and name the 5-byte difference.
2. **0x0093D270 / 0x006AA720, the TLC-era form of `CCombatManager::GetProximityForNavigation`** (`pending/2026-11-22_auth18/0093d270/TERMINAL.md`):
   a `const float&` return graded RELOCATION_MATCH on 0x0093D270 (oracle), against all three lane PDBs' by-value `float`. Is there any
   TLC-era source (header, PDB, map, string, FSE, EgoCore) that shows a reference return or a public member read? Also grade 0x006AA720
   with a fresh draft on the shared headers, with and without the reference form.
