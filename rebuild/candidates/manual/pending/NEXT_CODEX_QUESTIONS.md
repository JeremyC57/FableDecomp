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
4. **Gate every shared-header edit (pass 27 lesson).** Two pass-27 patches changed text for every includer: an unconditional
   `FABLE_RETAIL_ASSERT_ARM(index != 0)` in CBankFile::GetEntryDataType (12 landed rows stopped compiling) and an unconditional
   declaration-only IsAwareOfThing in CTCPerceiveThing.h (2 rows DIFFER). Both are now gated (`batches/2026-11-22_landing_p27/README.md`).
   Every edit to an existing line belongs under the row's opt-in macro. Before handing back, run equiv_check, then a recheck of every
   landed row whose preprocessed text changes.
5. **Inline-budget markers:** 16 pass-27 rows carry inline devices without the `// Inline-budget:` line and per-device ablation
   (`batches/2026-11-22_landing_p27/logs/inline_devices_without_marker.txt`). Add them; rows whose device turns out unnecessary
   drop it.
