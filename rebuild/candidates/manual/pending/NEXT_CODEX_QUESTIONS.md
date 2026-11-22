# Questions for the next Codex package (append; the package builder copies this into its brief)

1. **0x00CF4EC0 CFanMarker::Main, FableWin line 887** (`pending/2026-11-22_auth16/00cf4ec0/TERMINAL.md`): read FableWin's code for one
   macro expansion (Mod 0011) and name the 5-byte difference.
2. **The TLC-era form of three getters read like references** (`pending/2026-11-22_auth18/0093d270/TERMINAL.md`): 0x0093D270 LANDED as
   Inferred (owner, 2026-11-22) with `CCombatManager::GetProximityForNavigation` returning `const float&` (CCombatManager.h
   `_PROXIMITY_REF_INFERRED`), against all three lane PDBs' by-value `float`; 0x0066B370's two CAnimSet percentage accessors show the same
   mechanism (third site, second class). Evidence still wanted: any TLC-era source (header, PDB, map, string, FSE, EgoCore) showing a
   reference return or public member read. Also grade 0x006AA720 with a fresh draft on the shared headers and the new opt-in.
3. **0x00B4D370 CEngineShadowRenderer::UpdateShadowSpace, FableWin's code for Engine_Shadow_Renderer2.cpp line 590 and the tail lines
   636-657** (`pending/2026-11-22_auth20/00b4d370/ESCALATE.md`): line 590 (diag's GetMagnitude) is 96 B in FableWin and no measured
   spelling (68..112 B) gives 96; read the instructions and name the text.
