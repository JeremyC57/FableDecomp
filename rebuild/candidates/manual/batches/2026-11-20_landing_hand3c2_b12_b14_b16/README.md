# Landing hand3c2 b12, b14, b16 (Account B, 2026-11-20)

Five rows: 3 new, 2 re-ports. Strict vland `--land` gives each row the grade below with its test passing.
The rolling auditor audited all three sets ($S/stage/audit_roll; b12 first in audit_h3c2b). Each passed strict on the tree after A's nf118..nf124.

| address | row | kind | grade | notes |
|---|---|---|---|---|
| 0x009BF7E0 | CDisplayManager::CreateDevice | new | RELOCATION_MATCH | hand3c2 b14; same-TU copies of 0x009BE340, 0x009BE380, 0x009BE3C0 under Fidelity lines |
| 0x009BE340 | CDisplayManager::ReadDeviceRefreshRate | re-port | RELOCATION_MATCH | was "CDisplayManager::GetRefreshRate" (Ego_d: GetRefreshRate is the const public accessor, a different function) |
| 0x009BE380 | CDisplayManager::CalcPresentationIntervalTime | re-port | MATCH | was `_global::FUN_009be380` on a row-local stand-in class |
| 0x00BADE40 | CEnginePrimitiveManagerSprites::RenderSprite | new | RELOCATION_MATCH | hand3c2 b12 |
| 0x009C3C20 | CFileInstaller::ProcessActiveFileForDecompression | new | RELOCATION_MATCH | hand3c2 b16 |

## Notes
- **Mutants:**
  - 0x009BF7E0 128/128, 0x009BE340 13/13, 0x009BE380 11/11 (plus one equivalent).
  - 0x00BADE40 74/74 + 1, 0x009C3C20 146/146.
- **b14 audit fixes** (`audit_README_b14.md`):
  - The author's made-up `GetD3DFormat` became the existing CPixelFormat::GetAsD3DPixelFormat opt-in.
  - Expanded functions are same-TU copies in the row, with declarations only in the header.
  - `throw CGenericException();` in both ReadDeviceRefreshRate texts carries `// Inferred:` (Ego_d line 854 is 32 bytes). This is still listed
    as an owner review item.
- **The 0x009BE3C0 copy:** CDisplayManager::IsAntiAliasingModeValid has no catalogued row or oracle entry, so the Fidelity copy is its only text.
  The audit confirmed it compiles to retail's 53 bytes (relocation field masked).
- **Headers** (hdrcheck2 default=same for all five changed headers; the only DIFFs are the new opt-ins, which no landed row defines):
  - new: engine/IDirect3D9.h, engine/CGenericException.h, engine/CVsyncCallbackTimer.h, engine/CEngineSubPrimitiveSprite.h;
  - add-only opt-ins: CDisplayManager.h (CREATEDEVICE), CSurface.h, CCompressorZlib.h (STREAM_INPUT), CFileInstaller.h (CPENDINGWRITE_CTOR),
    CEnginePrimitiveManagerSprites.h (RENDERSPRITE);
  - the remaining asserts are pinned with FABLE_STATIC_ASSERT_AT.
- **Newly flagged rows** (owner decision 2026-11-20: land now, re-port later): the four new headers newly flag landed rows that carry their
  own copy of those classes under the widened HEADER_POLICY. The bytes are unchanged. The rows are listed in `newly_flagged_rows.tsv` and
  join the re-port backlog: 54 rows for b14, 2 for b12, none for b16.
- **NAME_FIXES.tsv** (for Account A): GPCamera / GPSubPrimitiveRenderer (NEngineInternal), DecompressStream's parameter type, the
  CFileInstaller buffer helpers, std::list::push_back, and others.
